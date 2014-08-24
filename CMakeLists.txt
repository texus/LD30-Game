cmake_minimum_required(VERSION 2.6)

project(Game)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -pedantic")

file(GLOB SRC "src/*.cpp")

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
  set(CMAKE_INSTALL_RPATH "$ORIGIN/lib/:$$ORIGIN/lib/")
endif()

include_directories("${PROJECT_SOURCE_DIR}/include")

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2 COMPONENTS audio graphics window system REQUIRED)
find_package(TGUI 0.7 REQUIRED)
find_package(Thor 2 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(temp/SFML-ef1b9d6b20fd2b301606ea4169b19bca750c2b0e/include)
include_directories(temp/texus-tgui-devel-f0b19c573b39/include)
include_directories(temp/Thor-d5ba1d02b51fad1b9ba8428567d435ac07179efe/include)
include_directories(temp/Thor-d5ba1d02b51fad1b9ba8428567d435ac07179efe/extlibs/aurora/include)

add_executable(Game ${SRC})
target_link_libraries(Game ${TGUI_LIBRARY} ${THOR_LIBRARY} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} ${OPENGL_gl_LIBRARY})

install(TARGETS Game DESTINATION ${PROJECT_SOURCE_DIR})
