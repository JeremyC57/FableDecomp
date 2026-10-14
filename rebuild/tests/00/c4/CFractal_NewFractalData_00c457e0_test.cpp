#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_calls = 0;
struct CFractalSource_00c457e0 { float GetValue() { return (float)++g_calls; } };
struct SFractalEntry_00c457e0 { float m_value; unsigned char _pad[0x18]; };
struct CFractal { unsigned char _pad[0x18]; SFractalEntry_00c457e0 m_entries[8]; void NewFractalData(CFractalSource_00c457e0* s) { for (int i = 0; i < 8; ++i) m_entries[i].m_value = s->GetValue(); } };
int main() {
    CFractal f; std::memset(&f, 0, sizeof f); CFractalSource_00c457e0 s; f.NewFractalData(&s); if (g_calls != 8 || f.m_entries[0].m_value != 1.0f || f.m_entries[7].m_value != 8.0f || f.m_entries[3].m_value != 4.0f) { std::printf("00c457e0_TEST FAIL\n"); return 1; }
    std::printf("00c457e0_TEST PASS\n"); return 0;
}