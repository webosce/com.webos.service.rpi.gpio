cmake_minimum_required(VERSION 2.8.7)
project(com.webos.service.rpi.gpio CXX)
set(CMAKE_BUILD_TYPE Debug)
  
include(webOS/webOS)
webos_modules_init(1 6 3)
webos_component(0 0 1)
  
include(FindPkgConfig)
  
pkg_check_modules(GLIB2 REQUIRED glib-2.0)
pkg_check_modules(JSONC REQUIRED json-c)
pkg_check_modules(PMLOGLIB REQUIRED PmLogLib)

include_directories(${GLIB2_INCLUDE_DIRS})
webos_add_compiler_flags(ALL ${GLIB2_CFLAGS_OTHER})
  
pkg_check_modules(LUNASERVICE2 REQUIRED luna-service2)
include_directories(${LUNASERVICE2_INCLUDE_DIRS})
webos_add_compiler_flags(ALL ${LUNASERVICE2_CFLAGS_OTHER})
include_directories(
	${CMAKE_SOURCE_DIR}/include
        ${PMLOGLIB_INCLUDE_DIRS}
)
find_library(wiringPi_LIB wiringPi)
find_library(json-c_LIB json-c)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SRCS main.cpp ./src/logging.cpp ./src/gpioBase.cpp)
  
add_executable(${PROJECT_NAME} ${SRCS})
  
SET (EXT_LIBS
    ${GLIB2_LDFLAGS}
    ${LUNASERVICE2_LDFLAGS}
)
  
target_link_libraries(${PROJECT_NAME} ${EXT_LIBS})
target_link_libraries(${PROJECT_NAME} ${wiringPi_LIB})
target_link_libraries(${PROJECT_NAME} ${json-c_LIB})
target_link_libraries(${PROJECT_NAME} ${PMLOGLIB_LDFLAGS})
install(TARGETS ${PROJECT_NAME} DESTINATION ${WEBOS_INSTALL_SBINDIR})
  
webos_build_system_bus_files()
webos_build_configured_file(files/systemd/com.webos.service.rpi.gpio SYSCONFDIR systemd/system)
