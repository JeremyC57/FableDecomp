// Behaviour test for 0x006360A0 CPlayer::InitModeChargeUpWillSpell: mock interfaces log every call; the body is the source's, verbatim.
#include <cstdio>
#include <cstring>

static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static int Check(const char* expect) { int ok = std::strcmp(g_log, expect) == 0; if (!ok) std::printf("got  %s\nwant %s\n", g_log, expect); g_log[0] = 0; return ok; }

enum EPlayerMode { PLAYER_MODE_NONE = 0 };
class CProcessedInput;
class CPlayerInterface { };
class CInputProcess {
public:
    const char* Name;
    CInputProcess(const char* name) : Name(name) { }
    void AddToInterface(CPlayerInterface&) { char b[96]; std::sprintf(b, "Add(%s)", Name); Log(b); }
    void RemoveFromInterface() { char b[96]; std::sprintf(b, "Remove(%s)", Name); Log(b); }
};
class CTCSpecialAbilities { public: void CancelCurrentMutexAbility() { Log("CancelMutex"); } };
class CTCInventoryMap { public: void OpenInventory() { Log("OpenInventory"); } };
class CTCInventory { public: void ClosePCInventory(bool b) { Log(b ? "ClosePC(1)" : "ClosePC(0)"); } };
class CTCCombat { public: void UnsetBlocking() { Log("UnsetBlocking"); } };
class CCreatureMode { public: int Type; int GetType() const { return Type; } };
class CTCCreatureModeManager {
public:
    CCreatureMode Current;
    CCreatureMode& GetCurrentMode() { return Current; }
    void AddMode(int t) { char b[32]; std::sprintf(b, "AddMode(%x)", t); Log(b); }
};
class CThingPlayerCreature {
public:
    bool Dead;
    bool HasAbilities;
    CTCSpecialAbilities Abilities;
    CTCInventoryMap Map;
    CTCInventory Inventory;
    CTCCombat Combat;
    CTCCreatureModeManager Modes;
    CThingPlayerCreature() : Dead(false), HasAbilities(true) { Modes.Current.Type = 0; }
    bool IsDead() const { return Dead; }
    bool GetTC(CTCSpecialAbilities** out) { Log("GetTC(a7)"); *out = &Abilities; return HasAbilities; }
    bool GetTC(CTCInventoryMap** out) { Log("GetTC(1b)"); *out = &Map; return true; }
    bool GetTC(CTCInventory** out) { Log("GetTC(11)"); *out = &Inventory; return true; }
    bool GetTC(CTCCombat** out) { Log("GetTC(30)"); *out = &Combat; return true; }
    bool GetTC(CTCCreatureModeManager** out) { Log("GetTC(31)"); *out = &Modes; return true; }
};
template <class T> class CIntelligentPointer {
public:
    T* Item;
    CIntelligentPointer() : Item(0) { }
    T* GetPItem() const { return Item; }
    T* operator->() const { return Item; }
};

class CPlayer {
public:
    CPlayerInterface* PlayerInterface;
    CThingPlayerCreature* Controlled;
    CInputProcess* InputProcessControlCreature_ptr;
    CInputProcess* InputProcessControlCreatureActivateZTarget_ptr;
    CInputProcess* InputProcessRightStickLookAround_ptr;
    CInputProcess* InputProcessControlCreatureRightStick_ptr;
    CInputProcess* InputProcessJumpingAndRolling_ptr;
    CInputProcess* InputProcessBlock_ptr;
    CInputProcess* InputProcessCreatureMovement_ptr;
    CInputProcess* InputProcessWatchForWillChargeUpThirdPersonModeTermination_ptr;
    CInputProcess* InputProcessSpecialAbilities_ptr;
    CThingPlayerCreature* GetPControlledCreature() { Log("GetControlled"); return Controlled; }
    void InitModeChargeUpWillSpell(EPlayerMode mode, CProcessedInput* input);
};
void CPlayer::InitModeChargeUpWillSpell(EPlayerMode mode, CProcessedInput* input)
{
    InputProcessControlCreature_ptr->AddToInterface(*PlayerInterface);
    InputProcessControlCreatureActivateZTarget_ptr->AddToInterface(*PlayerInterface);
    InputProcessRightStickLookAround_ptr->AddToInterface(*PlayerInterface);
    InputProcessControlCreatureRightStick_ptr->AddToInterface(*PlayerInterface);
    InputProcessJumpingAndRolling_ptr->AddToInterface(*PlayerInterface);
    InputProcessBlock_ptr->AddToInterface(*PlayerInterface);
    InputProcessCreatureMovement_ptr->AddToInterface(*PlayerInterface);
    InputProcessWatchForWillChargeUpThirdPersonModeTermination_ptr->AddToInterface(*PlayerInterface);
    InputProcessSpecialAbilities_ptr->AddToInterface(*PlayerInterface);
    CThingPlayerCreature* creature = GetPControlledCreature();
    if (creature && !creature->IsDead()) {
        CTCCreatureModeManager* modes = 0;
        creature->GetTC(&modes);
        if (modes->GetCurrentMode().GetType() != 0x19)
            modes->AddMode(0x19);
    }
}
int main()
{
    CPlayerInterface pi; CThingPlayerCreature hero;
    CInputProcess a("A"), b("B"), c("C"), d("D"), e("E"), f("F"), g("G"), h("H"), i("I");
    CPlayer p; p.PlayerInterface = &pi; p.Controlled = 0;
    p.InputProcessControlCreature_ptr = &a; p.InputProcessControlCreatureActivateZTarget_ptr = &b;
    p.InputProcessRightStickLookAround_ptr = &c; p.InputProcessControlCreatureRightStick_ptr = &d;
    p.InputProcessJumpingAndRolling_ptr = &e; p.InputProcessBlock_ptr = &f; p.InputProcessCreatureMovement_ptr = &g;
    p.InputProcessWatchForWillChargeUpThirdPersonModeTermination_ptr = &h; p.InputProcessSpecialAbilities_ptr = &i;
    const char* adds = "Add(A);Add(B);Add(C);Add(D);Add(E);Add(F);Add(G);Add(H);Add(I);GetControlled;";
    char want[256];
    int ok = 1;
    p.InitModeChargeUpWillSpell(PLAYER_MODE_NONE, 0);
    ok &= Check(adds);
    p.Controlled = &hero;
    p.InitModeChargeUpWillSpell(PLAYER_MODE_NONE, 0);
    std::sprintf(want, "%sGetTC(31);AddMode(19);", adds); ok &= Check(want);
    hero.Modes.Current.Type = 0x19;
    p.InitModeChargeUpWillSpell(PLAYER_MODE_NONE, 0);
    std::sprintf(want, "%sGetTC(31);", adds); ok &= Check(want);
    hero.Dead = true;
    p.InitModeChargeUpWillSpell(PLAYER_MODE_NONE, 0);
    ok &= Check(adds);
    if (ok) std::printf("006360a0_TEST PASS\n");
    return ok ? 0 : 1;
}