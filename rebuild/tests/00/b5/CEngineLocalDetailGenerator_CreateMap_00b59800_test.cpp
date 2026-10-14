#include <cstdio>
static int g_ctor = 0; static void* g_entry = 0;
struct SMapEntry;
struct CLocalDetailMap_00bdefc0 { unsigned char _body[0x70]; CLocalDetailMap_00bdefc0(SMapEntry* e) { ++g_ctor; g_entry = e; } };
struct SMapEntry { long m_id; CLocalDetailMap_00bdefc0* m_pMap; };
struct CEngineLocalDetailGenerator { void CreateMap(SMapEntry& e) { if (!e.m_pMap) e.m_pMap = new CLocalDetailMap_00bdefc0(&e); } };
int main() {
    CEngineLocalDetailGenerator g; SMapEntry e = { 1, 0 }; g.CreateMap(e);
    if (!e.m_pMap || g_ctor != 1 || g_entry != &e) { std::printf("00b59800_TEST FAIL\n"); return 1; }
    CLocalDetailMap_00bdefc0* first = e.m_pMap; g.CreateMap(e); if (e.m_pMap != first || g_ctor != 1) { std::printf("00b59800_TEST FAIL\n"); return 1; }
    delete first;
    std::printf("00b59800_TEST PASS\n"); return 0;
}