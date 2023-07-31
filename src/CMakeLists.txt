include(AICxxProject)

add_executable(solver solver.cxx Grid.cxx Vector.cxx BoolVector.cxx)
target_link_libraries(solver PRIVATE ${AICXX_OBJECTS_LIST})
