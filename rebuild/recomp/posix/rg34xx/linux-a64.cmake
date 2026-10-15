set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_ASM_COMPILER clang)
set(CMAKE_C_COMPILER_TARGET aarch64-linux-gnu)
set(CMAKE_CXX_COMPILER_TARGET aarch64-linux-gnu)
set(CMAKE_ASM_COMPILER_TARGET aarch64-linux-gnu)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# The RG34XX SP's Allwinner H700: in-order Cortex-A53 cores
set(CMAKE_C_FLAGS_INIT "-mcpu=cortex-a53 -fomit-frame-pointer")
set(CMAKE_CXX_FLAGS_INIT "-mcpu=cortex-a53 -fomit-frame-pointer")
# Code layout (CMakeLists.txt): lld places the hot functions listed in hot_symbols.txt
# (gen_hot_order.py) together, and the game moves them onto transparent huge pages at startup
# when the kernel has them. FABLE_SYMBOL_ORDER= (empty) and FABLE_HUGE_TEXT=OFF undo both.
set(FABLE_LLD ON CACHE BOOL "Link with lld")
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/hot_symbols.txt")
  set(FABLE_SYMBOL_ORDER "${CMAKE_CURRENT_LIST_DIR}/hot_symbols.txt" CACHE FILEPATH "Hot functions placed first")
endif()
set(FABLE_HUGE_TEXT ON CACHE BOOL "Hot code on transparent huge pages")
# The H700's A53 cores are most likely r0p4 (H6/H616 family; Fable-diag.sh logs /proc/cpuinfo),
# a revision with errata 835769 and 843419: the linker patches the few ADRP sequences of the
# second, the first costs nothing in the lifted code.
set(FABLE_A53_ERRATA ON CACHE BOOL "Cortex-A53 errata workarounds")
