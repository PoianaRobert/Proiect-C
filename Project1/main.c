#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
struct transaction
{
    /*
    struct to store financial data
    type date and description of transaction are saved as char arrays whilst amount
    is a float value used to store the money amount
    */
    char type[12], date[12], desc[400];
    float amount;
};

void menu(char str[])
{
    /*
    initialize str with a menu
    str: an array of chars    
    */
    strcpy(str, "");
    strcat(str, "1.Add new transaction\n");
    strcat(str, "2.Write a transaction to file for later use\n");
    strcat(str, "3.Calculate current balance\n");
    strcat(str, "4.Display a list of previous transactions\n");
    strcat(str, "5.Load data from file\n");
    strcat(str, "6.Print all transacions in a timeframe\n");
    strcat(str, "0.Exit program\n");
}

bool valid_date(char date[])
{
    /*
    checks if a given date is valid
    date[] : char array containing a date 
    */
   int k = 0,d[3]; // array to store the day, month and year 
   char cpy[12], *p;
   strcpy(cpy,date);
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
                else if(d[0] == 29 && d[1] == 2 && (d[2]%400 == 0 || (d[2]%4 == 0 && d[2] % 100 != 0)))
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
    do{
        printf("%s\n", "Transaction type(income/expense): ");
        fgets(tr->type, sizeof(tr->type), stdin); // reads the type of transaction, since the value is given to a pointer of t[k].type it laso changes globally
        tr->type[strcspn(tr->type, "\n")] = '\0'; // gets rid of the \n character that is read by the fgets() function
    } while(strcmp(tr->type, "income") != 0 && strcmp(tr->type, "expense") != 0);
    
    do{
    printf("%s\n", "Transaction date(DD.MM.YYYY): ");
    fgets(tr->date, sizeof(tr->date), stdin);
    tr->date[strcspn(tr->date, "\n")] = '\0';
    }while( !valid_date(tr->date));

    do{
        printf("%s\n", "Transaction desc: ");
        fgets(tr->desc, sizeof(tr->desc), stdin);
        tr->desc[strcspn(tr->desc, "\n")] = '\0';
    } while(strlen(tr->desc) == 0);

    do{
    printf("%s\n", "Transaction amount(>0): ");
    scanf("%g", &(tr->amount));
    while(getchar() != '\n'); // consumes newline character, making sure fgets() doesn' end up reading it
    } while(tr->amount < 0); 
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

float account_balance(struct transaction t[], int n)
{
    /*
    calculates total account balance of transactions currently in the struct array
    t[]: struct array that needs to have its balance calculated
    n: int value representing total lenght of array
    */
    float s = 0;

    for (int i=0; i<=n; i++)
        if(strstr(t[i].type, "expense") != NULL)
            s -= t[i].amount;
        else
            s += t[i].amount;
    return s;
}

void previous_transactions(struct transaction t[], int n)
{
    /*
    prints our struct array elements inside the console
    t[]:  struct array that needs to be printed 
    n: int value representing total lenght of array
    */

    for (int i=0; i<=n; i++)
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

    for (int i=0; i<=n; i++)
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

    for (int i=0; i<=n; i++)
    {
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

void load_from_file(char s[], struct transaction *t, int *count)
{
    /*
    loads transactions currently inside the file
    s: char array that contains the name of the file data will be printed to
    *t: a pointer to current struct array that data will be read into
    *count: pointer to how many transactions have been loaded from the file
    */
    FILE *f = fopen(s, "r");
    char temp[400];

    if (f == NULL)
    {
        printf("%s", "Error opening file");
        return;
    }

    while (fgets(temp, sizeof(temp), f) != NULL)
    {
        if (sscanf(temp, "%11s %11s  %f %349[^\n]",
            t[*count].type,
            t[*count].date,
            &(t[*count].amount),
            t[*count].desc) == 4)
            (*count)++;
    }
    fclose(f);
}
int main()
{
    struct transaction t[100];
    unsigned command;
    int k = -1;
    bool alive = 1;
    char s[500] = "";
    menu(s); //  append the menu to our string 

    while(alive)
    {
       printf("%s", s);
       scanf(" %u", &command);
       while(getchar() != '\n'); // consumes newline character and any before it, making sure fgets() doesn't end up reading it 
       switch (command)
       {
            case 0:
            {
                alive = 0;
                break;
            }

            case 1:
            {
                k++;
                new_transaction_data(&t[k]);           
                break;
            }

            case 2:
            {
                if(k == -1)
                    printf("%s", "No transactions found, add some!\n");
                else
                    {
                        char c='N';
                        int n;
                        do{
                        
                        previous_transactions(t, k);
                        
                        do{
                        printf("Which transaction would you like to print?\n");
                        scanf("%d", &n);
                        while(getchar() != '\n');
                        if(n > k+1 || n < 1)
                            printf("Invalid number, provide one of the given ones\n");
                        }while(n > k+1);
                        
                        write_to_file("output2.txt", t[n-1]);
                        
                        printf("Do you wish to add save another transaction to the file?(Y/N)\n");
                        do{
                        scanf("%c", &c);
                        while(getchar() != '\n');
                        if(strchr("YyNn", c) == NULL)
                            printf("Provide a Y/N character\n");
                        }while(strchr("YyNn", c) == NULL);
                        
                        }while(c == 'Y' || c == 'y');
                    }
                break;
            }

            case 3:
            {
                if(k == -1)
                    printf("%s", "No transactions found, add some!\n");
                else
                    printf("%g\n", account_balance(t, k));
                break;
            }

            case 4:
            {
                if(k == -1)
                    printf("%s", "No transactions loaded, add some!\n");
                else
                    previous_transactions(t, k);
                break;
            }

            case 5:
            {
                int count = 0;
                load_from_file("output2.txt", t, &count);
                printf("loaded %d transactions\n", count);
                k = count-1;
                break;
            }

            case 6:
            {
                char start_date[12], end_date[12];
                char flipped_start[12], flipped_end[12];

                do{
                printf("%s", "Give starting date(DD.MM.YYYY): \n");
                fgets(start_date, sizeof(start_date), stdin);
                start_date[strcspn(start_date, "\n")] = '\0';
                printf("%s", "Give end date(DD.MM.YYYY): \n");
                fgets(end_date, sizeof(end_date), stdin);
                end_date[strcspn(end_date, "\n")] = '\0';

                flip_date(start_date, flipped_start); // flip date format since we read it as DD.MM.YYYY and need it flipped to use strcmp
                flip_date(end_date, flipped_end);

                if(!valid_date(start_date) || !valid_date(end_date))
                    printf("%s", "invalid date\n");
                else if(strcmp(flipped_start, flipped_end) > 0)
                    {
                    printf("%s", "starting date cannot be bigger than end date\n");
                    }
                }while((!valid_date(start_date) || !valid_date(end_date)) || strcmp(flipped_start, flipped_end) > 0);
                previous_transactions_in_timeframe(t, k, start_date, end_date);
                income_breakdown_for_timeframe(t, k, start_date, end_date);
                break;
            }
            
            default:
                printf("%s", "Unknown command");
            break;
       }
    }
    return 0;
}
