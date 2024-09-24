set(SRCDIR ..)

target_sources(app PRIVATE
    hls.c
)

target_include_directories(app PRIVATE ${SRCDIR})
