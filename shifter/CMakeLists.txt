include_directories(${CMAKE_SOURCE_DIR}/shifter ${ROOT_INCLUDE_DIR})
add_executable(mchqashifter mchqashifter.cxx)
target_link_libraries(mchqashifter MUONmapping CDB MUONbase MUONrec STEER Core RAWDatarec STEERBase)
install(TARGETS mchqashifter RUNTIME DESTINATION bin)
