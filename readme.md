# Decode example
Receives UDP datagrams and attempts to decode specific mavlink messages

NOTE: This code has only been built/tested on OS X and Ubuntu 16.04.

## Prerequisites
  - g++
  - cmake
  - Python 2.7
  - The Python future package

## Building

    $ git submodule update --init --recursive
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make -j8

## Running

    $ ./bin/decode_example

An optional argument will specify the port to listen on, e.g.:

    $ ./bin/decode_example 12345
