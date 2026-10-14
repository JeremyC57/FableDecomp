#include <cstdio>

static int g_base = 0, g_copies = 0;
struct CCharString { void* m_pData; CCharString(void* p) : m_pData(p) { } CCharString(const CCharString& o) : m_pData(o.m_pData) { ++g_copies; } };
struct CThing;
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
struct __declspec(novtable) CAction : CBase { CThing* m_pThing; bool m_active; bool m_finished;
    CAction() : CBase(), m_pThing(0), m_active(false), m_finished(false) { } };
struct CActionPlayCombatAnimation : CAction { bool m_playing; bool m_loop; CCharString m_animation; bool m_interruptible; bool m_blendIn; bool m_blendOut; bool m_holdLastFrame;
    virtual void v00() { }
    CActionPlayCombatAnimation(const CCharString& animation, bool loop, bool interruptible, bool blendIn, bool blendOut, bool holdLastFrame)
        : CAction(), m_playing(false), m_loop(loop), m_animation(animation), m_interruptible(interruptible), m_blendIn(blendIn), m_blendOut(blendOut), m_holdLastFrame(holdLastFrame) { } };
int main()
{
    int s; CCharString name(&s); CActionPlayCombatAnimation a(name, true, false, true, false, true);
    if (g_base != 1 || g_copies != 1 || a.m_pThing != 0 || a.m_active || a.m_finished || a.m_playing || !a.m_loop || a.m_animation.m_pData != &s || a.m_interruptible || !a.m_blendIn || a.m_blendOut || !a.m_holdLastFrame || sizeof(a) != 0x18) { std::printf("00903590_TEST FAIL\n"); return 1; }
    std::printf("00903590_TEST PASS\n"); return 0;
}
