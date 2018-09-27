# Generate mavlink header files
add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/mavlink/c_library/planckAero/mavlink.h
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/mavlink
  COMMAND python -m pymavlink.tools.mavgen --lang C --wire-protocol 2.0 message_definitions/v1.0/planckAero.xml -o ${CMAKE_BINARY_DIR}/mavlink/c_library
  )

add_custom_target(
  mavlink_headers
  DEPENDS ${CMAKE_BINARY_DIR}/mavlink/c_library/planckAero/mavlink.h
  )

#create the binary
add_executable(decode_example decode_example.cpp udp.cpp)
target_include_directories(decode_example PUBLIC ${CMAKE_BINARY_DIR}/mavlink/c_library/planckAero)
add_dependencies(decode_example mavlink_headers)

add_executable(encode_example encode_example.cpp udp.cpp)
target_include_directories(encode_example PUBLIC ${CMAKE_BINARY_DIR}/mavlink/c_library/planckAero)
add_dependencies(encode_example mavlink_headers)
