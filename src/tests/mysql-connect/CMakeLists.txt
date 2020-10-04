set(EXENAME ucdavis-3D-electricity-map)

set(SOURCES ${SOURCES}
  main.cpp
)

add_executable(${EXENAME} ${SOURCES})

target_link_libraries(${EXENAME} PRIVATE
  BookFiler_Module_MySQL
)