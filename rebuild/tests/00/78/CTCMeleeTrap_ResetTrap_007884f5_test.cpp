// Behaviour test for 007884f5: body copied from src/007884f5.cpp (CTCMeleeTrap::ResetTrap), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CCharString { const char* text; };
struct CAnimFilter {
    virtual bool Accepts(const void* entry) const { return false; }
};
struct CAnimFilterNoGroup : CAnimFilter {
    CAnimFilterNoGroup() { }
    virtual bool Accepts(const void* entry) const { return true; }
};
static bool g_hasAnim = false; static int g_finds = 0; static const CCharString* g_findName = 0; static bool g_filterNoGroup = false;
struct CAnimationSet {
    bool FindAnimationByName(const CCharString& name, const CAnimFilter& filter) { ++g_finds; g_findName = &name; g_filterNoGroup = filter.Accepts(0); return g_hasAnim; }
};
static int g_destroyed = 0;
struct CAppearanceDef {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long          m_count;
    unsigned char _pad_0x08[0x34 - 0x08];
    CAnimationSet m_animations;
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
struct CAppearanceDefPointer {
    CAppearanceDef* m_pDef;
    CAppearanceDefPointer() : m_pDef(0) { }
    CAppearanceDef* operator->() const { return m_pDef; }
};
static CAppearanceDef* g_appearance = 0; static int g_getSubDef = 0;
struct CDefClassBase {
    bool GetSubDef(CAppearanceDefPointer& out) { ++g_getSubDef; ++g_appearance->m_count; out.m_pDef = g_appearance; return true; }
};
struct CThing {
    unsigned char  _pad_0x00[0x70];
    CDefClassBase* PBaseDef;
};
CCharString g_TrapResetAnimationName = { "TRAP_RESET" };
static int g_inits = 0; static const CCharString* g_initName = 0; static bool g_initLoop = true;
struct CTCMeleeTrap {
    void*         __vftable;
    CThing*       m_pThing;
    unsigned char _pad_0x08[0x60 - 0x08];
    bool          PlayingContinuous;
    void InitialiseAnimation(const CCharString& animationName, bool loop) { ++g_inits; g_initName = &animationName; g_initLoop = loop; }
    void ResetTrap();
};
void CTCMeleeTrap::ResetTrap()
{
    CAppearanceDefPointer appearance;
    PlayingContinuous = false;
    m_pThing->PBaseDef->GetSubDef(appearance);
    CAppearanceDef* def = appearance.m_pDef;
    if (def->m_animations.FindAnimationByName(g_TrapResetAnimationName, CAnimFilterNoGroup()))
        InitialiseAnimation(g_TrapResetAnimationName, false);
    if (def)
        def->Release();
}
int main()
{
    static CThing thing; static CDefClassBase classDef; static CAppearanceDef app; app.m_count = 1; g_appearance = &app; thing.PBaseDef = &classDef;
    CTCMeleeTrap trap; memset(&trap, 0, sizeof(trap)); trap.m_pThing = &thing; trap.PlayingContinuous = true;
    // no TRAP_RESET animation: the continuous flag is cleared, nothing started, the def released
    trap.ResetTrap();
    CHECK(!trap.PlayingContinuous && g_getSubDef == 1 && g_finds == 1 && g_findName == &g_TrapResetAnimationName && g_filterNoGroup); CHECK(g_inits == 0 && app.m_count == 1 && g_destroyed == 0);
    // the animation exists: started without looping
    g_hasAnim = true; trap.PlayingContinuous = true; trap.ResetTrap();
    CHECK(!trap.PlayingContinuous && g_inits == 1 && g_initName == &g_TrapResetAnimationName && !g_initLoop); CHECK(app.m_count == 1);
    // a def held only by the lookup is destroyed
    app.m_count = 0; trap.ResetTrap(); CHECK(g_destroyed == 1 && g_inits == 2);
    if (fails == 0) printf("007884f5_TEST PASS\n"); else printf("007884f5_TEST FAIL\n");
    return fails;
}