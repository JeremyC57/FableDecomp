// Behaviour test for 0x008C7FD0 CCreatureAction_BanditKingThrowWeapons ctor: base gets (owner,-1,0.75,anim("BK_THROW_WEAPONS"),{0,0},0,0); Target = target.
#include <cstdio>
#include <cstring>
class CThingCreatureBase { public: int id; };
class CAnimationEntry { public: int id; };
class CBaseObject;
class CCharString { public: const char* t; CCharString(const char* s, long = -1) : t(s) {} };
class CSoundPair { public: long FirstSound, LastSound; CSoundPair() : FirstSound(0), LastSound(0) {} };
static CAnimationEntry g_anim;
static const char* g_lookedUp = 0;
CAnimationEntry* GetAnimationEntry(CThingCreatureBase*, const CCharString& n) { g_lookedUp = n.t; return &g_anim; }
class CCreatureActionBase { public:
    CThingCreatureBase* owner; float a, b; CAnimationEntry* anim; long s0, s1, c, d;
    CCreatureActionBase(CThingCreatureBase* o, float a_, float b_, CAnimationEntry* an, const CSoundPair& s, long c_, long d_)
        : owner(o), a(a_), b(b_), anim(an), s0(s.FirstSound), s1(s.LastSound), c(c_), d(d_) {}
};
struct CBaseIntelligentPointer {
    CBaseObject* PItem;
    CBaseIntelligentPointer() : PItem(0) {}
    CBaseObject* GetPItem() const { return PItem; }
    void SetPItem(CBaseObject* i) { PItem = i; }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {
    CIntelligentPointerCreature() {}
    CIntelligentPointerCreature(CThingCreatureBase* i) { SetPItem((CBaseObject*)i); }
    CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& o) { SetPItem(o.GetPItem()); return *this; }
};
class CCreatureAction_BanditKingThrowWeapons : public CCreatureActionBase { public:
    CIntelligentPointerCreature Target;
    CCreatureAction_BanditKingThrowWeapons(CThingCreatureBase* owner, CThingCreatureBase* target)
        : CCreatureActionBase(owner, -1.0f, 0.75f, GetAnimationEntry(owner, "BK_THROW_WEAPONS"), CSoundPair(), 0, 0)
    { Target = CIntelligentPointerCreature(target); }
};
int main()
{
    CThingCreatureBase o, t;
    CCreatureAction_BanditKingThrowWeapons a(&o, &t);
    if (a.owner != &o || a.a != -1.0f || a.b != 0.75f || a.anim != &g_anim || a.s0 || a.s1 || a.c || a.d
        || !g_lookedUp || std::strcmp(g_lookedUp, "BK_THROW_WEAPONS") || a.Target.GetPItem() != (CBaseObject*)&t) {
        std::printf("008c7fd0_TEST FAIL\n"); return 1;
    }
    std::printf("008c7fd0_TEST PASS\n");
    return 0;
}