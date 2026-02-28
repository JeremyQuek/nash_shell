#!/bin/bash

# --- CONFIGURATION ---
REQUIRED_CPP_VERSION=17
MIN_NODE_VERSION=14

# Colors for readability
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

echo "🔍 Starting system compatibility check for NASH..."

# 1. Architecture/OS Check (Enforce macOS)
OS_TYPE=$(uname)
if [ "$OS_TYPE" != "Darwin" ]; then
    echo -e "${RED}❌ Error: This shell is only supported on macOS (Darwin). Current OS: $OS_TYPE${NC}"
    exit 1
fi
echo -e "${GREEN}✅ OS: macOS detected.${NC}"

# 2. Compiler Check (g++/clang++)
if ! command -v g++ &>/dev/null; then
    echo -e "${RED}❌ Error: g++ (Compiler) not found. Please install Xcode Command Line Tools.${NC}"
    exit 1
fi

# 3. C++17 Support Check
# Try to compile a tiny snippet to see if the compiler likes C++17
echo "int main(){}" | g++ -std=c++17 -x c++ -o /dev/null - &>/dev/null
if [ $? -ne 0 ]; then
    echo -e "${RED}❌ Error: Your compiler does not support C++17 standard.${NC}"
    exit 1
fi
echo -e "${GREEN}✅ Compiler: g++ supports C++17.${NC}"

# 4. Node.js & npm Check
if ! command -v node &>/dev/null; then
    echo -e "${RED}❌ Error: Node.js is not installed. (Required for Electron)${NC}"
    exit 1
fi

# 5. Architecture Check (Intel vs Apple Silicon)
# Useful if your C++ code has specific assembly or optimization flags
ARCH=$(uname -m)
echo -e "ℹ️  Architecture detected: $ARCH"

# 6. Check for Make
if ! command -v make &>/dev/null; then
    echo -e "${RED}❌ Error: 'make' utility not found.${NC}"
    exit 1
fi

echo -e "\n${GREEN}🚀 All systems go! You are ready to run shell.sh.${NC}"