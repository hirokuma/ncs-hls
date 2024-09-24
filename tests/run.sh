#!/bin/sh

TEST_ARGS=$@
LCOV_INFO=build/lcov.info
LCOV_OUTPUT=build/lcov-output

exec_test()
{
    DIR=$1
    BAK=`pwd`
    cd ${DIR}
    rm -rf build/CMakeFiles/app.dir/src/*.gc* \
      build/CMakeFiles/app.dir/runner/*.gc*
    west build -b native_sim_64 -t run ${TEST_ARGS}
    cnt=`find build/CMakeFiles/app.dir/src -name "*.gcno" | wc -l`
    if [ $cnt -ne 0 ]; then
      lcov --capture --directory build/CMakeFiles/app.dir/src/ --output-file ${LCOV_INFO} > /dev/null 2>&1
      genhtml ${LCOV_INFO} --output-directory ${LCOV_OUTPUT} --show-details --legend > /dev/null 2>&1
      if [ -f ${DIR}/${LCOV_OUTPUT}/index.html ]; then
        echo "\n======\nCOVERAGE: ${DIR}/${LCOV_OUTPUT}/index.html\n=====\n"
      fi
    fi
    cd ${BAK}
}

exec_test drivers/led
exec_test services/hls
