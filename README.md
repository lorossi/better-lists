# Better Lists

A simple implementation of doubly linked lists in C.

I made this mostly because I was bored, but I went on because I took this as a challenge. When I first studied C at university (it was my first year!), I couldn't get linked lists to work. It's a very satisfying goal to reach, about 5 years (and a degree) later.

Each function is well commented, and in this readme I have included some examples. The code is fully tested against memory leaks using Valgrind. Inside the `tests.c` file I have implemented a simple test suite using the `assert.h` library. It's not pretty, but it works.

[Check the documentation here](https://lorossi.github.io/better-lists/html/) or [take a look at some examples here](EXAMPLES.md).

## Data types

By default, lists store integers. In order to store different types of data, the final user must define, before importing, the constants:

- `CHARLIST` to store `char` inside the lists
- `STRINGLIST` to store `char*` inside the lists
- `FLOATLIST` to store `float` inside the lists
- `DOUBLE` to store `double` inside the lists

Furthermore, it the user wants to store another type of data, the constant `CUSTOMTYPE` must be defined along the type that will be stored into the list.
