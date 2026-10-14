#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CScreenFilterSource_00c596e0 { unsigned char _pad[0xf4]; void* m_pRecord; };
struct CEnvironmentSpellEffectInterface { unsigned char _pad[0xc]; long m_filterA; unsigned char _pad10[0x1c]; long m_filterB; unsigned char _pad30[0x20]; void* m_pFilterRecord;
    long ClearScreenFilter(CScreenFilterSource_00c596e0* source) { m_filterB = 0; m_filterA = 0; if (source) m_pFilterRecord = source->m_pRecord; else m_pFilterRecord = 0; return 0; } };
int main() {
    CEnvironmentSpellEffectInterface e; std::memset(&e, 0x11, sizeof e); CScreenFilterSource_00c596e0 s; int rec; s.m_pRecord = &rec;
    if (e.ClearScreenFilter(&s) != 0 || e.m_filterA != 0 || e.m_filterB != 0 || e.m_pFilterRecord != &rec) { std::printf("00c596e0_TEST FAIL\n"); return 1; }
    e.ClearScreenFilter(0); if (e.m_pFilterRecord != 0) { std::printf("00c596e0_TEST FAIL\n"); return 1; }
    std::printf("00c596e0_TEST PASS\n"); return 0;
}
