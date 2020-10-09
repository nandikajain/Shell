# Shell
### Execute
Make command: ```make``` 

### Commands Implemented:

#### <u>cd</u>

cd is used to change the current working directory to a specified folder.

**Flags Implemented :-**

- <u>-P</u>
  This flag uses physical directory structure without following symbolic links.
- <u>-e</u>
  When this flag is used, if the -P option is supplied, and the current working directory cannot be determined successfully, exit with a non-zero status.

**Usage :-**

To use relative or absolute paths, the command is used as it is with the flags required as ```cd [-P [-e]] path``` . You can also go directly to the shell home using the command as ```cd [-P [-e]]```. The path can also be relative to shell home using the ~ prefix.

#### <u>echo</u>

Displays a line of text on the terminal.

**Flags Implemented :-**

- <u>-n</u>
  This flag doesn’t append a new line.
- <u>-e</u>
  This flag enables interpretation of backslash escapes. 

**Usage :-**

The command can be called as ```echo [-e] [-n] text ```.

#### <u>history</u>

Maintains a history of all the commands used across all the sessions of the shell.

**Flags Implemented :-**

- <u>-c</u>
  This flag clears the history list by deleting all of the entries.
- <u>-d</u>
  This flag deletes the history entry at position *OFFSET*. Negative offsets count back from the end of the history list. 

**Usage :-**

To view the entire history, use command ```history```. To view the last OFFSET commands, use ```history OFFSET```. To delete the command at OFFSET, use ```history -d OFFSET```. To clear the entire history, use ```history -c```.

#### <u>pwd</u>

This command gives the present working directory of the shell.

**Flags Implemented :-**

- <u>-L</u>
  This flag uses PWD from the environment, even if it contains symlinks.
- <u>-P</u>
  This flag avoids all symlinks.

**Usage :-**

To use this command, simply run it as ```pwd [-P] [-L]```.

#### <u>exit</u>

This command exits the user from the shell.

**Usage :-**

Use as ```exit```.

#### <u>date</u>

Using this command, the user can print the system date and time.

**Flags Implemented :-**

- <u>-u</u>
  Prints the coordinated universal time (UTC).
- <u>-R</u>
  Outputs date and time in RFC 5322 format.

**Usage :-**

The command can be called as ```date [-u] [-R]```.

#### <u>cat</u>

This command concatenates file and prints on the standard output.

**Flags Implemented :-**

- <u>-E</u>
  This flag displays $ at end of each line.
- <u>-n</u>
  This flag numbers all output lines.

**Usage :-**

The command can be called as ```cat [-E] [-n] file1 file2 … ``` to concatenate files in those order.

#### <u>ls</u>

This command is used to list directory contents.

**Flags Implemented :-**

- <u>-a</u>
  Using this flag, the command does not ignore entries starting with .
- <u>-A</u>
  Using this flag, the command does not list implied . and ..

**Usage :-**

The command can be called as ```ls [-a] [-A] path1 path2 ...``` to list the contents of files or directory in that order.

#### <u>rm</u>

This command can be used to remove files or directories.

**Flags Implemented :-**

- <u>-i</u>
  This flag prompts before every removal.
- <u>-v</u>
  This flag explains what is being done.

**Usage :-**

The command can be called as ```rm [-i] [-v] file1 file2 ...```.

#### <u>mkdir</u>

Using this command, we can make directories.

**Flags Implemented :-**

- <u>-p</u>
  This flag creates all the directories recursively.
- <u>-v</u>
  This flag prints a message for each created directory

**Usage :-**

The command can be called as ```mkdir [-p] [-v] file1 file2 ...```.