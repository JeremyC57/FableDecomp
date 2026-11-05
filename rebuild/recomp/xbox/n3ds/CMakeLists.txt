# New 3DS build of the recompiled Xbox Fable (boot test).
#   lift_n3ds.sh <fable_recomp> <iso>/default.xbe <gen> --keep <functions the game runs>
#   cmake -S rebuild/recomp/xbox/n3ds -B build/n3ds -G Ninja -DCMAKE_TOOLCHAIN_FILE=$DEVKITPRO/cmake/3DS.cmake \
#         -DCMAKE_BUILD_TYPE=Release -DFABLE_GEN_DIR=<gen>
#   cmake --build build/n3ds       -> FableXbox.3dsx (and FableXbox.elf)
cmake_minimum_required(VERSION 3.22)
project(FableXbox3DS LANGUAGES C CXX ASM)

set(FABLE_GEN_DIR "" CACHE PATH "lift_n3ds.sh output (recomp_*.c)")
set(XBOX_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(RUNTIME_DIR ${XBOX_DIR}/../runtime)

# Guest memory: the 96-128 MB window at 0x08000000 (recomp.h), the compact layout (xhost.hpp).
set(N3DS_DEFS __3DS__ RECOMP_WINDOW_MEMORY FABLE_COMPACT_MEMORY)
# local-exec TLS: 3dsxtool cannot relocate the initial-exec model GCC picks for extern __thread.
set(N3DS_ARCH -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft -marm -ftls-model=local-exec)

# The lifted game code: -O2 in ARM mode; per-function sections so the linker drops what is unused.
set(FABLE_GEN_OPT "-O2" CACHE STRING "Optimisation flag for the lifted code")
file(GLOB GEN_SOURCES CONFIGURE_DEPENDS "${FABLE_GEN_DIR}/recomp_*.c")
add_library(fable_gen STATIC ${GEN_SOURCES} ${RUNTIME_DIR}/runtime.c)
target_include_directories(fable_gen PRIVATE ${RUNTIME_DIR} ${FABLE_GEN_DIR})
target_compile_definitions(fable_gen PRIVATE ${N3DS_DEFS})
target_compile_options(fable_gen PRIVATE ${N3DS_ARCH} ${FABLE_GEN_OPT} -ffunction-sections -fdata-sections -fsigned-char
                       -ffp-contract=off -fno-strict-aliasing -w)

set(HOST_SOURCES boot.cpp core.cpp memory.cpp kernel.cpp files.cpp disc.cpp hle.cpp xc.cpp nv2a.cpp soft_renderer.cpp
    coro.cpp settings.cpp audio.cpp music.cpp xmv.cpp)
list(TRANSFORM HOST_SOURCES PREPEND ${XBOX_DIR}/)
add_executable(FableXbox ${HOST_SOURCES} platform.cpp movie.cpp)
target_include_directories(FableXbox PRIVATE ${RUNTIME_DIR} ${XBOX_DIR} ${XBOX_DIR}/third_party)
target_compile_definitions(FableXbox PRIVATE ${N3DS_DEFS})
target_compile_features(FableXbox PRIVATE cxx_std_20)
target_compile_options(FableXbox PRIVATE ${N3DS_ARCH} -O2 -ffunction-sections -fdata-sections -fno-strict-aliasing
                       -Wall -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable)
target_link_libraries(FableXbox PRIVATE fable_gen ctru m)
target_link_options(FableXbox PRIVATE -Wl,--gc-sections -Wl,-Map=FableXbox.map)

ctr_generate_smdh(FableXbox.smdh NAME "Fable (boot test)" DESCRIPTION "Recompiled Xbox Fable: boot test" AUTHOR "FableDecomp")
ctr_create_3dsx(FableXbox SMDH FableXbox.smdh)
