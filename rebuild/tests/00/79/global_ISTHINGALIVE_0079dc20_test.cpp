#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
namespace NParticleEngine {
class CParticleEmitter { public:
    unsigned char _pad_0x00[0x40];
    unsigned short LowFlags : 5;
    unsigned short Dead : 1;
    unsigned short HighFlags : 10;
    bool IsDead() const { return Dead; }
};
}
static int g_calls = 0;
struct CBaseIntelligentPointer {
    void* VTable;
    void* PItem;
    void* GetPItem() const { ++g_calls; return PItem; }   // test stand-in for 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing)
{
    if (thing.GetPItem() != 0) {
        if (!thing.GetPItem()->IsDead())
            return true;
    }
    return false;
}
int main()
{
    NParticleEngine::CParticleEmitter live, dead;
    live.LowFlags = 0x1f; live.Dead = 0; live.HighFlags = 0x3ff;
    dead.LowFlags = 0; dead.Dead = 1; dead.HighFlags = 0;
    CIntelligentPointer<NParticleEngine::CParticleEmitter> p;
    int bad = 0;
    p.VTable = 0;
    p.PItem = 0;
    if (IS_THING_ALIVE(p) || g_calls != 1) bad = 1;
    p.PItem = &live;
    if (!IS_THING_ALIVE(p) || g_calls != 3) bad = 1;
    p.PItem = &dead;
    if (IS_THING_ALIVE(p)) bad = 1;
    if (bad) { std::printf("0079dc20_TEST FAIL\n"); return 1; }
    std::printf("0079dc20_TEST PASS\n");
    return 0;
}