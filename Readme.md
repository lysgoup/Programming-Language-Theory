# PLT HW3_Task2: Implment FAE in  your favoriate langauge

You can interprete your FAE syntax expression or convert the concrete syntax into FAE abstract syntax.

## How to run?
1. Build the program
- Use Makefile
```bash
$ make
```
This command will make a execution file named 'run'.

<br>
There can be several warnings about initializing non-static data member. However You can ignore it.
Please use c++11 extension.

<br>
If you want to delete the execution file, use the next command.

```bash
$ make clean
```

<br>
- Use command

```bash
$ g++ -o run Main.cpp
```

2. Execute the program
- Use next command

```bash
$ ./run <Concrete syntax>
```
If you need to use bracket, you can use only 'Brace' : { }<br>
For example
```bash
$ ./run {{fun {x} {+ x 1}} 5}
```

If you want only parsing your syntax without interpreting, use '-p' option.
<br>
For example

```bash
$ ./run -p {{fun {x} {+ x 1}} 5}
```