// Behaviour test for 0x008F39A0 CCreatureAction_DragonRearUpInto::Initialise.
#include <cstdio>
#include <cstring>
class CThing;
struct CModeBase { virtual ~CModeBase() {} };
struct CModeDragonRearUp : CModeBase { CThing* o; CThing* t; float timer; CModeDragonRearUp(CThing* a, float c) : o(a), t(0), timer(c) {} };
class CTCCreatureModeManager { public: CModeBase* added; void AddMode(CModeBase* m) { added = m; } };
class CThing { public:
    bool has; CTCCreatureModeManager mm;
    bool GetInterface(CTCCreatureModeManager** pp, long t) { if (t == 0x31 && has) { *pp = &mm; return true; } return false; }
};
struct CBaseIntelligentPointer { CThing* PItem; CThing* GetPItem() const { return PItem; } operator CThing*() const { return GetPItem(); } };
class CCharString { public: const char* t; CCharString(const char* s, long = -1) : t(s) {} };
class CCreatureAction_DragonRearUpInto { public:
    CBaseIntelligentPointer PCreature, PTarget; float Timer; const char* anim; int baseInit;
    void SetCombatAnimation(const CCharString& n) { anim = n.t; }
    void Initialise()
    {
        { CCharString name("REAR_UP_INTO"); SetCombatAnimation(name); }
        ++baseInit;
        CTCCreatureModeManager* modeManager;
        if (PCreature.GetPItem()->GetInterface(&modeManager, 0x31))
            modeManager->AddMode(new CModeDragonRearUp(PCreature, Timer));
    }
};
int main()
{
    CThing o, t; o.has = true; o.mm.added = 0;
    CCreatureAction_DragonRearUpInto a; a.PCreature.PItem = &o; a.PTarget.PItem = &t; a.Timer = 2.5f; a.baseInit = 0;
    a.Initialise();
    CModeDragonRearUp* m = (CModeDragonRearUp*)o.mm.added;
    bool ok = m && m->o == &o && m->t == 0 && m->timer == 2.5f && !std::strcmp(a.anim, "REAR_UP_INTO") && a.baseInit == 1;
    delete m;
    o.has = false; o.mm.added = 0; a.Initialise(); ok = ok && o.mm.added == 0;
    if (!ok) { std::printf("008f39a0_TEST FAIL\n"); return 1; }
    std::printf("008f39a0_TEST PASS\n");
    return 0;
}