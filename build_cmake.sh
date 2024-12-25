#!/bin/bash


BOLD='\033[1m'
BRIGHT_WHITE='\033[1;37m'
RESET='\033[0m' 

BUILD_DIR="build"
BIN_DIR="bin"
BIN_NAME='stegolyx'


configure() {
    echo -e "${BOLD}${BRIGHT_WHITE}Create build directories and config cmake${RESET}"
    mkdir -p ${BUILD_DIR}
    cmake -S . -B ${BUILD_DIR} -DCMAKE_VERBOSE_MAKEFILE=ON
}

build() {
    echo -e "${BOLD}${BRIGHT_WHITE}Building project...${RESET}"
    cmake --build ${BUILD_DIR} --target all -- -j$(nproc) --debug
}


./clean_cmake.sh

configure
build

./test_lsb.sh

echo ""
echo -e "${BOLD}${BRIGHT_WHITE}See test results and check build success ... bin should be in $BIN_DIR e.g ${BIN_DIR}/${BIN_NAME}${RESET}"
echo ""
echo -e "${BOLD}${BRIGHT_WHITE}E.g:${RESET}"
echo ""
echo -e "${BOLD}${BRIGHT_WHITE}${BIN_DIR}/${BIN_NAME} -h${RESET}"
