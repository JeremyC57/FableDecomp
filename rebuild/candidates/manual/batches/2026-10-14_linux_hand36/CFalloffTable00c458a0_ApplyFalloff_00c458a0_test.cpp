#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_calls = 0;
struct CFalloffCurve_00c1bda0 { float Calculate(float v) { ++g_calls; return v * 2.0f + 1.0f; } };
struct SFalloffEntry { float m_value; unsigned char _rest[0x18]; };
struct CFalloffTable_00c458a0 { unsigned char _pad[0x18]; SFalloffEntry m_entries[8]; void ApplyFalloff(CFalloffCurve_00c1bda0& c) { for (long i = 0; i < 8; ++i) m_entries[i].m_value = c.Calculate(m_entries[i].m_value); } };
int main() {
    CFalloffTable_00c458a0 t; CFalloffCurve_00c1bda0 c; for (int i = 0; i < 8; ++i) t.m_entries[i].m_value = (float)i; t.ApplyFalloff(c);
    if (g_calls != 8 || t.m_entries[0].m_value != 1.0f || t.m_entries[7].m_value != 15.0f) { std::printf("00c458a0_TEST FAIL\n"); return 1; }
    std::printf("00c458a0_TEST PASS\n"); return 0;
}
