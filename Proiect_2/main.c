#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "functions.h"
#include "directory_functions.h"

#define MAX 500
#define MAX_TRANSACTIONS 100
struct entity {
  char *name;
  char *contact_info;
  char *desc;
};

struct Account {
    char *name;
    char *password;
    struct transaction transactions[MAX_TRANSACTIONS];
    unsigned int transaction_count;
    struct entity entitites[MAX_TRANSACTIONS];
    unsigned int entity_number;
};

void menu(char s[]);
void load_accounts(struct Account *accounts, unsigned *count);
void load_entities(struct Account *accounts);
int login(char username[], char password[], unsigned count);
void initialize_account(struct Account *my_accout);
void free_account(struct Account *my_account);
void free_entity(struct entity *my_entity);
void account_menu(char s[]);
int balance(struct Account my_account);
void previous_entities(struct entity e[], int n);

int main()
{
  char *s = calloc(500, sizeof(char)); // calloc  to make sure string is null terminated
  struct Account *my_account = malloc(100 * sizeof(struct Account));
  if(s == NULL)
  {
    fprintf(stderr, "Error allocating string memory!\n");
    return 1;
  }
  menu(s);
  unsigned k = 0;
  int command;
  bool alive = 1;
  load_accounts(my_account, &k);
  load_entities(my_account);
  printf("%u\n", k);
  while(alive)
  {
    char path[50] = "./Users/";
    command = -1;
    printf("%s", s);
    if(scanf(" %u", &command) != 1);
      while (getchar() != '\n');
    switch (command)
    {
        case 0:
        {
          alive = 0;
          break;
        }

        case 1:
        {
          if(sprintf(path, "%s%u", "./Users/", k) < 0)
            {
              printf("error using sprintf\n");
              break;
            }
          char *name = calloc(50, sizeof(char)), *password = calloc(50, sizeof(char));
          if(name == NULL || password == NULL)
          {
            fprintf(stderr, "Error allocating memory!\n");
            return 1;
          }

          printf("Give username\n");
          fgets(name, 49, stdin);
          name[strcspn(name, "\n")] = '\0';
          printf("Give password\n");
          fgets(password, 49, stdin);
          password[strcspn(password, "\n")] = '\0';
          if(avalabile_credentials(name, password, k) ==  -1)
          {
            initialize_account(&my_account[k]);
            my_account[k].name = calloc(50, sizeof(char));
            my_account[k].password = calloc(50, sizeof(char));
             if(my_account[k].name == NULL || my_account[k].password == NULL)
              {
                fprintf(stderr, "Error allocating memory!\n");
                return 1;
              }
            strcpy(my_account[k].name, name);
            strcpy(my_account[k].password, password);
            save_account(path, my_account[k].name, my_account[k].password, k);
            k++;
            free(name);
            free(password);
          }
          else
          {
            free(name);
            free(password);
            printf("Username or password already taken, use different credentials\n");
          }
          break;
        }
        case 2:
        {
          int index;
          char *username = calloc(50, sizeof(char)), *password  = calloc(50, sizeof(char));
          
          printf("Give username\n");
          fgets(username, 49, stdin);
          username[strcspn(username, "\n")] = '\0';
          
          printf("Give password\n");
          fgets(password, 49, stdin);
          password[strcspn(password, "\n")] = '\0';
          index = login(username, password, k);
          
          free(username);
          free(password);
          
          if(index >= 0)
          {
            char *user_menu = calloc(500, sizeof(char));
            account_menu(user_menu);
            bool used = 1;
            char ans = '\n';
            char path[50];
            if (sprintf(path, "%s%u/%u%s", "./Users/", index, index, ".txt") < 0) 
            {
            printf("Error using sprintf\n");
            return -1;
            }
            while(used)
            {
              printf("What would you like to do with your account?\n%s", user_menu);
              command = -1;
              do{
                scanf(" %u", &command);
                while (getchar() != '\n');
              }while(command == -1);
              
              switch (command)
              {
                case 0:
                {
                  used = 0;
                  break;
                }
                case 1:
                  {
                    do{
                    printf("Are you sure you would you like to update your account(y/n) ?\n");
                    scanf(" %c", &ans);
                    while (getchar() != '\n');

                    if(tolower(ans) != 'y' && tolower(ans) != 'n')
                    {
                      printf("Invalid command!\n");
                      ans = '\n';
                    }
                    }while(ans == '\n');
                    
                    if(ans == 'y')
                    {
                      char *new_data = calloc(MAX, sizeof(char));
                      do
                      {
                      strcpy(new_data, "");
                      printf("Give new username\n");
                      fgets(my_account[index].name, 49, stdin);
                      my_account[index].name[strcspn(my_account[index].name, "\n")] = '\0';
                      strcat(new_data, my_account[index].name);
                      strcat(new_data, " ");

                      printf("Give new password\n");
                      fgets(my_account[index].password, 49, stdin);
                      strcat(new_data, my_account[index].password);
                      }while(login(my_account[index].name, my_account[index].password, index) != -1);
                      
                      update_account(path, &k, index, new_data);
                      free(new_data);
                    }
                    break;
                  }
                  
                  case 2:
                  {
                    do{
                    printf("Are you sure you would you like to delete your account(y/n) ?\n");
                    scanf(" %c", &ans);
                    while (getchar() != '\n');

                    if(tolower(ans) != 'y' && tolower(ans) != 'n')
                    {
                      printf("Invalid command!\n");
                      ans = '\n';
                    }
                    }while(ans == '\n');

                    if(ans == 'y')
                      delete_account(&k, index);
                    break;
                  }

                  case 3:
                  {
                    new_transaction_data(&(my_account[index].transactions[my_account[index].transaction_count]));
                    write_to_file(path, my_account[index].transactions[my_account[index].transaction_count]);
                    my_account[index].transaction_count++;

                    char msg[MAX];
                    if(sprintf(msg, "%s %u\n", "Added a transaction for account number", index) < 0)
                      {
                        printf("error using sprintf\n");
                        break;
                      }
                    log_msg(msg);
                    break;
                  }

                  case 4:
                  {
                    printf("%d\n", balance(my_account[index]));
                    break;
                  }

                  case 5:
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
                    
                    income_breakdown_for_timeframe(my_account[index].transactions, my_account[index].transaction_count, start_date, end_date);
                    
                    break;
                  }
                  
                  case 6:
                  {
                    if(sprintf(path, "%s%u/%s", "./Users/", index, "entities.txt") < 0)
                    {
                      printf("error using sprintf\n");
                      break;
                    }

                    char *name = calloc(50, sizeof(char)), *contact_info = calloc(256, sizeof(char)), *desc = NULL;
                    if(name == NULL || contact_info == NULL)
                    {
                      fprintf(stderr, "Error allocating memory!\n");
                      free(desc);
                      free(name);
                      free(contact_info);
                      break;
                    }

                    printf("Give entity name\n");
                    fgets(name, 49, stdin);
                    name[strcspn(name, "\n")] = '\0';
                    printf("Give entity contact info\n");
                    fgets(contact_info, 254, stdin);
                    contact_info[strcspn(contact_info, "\n")] = '\0';
                    
                    size_t len = 0;
                    printf("Give description of entity\n");
                    if(getline(&desc, &len, stdin) == -1) 
                    {
                      printf("Error using getline()\n");
                      free(desc);
                      free(name);
                      free(contact_info);
                      break;
                    }

                    desc[strcspn(desc, "\n")] = '\0';

                    my_account[index].entitites[my_account[index].entity_number].name = strdup(name);
                    my_account[index].entitites[my_account[index].entity_number].contact_info = strdup(contact_info);
                    my_account[index].entitites[my_account[index].entity_number].desc = strdup(desc);
                    my_account[index].entity_number++;

                    write_entity_to_file(path, name, contact_info, desc);

                    free(desc);
                    free(name);
                    free(contact_info);

                    char msg[MAX];
                    if(sprintf(msg, "%s %u\n", "Added an entity for account number", index) < 0)
                      {
                        printf("error using sprintf\n");
                        break;
                      }
                    log_msg(msg);
                    break;
                  }

                  case 7:
                  {
                    previous_transactions(my_account[index].transactions, my_account[index].transaction_count);
                    printf("\nPress any key to continue\n");
                    getchar();
                    break;
                  }

                  case 8:
                  {
                    previous_entities(my_account[index].entitites, my_account[index].entity_number);
                    printf("\nPress any key to continue\n");
                    getchar();
                    break;
                  }

                  case 9:
                  {
                    char *new_data = calloc(MAX, sizeof(char));
                    int money, index_2;

                    do{  
                    printf("Please provide the name of the account you wish to make a transaction to:\n"); 
                    fgets(new_data, MAX, stdin);
                    }while(find_name(new_data, k) == -1);
                    
                    index_2 = find_name(new_data, k);

                    do{
                      printf("Please provide amount of money you wish to make a transaction with(must be greater than 0 and affordable)\n");
                      if(scanf(" %u", &money) != 1);
                        while (getchar() != '\n');
                    }while(money <= 0 || money > balance(my_account[index]));
                    
                    new_transfer_data(&my_account[index].transactions[my_account[index].transaction_count], money);
                    my_account[index_2].transactions[my_account[index_2].transaction_count].amount = my_account[index].transactions[my_account[index].transaction_count].amount;
                    my_account[index_2].transactions[my_account[index_2].transaction_count].type = malloc(strlen("income") + 1);
                    strcpy(my_account[index_2].transactions[my_account[index_2].transaction_count].type, "income");
                    my_account[index_2].transactions[my_account[index_2].transaction_count].date = strdup(my_account[index].transactions[my_account[index].transaction_count].date);
                    my_account[index_2].transactions[my_account[index_2].transaction_count].desc= strdup(my_account[index].transactions[my_account[index].transaction_count].desc);
                    
                    char path_2[50];
                    if (sprintf(path_2, "%s%u/%u%s", "./Users/", index_2, index_2, ".txt") < 0) 
                      {
                        printf("Error using sprintf\n");
                        return -1;
                      }
                    write_to_file(path, my_account[index].transactions[my_account[index].transaction_count]);
                    write_to_file(path_2, my_account[index_2].transactions[my_account[index_2].transaction_count]);

                    my_account[index].transaction_count++;
                    my_account[index_2].transaction_count++;
                    free(new_data);
                    break;
                  }

                  default:
                  {
                    printf("Invalid commad! \n");
                    break;
                  }
              }
            }
            free(user_menu);
        }
        else
          printf("Wrong user data, try again!\n");
        break;
        }
        
        default:
        {
          printf("Unrecognized command, try again!\n");
          break;
        }
    }
  }
  
  free(s);
  for (int i = 0; i < k; ++i) 
     free_account(&my_account[i]);
  free(my_account);
  printf(my_account[1].transactions[0].desc);
  return 0;
}

void menu(char s[])
{
  /*
  function to print starting menu
  s: char array to hold the menu
  */
  strcpy(s, "");
  strcat(s, "0.Exit out of program\n");
  strcat(s, "1.Make a new account\n");
  strcat(s, "2.Manage a financial account\n");
}

void account_menu(char s[])
{
  /*
  function to print a menu with the commands for an account
  s: char array to hold the menu
  */
  strcpy(s, "");
  strcat(s, "0.Exit out of account\n");
  strcat(s, "1.Update account\n");
  strcat(s, "2.Delete account\n");
  strcat(s, "3.Add a new transaction\n");
  strcat(s, "4.Get account balance\n");
  strcat(s, "5.Get financial report for timeframe\n");
  strcat(s, "6.Add an entity with whom you do business with\n");
  strcat(s, "7.Print transactions\n"); 
  strcat(s, "8.Print entities\n");
  strcat(s, "9.Transfer to an account\n"); 
}

void initialize_account(struct Account *my_accout)
{
  /*
  function that initializes an account element by setting its parameters to default values
  my_account: account type element to initialize
  */
  my_accout->name = NULL;                                                      
  my_accout->password = NULL;
  my_accout->transaction_count = 0;
}

void free_account(struct Account *my_account) 
{
  /*
  function to free an account element
  my_account: account type element to free
  */
  free(my_account->name);
  free(my_account->password);
  for(unsigned i = 0; i < my_account->transaction_count; i++)
      free_transaction(&(my_account->transactions[i]));
  free(my_account->transactions);
  for(unsigned i = 0; i < my_account->entity_number; i++)
      free_entity(&(my_account->entitites[i]));
  free(my_account->entitites);
}

void free_entity(struct entity *my_entity)
{
  /*
  function to free an entity element
  my_entity: entity type element to free
  */
  free(my_entity->name);
  free(my_entity->contact_info);
  free(my_entity->desc);
}

void load_accounts(struct Account *accounts, unsigned *count)
{
  /*
  function for loading already made accounts from a file
  accounts: account array to save the data into
  count: current number of account being loaded 
  */
  char *temp = calloc(500, sizeof(char));
  
  while(1)
  {
    char path[50];
    if(sprintf(path, "%s%u/%u%s", "./Users/", (*count), (*count), ".txt") < 0)
    {
      printf("Error in sprintf \n");
      break;
    }
    
    FILE *f = fopen(path, "r");
    if (f == NULL)
    break;
    
    fgets(temp, 500, f);
    accounts[*count].name = calloc(50, sizeof(char));
    accounts[*count].password = calloc(50, sizeof(char));
    if(sscanf(temp, "%49s %49s", accounts[*count].name, accounts[*count].password) != 2)
    {
      free(accounts[*count].name);
      free(accounts[*count].password);
      fclose(f);
      printf("Error loading accounts!\n");
      break;
    }

    int k = 0;
    while(fgets(temp, 500, f) != NULL)
    {
      accounts[*count].transactions[k].type = calloc(50, sizeof(char));
      accounts[*count].transactions[k].date = calloc(12, sizeof(char));
      accounts[*count].transactions[k].desc = calloc(400, sizeof(char));
      if(sscanf(temp, "%49s %11s %g %399[^\n]",
      accounts[*count].transactions[k].type,
      accounts[*count].transactions[k].date,
      &(accounts[*count].transactions[k].amount),
      accounts[*count].transactions[k].desc) == 4)
      {
      k++;
      }
    }
    accounts[*count].transaction_count = k;
    (*count)++;
    fclose(f);
  }
  free(temp);
}

void load_entities(struct Account *accounts)
{
  /*
  function for loading already made entities from a file
  accounts: account array to save the data into
  count: current number of account entities are being loaded into
  */
  char *temp = calloc(500, sizeof(char));
  unsigned count = 0;
  while(1)
  {
    char path[50];
    if(sprintf(path, "%s%u/%s", "./Users/", count, "entities.txt") < 0)
    {
      printf("Error in sprintf \n");
      break;
    }

    FILE *f = fopen(path, "r");
    if (f == NULL)
    break;
    int k = 0;
    while(fgets(temp, 500, f) != NULL)
    {
      accounts[count].entitites[k].name = calloc(50, sizeof(char));
      accounts[count].entitites[k].contact_info = calloc(256, sizeof(char));
      accounts[count].entitites[k].desc = calloc(400, sizeof(char));
      
      if (accounts[count].entitites[k].name == NULL || accounts[count].entitites[k].contact_info == NULL || accounts[count].entitites[k].desc == NULL) 
        {
        printf("Memory allocation error.\n");
        break;
        }

      if(sscanf(temp, "%49[^|]|%255[^|]|%399[^\n]", 
      accounts[count].entitites[k].name,
      accounts[count].entitites[k].contact_info,
      accounts[count].entitites[k].desc) == 3)
        {
          k++;
        }
    }
    accounts[count].entity_number = k;
    count++;
    fclose(f);
  }
  free(temp);
}

int balance(struct Account my_account)
{
  int sum = 0;
  printf("%d\n", my_account.transaction_count);
  for(unsigned i=0; i < my_account.transaction_count; i++)
    if(strcmp(my_account.transactions[i].type, "income") == 0)
      {
        sum += my_account.transactions[i].amount;
      }
    else
      {
        sum -= my_account.transactions[i].amount;
      }
  return sum;
}

void previous_entities(struct entity e[], int n)
{
    /*
    prints our struct array elements inside the console
    t[]:  struct array that needs to be printed 
    n: int value representing total lenght of array
    */

    for (int i=0; i< n; i++)
    {
        printf("Entity number %d\n", i+1);
        printf("Entity name: %s\n", e[i].name);
        printf("Transaction date: %s\n", e[i].contact_info);
        printf("Entity description: %s\n\n", e[i].desc);
    }
}