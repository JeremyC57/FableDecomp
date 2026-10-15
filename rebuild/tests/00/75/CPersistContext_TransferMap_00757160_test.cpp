#include <cstdio>
#include <cstring>
// CPersistContext::TransferMap(name, map&): forwards with a zeroed two-word CObjectAugmentationParticleSet default
struct CObjectAugmentationParticleSetMap { long dummy; };
struct CObjectAugmentationParticleSet { long m_first; long m_second; CObjectAugmentationParticleSet() : m_first(0), m_second(0) { } };
static const char* g_name; static CObjectAugmentationParticleSetMap* g_map; static long g_a = 9, g_b = 9; static long g_calls;
struct CPersistContext {
    long pad[11];
    void TransferMap(const char* name, CObjectAugmentationParticleSetMap& map, const CObjectAugmentationParticleSet& def) { g_name = name; g_map = &map; g_a = def.m_first; g_b = def.m_second; ++g_calls; }
    void TransferMap(const char* name, CObjectAugmentationParticleSetMap& map) { TransferMap(name, map, CObjectAugmentationParticleSet()); }
};
int main()
{
    CPersistContext c; CObjectAugmentationParticleSetMap m;
    c.TransferMap("AugmentationParticles", m);
    if (g_calls != 1 || std::strcmp(g_name, "AugmentationParticles") != 0 || g_map != &m || g_a != 0 || g_b != 0) { std::printf("00757160_TEST FAIL\n"); return 1; }
    std::printf("00757160_TEST PASS\n");
    return 0;
}