#include <cstdio>
#include <cstring>
static const bool g_is32 = sizeof(void*) == 4 && sizeof(long) == 4;   // the layout checks hold for the 32-bit VC7.1 build
static char g_seq[32] = ""; static const void* g_init = 0;
struct CInputProcessGameBaseInit { void* World; };
struct CBaseIntelligentPointer { virtual void v00() { } void* m_pItem; CBaseIntelligentPointer() { m_pItem = 0; std::strcat(g_seq, "p"); } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() { } CIntelligentPointer() : CBaseIntelligentPointer() { } };
struct CInputProcessGameBase { virtual void v00() { } unsigned char _pad04[0x10]; void* World; void* MyPlayer; void* DefinitionManager; void* Component; void* PGamePlayerInterface;
    CInputProcessGameBase(const CInputProcessGameBaseInit& init) { std::strcat(g_seq, "B"); g_init = &init; World = init.World; } };
struct CInputProcessControlSpirit : CInputProcessGameBase { virtual void v00() { } CIntelligentPointer PCreature; CInputProcessControlSpirit(const CInputProcessGameBaseInit& init) : CInputProcessGameBase(init), PCreature() { } };
int main()
{
    CInputProcessGameBaseInit init; int w; init.World = &w; CInputProcessControlSpirit s(init);
    if (std::strcmp(g_seq, "Bp") || g_init != &init || s.World != &w || s.PCreature.m_pItem != 0 || (g_is32 && (char*)&s.PCreature - (char*)&s != 0x28)) { std::printf("006a9140_TEST FAIL\n"); return 1; }
    std::printf("006a9140_TEST PASS\n");
    return 0;
}
