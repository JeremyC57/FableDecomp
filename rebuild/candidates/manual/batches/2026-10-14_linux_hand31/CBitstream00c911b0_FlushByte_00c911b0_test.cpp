#include <cstdio>
struct CBitstream; static long g_value = -1, g_count = -1; static int g_writes = 0;
static void WriteBits(CBitstream* s, long v, long c) { ++g_writes; g_value = v; g_count = c; }
struct CBitstream { unsigned char _pad[4]; long m_bitsUsed; void FlushByte() { long pad = 8 - m_bitsUsed; if (pad < 8) WriteBits(this, 0, pad); } };
int main() { CBitstream b; b.m_bitsUsed = 0; b.FlushByte(); if (g_writes) { std::printf("00c911b0_TEST FAIL\n"); return 1; } b.m_bitsUsed = 3; b.FlushByte(); if (g_writes != 1 || g_value != 0 || g_count != 5) { std::printf("00c911b0_TEST FAIL\n"); return 1; } std::printf("00c911b0_TEST PASS\n"); return 0; }
