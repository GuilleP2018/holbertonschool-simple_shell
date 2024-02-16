# ʜᴏʟʙᴇʀᴛᴏɴꜱᴄʜᴏᴏʟ-Simple Shell :smiley:

# --PROJECT DESCRIPTION--

:computer: This project aims to recreate a UNIX shell command line interpreter. It will take
user input, either interactively or not, and process the correct command as instructed by the
user.

### 📁 On the project:

- main.c
  -main = verifies whether it is interactive or non interactive and gets
  -exec_command = tokenizes command
  -child_exec = creates child process
  -find_path = attaches path to given input
- assistive_funcs.c
  -trim = removes whitespace before and after string of chars
  -free_array = frees memory in each string of the array
- simple_shell.h
- man_1_simple_shell.3
- README.md
- AUTHORS

### What is the simple shell?

The Simple Shell is a command line interpreter, it recieves user input and breaks it down word
by word. If it finds a command at the start of the input it will compare the input vs the PATH
commands of the kernel to execute a child process which is a copy of the original found function.
It will then execute the command and return 0 to signal success followed by a new line to receive
new input.

Here is a blog we wrote explaining it more thoroughly:

https://www.linkedin.com/pulse/what-happens-shell-stays-well-sorta-h%2525C3%2525A9ctor-v%2525C3%2525A1zquez%3FtrackingId=ARmsQStUThuKza90P9EowA%253D%253D/?trackingId=ARmsQStUThuKza90P9EowA%3D%3D

### Example

- Compiler = gcc -Wall -Werror -Wextra -pedantic -std=gnu89 \*.c -o hsh

vagrant@ubuntu-focal:~/holbertonschool-simple_shell$ ./hsh
$ ls
AUTHORS README.md assistive_funcs.c hsh main.c man_1_simple_shell.3 simple_shell.h
$ /bin/ls
AUTHORS README.md assistive_funcs.c hsh main.c man_1_simple_shell.3 simple_shell.h
$ exit
vagrant@ubuntu-focal:~/holbertonschool-simple_shell$ echo "ls" | ./hsh
AUTHORS README.md assistive_funcs.c hsh main.c man_1_simple_shell.3 simple_shell.h

### Credits:

Hi! I'm Guillermo, one of the developers behind this project! Here is my github if you are interested to learn more about my projects! : https://github.com/GuilleP2018

Hi! I'm Hector J. Vazquez a passionate software developer dedicated to creating technology as a tool for creating inclusivity and accessibility for marginalized communities. Committed to utilizing innovative solutions to break down barriers and empower individuals through technology. [Github](https://github.com/Pepesaur96) | [LinkedIn](https://www.linkedin.com/in/h%C3%A9ctor-v%C3%A1zquez-4b2a34170/)

![post](./wwelcome%20to%20shell.jpeg)
