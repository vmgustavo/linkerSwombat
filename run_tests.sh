#!/bin/bash

CURR_DIR="$(pwd)"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"


# Configurable parameters
TESTS_PATH="${SCRIPT_DIR}/testes"
WORK_DIR="aluno1_aluno2"
TEST_OUT_PATH="my_tests"
ASB_WORK_DIR="assembler"
LNK_WORK_DIR="linker"
DEBUG=1 # 0/1


function echo_dbg() {
    if [[ $DEBUG -eq '1' ]]; then
        echo $@
    else
        $@
    fi
}

function go_compile_back() {
    pth="$1"
    curr_pth="$(pwd)"
    echo "Going to $pth"
    cd "$pth"
    echo make
    echo "Going back to $curr_pth"
    cd "$curr_pth"
}

# To make relative paths work
echo "Going to ${SCRIPT_DIR}/${WORK_DIR}"
cd "${SCRIPT_DIR}/${WORK_DIR}"

# Compile the executables
echo "Compiling Assembler"
go_compile_back $ASB_WORK_DIR
echo "Compiling Linker"
go_compile_back $LNK_WORK_DIR

# Run the tests
echo "Running tests"
echo_dbg mkdir -p "${TEST_OUT_PATH}"
for tst in "${TESTS_PATH}/"*/; do
    tstname="${tst%*/}"
    tstname="${tstname##*/}"
    echo -e "\\t Test: ${tstname}"
    echo_dbg mkdir -p "${TEST_OUT_PATH}/${tstname}"
    for file in "${TESTS_PATH}/${tstname}/"*.a; do
        filename=$(basename "${file}")
        echo -e "\\t\\t $filename"
        echo_dbg "${ASB_WORK_DIR}"/mont "$file" "${TEST_OUT_PATH}/${tstname}/${filename%.*}.o"
    done
    obj_lst=$(find "${TEST_OUT_PATH}/${tstname}/" -name *.o ! -name "main.o" )
    echo_dbg "${LNK_WORK_DIR}"/link "${TEST_OUT_PATH}/${tstname}/${tstname}" "${TEST_OUT_PATH}/${tstname}/main.o" ${obj_lst}
done

cd "${CURR_DIR}"