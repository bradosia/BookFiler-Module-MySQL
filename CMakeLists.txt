cmake_minimum_required(VERSION 3.4)

set(PROJECT_NAME BookFiler-Module-MySQL)
project(${PROJECT_NAME} VERSION 1.0.0 LANGUAGES CXX)
set(RELEASE_DIR "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RELEASE_DIR})

option(BUILD_SHARED_LIBS "Build shared library" ON)
option(BUILD_STATIC_LIBS "Build static library" OFF)
option(BUILD_TESTS "Build test executables" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.10")

find_package(Boost 1.56 REQUIRED COMPONENTS
             system filesystem)

include_directories(
  include
  third_party/rapidjson/include
)

# Set up source files
set(SOURCES
  src/Module.cpp
  src/core/Curl.cpp
  src/core/httpCurl.cpp
  src/core/smtpCurl.cpp
  src/core/imapCurl.cpp
  src/core/git.cpp
  src/core/mySQL.cpp
  src/core/ssh.cpp
)

set(HEADERS
  src/Module.hpp
  src/core/config.hpp
  src/core/Curl.hpp
  src/core/httpCurl.hpp
  src/core/smtpCurl.hpp
  src/core/imapCurl.hpp
  src/core/git.hpp
  src/core/mySQL.hpp
  src/core/ssh.hpp

  include/BookFiler-Module-MySQL/Interface.hpp
)

set(SHARED_COMPILE_DEFINITIONS
  -DSHARED_LIBRARY
)

set(STATIC_COMPILE_DEFINITIONS
  -DSTATIC_LIBRARY
)

set(LIBRARIES ${LIBRARIES}
  # sqlite 3
  sqlite3

  # Boost
  Boost::system
  Boost::filesystem

  # curl
  curl
  nghttp2
  psl.dll
  idn2
  ssl
  crypto
  cares
  ssh2
  brotlidec-static
  brotlienc-static
  brotlicommon-static
  unistring
  wldap32
  libz
  bcrypt
  crypto
  crypt32

  # git2
  git2

  # libmariadbclient
  mariadbclient

  # ssh
  ssh
)

if(WIN32)
    set(LIBRARIES ${LIBRARIES}
        # Windows Libraries
        ws2_32
        secur32
        wsock32
        shlwapi
    )
elseif(UNIX)
    set(LIBRARIES ${LIBRARIES}
        # Unix Libraries
    )
endif()

set(SHARED_LINK_LIBRARIES ${LIBRARIES})
set(STATIC_LINK_LIBRARIES ${LIBRARIES})

#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

include(CMakePackageConfigHelpers)

if(BUILD_SHARED_LIBS)
  set(lib_shared_name ${PROJECT_NAME}-LibShared)
  message( STATUS "-- Building ${lib_shared_name}" )
  add_library(${lib_shared_name} SHARED  ${SOURCES} ${HEADERS})
  set_property(TARGET ${lib_shared_name} PROPERTY VERSION "${PROJECT_VERSION}")
  target_include_directories(${lib_shared_name} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )
  #set_target_properties(${lib_shared_name} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS "ON")

  # Compile Definitions
  target_compile_definitions(${lib_shared_name} PUBLIC ${SHARED_COMPILE_DEFINITIONS})
  # Libraries to link
  target_link_libraries(${lib_shared_name} PUBLIC ${SHARED_LINK_LIBRARIES})
endif()

if(BUILD_STATIC_LIBS)
  set(lib_name ${PROJECT_NAME}-Lib)
  message( STATUS "-- Building ${lib_name}" )
  add_library(${lib_name} STATIC  ${SOURCES} ${HEADERS})
  set_property(TARGET ${lib_name} PROPERTY VERSION "${PROJECT_VERSION}")
  target_include_directories(${lib_name} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  )

  # Compile Definitions
  target_compile_definitions(${lib_name} PUBLIC ${STATIC_COMPILE_DEFINITIONS})
  # Libraries to link
  target_link_libraries(${lib_name} PUBLIC ${STATIC_LINK_LIBRARIES})
endif()

# TESTS
add_subdirectory(src/tests/mysql-connect)
add_subdirectory(src/tests/git-local-connect)
add_subdirectory(src/tests/git-remote-connect)
add_subdirectory(src/tests/smtp-connect)
add_subdirectory(src/tests/http-connect)
add_subdirectory(src/tests/imap-connect)

# Post build
if(BUILD_SHARED_LIBS)
    add_custom_command(TARGET ${lib_shared_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/modules/")
    add_custom_command(TARGET ${lib_shared_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${lib_shared_name}> "${CMAKE_BINARY_DIR}/modules/")
endif(BUILD_SHARED_LIBS)

# Installation
if(BUILD_SHARED_LIBS)
  message(STATUS "INSTALL: ${lib_shared_name} TO root=${CMAKE_BINARY_DIR} and current=${CMAKE_CURRENT_BINARY_DIR}")
  install(TARGETS ${lib_shared_name}
    RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/${BIN_NAME}/modules"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/${BIN_NAME}/modules"
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/${BIN_NAME}/modules"
  )
endif(BUILD_SHARED_LIBS)

if(BUILD_STATIC_LIBS)
  install(TARGETS ${lib_name} EXPORT ${lib_name}_TARGETS
    RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/${BIN_NAME}/lib"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/${BIN_NAME}/lib"
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/${BIN_NAME}/lib"
  )
endif(BUILD_STATIC_LIBS)

install(FILES ${HEADERS}
  DESTINATION "${CMAKE_INSTALL_PREFIX}/${BIN_NAME}/headers"
COMPONENT Headers)
