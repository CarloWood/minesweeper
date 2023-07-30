include(AICxxProject)

add_executable(solver solver.cxx)
target_link_libraries(solver PRIVATE ${AICXX_OBJECTS_LIST})
