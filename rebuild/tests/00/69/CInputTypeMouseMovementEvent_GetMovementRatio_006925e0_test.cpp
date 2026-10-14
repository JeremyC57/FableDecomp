#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_origin = 0, g_calls = 0; struct CEvent { virtual long GetOrigin() { ++g_calls; return g_origin; } unsigned char _pad04[0xc]; long m_10; unsigned char _pad14[8]; long m_1c; unsigned char _pad20[0x28]; long m_base;
    float GetMovementRatio() { long denominator = m_10 - m_base - GetOrigin(); long numerator = m_1c - m_base - GetOrigin(); return (float)numerator / (float)denominator; } };

int main() {
    CEvent e; e.m_10 = 110; e.m_1c = 60; e.m_base = 10; g_origin = 50; float r = e.GetMovementRatio(); if (r != 0.0f || g_calls != 2) { std::printf("006925e0_TEST FAIL\n"); return 1; } e.m_1c = 110; e.m_10 = 85; if (e.GetMovementRatio() != 2.0f) { std::printf("006925e0_TEST FAIL\n"); return 1; }
    std::printf("006925e0_TEST PASS\n"); return 0;
}