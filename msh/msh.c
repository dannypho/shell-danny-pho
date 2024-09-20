// The MIT License (MIT)
// 
// Copyright (c) 2024 Trevor Bakker 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <dirent.h>

#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
                                // so we need to define what delimits our tokens.
                                // In this case  white space
                                // will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255    // The maximum command-line size

#define MAX_NUM_ARGUMENTS 32

void print_error_message()
{
  char error_message[30] = "An error has occurred\n";
  write(STDERR_FILENO, error_message, strlen(error_message)); 
}


// Checks if the shell command is valid by appending it to the four required directories
// and passing it through access()
// Returns the path (string) if access is successful
// Returns NULL if access() is not succcessful or if malloc() fails
char *search_directories(char shell_commmand[])
{
  char path_one[] = "/bin/";
  char path_two[] = "/usr/bin/";
  char path_three[] = "/usr/local/bin/";
  char path_four[] = "./";

  char *full_path_one = malloc((strlen(path_one) + strlen(shell_commmand) + 1) * sizeof(char));
  if(full_path_one == NULL)
  {
    return NULL;
  }
  strcpy(full_path_one, path_one);
  strcat(full_path_one, shell_commmand);

  char *full_path_two = malloc((strlen(path_two) + strlen(shell_commmand) + 1) * sizeof(char));
  if(full_path_two == NULL)
  {
    free(full_path_one);
    return NULL;
  }
  strcpy(full_path_two, path_two);
  strcat(full_path_two, shell_commmand);

  char *full_path_three = malloc((strlen(path_three) + strlen(shell_commmand) + 1) * sizeof(char));
  if(full_path_three == NULL)
  {
    free(full_path_one);
    free(full_path_two);
    return NULL;
  }
  strcpy(full_path_three, path_three);
  strcat(full_path_three, shell_commmand);

  char *full_path_four = malloc((strlen(path_four) + strlen(shell_commmand) + 1) * sizeof(char));
  if(full_path_four == NULL)
  {
    free(full_path_one);
    free(full_path_two);
    free(full_path_three);
    return NULL;
  }
  strcpy(full_path_four, path_four);
  strcat(full_path_four, shell_commmand);

  if(access(full_path_one, X_OK) == 0)
  { 
    free(full_path_two);
    free(full_path_three);
    free(full_path_four);
    return full_path_one;
  }
  else if(access(full_path_two, X_OK)== 0)
  { 
    free(full_path_one);
    free(full_path_three);
    free(full_path_four);
    return full_path_two;
  }
  else if (access(full_path_three, X_OK) == 0)
  { 
    free(full_path_one);
    free(full_path_two);
    free(full_path_four);
    return full_path_three;
  }
  else if(access(full_path_four, X_OK) == 0)
  { 
    free(full_path_one);
    free(full_path_two);
    free(full_path_three);
    return full_path_four;
  }

  free(full_path_one);
  free(full_path_two);
  free(full_path_three);
  free(full_path_four);
  
  return NULL;
}

int main()
{

  char * command_string = (char*) malloc( MAX_COMMAND_SIZE );

  while( 1 )
  {
    // Print out the msh prompt
    printf ("msh> ");

    // Read the command from the command line.  The
    // maximum command that will be read is MAX_COMMAND_SIZE
    // This while command will wait here until the user
    // inputs something.
    while( !fgets (command_string, MAX_COMMAND_SIZE, stdin) );

    /* Parse input */
    char *token[MAX_NUM_ARGUMENTS];

    int token_count = 0;                                 
                                                           
    // Pointer to point to the token
    // parsed by strsep
    char *argument_pointer;                                         
                                                           
    char *working_string  = strdup( command_string );                

    // we are going to move the working_string pointer so
    // keep track of its original value so we can deallocate
    // the correct amount at the end
    
    char *head_ptr = working_string;
    
    // Tokenize the input with whitespace used as the delimiter
    while ( ( (argument_pointer = strsep(&working_string, WHITESPACE ) ) != NULL) &&
              (token_count<MAX_NUM_ARGUMENTS))
    {
      token[token_count] = strndup( argument_pointer, MAX_COMMAND_SIZE );
      if( strlen( token[token_count] ) == 0 )
      {
        token[token_count] = NULL;
      }
        token_count++;
    }

    // Now print the tokenized input as a debug check
    // \TODO Remove this code and replace with your shell functionality

    // int token_index  = 0;
    // for( token_index = 0; token_index < token_count; token_index ++ ) 
    // {
    //   printf("token[%d] = %s\n", token_index, token[token_index] );
    // }

    // Handle built-in commands
    if ((strcmp(token[0], "exit") == 0 && token[1] == NULL) || 
        (strcmp(token[0], "quit") == 0 && token[1] == NULL))
    { 
      exit(0);
    }

    if (strcmp(token[0], "cd") == 0)
    { 
      if (token[1] != NULL && token[2] == NULL)
      {
        chdir(token[1]);
      }
      else
      {
        print_error_message();
      }
    }
    
    // Handle shell commands
    // Avoids forking if built-in commands are entered 
    if (strcmp(token[0], "cd") != 0 && 
        strcmp(token[0], "quit") != 0 && 
        strcmp(token[0], "exit") != 0)
    {
      pid_t pid = fork();
      
      if (pid == -1)
      {
        // When fork() returns -1, an error happened.
        perror("fork failed: ");
        exit( EXIT_FAILURE );
      }
      else if (pid == 0)
      {
        // When fork() returns 0, we are in child process
        char *full_path = search_directories(token[0]);
        if(full_path == NULL)
        { 
          print_error_message();
          exit(0);
        }
        else
        {
          execv(full_path, token);
        }

      }
      else
      {
        // Back in parent process
        int status;
        wait(&status);
      }
    }
    free(head_ptr);
  }
  return 0;
  // e2520ca2-76f3-90d6-0242ac1210022
}