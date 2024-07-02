// Include the shell header file for necessary constants and function declarations
#include "shell.h"
#include <stdbool.h>
// Function to read a command from the user input
void read_command(char **cmd)
{
  // Define a character array to store the command line input
  char line[MAX_LINE];
  // Initialize count to keep track of the number of characters read
  int count = 0, i = 0;
  // Array to hold pointers to the parsed command arguments
  char *array[MAX_ARGS], *command_token;

  // Infinite loop to read characters until a newline or maximum line length is reached
  for (;;)
  {
    // Read a single character from standard input
    int current_char = fgetc(stdin);
    // Store the character in the line array and increment count
    line[count++] = (char)current_char;
    // If a newline character is encountered, break out of the loop
    if (current_char == '\n')
      break;
    // If the command exceeds the maximum length, print an error and exit
    if (count >= MAX_LINE)
    {
      printf("Command is too long, unable to process\n");
      exit(1);
    }
  }
  // Null-terminate the command line string
  line[count] = '\0';

  // If only the newline character was entered, return without processing
  if (count == 1)
    return;

  // Use strtok to parse the first token (word) of the command
  command_token = strtok(line, " \n");

  // Continue parsing the line into words and store them in the array
  while (command_token != NULL)
  {
    array[i++] = strdup(command_token);  // Duplicate the token and store it
    command_token = strtok(NULL, " \n"); // Get the next token
  }

  // Copy the parsed command and its parameters to the cmd array
  for (int j = 0; j < i; j++)
  {
    cmd[j] = array[j];
  }
  // Null-terminate the cmd array to mark the end of arguments
  cmd[i] = NULL;
}

// Function to display the shell prompt
void type_prompt()
{
  // Use a static variable to check if this is the first call to the function
//  static int first_time = 1;
//  if (first_time)
//  {
//    // Clear the screen on the first call
//#ifdef _WIN32
//    system("cls"); // Windows command to clear screen
//#else
//    system("clear"); // UNIX/Linux command to clear screen
//#endif
//    first_time = 0;
//  }
//  fflush(stdout); // Flush the output buffer
//
  printf("$$ ");  // Print the shell prompt
}

int shell_cd(char **args)
{
  char cwd[1024];

  if (args[1] == NULL)
  {
    printf("Expected \"cd <directory>\"\n");
  }
  else
  {
    if (chdir(args[1]) != 0)
    {
      switch (errno)
      {
        case ENOENT:
          printf("Directory does not exist: %s\n", args[1]);
          break;
        case ENOTDIR:
          printf("Not a directory: %s\n", args[1]);
          break;
        default:
          printf("There is an invalid input: %s", args[1]);
      }
    }
    else
    {
      if (getcwd(cwd, sizeof(cwd)) != NULL)
      {
        printf("Current working directory: %s\n", cwd);
      }
      else
      {
        printf("Failed to get current working directory");
      }
    }
  }
  return 1;
}

int shell_help(char **args)
{
  printf("CSEShell Interface\nUsage: command arguments\nThe following commands are implemented within the shell:\n");
  for (int i = 0; i < num_builtin_functions(); i++)
  {
    printf("  %s\n", builtin_commands[i]);
  }
  return 1;
}

int shell_exit(char **args)
{
  return 0;
}

int shell_usage(char **args)
{
  if (args[1] == NULL)
  {
    printf("Command not given. Type usage <command>.\n");
  }
  else if (strcmp(args[1], "cd") == 0)
  {
    printf("Type: cd directory_name to change the current working directory of the shell\n");
  }
  else if (strcmp(args[1], "help") == 0)
  {
    printf("Type: help for supported commands\n");
  }
  else if (strcmp(args[1], "exit") == 0)
  {
    printf("Type: exit to terminate the shell gracefully\n");
  }
  else if (strcmp(args[1], "usage") == 0)
  {
    printf("Type: usage cd/help/exit/env/setenv/unsetenv\n");
  }
  else if (strcmp(args[1], "env") == 0)
  {
    printf("Type: env to list all registered env variables\n");
  }
  else if (strcmp(args[1], "setenv") == 0)
  {
    printf("Type: setenv ENV=VALUE to set a new env variable\n");
  }
  else if (strcmp(args[1], "unsetenv") == 0)
  {
    printf("Type: unsetenv ENV to remove this env from the list of env variables\n");
  }
  else
  {
    printf("The command you gave: %s, is not part of CSEShell's builtin command\n", args[1]);
  }
  return 1;
}

int list_env(char **args)
{
  extern char **environ;
  char **env = environ; // Pointer to the array of environment strings

  // Loop until NULL pointer is encountered
  while (*env)
  {
    printf("%s\n", *env); // Print the current environment variable
    env++; // Move to the next environment variable
  }

  return 1;
}

int set_env_var(char **args)
{
  putenv(args[1]);
  return 1;
}

int unset_env_var(char **args)
{
  unsetenv(args[1]);
  return 1;
}

// The main function where the shell's execution begins
int main(void)
{
  // Define an array to hold the command and its arguments
  char *cmd[MAX_ARGS];
  int status;
  int child_exit_status;
  pid_t pid;
  char wd[1024];
  int inbuilt_status;
  int inbuilt_flag;
  int init_path;
  char full_path[PATH_MAX];
  char *rc_cmd[MAX_ARGS][MAX_ARGS];
  int num_rc_cmd = 0;

  //run .rc
  FILE *fptr;

  // Open rc file in read mode
  fptr = fopen(".cseshellrc", "r");
  if (fptr == NULL) {
    perror("Failed to open file");
    return EXIT_FAILURE;
  }

  char line[MAX_LINE];
  while (fgets(line, sizeof(line), fptr)) {

    // Remove newline character
    //line[strcspn(line, "\n")] = 0;

    char *newline = strchr(line, '\n');
    if (newline != NULL) {
      *newline = '\0';
    }

    // Check for "PATH"
    if (strncmp(line, "PATH", 4) == 0) {
      char *path = line + 5;
      setenv("PATH", path, 1);
      continue;
    }
    // Store commands
    else {
      int i = 0;
      char *cmd[MAX_ARGS], *cmd_token;

      // Use strtok to parse the first token (word) of the command
      cmd_token = strtok(line, " ");

      // Continue parsing the line into words and store them in the array
      while (cmd_token != NULL) {
        cmd[i++] = strdup(cmd_token);  // Duplicate the token and store it
        cmd_token = strtok(NULL, " "); // Get the next token
      }

      // Null-terminate the cmd array to mark the end of arguments
      cmd[i] = NULL;

      // Store commands in rc_cmd
      for (int j = 0; j < i; j++) {
        rc_cmd[num_rc_cmd][j] = cmd[j];
      }
      rc_cmd[num_rc_cmd][i] = NULL;
      num_rc_cmd++;
    }
  }
  fclose(fptr);

  if (init_path == 0)
  {
    init_path = 1;
    // Formulate the full path of the command to be executed
    if (getcwd(wd, sizeof(wd)) != NULL)
    {
    }
    else
    {
      printf("Failed to get current working directory.");
      exit(1);
    }
  }

  // Execute .cseshellrc
  for (int i = 0; i < num_rc_cmd; i++) {
    // Copy command into cmd
    for (int j = 0; rc_cmd[i][j] != NULL; j++) {
      cmd[j] = rc_cmd[i][j];
    }
    // Null-terminate the cmd array to mark the end of arguments
    cmd[num_rc_cmd] = NULL;

    // skip execution if command is empty
    if (cmd[0] == NULL)
      continue;

    inbuilt_status = 2;
    // Loop through our command list and check if the commands exist in the builtin command list
    for (int command_index = 0; command_index < num_builtin_functions(); command_index++)
    {
        if (strcmp(cmd[0], builtin_commands[command_index]) == 0) // Assume args[0] contains the first word of the command
        {
          // We will create new process to run the function with the specific command except for builtin commands.
          // These have to be done by the shell process.
          inbuilt_status = (*builtin_command_func[command_index])(cmd);
          break;
        }
    }
    if (inbuilt_status == 0)
    {
      return 0;
    }
    else if (inbuilt_status == 1)
    {
      // clear the cmd array
      memset(cmd, '\0', sizeof(cmd));
      continue;
    }
    pid = fork();

    if (pid < 0)
    {
      printf("Fork failed.");
      exit(1);
    }
    // child process
    else if (pid == 0)
    {
      execvp(cmd[0], cmd);

      // if execvp returns, command execution has failed
      printf("Command %s not found\n", cmd[0]);
      exit(0);

      // Free the allocated memory for the command arguments before exiting
      for (int i = 0; cmd[i] != NULL; i++)
      {
        free(cmd[i]);
      }
    }

    // parent process
    else
    {
      waitpid(pid, &status, WUNTRACED);
      // if child terminates properly
      if (WIFEXITED(status))
      {
        child_exit_status = WEXITSTATUS(status);
      }
      // checks child_exit_status and do something about it
      if (child_exit_status != 0)
      {
        printf("Command %s failed with exit code %d\n", cmd[0], child_exit_status);
      }
    }

    // clear the cmd array
    memset(cmd, '\0', sizeof(cmd));
  }

  for (;;)
  {
    type_prompt();     // Display the prompt
    read_command(cmd); // Read a command from the user

    // skip execution if command is empty
    if (cmd[0] == NULL)
      continue;

    inbuilt_status = 2;
    // Loop through our command list and check if the commands exist in the builtin command list
    for (int command_index = 0; command_index < num_builtin_functions(); command_index++)
    {
        if (strcmp(cmd[0], builtin_commands[command_index]) == 0) // Assume args[0] contains the first word of the command
        {
          // We will create new process to run the function with the specific command except for builtin commands.
          // These have to be done by the shell process.
          inbuilt_status = (*builtin_command_func[command_index])(cmd);
          break;
        }
    }
    if (inbuilt_status == 0)
    {
      return 0;
    }
    else if (inbuilt_status == 1)
    {
      // clear the cmd array
      memset(cmd, '\0', sizeof(cmd));
      continue;
    }

    pid = fork();

    if (pid < 0)
    {
      printf("Fork failed.");
      exit(1);
    }
    // child process
    else if (pid == 0)
    {
      snprintf(full_path, sizeof(full_path), "%s/bin/%s", wd, cmd[0]);

      execv(full_path, cmd);

      // if execv returns, command execution has failed
      printf("Command %s not found\n", cmd[0]);
      exit(0);

      // Free the allocated memory for the command arguments before exiting
      for (int i = 0; cmd[i] != NULL; i++)
      {
        free(cmd[i]);
      }
    }

    // parent process
    else
    {
      waitpid(pid, &status, WUNTRACED);
      // if child terminates properly
      if (WIFEXITED(status))
      {
        child_exit_status = WEXITSTATUS(status);
      }
      // checks child_exit_status and do something about it
      if (child_exit_status != 0)
      {
        printf("Command %s failed with exit code %d\n", cmd[0], child_exit_status);
      }
    }

    // clear the cmd array
    memset(cmd, '\0', sizeof(cmd));

  }

  return 0;
}
