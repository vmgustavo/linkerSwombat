#!/bin/bash

CURR_DIR="$(pwd)"
SCRIPT_DIR="$(pwd)"
CURR_DIR=$(printf %q "$CURR_DIR")
SCRIPT_DIR=$(printf %q "$SCRIPT_DIR")

# Configurable parameters
TESTS_PATH="${SCRIPT_DIR}/testFiles"
WORK_DIR="../${SCRIPT_DIR}"
TEST_OUT_PATH="${TESTS_PATH}"
ASB_WORK_DIR="assembler"
LNK_WORK_DIR="linker"
DEBUG=0 # 0/1


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
    echo_dbg make
    echo "Going back to $curr_pth"
    cd "$curr_pth"
}

# To make relative paths work
# echo "Going to ${SCRIPT_DIR}/${WORK_DIR}"
# cd "${SCRIPT_DIR}/${WORK_DIR}"

# Compile the executables
echo "Compiling Assembler"
go_compile_back $ASB_WORK_DIR
echo "Compiling Linker"
go_compile_back $LNK_WORK_DIR

# Run the tests
echo "Running tests"
for tst in "${TESTS_PATH}/"*/; do
    tstname="${tst%*/}"
    tstname="${tstname##*/}"
    echo -e "\\t Test: ${tstname}"
    for file in "${TESTS_PATH}/${tstname}/"*.a; do
        filename=$(basename "${file}")
        echo -e "\\t\\t $filename"
        echo_dbg "${ASB_WORK_DIR}"/mont "$file" "${TEST_OUT_PATH}/${tstname}/${filename%.*}.o"
    done
    obj_lst=$(find "${TEST_OUT_PATH}/${tstname}/" -name *.o ! -name "main.o" )
    echo_dbg "${LNK_WORK_DIR}"/link "${TEST_OUT_PATH}/${tstname}/${tstname}" "${TEST_OUT_PATH}/${tstname}/main.o" ${obj_lst}
done

cd "${CURR_DIR}"
