project(sistemaSolar)
add_executable(${PROJECT_NAME} planet.cpp moon.cpp camera.cpp main.cpp window.cpp)
enable_abcg(${PROJECT_NAME})