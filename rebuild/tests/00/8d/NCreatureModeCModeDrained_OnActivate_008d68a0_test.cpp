// Behaviour test for NCreatureMode::CModeDrained::OnActivate (0x008D68A0): type 1 (AI creature) -> SuspendAI(true, 0); type 2 (player
// creature) -> its player AddMode(31, false); any other type -> neither; the stand-mode OnActivate always runs last.
#include <cstdio>
enum EPlayerMode { };
static int gOrder, gSuspendAt, gAddModeAt, gBaseAt; static bool gSuspendArg, gExclusive; static void* gSuspender; static int gMode;
class CPlayer { public:
    void AddMode(EPlayerMode mode, bool exclusive) { gAddModeAt = ++gOrder; gMode = (int)mode; gExclusive = exclusive; }
};
class CClass { public: virtual ~CClass() { } unsigned char Type; unsigned long GetType() const { return Type; } };
static CPlayer gPlayer;
class CThing : public CClass { public: CPlayer& GetPlayer() { return gPlayer; } };
class CThingAICreature : public CThing { public:
    void SuspendAI(bool suspend, void* suspender) { gSuspendAt = ++gOrder; gSuspendArg = suspend; gSuspender = suspender; }
};
class CModeBase { public:
    virtual ~CModeBase() { }
    virtual void OnActivate() { }
    CThing* ParentThing;
};
class CModeStandBase : public CModeBase { public: virtual void OnActivate() { gBaseAt = ++gOrder; } };
class CModeDrained : public CModeStandBase { public: virtual void OnActivate(); };
void CModeDrained::OnActivate()
{
    if (ParentThing->GetType() == 1)
        static_cast<CThingAICreature*>(ParentThing)->SuspendAI(true, 0);
    else if (ParentThing->GetType() == 2)
        ParentThing->GetPlayer().AddMode((EPlayerMode)31, false);
    CModeStandBase::OnActivate();
}
static void Reset() { gOrder = gSuspendAt = gAddModeAt = gBaseAt = 0; gMode = 0; gExclusive = true; gSuspendArg = false; gSuspender = (void*)1; }
int main()
{
    CThingAICreature creature; CModeDrained mode; CModeBase* m = &mode; mode.ParentThing = &creature;
    creature.Type = 1; Reset(); m->OnActivate();
    if (gSuspendAt != 1 || !gSuspendArg || gSuspender != 0 || gAddModeAt != 0 || gBaseAt != 2) { std::printf("008d68a0_TEST FAIL\n"); return 1; }
    creature.Type = 2; Reset(); m->OnActivate();
    if (gSuspendAt != 0 || gAddModeAt != 1 || gMode != 31 || gExclusive || gBaseAt != 2) { std::printf("008d68a0_TEST FAIL\n"); return 1; }
    creature.Type = 5; Reset(); m->OnActivate();
    if (gSuspendAt != 0 || gAddModeAt != 0 || gBaseAt != 1) { std::printf("008d68a0_TEST FAIL\n"); return 1; }
    std::printf("008d68a0_TEST PASS\n");
    return 0;
}