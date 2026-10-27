// Behaviour test for 0x00634890 CPlayer::InitModeClickPastText: mock interfaces log every call; the body is the source's, verbatim.
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
    CInputProcess* InputProcessClickPastText_ptr;
    CInputProcess* InputProcessCutScene_ptr;
    void InitModeClickPastText(EPlayerMode mode, CProcessedInput* input);
};
void CPlayer::InitModeClickPastText(EPlayerMode mode, CProcessedInput* input)
{
    InputProcessClickPastText_ptr->AddToInterface(*PlayerInterface);
    InputProcessCutScene_ptr->AddToInterface(*PlayerInterface);
    CIntelligentPointer<CThingPlayerCreature>& character = *reinterpret_cast<CIntelligentPointer<CThingPlayerCreature>*>(PlayerCharacter);
    if (character.GetPItem() && !character->IsDead()) {
        CTCSpecialAbilities* abilities;
        if (character->GetTC(&abilities))
            abilities->CancelCurrentMutexAbility();
    }
}
int main()
{
    CPlayerInterface pi; CInputProcess c("ClickPastText"), s("CutScene"); CThingPlayerCreature hero;
    CPlayer p; p.PlayerInterface = &pi; p.InputProcessClickPastText_ptr = &c; p.InputProcessCutScene_ptr = &s;
    int ok = 1;
    p.InitModeClickPastText(PLAYER_MODE_NONE, 0);
    ok &= Check("Add(ClickPastText);Add(CutScene);");
    p.PlayerCharacter[0].Item = &hero;
    p.InitModeClickPastText(PLAYER_MODE_NONE, 0);
    ok &= Check("Add(ClickPastText);Add(CutScene);GetTC(a7);CancelMutex;");
    hero.HasAbilities = false;
    p.InitModeClickPastText(PLAYER_MODE_NONE, 0);
    ok &= Check("Add(ClickPastText);Add(CutScene);GetTC(a7);");
    hero.Dead = true;
    p.InitModeClickPastText(PLAYER_MODE_NONE, 0);
    ok &= Check("Add(ClickPastText);Add(CutScene);");
    if (ok) std::printf("00634890_TEST PASS\n");
    return ok ? 0 : 1;
}