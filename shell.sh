#!/bin/bash

# Default: do not force recompile
RECOMPILE=false

#Parse flags
while getopts "c" opt; do
  case $opt in
    c)
      RECOMPILE=true
      ;;
    *)
      echo "Usage: $0 [-c]"
      exit 1
      ;;
  esac
done

# Pre-check dependencies
if ! command -v npm &>/dev/null; then
    echo "Error: npm is not installed."
    exit 1
fi

if ! command -v g++ &>/dev/null; then
    echo "Error: g++ is not installed."
    exit 1
fi

# 2. Handle Compilation
cd ./src/shell || exit

# Delete file is force recompile
if [ "$RECOMPILE" = true ] && [ -f "./shell" ]; then
    echo "Force flag detected. Cleaning old binary..."
    rm ./shell
fi

# Compile if the file is missing (first time or forced recompile)
if [ ! -f "./shell" ]; then
    echo "Compiling shell binary..."
    make
    echo "Successfully compiled."
else
    echo "Shell binary already exists. Skipping compilation (use -c to force)."
fi

# 3. npm i && start
cd ../pseudo_terminal || exit
npm i -s
echo "Spawning NASH terminal..."
npm start