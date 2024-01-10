#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Check if code is run on Windows or linux and uses the correct functions accordingly

#ifdef _WIN32
#include <direct.h>
#include <Windows.h>
#define PLATFORM_MKDIR(dir) _mkdir(dir)
#define PLATFORM_RMDIR(dir) _rmdir(dir)
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define PLATFORM_MKDIR(dir) mkdir(dir, 0777)
#define PLATFORM_RMDIR(dir) rmdir(dir)
#endif

#pragma once

#define MAX 500

void log_msg(char message[])
{
  /*
  logs an acction with a given message to the log file
  message: char array holding the message to be printed to the file
  */
  char path[50];
  
  if(sprintf(path, "%s", "./Users/log.txt") < 0)
    {
    printf("error using sprintf\n");
    return;
    }

  FILE *f = fopen(path, "ab");
  if (f == NULL)
  {
    printf("%s", "Error opening file here");
    return;
  }
  
  if (fputs(message, f) == EOF) {
        perror("Error writing to file");
    }
  fclose(f);
}

void save_account(char name[], char username[], char password[], unsigned k)
{
  /*
  function to save user account data to a file
  name: char array with file name
  usernqme; username to save
  password: account password to save
  k: number of the account to save
  */
  if (PLATFORM_MKDIR(name) != 0)
    {
        perror("Error adding user directory");
        return;
    }

  char path[100];
  sprintf(path, "%s/%u%s", name, k, ".txt");
  printf("%s\n", path);
  
  FILE *f = fopen(path, "w");
  
  if (f == NULL)
  {
    printf("%s", "Error opening file here");
    return;
  }
  
  fprintf(f, "%s ", username);
  fprintf(f, "%s\n", password);
  char msg[MAX];
  if(sprintf(msg, "%s %u\n", "Saved account number", k) < 0)
    {
      printf("error using sprintf\n");
      return;
    }
  log_msg(msg);
  fclose(f);
}

void delete_account(unsigned *count, unsigned index)
{
  /*
  function to delete an account and rename the folders of the remaining accounts accordingly
  count: how many accounts there are in total, prevents calling the function with an invalid value
  index: index of account to delete
  */
  char path[50],prev_name[50],new_name[50],prev_file_name[50],new_file_name[50];
  if(index >= *count)
    {
      printf("No account at given index\n");
      return;
    }

  if(sprintf(prev_name, "%s%u", "./Users/", index) < 0)
    {
      printf("sprintf error\n");
      return;
    }
  
  if(sprintf(prev_file_name, "%s%u/%u%s", "./Users/", index, index, ".txt") < 0)
    {
      printf("sprintf error\n");
      return;
    }
  
  if (remove(prev_file_name) != 0)
    {
        perror("Error removing file");
        return;
    }
  
  if(sprintf(prev_file_name, "%s%u/%s", "./Users/", index, "entities.txt") < 0)
    {
      printf("sprintf error\n");
      return;
    }

  if (remove(prev_file_name) != 0)
    {
        perror("Error removing file");
        return;
    }

  if (PLATFORM_RMDIR(prev_name) != 0)
    {
        perror("Error removing user directory");
        return;
    }
    index++;

  while(index < *count)
        {
            if(sprintf(prev_file_name, "%s%u/%u%s", "./Users/", index, index, ".txt") < 0)
            {
                printf("sprintf error\n");
                return;
            }
            
            if(sprintf(prev_name, "%s%u", "./Users/", index) < 0)
            {
                printf("sprintf error\n");
                return;
            }
            
            if(sprintf(new_file_name, "%s%u/%u%s", "./Users/", index, index-1, ".txt") < 0)
            {
                printf("sprintf error\n");
                return;
            }

            if(sprintf(new_name, "%s%u", "./Users/", index-1) < 0)
            {
                printf("sprintf error\n");
                return;
            }
            
            if (rename(prev_file_name, new_file_name) != 0)
            {
                perror("Error renaming file");
                return;
            
            }
            
            if (rename(prev_name, new_name) != 0)
            {
                perror("Error renaming directory");
                return;
            
            }
            index++;
        }
  (*count)--;
  char msg[MAX];
  if(sprintf(msg, "%s %u%s\n", "Deleted account number", (*count) + 1, ",all remaining account folder names have been updated accordingly") < 0)
    {
      printf("error using sprintf\n");
      return;
    }
  log_msg(msg);
}

void update_account(char path[],unsigned *count, unsigned index, char new_data[])
{
  /*
  function to update the data of an account
  count: how many accounts there are in total, prevents calling the function with an invalid value
  index: index of account to update
  new_data: new data to replace the old with
  */
   if(index >= *count)
  {
    printf("No account at given index\n");
    return;
  }
  FILE *f = fopen(path, "r");
  if (f == NULL)
  {
    printf("Error opening file\n");
    return;
  }
  char tempfile_name[50];
  if(sprintf(tempfile_name, "%s%u/%s%s", "./Users/", index, "tempfile", ".txt") < 0)
    {
        printf("error using sprintf\n");
        return;
    }
  
  FILE *tempfile = fopen(tempfile_name, "w");
  if(tempfile == NULL)
  {
    printf("Error creating a temporary file!\n");
    fclose(f);
    return;
  }
  char *temp = calloc(MAX, sizeof(char));
  unsigned i = 0;
  while(fgets(temp, MAX, f) != NULL)
  {
    if(temp[0] == '\n' || temp[0] == '\0')
      continue;

    if(i != 0)
      fputs(temp, tempfile);
    else
      fputs(new_data, tempfile);
    i++;    
  }
  fclose(f);
  fclose(tempfile);
  remove(path);
  rename(tempfile_name, path);
  char msg[MAX];
  if(sprintf(msg, "%s %u\n", "Updated account number", index) < 0)
    {
      printf("error using sprintf\n");
      return;
    }
  log_msg(msg);
  free(temp);
}

int login(char username[], char password[], unsigned count)
{
  /*
  function for user login: checks if the given userame and password exists, if they dont it returns -1
  username: char array with given username
  password: char array with given password
  count: number of acccounts 
  */
  char *temp = malloc(MAX * sizeof(char)), file_name[50], file_password[50], *p;
  int k = 0;
  char path[50];
  for(unsigned k=0; k < count; k++)
  {
    if (sprintf(path, "%s%u/%u%s", "./Users/", k, k, ".txt") < 0) 
    {
      printf("Error using sprintf\n");
      free(temp);
      return -1;
    }
    FILE *f = fopen(path, "r");
    if (f == NULL) 
    {
      printf("Error opening file\n");
      free(temp);
      return -1;
    }
      
      if(fgets(temp, MAX, f) != NULL)
      {
        p = strtok(temp, " \n");
        if(p != NULL)
          {
            strcpy(file_name, p);
            p = strtok(NULL, " \n");
            if(p != NULL)
              {
                strcpy(file_password, p);
                if(strcmp(username, file_name) == 0 && strcmp(password, file_password) == 0)
                  {
                    fclose(f);
                    free(temp);
                    return k;
                  }
              }
          }
      }
    fclose(f);
  }
  free(temp);
  return -1;
}

int avalabile_credentials(char username[], char password[], unsigned count)
{
  /*
  function to check if given credentials are currently avalabile: checks if either the given userame or password exists, if they dont it returns -1
  username: char array with given username
  password: char array with given password
  count: number of acccounts 
  */
  char *temp = malloc(MAX * sizeof(char)), file_name[50], file_password[50], *p;
  int k = 0;
  char path[50];
  for(unsigned k=0; k < count; k++)
  {
    if (sprintf(path, "%s%u/%u%s", "./Users/", k, k, ".txt") < 0) 
    {
      printf("Error using sprintf\n");
      free(temp);
      return -1;
    }
    FILE *f = fopen(path, "r");
    if (f == NULL) 
    {
      printf("Error opening file\n");
      free(temp);
      return -1;
    }
      
      if(fgets(temp, MAX, f) != NULL)
      {
        p = strtok(temp, " \n");
        if(p != NULL)
          {
            strcpy(file_name, p);
            p = strtok(NULL, " \n");
            if(p != NULL)
              {
                strcpy(file_password, p);
                if(strcmp(username, file_name) == 0 || strcmp(password, file_password) == 0)
                  {
                    fclose(f);
                    free(temp);
                    return k;
                  }
              }
          }
      }
    fclose(f);
  }
  free(temp);
  return -1;
}

int find_name(char username[], unsigned count)
{
  /*
  function for user login: checks if the given userame and password exists, if they dont it returns -1
  username: char array with given username
  password: char array with given password
  count: number of acccounts 
  */
  char *temp = malloc(MAX * sizeof(char)), file_name[50],*p;
  int k = 0;
  char path[50];
  username[strcspn(username, "\n")] = '\0';
  for(unsigned k=0; k < count; k++)
  {
    if (sprintf(path, "%s%u/%u%s", "./Users/", k, k, ".txt") < 0) 
    {
      printf("Error using sprintf\n");
      free(temp);
      return -1;
    }
    FILE *f = fopen(path, "r");
    if (f == NULL) 
    {
      printf("Error opening file\n");
      free(temp);
      return -1;
    }
      if(fgets(temp, MAX, f) != NULL)
      {
        p = strtok(temp, " \n");
        if(p != NULL)
          {
            strcpy(file_name, p);
            p = strtok(NULL, " \n");
            if(p != NULL)
              { 
                if(strcmp(username, file_name) == 0)
                  {
                    fclose(f);
                    free(temp);
                    return k;
                  }
              }
          }
      }
    fclose(f);
  }
  free(temp);
  return -1;
}

void write_entity_to_file(char s[], char name[], char contact_info[], char desc[])
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

    fprintf(f, "%s|", name);
    fprintf(f, "%s|", contact_info);
    fprintf(f, "%s\n ", desc);
    fclose(f);
}
