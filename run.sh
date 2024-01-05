#!/bin/bash

rm -r bin
make && ./bin/test > logs/outputs/output.txt 2> logs/errors/error.txt
make valgrind > logs/outputs/output_valgrind.txt 2> logs/errors/error_valgrind.txt
make sanitize && ./bin/test > logs/outputs/output_sanitizer.txt 2> logs/errors/error_sanitizer.txt

#rm -r bin
#make && ./bin/test
#make valgrind 
#make sanitize && ./bin/test 