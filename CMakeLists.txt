cmake_minimum_required(VERSION 2.6.2)
project(libev CXX C)
set(CMAKE_VERBOSE_MAKEFILE ON)

macro(fix_default_compiler_settings_)
  if (MSVC)
    foreach (flag_var
             CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
             CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
      # We prefer more strict warning checking for building Google Test.
      # Replaces /W3 with /W4 in defaults.
      string(REPLACE "/W3" "/W4" ${flag_var} "${${flag_var}}")
    endforeach()
  endif()
endmacro()

macro(config_compiler_and_linker)
  find_package(Threads)
  fix_default_compiler_settings_()

  if (MSVC)
    set(cxx_base_flags "-GS -W4 -wd4251 -wd4275 -nologo -J -Zi -DWIN32")
    if (MSVC_VERSION LESS 1400)  # 1400 is Visual Studio 2005
      # Suppress spurious warnings MSVC 7.1 sometimes issues.
      # Forcing value to bool.
      set(cxx_base_flags "${cxx_base_flags} -wd4800")
      # Copy constructor and assignment operator could not be generated.
      set(cxx_base_flags "${cxx_base_flags} -wd4511 -wd4512")
      # Compatibility warnings not applicable to Google Test.
      # Resolved overload was found by argument-dependent lookup.
      set(cxx_base_flags "${cxx_base_flags} -wd4675")
    endif()
    if (MSVC_VERSION LESS 1500)  # 1500 is Visual Studio 2008
      # Conditional expression is constant.
      # When compiling with /W4, we get several instances of C4127
      # (Conditional expression is constant). In our code, we disable that
      # warning on a case-by-case basis. However, on Visual Studio 2005,
      # the warning fires on std::list. Therefore on that compiler and earlier,
      # we disable the warning project-wide.
      set(cxx_base_flags "${cxx_base_flags} -wd4127")
    endif()
    if (NOT (MSVC_VERSION LESS 1700))  # 1700 is Visual Studio 2012.
      # Suppress "unreachable code" warning on VS 2012 and later.
      # http://stackoverflow.com/questions/3232669 explains the issue.
      set(cxx_base_flags "${cxx_base_flags} -wd4702")
    endif()
#    if (NOT (MSVC_VERSION GREATER 1900))  # 1900 is Visual Studio 2015
#      # BigObj required for tests.
#      set(cxx_base_flags "${cxx_base_flags} -bigobj")
#    endif()

    set(cxx_flags "${cxx_base_flags} -D_UNICODE -DUNICODE -DWIN32 -D_WIN32")
    set(cxx_flags "${cxx_base_flags} -DSTRICT -DWIN32_LEAN_AND_MEAN")
    set(c_flags "${cxx_flags}")
    set(CMAKE_CXX_FLAGS "${cxx_flags}")
    set(CMAKE_C_FLAGS "${c_flags}")
  endif()
endmacro()

config_compiler_and_linker()

include_directories(${PROJECT_SOURCE_DIR})
set(LIBEV_SOURCES "ev.c;event.c")

aux_source_directory(${PROJECT_SOURCE_DIR} SSDIR) 
message("sources ${SSDIR}")

# build libkcpev
add_library(libev_static STATIC ${LIBEV_SOURCES})
target_link_libraries(libev_static ws2_32)

add_library(libev SHARED ${LIBEV_SOURCES})
target_link_libraries(libev ws2_32)

add_executable(libev_test libev_test.cpp)
target_link_libraries(libev_test libev_static)

