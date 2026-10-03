add_library(fable_assets
    src/BigArchive.cpp
    src/GameInstall.cpp
    src/Lzo.cpp
    src/TextBank.cpp
    src/Texture.cpp
)

target_include_directories(fable_assets
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_features(fable_assets PUBLIC cxx_std_23)
set_target_properties(fable_assets PROPERTIES
    CXX_EXTENSIONS OFF
)

set(FABLE_ASSETS_WARNINGS
    $<IF:$<CXX_COMPILER_ID:MSVC>,/W4;/permissive-,-Wall;-Wextra;-Wconversion;-Wpedantic>
)
target_compile_options(fable_assets PRIVATE ${FABLE_ASSETS_WARNINGS})

add_executable(fable_assets_cli tools/fable_assets.cpp)
set_target_properties(fable_assets_cli PROPERTIES OUTPUT_NAME fable_assets)
target_link_libraries(fable_assets_cli PRIVATE fable_assets)
target_compile_options(fable_assets_cli PRIVATE ${FABLE_ASSETS_WARNINGS})

if(BUILD_TESTING)
    add_executable(fable_assets_tests tests/AssetsTests.cpp)
    target_link_libraries(fable_assets_tests PRIVATE fable_assets)
    target_compile_options(fable_assets_tests PRIVATE ${FABLE_ASSETS_WARNINGS})

    add_test(NAME fable_assets.Unit COMMAND fable_assets_tests)
endif()
