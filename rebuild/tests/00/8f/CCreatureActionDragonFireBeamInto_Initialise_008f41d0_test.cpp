// Behaviour test for 0x008F41D0 CCreatureAction_DragonFireBeamInto::Initialise.
#include <cstdio>
#include <cstring>
class CThing;
struct CModeBase { virtual ~CModeBase() {} };
struct CModeDragonFireBeam : CModeBase { CThing* o; CThing* t; float timer; CModeDragonFireBeam(CThing* a, CThing* b, float c) : o(a), t(b), timer(c) {} };
class CTCCreatureModeManager { public: CModeBase* added; void AddMode(CModeBase* m) { added = m; } };
class CThing { public:
    bool has; CTCCreatureModeManager mm;
    bool GetInterface(CTCCreatureModeManager** pp, long t) { if (t == 0x31 && has) { *pp = &mm; return true; } return false; }
};
struct CBaseIntelligentPointer { CThing* PItem; CThing* GetPItem() const { return PItem; } operator CThing*() const { return GetPItem(); } };
class CCharString { public: const char* t; CCharString(const char* s, long = -1) : t(s) {} };
class CCreatureAction_DragonFireBeamInto { public:
    CBaseIntelligentPointer PCreature, PTarget; float Timer; const char* anim; int baseInit;
    void SetCombatAnimation(const CCharString& n) { anim = n.t; }
    void Initialise()
    {
        { CCharString name("FIRE_BEAM_INTO"); SetCombatAnimation(name); }
        ++baseInit;
        CTCCreatureModeManager* modeManager;
        if (PCreature.GetPItem()->GetInterface(&modeManager, 0x31))
            modeManager->AddMode(new CModeDragonFireBeam(PCreature, PTarget, Timer));
    }
};
int main()
{
    CThing o, t; o.has = true; o.mm.added = 0;
    CCreatureAction_DragonFireBeamInto a; a.PCreature.PItem = &o; a.PTarget.PItem = &t; a.Timer = 2.5f; a.baseInit = 0;
    a.Initialise();
    CModeDragonFireBeam* m = (CModeDragonFireBeam*)o.mm.added;
    bool ok = m && m->o == &o && m->t == &t && m->timer == 2.5f && !std::strcmp(a.anim, "FIRE_BEAM_INTO") && a.baseInit == 1;
    delete m;
    o.has = false; o.mm.added = 0; a.Initialise(); ok = ok && o.mm.added == 0;
    if (!ok) { std::printf("008f41d0_TEST FAIL\n"); return 1; }
    std::printf("008f41d0_TEST PASS\n");
    return 0;
}