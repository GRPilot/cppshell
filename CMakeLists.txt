cmake_minimum_required(VERSION 1.13)

project(shell)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(root ${PROJECT_SOURCE_DIR})

include_directories(
    ${root}/core
    ${root}/core/shell
    ${root}/core/user
    ${root}
)

set(sources
    ${root}/core/core.cpp
    ${root}/core/shell/shell.cpp
    ${root}/core/shell/shellcmd.cpp
    ${root}/core/shell/cmdgen.cpp
    ${root}/main.cpp
)
set(headers
    ${root}/core/core.hpp
    ${root}/core/shell/shell.hpp
    ${root}/core/shell/shellcmd.hpp
    ${root}/core/shell/cmdgen.hpp
    ${root}/deps.hpp
)

add_executable(${PROJECT_NAME} ${sources} ${headers})
