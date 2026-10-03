cmake_minimum_required(VERSION 3.25)
project(FableRecomp LANGUAGES C CXX)

# Lifter (host tool): needs Zydis (https://github.com/zyantific/zydis).
find_package(Zydis REQUIRED)
add_executable(fable_recomp lifter/Lifter.cpp)
target_compile_features(fable_recomp PRIVATE cxx_std_20)
target_link_libraries(fable_recomp PRIVATE Zydis::Zydis)
