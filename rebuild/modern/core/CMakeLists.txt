add_library(fable_core
    src/Math.cpp
)

target_include_directories(fable_core
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_features(fable_core PUBLIC cxx_std_23)
set_target_properties(fable_core PROPERTIES
    CXX_EXTENSIONS OFF
)

# Bit-exact retail float behaviour needs plain IEEE double/float arithmetic:
# no fused multiply-add contraction (Clang/ARM64 contract by default).
if(MSVC)
    target_compile_options(fable_core PRIVATE /W4 /permissive- /fp:precise)
else()
    target_compile_options(fable_core PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -ffp-contract=off
        -fno-fast-math
    )
endif()
