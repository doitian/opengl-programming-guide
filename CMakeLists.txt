cmake_minimum_required(VERSION 3.29)
project(opg C)

find_package(glfw3 REQUIRED)
find_package(glew REQUIRED)
find_package(Vulkan COMPONENTS glslc)
find_program(glslc_executable NAMES glslc glslc.exe HINTS Vulkan::glslc REQUIRED)

function(compile_shaders target)
  # Loop through each shader file
  foreach(shader ${ARGN})
    # Get the filename and extension
    get_filename_component(shader_file "${shader}" NAME)

    # Determine the output file path
    set(spirv_file "${CMAKE_CURRENT_BINARY_DIR}/shaders/${shader_file}.spv")

    add_custom_command(
      OUTPUT ${spirv_file}
      COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/shaders/"
      COMMAND ${glslc_executable} ${shader} -o ${spirv_file}
      DEPENDS ${shader}
      VERBATIM
    )

    add_custom_command(
      TARGET ${target} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${target}>/shaders/"
      COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_BINARY_DIR}/shaders/" "$<TARGET_FILE_DIR:${target}>/shaders/"
    )

    # Add the compiled shader to the list of generated files
    list(APPEND spirv_files ${spirv_file})
  endforeach()

  # Add the compiled shaders as dependencies of the target
  add_custom_target(${target}_shaders DEPENDS ${spirv_files})
  add_dependencies(${target} ${target}_shaders)
endfunction()

add_executable(a1 src/a1.c)
target_link_libraries(a1 glfw)


add_executable(c1-1 src/c1-1.c)
target_link_libraries(c1-1 glfw GLEW::GLEW opengl::opengl)
compile_shaders(c1-1 "${CMAKE_SOURCE_DIR}/shaders/triangle.vert" "${CMAKE_SOURCE_DIR}/shaders/triangle.frag")
