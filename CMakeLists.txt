# CMake entry point.
# Note that this CMakeLists.txt in fully independent from the one in the parent directory
# (since that's the goal of this tutorial !)

# For an introduction to CMake, see
# http://www.cmake.org/cmake/help/cmake_tutorial.html (at least steps 1 and 2)

cmake_minimum_required (VERSION 2.6)
project (BuildingYourOwnOpenglApp)

find_package(OpenGL REQUIRED)

add_subdirectory(external)

# Ask the user to give the paths to GLFW, GLEW and GLM.
# On Linux this can be found automatically with pkg-config;
# see http://stackoverflow.com/questions/6889264/cmake-flags-for-opengl-using-glew-and-glfw, for instance.

set(GLFW_INCLUDE_PATH "external/glfw-3.1.2/include/GLFW/")
set(GLFW_LIBRARY glfw)

set(GLEW_INCLUDE_PATH "external/glew-1.13.0/include/")
set(GLEW_LIBRARY GLEW_1130)

set(GLM_INCLUDE_PATH "external/glm-0.9.7.1/")

#set(ASSIMP_INCLUDE_PATH "external/assimp-3.0.1279/include/")
#set(ASSIMP_LIBRARY ASSIMP)

set (LUGGCGL_MIN_ASSIMP_VERSION 4.0.1)
find_package (assimp QUIET ${LUGGCGL_MIN_ASSIMP_VERSION})
if (NOT assimp_FOUND)
	if (NOT EXISTS "${CMAKE_BINARY_DIR}/assimp")
		file (MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/assimp")
	endif ()
	if (NOT EXISTS "${CMAKE_BINARY_DIR}/assimp/code")
		message (STATUS "As ASSIMP >= ${LUGGCGL_MIN_ASSIMP_VERSION} was not found on your computer, we will fetch it from the source.")

		find_package (Git REQUIRED)
		execute_process (
			COMMAND ${GIT_EXECUTABLE} clone -b "v${LUGGCGL_MIN_ASSIMP_VERSION}" "https://github.com/assimp/assimp.git" "code"
			WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/assimp"
		)
		# assimp will not compile under VS2017.3 without that commit,
		# which is not part of a release yet.
		execute_process (
			COMMAND ${GIT_EXECUTABLE} apply ${CMAKE_SOURCE_DIR}/0001-fix-compiling-problems-under-vs2017-3.patch
			WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/assimp/code"
		)
	endif ()
	if (NOT EXISTS "${CMAKE_BINARY_DIR}/assimp/build")
		file (MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/assimp/build")
	endif ()
	set (ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
	set (ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
	set (ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
	add_subdirectory ("${CMAKE_BINARY_DIR}/assimp/code" "${CMAKE_BINARY_DIR}/assimp/binary")
	set (USE_SYSTEM_ASSIMP FALSE)
	set (ASSIMP_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/assimp/code/include" "${CMAKE_BINARY_DIR}/assimp/binary/include")
endif ()


set(SOURCE_FILES
	# put all your .c/.cpp here.
	src/playground.cpp
	../common/shader.cpp
	src/mesh_gen.cpp
	common/objloader.cpp
	src/Node.cpp
	src/Transform.cpp

)



include_directories(
	${GLFW_INCLUDE_PATH}
	${GLEW_INCLUDE_PATH}
	${GLM_INCLUDE_PATH}
	#${Assimp_INCLUDE_DIRS}
)
add_executable(BuildingYourOwnOpenglApp
	${SOURCE_FILES}
)
target_link_libraries(BuildingYourOwnOpenglApp
	${OPENGL_LIBRARY} # filled by "find_package(OpenGL REQUIRED)"
	${GLFW_LIBRARY}
	${GLEW_LIBRARY}
	#${ASSIMP_LIBRARIES}
)




# HOW-TO. You can safely remove anything below.

# test if linux, mac or windows :

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	message("You're on Mac OS !")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	message("You're on Linux !")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	message("You're on Windows !")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") # Yes this is weird but you have to repeat the "if" here.


# Add a checkbox in CMakeGUI :

set(SOME_STUFF OFF CACHE BOOL "Check this to print an extra message at the beginning of main()")

# Add a preprocessor define :

if(SOME_STUFF)
	add_definitions(
		-DCOMPILE_WITH_SOME_PREPROCESSOR_DIRECTIVE
	)
endif(SOME_STUFF)
