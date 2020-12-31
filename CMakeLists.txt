cmake_minimum_required(VERSION 3.5)
project(armLearn)
set(sourceDir src)
add_executable(${PROJECT_NAME} ${sourceDir}/main.c)

set_target_properties(${PROJECT_NAME}  PROPERTIES COMPILE_FLAGS ${COMPILE_FLAGS} "-save-temps") 

add_subdirectory(genMachineCode)