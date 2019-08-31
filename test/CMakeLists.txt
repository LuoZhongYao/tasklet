add_executable(tsklet_test tsklet_test.c)
target_link_libraries(tsklet_test core)

add_test(NAME tsklet CONFIGURATIONS Release COMMAND tsklet_test)
