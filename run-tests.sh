#!/bin/bash

############################################
# TEST RUNNER                              #
#                                          #
# You can run the test runner two ways     #
# 1. No arguments (All tests)              #
# 2. TWO argument (Some tests)             #
#      arg1: test executable file name     #
#      arg2: test pattern                  #
#                                          #
############################################

make

TEST_DIR="test/build"
if [ $? -eq 0 ] ; then
  echo $1;
  # if [ -n $1 ]; then
  #   exec $TEST_DIR/$1 --gtest_filter=$2*
  # else
    for f in $TEST_DIR/*test
    do
        echo "Running test - $f"
        exec $f
    done
  # fi
fi