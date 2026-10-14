#include <cstdio>
#include <cstring>
#include <cmath>

static bool g_flag;
void ClearPendingFlag() { if (g_flag) g_flag = false; }
int main() {
    g_flag = true; ClearPendingFlag(); if (g_flag) { std::printf("009d81e0_TEST FAIL\n"); return 1; }
    g_flag = false; ClearPendingFlag(); if (g_flag) { std::printf("009d81e0_TEST FAIL\n"); return 1; }
    std::printf("009d81e0_TEST PASS\n"); return 0;
}