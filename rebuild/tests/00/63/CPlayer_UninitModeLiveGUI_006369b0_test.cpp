// Behaviour test for 0x006369B0 CPlayer::UninitModeLiveGUI: mock interfaces log every call; the body is the source's, verbatim.
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
    CIntelligentPointer<CThingPlayerCreature> PlayerCharacter[1];
    CPlayerInterface* PlayerInterface;
    CThingPlayerCreature* Controlled;
    CInputProcess* InputProcessHeroInformationScreens_ptr;
    CInputProcess* InputProcessInventory_ptr;
    CInputProcess* InputProcessMain_ptr;
    CThingPlayerCreature* GetPControlledCreature() { Log("GetControlled"); return Controlled; }
    void UninitModeLiveGUI(EPlayerMode mode, CProcessedInput* input);
};
void CPlayer::UninitModeLiveGUI(EPlayerMode mode, CProcessedInput* input)
{
    CIntelligentPointer<CThingPlayerCreature>& character = *reinterpret_cast<CIntelligentPointer<CThingPlayerCreature>*>(PlayerCharacter);
    CTCInventory* inventory = 0;
    character->GetTC(&inventory);
    inventory->ClosePCInventory(true);
    CThingPlayerCreature* creature = GetPControlledCreature();
    if (creature && !creature->IsDead()) {
        CTCCombat* combat = 0;
        creature->GetTC(&combat);
        combat->UnsetBlocking();
    }
    InputProcessHeroInformationScreens_ptr->RemoveFromInterface();
    InputProcessInventory_ptr->RemoveFromInterface();
    InputProcessMain_ptr->AddToInterface(*PlayerInterface);
}
int main()
{
    CPlayerInterface pi; CThingPlayerCreature hero, other; CInputProcess h("HeroInfo"), i("Inventory"), m("Main");
    CPlayer p; p.PlayerInterface = &pi; p.PlayerCharacter[0].Item = &hero; p.Controlled = &other;
    p.InputProcessHeroInformationScreens_ptr = &h; p.InputProcessInventory_ptr = &i; p.InputProcessMain_ptr = &m;
    int ok = 1;
    p.UninitModeLiveGUI(PLAYER_MODE_NONE, 0);
    ok &= Check("GetTC(11);ClosePC(1);GetControlled;GetTC(30);UnsetBlocking;Remove(HeroInfo);Remove(Inventory);Add(Main);");
    other.Dead = true;
    p.UninitModeLiveGUI(PLAYER_MODE_NONE, 0);
    ok &= Check("GetTC(11);ClosePC(1);GetControlled;Remove(HeroInfo);Remove(Inventory);Add(Main);");
    p.Controlled = 0;
    p.UninitModeLiveGUI(PLAYER_MODE_NONE, 0);
    ok &= Check("GetTC(11);ClosePC(1);GetControlled;Remove(HeroInfo);Remove(Inventory);Add(Main);");
    if (ok) std::printf("006369b0_TEST PASS\n");
    return ok ? 0 : 1;
}