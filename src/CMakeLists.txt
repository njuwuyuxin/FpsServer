add_subdirectory(thread)
add_subdirectory(utils)
add_subdirectory(log)

aux_source_directory(. srcs)
add_library(src ${srcs})

target_link_libraries(src thread)
target_link_libraries(src utils)
target_link_libraries(src log)