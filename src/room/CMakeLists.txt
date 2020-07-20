aux_source_directory(. room_srcs)
add_library(room ${room_srcs})
target_link_libraries(room -luuid)