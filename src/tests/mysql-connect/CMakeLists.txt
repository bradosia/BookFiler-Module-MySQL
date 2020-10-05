set(EXENAME mysql-connect)

set(SOURCES_SUB ${SOURCES_SUB}
  main.cpp
)

set(HEADERS_SUB
  src/Module.hpp
  src/core/config.hpp
  src/core/http_curl.hpp

  include/BookFiler-Module-Filesystem-Database/Interface.hpp
)

include_directories(
    ../../
    ../../../include
    ../../../third_party/rapidjson/include
)

add_executable(${EXENAME} ${SOURCES_SUB})

target_link_libraries(${EXENAME} PRIVATE
  ${lib_shared_name}
)
