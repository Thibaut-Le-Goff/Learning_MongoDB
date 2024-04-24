#!/bin/bash

bin_path="bin"
logs_path="logs"


if [ -e "$bin_path" ]; then
    rm -r bin
else
    echo "There is no binary files."
fi

if [ -e "$logs_path" ]; then
    echo "There is already the logs folders."
else
    mkdir logs
    mkdir logs/outputs
    mkdir logs/errors    
fi

make && ./bin/test > logs/outputs/output.txt 2> logs/errors/error.txt
make valgrind > logs/outputs/output_valgrind.txt 2> logs/errors/error_valgrind.txt
make sanitize && ./bin/test > logs/outputs/output_sanitizer.txt 2> logs/errors/error_sanitizer.txt

#rm -r bin
#make && ./bin/test
#make valgrind 
#make sanitize && ./bin/test 
