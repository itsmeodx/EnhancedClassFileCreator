#!/bin/bash

# Clone the repository
clone() {
	git clone https://github.com/itsmeodx/ClassFileCreator.git 2> /dev/null > /dev/null
}

# Change to the temporary directory
cd /tmp

# Check if the repository is already cloned or not
echo "Cloning the repository..."
clone  || (rm -rf ClassFileCreator && clone)

# Change to the repository directory
cd ClassFileCreator

# Make the executable
make
