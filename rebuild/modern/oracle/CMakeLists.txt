# Optional developer tool: behavioural oracle over the user's own retail Fable.exe.
# Built only when Unicorn (https://github.com/unicorn-engine/unicorn) is installed;
# never linked into the port. Point CMAKE_PREFIX_PATH at a Unicorn install.
find_path(UNICORN_INCLUDE_DIR unicorn/unicorn.h)
find_library(UNICORN_LIBRARY NAMES unicorn)

if(NOT UNICORN_INCLUDE_DIR OR NOT UNICORN_LIBRARY)
    message(STATUS "fable_oracle: Unicorn not found, skipping the retail behavioural oracle")
    return()
endif()

add_library(fable_oracle src/RetailOracle.cpp)
target_include_directories(fable_oracle
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${UNICORN_INCLUDE_DIR}")
target_link_libraries(fable_oracle PUBLIC "${UNICORN_LIBRARY}")
target_compile_features(fable_oracle PUBLIC cxx_std_23)
set_target_properties(fable_oracle PROPERTIES CXX_EXTENSIONS OFF)
if(MSVC)
    target_compile_options(fable_oracle PRIVATE /W4 /permissive-)
else()
    target_compile_options(fable_oracle PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(BUILD_TESTING)
    file(GLOB FABLE_ORACLE_TESTS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
    add_executable(fable_oracle_tests ${FABLE_ORACLE_TESTS})
    target_link_libraries(fable_oracle_tests PRIVATE fable_oracle fable_assets fable_core)
    # Needs the user's Fable.exe: set FABLE_EXE, otherwise the test reports SKIPPED.
    add_test(NAME fable_oracle.Retail COMMAND fable_oracle_tests)
    set_tests_properties(fable_oracle.Retail PROPERTIES SKIP_RETURN_CODE 77)
endif()
