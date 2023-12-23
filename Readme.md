# [PLT] RBMRLFAE: A language supporting Recursion/Mutable Data Structure/Variable/Laziness

You can interpret your FAE syntax expression or convert the concrete syntax into FAE abstract syntax.

## How to build?
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

- Use command

```bash
$ g++ -o run Main.cpp
```

## How to run?
- Use next command

```bash
$ ./run <Concrete syntax>
```
If you need to use bracket, you can use only 'Brace' : { }<br>
For example
```bash
$ ./run "{{fun {x} {+ x 1}} 5}"
```

If you want only parsing your syntax without interpreting, use '-p' option.
<br>
For example

```bash
$ ./run -p "{{fun {x} {+ x 1}} 5}"
```
If you have recursion function in your concrete code and want to see a parsing result after desugaring, use '-d' option with '-p'option.<br>
You don't need to use '-d' option when you interpret your code. Because it can check whether there is a recursion function in the code automatically and desugar the concrete code before interpreting.<br>
```bash
$ ./run -p -d "{with {fac {fun {n}
                        {if {= n 0}
                                1
                        {* n {fac {- n 1}}}}}}
                        {fac 5}}"
```


## Test cases
```bash
./run "{+ 1 1}"
./run "{with {b 7} b}"
./run "{{fun {b} b} 7}"
./run "{newbox 7}"
./run "{with {b 7} b}"
./run "{with {b {newbox 7}} {setbox b 10}}"
./run "{with {b {newbox 7}} {openbox b}}"
./run "{with {b {newbox 7}} {seqn {setbox b 10} {openbox b}}}"
./run "{with {a 3} {setvar a 5}}"
./run "{with {a 3} {seqn {{fun {x} {setvar x 5}} a} a}}"
./run "{with {a {newbox 3}} {seqn {{fun {x} {setbox x 5}} a} {openbox a}}}"
```
Recursion
```bash
./run "{with {mk-rec {fun {body-proc}
          {with {fX {fun {fY}
              {with {f {fun {x}
                   {{fY fY} x}}}
                    {body-proc f}}}}
                      {fX fX}}}}
          {with {fac {mk-rec
                {fun {fac}
                  {fun {n} {if {= n 0}
                               1
                    {* n {fac {- n 1}}}}}}}}
                  {fac 10}}}"

./run "{with {fac {fun {n}
                            {if {= n 0}
                                   1
                            {* n {fac {- n 1}}}}}}
      {fac 5}}"
```

SwapCases
```bash
./run "{with {swap {fun {x} {fun {y}with {z x} {seqn {setvar x y} {setvar y z}}}}}} {with {a 10} {with {b 20} {seqn {{swap a} b} a}}}} (mtSub) (mtSto)"

./run "{with {swap {fun {x} {fun {y} {seqn x {with {z x} {seqn z {seqn {setvar x y} {setvar y z}}}}}}}} {with {a 10} {with {b 20} {seqn {{swap a} b} a}}}} (mtSub) (mtSto)"

./run "{with {swap {fun {x} {fun {y} {with {z x} {seqn {setvar x y} {setvar y z}}}}}} {with {a 10} {with {b 20} {seqn {{swap a} b} b}}}} (mtSub) (mtSto)"

./run "{with {swap {fun {x} {fun {y} {seqn x {with {z x} {seqn z {seqn {setvar x y} {setvar y z}}}}}}}} {with {a 10} {with {b 20} {seqn {{swap a} b} b}}}} (mtSub) (mtSto)"

./run "{with {swap {refun {x} {refun {y} {with {z x} {seqn {setvar x y} {setvar y z}}}}}} {with {a 10} {with {b 20} {seqn {{swap a} b} a}}}} (mtSub) (mtSto)"

./run "{with {swap {refun {x} {refun {y} {with {z x} {seqn z {seqn {setvar x y} {setvar y z}}}}}}} {with {a 10} {with {b 20} {seqn {{swap a} b} a}}}} (mtSub) (mtSto)"

./run "{with { {refun {x} {refun {y} {with {z x} {seqn {setvar x y} {setvar y z}}}}}} {with {a 10} {with {b 20} {seqn {{swap a} b} b}}}} (mtSub) (mtSto)"

./run "{with {swap {refun {x} {refun {y} {with {z x} {seqn z {seqn {setvar x y} {setvar y z}}}}}}} {with {a 10} {with {b 20} {seqn {{swap a} b} b}}}} (mtSub) (mtSto)"
```