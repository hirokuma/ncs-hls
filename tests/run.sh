#!/bin/sh

TEST_ARGS=$@
TEST_LOGFILE=test.log
LCOV_INFO=build/lcov.info
LCOV_OUTPUT=build/lcov-output

test_and_lcov()
{
  DIR=$1
  BAK=`pwd`
  cd ${DIR}
  # rm -rf build/CMakeFiles/app.dir/src/*.gc* \
  #   build/CMakeFiles/app.dir/runner/*.gc*
  west build -b native_sim_64 -t run ${TEST_ARGS}
  cnt=`find build/CMakeFiles/app.dir/src -name "*.gcno" | wc -l`
  if [ $cnt -ne 0 ]; then
    lcov --quiet --capture --directory build/CMakeFiles/app.dir/src/ --output-file ${LCOV_INFO} 2> /dev/null
    genhtml --quiet ${LCOV_INFO} --output-directory ${LCOV_OUTPUT} --show-details --legend 2> /dev/null
    if [ -f ${DIR}/${LCOV_OUTPUT}/index.html ]; then
      echo "\n======\nCOVERAGE: ${DIR}/${LCOV_OUTPUT}/index.html\n=====\n"
    fi
  fi
  cd ${BAK}
}

exec_test()
{
  test_and_lcov $1 >> ${TEST_LOGFILE}
  sed -i -e "s|\*\*\* Booting nRF Connect SDK.*$|=====\n===== TEST: $1 =====\n=====|g" ${TEST_LOGFILE}
}

rm -f ${TEST_LOGFILE}

exec_test drivers/led
exec_test services/hls

# test result
sed -n '/^=====$/,/^PROJECT EXECUTION SUCCESSFUL$/p' ${TEST_LOGFILE}
