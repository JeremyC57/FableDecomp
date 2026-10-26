// Behaviour test for CTCActionUseBlowHorn::vfunc_28 @ 0x007EB9B0: the base slot always runs; a live creature
// gets a BlowHorn action aimed at the horn thing, a null or dead creature gets nothing; the temporary
// action is destroyed afterwards.
#include <cstdio>

class CThing { public: unsigned char m_flags91; };
class CCreatureActionBase;
static int g_baseCalls = 0, g_actionsAlive = 0;

class CCreatureActionBase { public:
    CThing* m_target;
    CCreatureActionBase() : m_target(0) { ++g_actionsAlive; }
    virtual ~CCreatureActionBase() { --g_actionsAlive; }
};

class CThingCreatureBase : public CThing { public:
    CThing* lastTarget; int actions;
    bool SetCurrentAction(CCreatureActionBase* action) { lastTarget = action->m_target; ++actions; return true; }
};

struct CBaseIntelligentPointer {
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    ~CBaseIntelligentPointer() {}
};

class CCreatureAction_BlowHorn : public CCreatureActionBase { public:
    CBaseIntelligentPointer PObjectToUse;
    CCreatureAction_BlowHorn(CThingCreatureBase*, CThing* horn) { m_target = horn; PObjectToUse.m_pItem = horn; }
};

class CTCActionUseBase { public:
    CThing* ParentThing;
    void vfunc_28(CThingCreatureBase*) { ++g_baseCalls; }
};

struct CTCActionUseBlowHorn : CTCActionUseBase {
    void vfunc_28(CThingCreatureBase* creature)
    {
        CTCActionUseBase::vfunc_28(creature);
        if (creature != 0 && !(creature->m_flags91 & 1)) {
            CThing* horn = ParentThing;
            CCreatureAction_BlowHorn action(creature, horn);
            creature->SetCurrentAction(&action);
        }
    }
};

int main()
{
    CThing horn; horn.m_flags91 = 0;
    CThingCreatureBase hero; hero.m_flags91 = 0; hero.lastTarget = 0; hero.actions = 0;
    CTCActionUseBlowHorn use; use.ParentThing = &horn;
    use.vfunc_28(&hero);
    bool ok = g_baseCalls == 1 && hero.actions == 1 && hero.lastTarget == &horn && g_actionsAlive == 0;
    hero.m_flags91 = 1;                         // dead
    use.vfunc_28(&hero);
    if (g_baseCalls != 2 || hero.actions != 1) ok = false;
    use.vfunc_28(0);
    if (g_baseCalls != 3 || hero.actions != 1) ok = false;
    if (!ok) { std::printf("007eb9b0_TEST FAIL\n"); return 1; }
    std::printf("007eb9b0_TEST PASS\n");
    return 0;
}