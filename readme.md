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

To send an example STATEINFO message (argument specifices the ip/port to send to):

    $ ./bin/encode_example 127.0.0.1 12345

NOTE: you can run the `decode_example` in the background listening to the same port which the `encode_example` sends.  The `decode_example` will spit out a subset of the variables in the `STATEINFO` message.

## Docker
A Docker implementation is available as well. To build:

    $ ./docker/build_image.sh

To run, two scripts are provided:

    $ ./docker/run_listener.sh

and

   $ ./docker/run_sender.sh

