#!/bin/bash

# clone the repository
cd /tmp && git clone git@github.com:itsmeodx/ClassFileCreator.git 2>&1 > /dev/null
cd ClassFileCreator

# Make the executable
make 2>&1 > /dev/null
