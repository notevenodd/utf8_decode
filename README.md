# Flexible UTF-8 decoder

## What 

a very simple UTF-8 decoding facility in C :

- API is a **single C function**
- available as a **single header**

## To use in your project

This works like the typical C single-file library:

1. get `utf8_decode.h` and add to your project files.
2. in exactly one C file, also do: `#define UTF8_DECODE_IMPLEMENTATION`

## Credits / License

Idea, explanation in `index.html` and real code in `utf8_decode.h` is by: Bjoern Hoehrmann

I merely packaged his code as a single-file library and created a GitHub repo to facilitate re-use in my C projects.

License is MIT.

## Read more / How to Use

The idea is to implement a UTF-8 decoder as a Deterministic Finite Automaton (DFA).

How to use, see: http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ (also included here as `index.html`)
