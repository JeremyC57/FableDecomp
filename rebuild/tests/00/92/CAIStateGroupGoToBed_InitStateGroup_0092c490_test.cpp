#include <cstdio>
// Behaviour model of CAIStateGroup_GoToBed::InitStateGroup @ 0x0092C490: LCG step + ROR13 on the world seed, %10.
class CWorld { public: int x; }; class CThingAICreature { public: int x; };
static unsigned int ROR13(unsigned int v) { return (v >> 13) | (v << 19); }
inline unsigned int WorldRandomStep(unsigned int& seed) { seed = seed * 0x24A1 + 0x24DF; seed = ROR13(seed); return seed; }
static unsigned int g_seed = 12345;
class CAIStateGroupBase { public: virtual void v00() {} unsigned int* GetPWorldSeed() { return &g_seed; } };
struct CAIStateGroup_GoToBed : CAIStateGroupBase { unsigned int m_bedTimeDelay; long m_state;
    void InitStateGroup(const CWorld&, CThingAICreature&) { m_state = -1; m_bedTimeDelay = WorldRandomStep(*GetPWorldSeed()) % 10; } };
int main()
{
    CWorld w; CThingAICreature c; CAIStateGroup_GoToBed g; g.m_state = 3; g.m_bedTimeDelay = 77;
    unsigned int expected = ROR13(12345u * 0x24A1u + 0x24DFu);
    g.InitStateGroup(w, c);
    bool ok = g.m_state == -1 && g_seed == expected && g.m_bedTimeDelay == expected % 10 && g.m_bedTimeDelay < 10;
    if (!ok) { std::printf("0092c490_TEST FAIL\n"); return 1; }
    std::printf("0092c490_TEST PASS\n");
    return 0;
}