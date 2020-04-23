# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/cruvadio/shallow_water_simulation

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cruvadio/shallow_water_simulation/build

# Include any dependencies generated for this target.
include external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/depend.make

# Include the progress variables for this target.
include external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/progress.make

# Include the compile flags for this target's objects.
include external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/flags.make

external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/gamma.c.o: external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/flags.make
external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/gamma.c.o: ../external/glfw-3.2.1/tests/gamma.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cruvadio/shallow_water_simulation/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/gamma.c.o"
	cd /home/cruvadio/shallow_water_simulation/build/external/glfw-3.2.1/tests && /usr/bin/gcc-7 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/gamma.dir/gamma.c.o   -c /home/cruvadio/shallow_water_simulation/external/glfw-3.2.1/tests/gamma.c

external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/gamma.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gamma.dir/gamma.c.i"
	cd /home/cruvadio/shallow_water_simulation/build/external/glfw-3.2.1/tests && /usr/bin/gcc-7 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cruvadio/shallow_water_simulation/external/glfw-3.2.1/tests/gamma.c > CMakeFiles/gamma.dir/gamma.c.i

external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/gamma.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gamma.dir/gamma.c.s"
	cd /home/cruvadio/shallow_water_simulation/build/external/glfw-3.2.1/tests && /usr/bin/gcc-7 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cruvadio/shallow_water_simulation/external/glfw-3.2.1/tests/gamma.c -o CMakeFiles/gamma.dir/gamma.c.s

external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/gamma.c.o.requires:

.PHONY : external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/gamma.c.o.requires

external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/gamma.c.o.provides: external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/gamma.c.o.requires
	$(MAKE) -f external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/build.make external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/gamma.c.o.provides.build
.PHONY : external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/gamma.c.o.provides

external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/gamma.c.o.provides.build: external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/gamma.c.o


external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/getopt.c.o: external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/flags.make
external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/getopt.c.o: ../external/glfw-3.2.1/deps/getopt.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cruvadio/shallow_water_simulation/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/getopt.c.o"
	cd /home/cruvadio/shallow_water_simulation/build/external/glfw-3.2.1/tests && /usr/bin/gcc-7 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/gamma.dir/__/deps/getopt.c.o   -c /home/cruvadio/shallow_water_simulation/external/glfw-3.2.1/deps/getopt.c

external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/getopt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gamma.dir/__/deps/getopt.c.i"
	cd /home/cruvadio/shallow_water_simulation/build/external/glfw-3.2.1/tests && /usr/bin/gcc-7 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cruvadio/shallow_water_simulation/external/glfw-3.2.1/deps/getopt.c > CMakeFiles/gamma.dir/__/deps/getopt.c.i

external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/getopt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gamma.dir/__/deps/getopt.c.s"
	cd /home/cruvadio/shallow_water_simulation/build/external/glfw-3.2.1/tests && /usr/bin/gcc-7 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cruvadio/shallow_water_simulation/external/glfw-3.2.1/deps/getopt.c -o CMakeFiles/gamma.dir/__/deps/getopt.c.s

external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/getopt.c.o.requires:

.PHONY : external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/getopt.c.o.requires

external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/getopt.c.o.provides: external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/getopt.c.o.requires
	$(MAKE) -f external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/build.make external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/getopt.c.o.provides.build
.PHONY : external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/getopt.c.o.provides

external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/getopt.c.o.provides.build: external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/getopt.c.o


external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/glad.c.o: external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/flags.make
external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/glad.c.o: ../external/glfw-3.2.1/deps/glad.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cruvadio/shallow_water_simulation/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/glad.c.o"
	cd /home/cruvadio/shallow_water_simulation/build/external/glfw-3.2.1/tests && /usr/bin/gcc-7 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/gamma.dir/__/deps/glad.c.o   -c /home/cruvadio/shallow_water_simulation/external/glfw-3.2.1/deps/glad.c

external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/gamma.dir/__/deps/glad.c.i"
	cd /home/cruvadio/shallow_water_simulation/build/external/glfw-3.2.1/tests && /usr/bin/gcc-7 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/cruvadio/shallow_water_simulation/external/glfw-3.2.1/deps/glad.c > CMakeFiles/gamma.dir/__/deps/glad.c.i

external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/gamma.dir/__/deps/glad.c.s"
	cd /home/cruvadio/shallow_water_simulation/build/external/glfw-3.2.1/tests && /usr/bin/gcc-7 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/cruvadio/shallow_water_simulation/external/glfw-3.2.1/deps/glad.c -o CMakeFiles/gamma.dir/__/deps/glad.c.s

external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/glad.c.o.requires:

.PHONY : external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/glad.c.o.requires

external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/glad.c.o.provides: external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/glad.c.o.requires
	$(MAKE) -f external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/build.make external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/glad.c.o.provides.build
.PHONY : external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/glad.c.o.provides

external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/glad.c.o.provides.build: external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/glad.c.o


# Object files for target gamma
gamma_OBJECTS = \
"CMakeFiles/gamma.dir/gamma.c.o" \
"CMakeFiles/gamma.dir/__/deps/getopt.c.o" \
"CMakeFiles/gamma.dir/__/deps/glad.c.o"

# External object files for target gamma
gamma_EXTERNAL_OBJECTS =

external/glfw-3.2.1/tests/gamma: external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/gamma.c.o
external/glfw-3.2.1/tests/gamma: external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/getopt.c.o
external/glfw-3.2.1/tests/gamma: external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/glad.c.o
external/glfw-3.2.1/tests/gamma: external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/build.make
external/glfw-3.2.1/tests/gamma: external/glfw-3.2.1/src/libglfw3.a
external/glfw-3.2.1/tests/gamma: /usr/lib/x86_64-linux-gnu/librt.so
external/glfw-3.2.1/tests/gamma: /usr/lib/x86_64-linux-gnu/libm.so
external/glfw-3.2.1/tests/gamma: /usr/lib/x86_64-linux-gnu/libX11.so
external/glfw-3.2.1/tests/gamma: /usr/lib/x86_64-linux-gnu/libXrandr.so
external/glfw-3.2.1/tests/gamma: /usr/lib/x86_64-linux-gnu/libXinerama.so
external/glfw-3.2.1/tests/gamma: /usr/lib/x86_64-linux-gnu/libXxf86vm.so
external/glfw-3.2.1/tests/gamma: /usr/lib/x86_64-linux-gnu/libXcursor.so
external/glfw-3.2.1/tests/gamma: external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cruvadio/shallow_water_simulation/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable gamma"
	cd /home/cruvadio/shallow_water_simulation/build/external/glfw-3.2.1/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gamma.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/build: external/glfw-3.2.1/tests/gamma

.PHONY : external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/build

external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/requires: external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/gamma.c.o.requires
external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/requires: external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/getopt.c.o.requires
external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/requires: external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/__/deps/glad.c.o.requires

.PHONY : external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/requires

external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/clean:
	cd /home/cruvadio/shallow_water_simulation/build/external/glfw-3.2.1/tests && $(CMAKE_COMMAND) -P CMakeFiles/gamma.dir/cmake_clean.cmake
.PHONY : external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/clean

external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/depend:
	cd /home/cruvadio/shallow_water_simulation/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cruvadio/shallow_water_simulation /home/cruvadio/shallow_water_simulation/external/glfw-3.2.1/tests /home/cruvadio/shallow_water_simulation/build /home/cruvadio/shallow_water_simulation/build/external/glfw-3.2.1/tests /home/cruvadio/shallow_water_simulation/build/external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : external/glfw-3.2.1/tests/CMakeFiles/gamma.dir/depend

