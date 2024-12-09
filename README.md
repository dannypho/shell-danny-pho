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
```bash
make
