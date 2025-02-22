# activate c++14 for the 3 OS
set(CMAKE_CXX_STANDARD 14) # 17 for Qt6
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#------------------------------------------------------------------------------
# Specific compiler options
#------------------------------------------------------------------------------
# GNUCXX
#------------------------------------------------------------------------------
if(CMAKE_COMPILER_IS_GNUCXX)
  message( STATUS "GCC compiler detected" )
  if( (DEFINED ENV{WARN_ERROR_LIN_GCC}) AND ("$ENV{WARN_ERROR_LIN_GCC}" STREQUAL "TRUE") )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror" )
    message("--> Treat GCC warnings as errors")
  endif()
endif()
#------------------------------------------------------------------------------
# Clang (export CXX=clang++ CC=clang)
#------------------------------------------------------------------------------
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_COMPILER_IS_CLANGXX 1)
endif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")

if(CMAKE_COMPILER_IS_CLANGXX)
	message( STATUS "Clang compiler detected" )
endif()
#------------------------------------------------------------------------------
# Fix link error undefined reference to `boost::filesystem::detail::copy_file`
# on Travis/Ubuntu 14.04, see
# https://stackoverflow.com/questions/35007134/c-boost-undefined-reference-to-boostfilesystemdetailcopy-file
#------------------------------------------------------------------------------
if( DEFINED ENV{TRAVIS} )
	if( NOT APPLE )
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_NO_CXX11_SCOPED_ENUMS" )
	endif()
	if(CMAKE_COMPILER_IS_CLANGXX)
		# fix compilation being canceled by Travis because of log file too big 
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-register -Wno-deprecated-declarations" )
	endif()
endif()

#------------------------------------------------------------------------------
# MSVC
#------------------------------------------------------------------------------
# /MP for multiple compilation units (cl.exe) ; without any value, it is set automatically depending on your number of main threads
if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
	if( (DEFINED ENV{WARN_ERROR_WIN}) AND ("$ENV{WARN_ERROR_WIN}" STREQUAL "TRUE") )
	  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
	  message("--> /WX ON (Treat linker warnings as errors)")
	else()
	  message("--> /WX OFF (Don't treat linker warnings as errors)")
	endif()

	add_definitions(-DWIN32_LEAN_AND_MEAN) # 'include <winsock2.h>' before 'include <Windows.h>' or set that line (for Boost.Beast)
	add_definitions(-D_WIN32_WINNT=0x0601) # Assuming _WIN32_WINNT=0x0601 (i.e. Windows 7 target) for Boost.Beast

	set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS 
		_CRT_SECURE_NO_WARNINGS
		_SCL_SECURE_NO_WARNINGS
		_USE_MATH_DEFINES)

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")

	if(0)
		# Create filters in VS project
		macro(GroupSources curdir)
			file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir}
				${PROJECT_SOURCE_DIR}/${curdir}/*)
			foreach(child ${children})
				if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
					GroupSources(${curdir}/${child})
				else()
					string(REPLACE "/" "\\" groupname ${curdir})
					string(REPLACE "src" "" groupname ${groupname})
					source_group(${groupname} FILES
						${PROJECT_SOURCE_DIR}/${curdir}/${child})
				endif()
			endforeach()
		endmacro()
	endif()

	# --> better for future use : https://cmake.org/cmake/help/v3.0/command/source_group.html

	include("${CMAKE_CURRENT_LIST_DIR}/msvc/kit.cmake")
endif()
#------------------------------------------------------------------------------
