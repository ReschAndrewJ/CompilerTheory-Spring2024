cmake_minimum_required(VERSION 3.26)

project(FirstCompiler
	LANGUAGES CXX C)

add_executable(Compiler
	src/main.cpp
	src/extern.cpp
	src/input.cpp
	src/error.cpp
	src/token.cpp
	src/tables.cpp
	src/scanner.cpp
	src/parser.cpp
	src/enums_strings.cpp
	src/code_generator.cpp
)

add_library(Runtime STATIC
	src/runtimelib.c
)

target_compile_features(Compiler PRIVATE cxx_std_17)
target_compile_features(Runtime PRIVATE c_std_17)

