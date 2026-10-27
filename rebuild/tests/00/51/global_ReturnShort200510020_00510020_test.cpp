// Behaviour test for 00510020 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
short ReturnShort2_00510020() { return 2; }
int main() { if (ReturnShort2_00510020() != 2) { std::printf("00510020_TEST FAIL\n"); return 1; } { std::printf("00510020_TEST PASS\n"); return 0; } }