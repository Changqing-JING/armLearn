cmake_minimum_required(VERSION 3.5)
project(armLearn CXX C ASM)
aux_source_directory(src sourceFiles)
add_executable(${PROJECT_NAME} ${sourceFiles} src/asmVsMachinecode.s)

set_target_properties(${PROJECT_NAME}  PROPERTIES COMPILE_FLAGS ${COMPILE_FLAGS} "-save-temps") 

