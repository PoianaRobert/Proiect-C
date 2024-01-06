#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#pragma once
struct transaction
{
    /*
    struct to store financial data
    type date and description of transaction are saved as char arrays whilst amount
    is a float value used to store the money amount
    */
    char *type, *date, *desc;
    float amount;
};
void initialize_transaction(struct transaction *tr)
{
    /*
    function that initializes a transaction element by setting its parameters to default values
    tr: transaction type element to initialize
    */
    tr->type = NULL;
    tr->date = NULL;
    tr->desc = NULL;
    tr->amount = 0.0;
}

void free_transaction(struct transaction *tr)
{
    /*
    function to free a transaction element
    my_entity: entity type element to free
    */
    free(tr->type);
    free(tr->date);
    free(tr->desc);
}

bool valid_date(char date[])
{
    /*
    checks if a given date is valid
    date[] : char array containing a date 
    */
   int k = 0,d[3]; // array to store the day, month and year 
   char *cpy, *p;
   cpy = strdup(date);
   p = strtok(cpy, ".\n");
   while(p != NULL)
   {
        if(k >= 3)
            return false;

        for(int i=0; p[i] != '\0'; i++)
            if(!isdigit(p[i])) // if every character of my date is not a digit return false
                return false;
        
        if(p[0]== '0' && (p[1] == '\0' || p[1] == '0')) // leading 0s return false
            return false;
        d[k] = atoi(p);
        k++;
        p = strtok(NULL, ".\n");
   }
    free(cpy);
    //printf("%d %d %d", d[0], d[1], d[2]);
    if(d[2] >= 1900 && d[2] <= 9999)
      {
        if(d[1] >= 1 && d[1] <= 12)
            {
                if((d[0] >= 1 && d[0] <= 31) && (d[1] == 1 || d[1] == 3 || d[1] == 5 || d[1] == 7 || d[1] == 8 || d[1] == 10 || d[1] == 12))
                    return true;
                else if((d[0] >= 1 && d[0] <= 30) && (d[1] == 4 || d[1] == 6 || d[1] == 9 || d[1] == 11))
                    return true;
                else if((d[0] >= 1 && d[0] <= 28) && d[1] == 2)
                    return true;
                else if(d[0] == 29 && d[1] == 2 && ((d[2] % 4 == 0 && d[2] % 100 != 0) || d[2] % 400 == 0))
                    return true;
            }
      }
    return false;
}
void new_transaction_data(struct transaction *tr)
{
    /* 
    reads a treansaction from keyboard and stores in an element of the array of transaction structs
    *tr: a pointer to current struct array that data will be read into
    */
    initialize_transaction(tr);
    do{
        char *temp = NULL;
        size_t len = 0;
        printf("%s\n", "Transaction type(income/expense): ");
        if(getline(&temp, &len, stdin) == -1) // reads the type of transaction, since the value is given to a pointer of t[k].type it laso changes globally
        {
            printf("Error using getline()\n");
            free(temp);
            return;
        }
        temp[strcspn(temp, "\n")] = '\0'; // gets rid of the \n character that is read by the fgets() function
        tr->type = strdup(temp);
        printf("tr type: %s\n", tr->type);
        free(temp);
    } while(strcmp(tr->type, "income") != 0 && strcmp(tr->type, "expense") != 0);
    
    do{
        char *temp = NULL;
        size_t len = 0;
        printf("%s\n", "Transaction date(DD.MM.YYYY): ");
        if(getline(&temp, &len, stdin) == -1) 
        {
            printf("Error using getline()\n");
            free(temp);
            return;
        }
        temp[strcspn(temp, "\n")] = '\0';
        tr->date = strdup(temp);
        free(temp);
    } while( !valid_date(tr->date));

    do{
        char *temp = NULL;
        size_t len = 0;
        printf("%s\n", "Transaction desc: ");
        if(getline(&temp, &len, stdin) == -1) 
        {
            printf("Error using getline()\n");
            free(temp);
            return;
        }
        temp[strcspn(temp, "\n")] = '\0';
        tr->desc = strdup(temp);
        free(temp);
    } while(strlen(tr->desc) == 0);

    do{
        printf("%s\n", "Transaction amount(>0): ");
        scanf("%g", &(tr->amount));
        while(getchar() != '\n'); // consumes newline character, making sure fgets() doesn' end up reading it
        } while(tr->amount <= 0); 
}

void write_to_file(char s[], struct transaction t)
{
    /*
    writes current transaction to file
    s: char array that contains the name of the file data will be printed to
    t: current transaction array element we will print  
    */
    FILE *f = fopen(s, "ab"); // open in append binary mode
    if (f == NULL)
    {
        printf("%s", "Error opening file");
        return;
    }
    
    fprintf(f, "%s ", t.type);
    fprintf(f, "%s ", t.date);
    fprintf(f, "%g ", t.amount);
    fprintf(f, "%s\n", t.desc);
    fclose(f);
}

void previous_transactions(struct transaction t[], int n)
{
    /*
    prints our struct array elements inside the console
    t[]:  struct array that needs to be printed 
    n: int value representing total lenght of array
    */

    for (int i=0; i< n; i++)
    {
        printf("Transaction number %d\n", i+1);
        printf("Transaction type: %s\n", t[i].type);
        printf("Transaction date: %s\n", t[i].date);
        printf("Transaction amount: %g\n", t[i].amount);
        printf("Transaction description: %s\n\n", t[i].desc);
    }
}

void flip_date(char date[], char flipped_date[])
{
    /*
    flips a given date from DD.MM.YYYY to YYYY.MM.DD
    date[]: given date as an array of chars
    flipped_date[]: flipped date as a char array
    */
    char day[3], month[3], year[6];
    if(sscanf(date, "%2s.%2s.%4s", day, month, year) == 3)
        snprintf(flipped_date, 11, "%4s.%2s.%2s", year, month, day);
}

void previous_transactions_in_timeframe(struct transaction t[], int n, char starting_date[], char end_date[])
{
    /*
    prints our struct array elements that are inside a given timeframe
    into the console
    t[]:  struct array that needs to be printed 
    n: int value representing total lenght of array
    starting_date[]: start date of the timeframe
    end_date[]: end date of timeframe
    */
    char flipped_start[12], flipped_end[12];

    flip_date(starting_date, flipped_start); // flip date format since we read it as DD.MM.YYYY and need it flipped to use strcmp
    flip_date(end_date, flipped_end);

    for (int i=0; i<n; i++)
    {
        char flipped_date[12];
        flip_date(t[i].date, flipped_date);
        if(strcmp(flipped_date, flipped_start) >= 0 && strcmp(flipped_date, flipped_end) <= 0)
            {
                printf("Transaction type: %s\n", t[i].type);
                printf("Transaction date: %s\n", t[i].date);
                printf("Transaction amount: %g\n", t[i].amount);
                printf("Transaction description: %s\n\n", t[i].desc);
            }
    }
}

void income_breakdown_for_timeframe(struct transaction t[], int n, char starting_date[], char end_date[])
{
    /*
    calculates and prints total account balance of transactions in given timeframe currently in the struct array
    t[]: struct array that needs to have its balance calculated
    n: int value representing total lenght of array
    starting_date[]: start date of the timeframe
    end_date[]: end date of timeframe
    */
    float spent = 0, got = 0;
    char flipped_start[12], flipped_end[12];
    flip_date(starting_date, flipped_start);
    flip_date(end_date, flipped_end);

    for (int i=0; i<n; i++)
    {
        printf("%d\n", i);
    char flipped_date[12];
    flip_date(t[i].date, flipped_date);
        if(strcmp(flipped_date, flipped_start) >= 0 && strcmp(flipped_date, flipped_end) <= 0)
            if(strstr(t[i].type, "expense") != NULL)
                spent += t[i].amount;
            else
                got += t[i].amount;
    }

    if(spent !=n || got !=0)
        printf("Income for timeframe: %g \nExpenses for timeframe: %g\n", got, spent);
    else
        printf("%s", "No avalabile data for given timeframe\n");
}
