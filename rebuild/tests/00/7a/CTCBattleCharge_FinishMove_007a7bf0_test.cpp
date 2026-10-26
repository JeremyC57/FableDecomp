// Behaviour test for CTCBattleCharge::FinishMove (0x007A7BF0).
#include <cstdio>
#include <cstring>

class CThing { public: unsigned char m_stateFlags; };
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer() {}
    CIntelligentPointer(const CIntelligentPointer& o) : CBaseIntelligentPointer() { SetPItem(o.GetPItem()); }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) { if (!(p.GetPItem()->m_stateFlags & 1)) return true; }
    return false;
}
class CCharString { public: const char* text; CCharString(const char* t, long) : text(t) {} };
class CCreatureActionBase { public: int kind; };
class CCreatureAction_UnsheatheItemFromInventory : public CCreatureActionBase { public:
    const char* anim; CThing* item; long a, b;
    CCreatureAction_UnsheatheItemFromInventory(void*, const CCharString& n, CThing* i, long p, long t, CCreatureActionBase*)
    { kind = 1; anim = n.text; item = i; a = p; b = t; }
};
class CCreatureAction_BattleChargeImpact : public CCreatureActionBase { public:
    CThing* left; CThing* right;
    CCreatureAction_BattleChargeImpact(void*, CIntelligentPointer<CThing> l, CIntelligentPointer<CThing> r)
    { kind = 2; left = l.GetPItem(); right = r.GetPItem(); }
};
static int g_kinds[4]; static int g_count = 0; static const char* g_anim = 0; static CThing* g_left = 0; static CThing* g_item = 0;
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(CCreatureActionBase* a)
    {
        g_kinds[g_count++] = a->kind;
        if (a->kind == 1) { g_anim = ((CCreatureAction_UnsheatheItemFromInventory*)a)->anim; g_item = ((CCreatureAction_UnsheatheItemFromInventory*)a)->item; }
        if (a->kind == 2) g_left = ((CCreatureAction_BattleChargeImpact*)a)->left;
        return true;
    }
};
static int g_cleaned = 0;
class CTCBattleCharge {
public:
    CIntelligentPointer<CThing> PItemRemoved, LeftFeetEffect, RightFeetEffect;
    bool StartPoseDelay, HitSomething;
    void CleanUpEffects() { ++g_cleaned; }
    void FinishMove(CThingCreatureBase* creature, bool hitSomething)
    {
        CleanUpEffects();
        StartPoseDelay = true;
        HitSomething = hitSomething;
        if (IS_THING_ALIVE(PItemRemoved)) {
            CCharString animation("ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE", -1);
            CCreatureAction_UnsheatheItemFromInventory a(creature, animation, PItemRemoved.GetPItem(), 100, 250, 0);
            creature->SetCurrentAction(&a);
        }
        if (hitSomething) {
            CCreatureAction_BattleChargeImpact a(creature, LeftFeetEffect, RightFeetEffect);
            creature->SetCurrentAction(&a);
        }
    }
};

int main()
{
    int ok = 1;
    CThing sword; sword.m_stateFlags = 0; CThing foot; foot.m_stateFlags = 0;
    CThingCreatureBase hero; hero.m_stateFlags = 0;
    CTCBattleCharge c; c.PItemRemoved.SetPItem(&sword); c.LeftFeetEffect.SetPItem(&foot); c.StartPoseDelay = false; c.HitSomething = false;
    c.FinishMove(&hero, true);
    if (g_cleaned != 1 || !c.StartPoseDelay || !c.HitSomething || g_count != 2 || g_kinds[0] != 1 || g_kinds[1] != 2) ok = 0;
    if (std::strcmp(g_anim, "ST_ABILITY_FAST_UNSHEATHE_TO_COMBAT_POSE") != 0 || g_item != &sword || g_left != &foot) ok = 0;
    sword.m_stateFlags = 1;
    c.FinishMove(&hero, false);
    if (g_count != 2 || c.HitSomething) ok = 0;
    if (!ok) { std::printf("007a7bf0_TEST FAIL\n"); return 1; }
    std::printf("007a7bf0_TEST PASS\n");
    return 0;
}