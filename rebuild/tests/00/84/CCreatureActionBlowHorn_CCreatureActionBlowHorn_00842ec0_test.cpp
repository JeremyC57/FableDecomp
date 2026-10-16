// Behaviour test for 0x00842EC0 CCreatureAction_BlowHorn ctor.
#include <cstdio>
#include <cstring>
struct CThing { int id; };
struct CAnimationEntry { int id; };
struct CCharString { const char* t; CCharString(const char* s, long = -1) : t(s) {} };
struct CSoundPair { long FirstSound, LastSound; CSoundPair() : FirstSound(0), LastSound(0) {} };
static CAnimationEntry g_anim; static const char* g_animName = 0;
struct CThingCreatureBase {
    const char* soundName;
    CSoundPair GetSoundPair(const CCharString& n) { soundName = n.t; CSoundPair s; s.FirstSound = 5; s.LastSound = 6; return s; }
};
CAnimationEntry* GetAnimationEntry(CThingCreatureBase*, const CCharString& n) { g_animName = n.t; return &g_anim; }
struct CCreatureActionBase {
    CThingCreatureBase* owner; float a, b; CAnimationEntry* anim; long s0, s1, c, d;
    CCreatureActionBase(CThingCreatureBase* o, float a_, float b_, CAnimationEntry* an, const CSoundPair& s, long c_, long d_)
        : owner(o), a(a_), b(b_), anim(an), s0(s.FirstSound), s1(s.LastSound), c(c_), d(d_) {}
};
struct CIntelligentPointerThing { CThing* PItem; CIntelligentPointerThing(CThing* i) : PItem(i) {} };
struct CCreatureAction_BlowHorn : CCreatureActionBase {
    CIntelligentPointerThing PObjectToUse;
    CCreatureAction_BlowHorn(CThingCreatureBase* owner, CThing* horn)
        : CCreatureActionBase(owner, -1.0f, 1.0f, GetAnimationEntry(owner, "BLOW_KNOTHOLE_HORN"), owner->GetSoundPair("HORN_BLOW"), 0, 0)
        , PObjectToUse(horn) {}
};
int main()
{
    CThingCreatureBase o; o.soundName = 0; CThing h;
    CCreatureAction_BlowHorn a(&o, &h);
    if (a.owner != &o || a.a != -1.0f || a.b != 1.0f || a.anim != &g_anim || a.s0 != 5 || a.s1 != 6 || a.c || a.d
        || std::strcmp(g_animName, "BLOW_KNOTHOLE_HORN") || std::strcmp(o.soundName, "HORN_BLOW") || a.PObjectToUse.PItem != &h) {
        std::printf("00842ec0_TEST FAIL\n"); return 1;
    }
    std::printf("00842ec0_TEST PASS\n");
    return 0;
}