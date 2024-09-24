cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS {ZEPHYR_BASE})
project(hls_test)

# Generate runner for the test
test_runner_generate(src/hls_test.c)

set(ROOTDIR ../../..)

# Create mocks
# cmock_handle(${ZEPHYR_BASE}/include/zephyr/bluetooth/bluetooth.h)
# cmock_handle(${ZEPHYR_BASE}/include/zephyr/bluetooth/gatt.h)
cmock_handle(${ROOTDIR}/src/drivers/led.h)

# add test file
target_sources(app PRIVATE src/hls_test.c)

target_include_directories(app PRIVATE .)
target_include_directories(app PRIVATE src)
target_include_directories(app PRIVATE ${ROOTDIR}/src)

target_compile_options(app PRIVATE
    -fprofile-arcs
    -ftest-coverage
    -ggdb
)

target_link_libraries(app PRIVATE
    -lgcov
)
