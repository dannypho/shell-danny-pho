# Unix Shell

This project is a custom implementation of a Unix-like shell, providing a command-line interface for executing commands interactively or in batch mode. The shell replicates fundamental features of traditional Unix shells, allowing users to run programs, handle redirection, and execute built-in commands such as `cd` and `exit`.

## Overview

The Unix Shell operates by repeatedly displaying a prompt (`msh> `), accepting user input, and executing the specified commands. It is capable of parsing arguments, creating processes, and managing command execution, all while providing essential error handling and built-in functionality. The shell supports both interactive user input and batch processing via input files.

## Features

- **Interactive Mode**: The shell runs interactively when started without arguments, allowing users to enter commands directly.
- **Batch Mode**: Users can provide a file containing commands as an argument to execute them in sequence without manual input.
- **Built-in Commands**:
  - `exit` or `quit`: Terminates the shell.
  - `cd`: Changes the current directory.
- **Redirection**: Supports redirecting output and error streams to a file using the `>` symbol.
- **Command Execution**: Executes commands from the specified directories (`/bin`, `/usr/bin`, `/usr/local/bin`, and the current directory `./`).

## Usage

### Build
To build the shell, use:
```
make
```

### Running the Shell
Launch the shell in interactive mode:
```
./msh
```
To run the shell in batch mode with a file of commands:
```
./msh batch.txt
```

### Running Tests
```
./test-msh.sh
```

## Technical Details

The shell is implemented as an interactive loop: 
1. Prompts the user for input.
2. Parses the command and its arguments.
3. Executes the command, either as a built-in or an external program.
4. Handles errors gracefully and provides feedback to the user.

### Built-in Commands
* exit: Terimantes the shell
* cd<path>: Changes the working directory to <path>. If the path is invalid, an error message is displayed.
* quit: An alternative to exit, also terminates the shell.

### Redirection
Output redirection is implemented with the > operator. For example:
```
ls -la > output.txt
```
This writes both standard output and error output to the specified file.

### Error Handling
Errors are handled consistently, displaying a single message:
```
An error has occurred
```

### Path Handling
Commands are searched for in the following directories:

1. /bin
2. /usr/bin
3. /usr/local/bin
4. ./ (current directory)

### Notes
This project demonstrates foundational concepts in Unix programming and process management. It can be extended to include advanced shell features such as pipes, additional built-in commands, and environment variable support.


