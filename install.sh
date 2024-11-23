#!/bin/bash

# Change to the temporary directory
cd /tmp

# Clone the repository or update it
echo "Cloning the repository..."
[ -d ClassFileCreator ] && rm -rf ClassFileCreator
git clone -q https://github.com/itsmeodx/ClassFileCreator.git

# Change to the repository directory
cd ClassFileCreator

# Make the executable
make
