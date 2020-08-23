# myBrainFk
This is my own implementation of the [Brainfuck](https://zh.wikipedia.org/wiki/Brainfuck) language.   
Maybe it is not good enough right now, I hope you could find something fun in this programme language.

## Build And Run
`git clone` this repo and run `make` to build it.

Show Options:
```bash
kai@kai:~/myBrainFk$ ./bin/bf --help
Usage: bf [option] | file
A Sample BrainFuck interpreter.

-h --help      show usage message .
-e --exec      execute BrainFuck code .
   --version   output version information and exit .
```

Run example:
```bash
kai@kai:~/myBrainFk$ ./bin/bf example/hello.bf
Hello World!
kai@kai:~/myBrainFk$ cat example/hello.bf
++++++++++[>+++++++>++++++++++>+++>+<<<<-]
>++.>+.+++++++..+++.>++.<<+++++++++++++++.
>.+++.------.--------.>+.>.
kai@kai:~/myBrainFk$
```

## Other Funny code
https://github.com/moky/BrainFuck

## License
MIT license (&copy; 2020 Kerwin Sun)
