# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\appBorisMaherskaya_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\appBorisMaherskaya_autogen.dir\\ParseCache.txt"
  "appBorisMaherskaya_autogen"
  )
endif()
