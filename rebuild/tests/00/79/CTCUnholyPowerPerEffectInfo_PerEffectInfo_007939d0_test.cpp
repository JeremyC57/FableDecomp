// Behaviour test for 0x007939D0 CTCUnholyPower::PerEffectInfo::PerEffectInfo(): the Position is left as it was, the Effect intelligent pointer is default-constructed (empty)
#include <cstdio>
#include <new>
class CThing;
static int g_pointers = 0;
class C3DVector { public:
    float X; float Y; float Z;
    C3DVector() { }
};
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() { }
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_pointers; }
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {
    CIntelligentPointerThing() : CBaseIntelligentPointer() { }
};
class CTCUnholyPower {
public:
    struct PerEffectInfo {
        C3DVector Position;
        CIntelligentPointerThing Effect;
        PerEffectInfo();
    };
};
CTCUnholyPower::PerEffectInfo::PerEffectInfo()
{
}
int main()
{
    union { double align; unsigned char raw[sizeof(CTCUnholyPower::PerEffectInfo)]; } u;
    unsigned int i;
    for (i = 0; i < sizeof(u.raw); ++i) u.raw[i] = 0;
    float* pos = (float*)u.raw;
    pos[0] = 1.5f; pos[1] = 2.5f; pos[2] = 3.5f;
    CTCUnholyPower::PerEffectInfo* info = new (u.raw) CTCUnholyPower::PerEffectInfo();
    if (info->Position.X != 1.5f || info->Position.Z != 3.5f || info->Effect.m_pItem != 0 || g_pointers != 1)
        { std::printf("007939d0_TEST FAIL\n"); return 1; }
    info->~PerEffectInfo();
    std::printf("007939d0_TEST PASS\n");
    return 0;
}