cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS {ZEPHYR_BASE})
project(led_test)

# Generate runner for the test
test_runner_generate(src/led_test.c)

set(ROOTDIR ../..)

# Create mocks

# Include target sources
cmock_handle(${ZEPHYR_BASE}/include/zephyr/drivers/gpio.h)

# add test file
target_sources(app PRIVATE src/led_test.c)

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
