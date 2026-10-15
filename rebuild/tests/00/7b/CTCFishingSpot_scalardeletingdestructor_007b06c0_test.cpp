#include <cstdio>
// Behaviour test for CTCFishingSpot scalar deleting destructor (0x007B06C0): RemoveParticles runs first, then
// the three intelligent pointers in reverse order (fish, particle 2, particle 1), then the CTCBase base.
static int g_order[8]; static int g_n = 0;
struct CTCBase { virtual ~CTCBase() { g_order[g_n++] = 0; } long ParentThing; bool ToBeRemoved; };
struct CIntelligentPointer { int m_id; void* m_pItem; ~CIntelligentPointer() { g_order[g_n++] = m_id; } };
struct CTCFishingSpot : CTCBase {
    CIntelligentPointer PParticle1, PParticle2, PParticleFish;
    bool Enabled;
    void RemoveParticles() { g_order[g_n++] = 9; }
    virtual ~CTCFishingSpot() { RemoveParticles(); }
};
int main()
{
    CTCFishingSpot* s = new CTCFishingSpot;
    s->PParticle1.m_id = 1; s->PParticle2.m_id = 2; s->PParticleFish.m_id = 3;
    CTCBase* p = s;
    delete p;
    int want[5] = { 9, 3, 2, 1, 0 };
    if (g_n != 5) { std::printf("007b06c0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        if (g_order[i] != want[i]) { std::printf("007b06c0_TEST FAIL\n"); return 1; }
    std::printf("007b06c0_TEST PASS\n");
    return 0;
}