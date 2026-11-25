// CInputProcessInGameMenu::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- drive the in-game menu while no sub-page (+0xC8) is open: analogue keys 0x11/0x12 past +-0.6 send front-end events, pressed actions 0x25..0x28/0x21/0x22 send navigation events (left/right go to an active slider first), actions 3/2 close the menu (2 also queues game event 0x1A for the live hero), released 0x25/0x26 send 0/1
// Leaf: ProcessInput
// NF64 (round 7, q10652): the local IsEventButtonPressed / Held / Released redeclarations on the derived class are gone; the calls bind
//   the Ego_r.pdb + FableWin.pdb protected const CInputProcessGameBase members (overlay opt-in FABLE_RETAIL_CINPUTPROCESSGAMEBASE_ISEVENTBUTTON).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00690480, 848 bytes, `ret 8` (/GS cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9).
#define FABLE_RETAIL_INPUTPROCESS_GAMEACTIONS   // NF37: the base-class queries (Ego_r/FableWin spellings), hdr/README.md
#include "engine/EGameAction.h"
#define FABLE_RETAIL_CINPUTPROCESSGAMEBASE_ISEVENTBUTTON   // NF64 (q10652): protected const base button queries, hdr/README.md
#include "engine/CInputProcessGameBase.h"
struct CMemoryBuffer {
};
// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x009F1760 ?AddData@CGameEvent@@QAEXJPBX@Z (was ?Copy@CMemoryBuffer@@QAEXKPBX@Z)
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddLong(long value) { reinterpret_cast<CGameEvent*>(this)->AddData(sizeof(value), &value); }
 public: void AddData(long, void const*); };
class CProcessedInput {
public:
    void SetAsPerformedEvent();                             // 0x00A0D390 (Ego_r 0x00B74CA0 + FableWin 0x0318F830 ?SetAsPerformedEvent@CProcessedInput@@QAEXXZ; NF33, was SetAsPerformedEvent)
};
class C2DVector { public: float X, Y; };
// Naming (NF160, 2026-11-20, q4300): the local CInputEvent model replaced by the shared engine/CInputEvent.h (Ego_r.pdb / FableWin.pdb
//   spellings): 0x00A03B40 GetKey_00a03b40 -> GetType; 0x00A043A0 GetAnalogueVector -> GetJoystickPosition.
#define FABLE_RETAIL_CINPUTEVENT_ACCESSORS
#include "engine/CInputEvent.h"
namespace NUISystem {
class CManager {
public:
    virtual void SendEvent(long event);            // slot 0
    static CManager* __fastcall GetInstance();   // 0x0041E5F2
};
}
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)   // 0x0043DBD0 (T = CThingPlayerCreature*: fed by GetPControlledCreature 0x00487DC0 at 25 of 32 retail call sites)
{
    return thing != 0 && !(reinterpret_cast<const unsigned char*>(thing)[0x91] & 1);   // CThing dead bit +0x91
}
class CThingCreatureBase : public CThing { public:
};
class CThingPlayerCreature : public CThingCreatureBase { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;         // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;                 // +0x1C
    void AddGameEvent(const CGameEvent& event);    // 0x0041649C
};
class CTCInGameMenu { public:
    unsigned char _pad00[0xc8];
    long          CurrentSubPage;                  // +0xC8 (descriptive; -1 = none)
    bool IsAnySliderReceivingInput() const;              // 0x0047D483
    bool IsQuitMenuReceivingInput() const;               // 0x0047D537
    bool IsListReceivingInputA();                  // 0x0047D6F2 (descriptive)
    bool IsLoadMenuReceivingInput() const;         // 0x0047D648 (NF334: Ego_r + FableWin)
    void CloseInGameMenuViaGameEvent();            // 0x0047D21C
};
// Layout: engine/CInputProcessInGameMenu.h is a flattened view (PInGameMenu +0x28); the game-base methods need the
// CInputProcessGameBase base, so the derived member is declared here.
class CInputProcessInGameMenu : public CInputProcessGameBase {
public:
    CTCInGameMenu* PInGameMenu;                    // +0x28
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessInGameMenu::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (PInGameMenu->CurrentSubPage != -1)
        return;
    if (event.GetType() == 0x11) {
        C2DVector stick = event.GetJoystickPosition();
        if (stick.Y > 0.6f) {
            NUISystem::CManager::GetInstance()->SendEvent(1);
            processed.SetAsPerformedEvent();
        } else if (stick.Y < -0.6f) {
            NUISystem::CManager::GetInstance()->SendEvent(0);
            processed.SetAsPerformedEvent();
        }
        if (stick.X > 0.6f) {
            NUISystem::CManager::GetInstance()->SendEvent(0x15);
            processed.SetAsPerformedEvent();
        } else if (stick.X < -0.6f) {
            NUISystem::CManager::GetInstance()->SendEvent(0x14);
            processed.SetAsPerformedEvent();
        }
    } else if (event.GetType() == 0x12) {
        C2DVector stick = event.GetJoystickPosition();
        if (stick.Y > 0.6f) {
            NUISystem::CManager::GetInstance()->SendEvent(7);
            processed.SetAsPerformedEvent();
        } else if (stick.Y < -0.6f) {
            NUISystem::CManager::GetInstance()->SendEvent(6);
            processed.SetAsPerformedEvent();
        }
    } else if (IsEventButtonPressed(event)) {
        if (IsEventGameAction(GAME_ACTION_INVENTORY_UP, event)) {
            NUISystem::CManager::GetInstance()->SendEvent(0);
            processed.SetAsPerformedEvent();
        } else if (IsEventGameAction(GAME_ACTION_INVENTORY_DOWN, event)) {
            NUISystem::CManager::GetInstance()->SendEvent(1);
            processed.SetAsPerformedEvent();
        } else if (IsEventGameAction(GAME_ACTION_INVENTORY_LEFT, event)) {
            if (PInGameMenu->IsAnySliderReceivingInput())
                NUISystem::CManager::GetInstance()->SendEvent(0x14);
            else if (!PInGameMenu->IsQuitMenuReceivingInput() && !PInGameMenu->IsListReceivingInputA() && !PInGameMenu->IsLoadMenuReceivingInput())
                NUISystem::CManager::GetInstance()->SendEvent(5);
            processed.SetAsPerformedEvent();
        } else if (IsEventGameAction(GAME_ACTION_INVENTORY_RIGHT, event)) {
            if (PInGameMenu->IsAnySliderReceivingInput())
                NUISystem::CManager::GetInstance()->SendEvent(0x15);
            else if (!PInGameMenu->IsQuitMenuReceivingInput() && !PInGameMenu->IsListReceivingInputA() && !PInGameMenu->IsLoadMenuReceivingInput())
                NUISystem::CManager::GetInstance()->SendEvent(4);
            processed.SetAsPerformedEvent();
        } else if (IsEventGameAction(GAME_ACTION_INVENTORY_A, event)) {
            NUISystem::CManager::GetInstance()->SendEvent(4);
            processed.SetAsPerformedEvent();
        } else if (IsEventGameAction(GAME_ACTION_INVENTORY_B, event)) {
            NUISystem::CManager::GetInstance()->SendEvent(5);
            processed.SetAsPerformedEvent();
        } else if (IsEventGameAction(GAME_ACTION_OPEN_IN_GAME_MENU, event)) {
            PInGameMenu->CloseInGameMenuViaGameEvent();
            processed.SetAsPerformedEvent();
        } else if (IsEventGameAction(GAME_ACTION_OPEN_INVENTORY, event)) {
            PInGameMenu->CloseInGameMenuViaGameEvent();
            CThingPlayerCreature* const& hero = CMainGameComponent::Get()->PlayerManager->GetMainPlayer().GetPControlledCreature();
            if (IS_THING_ALIVE(hero)) {
                CGameEvent gameEvent(0x1a);
                gameEvent.Player = hero->GetPlayerNumber();
                gameEvent.AddLong(4);
                CMainGameComponent::Get()->AddGameEvent(gameEvent);
            }
            processed.SetAsPerformedEvent();
        }
    } else if (IsEventButtonHeld(event)) {
        if (IsEventGameAction(GAME_ACTION_INVENTORY_UP, event)) {
            NUISystem::CManager::GetInstance()->SendEvent(0);
            processed.SetAsPerformedEvent();
        } else if (IsEventGameAction(GAME_ACTION_INVENTORY_DOWN, event)) {
            NUISystem::CManager::GetInstance()->SendEvent(1);
            processed.SetAsPerformedEvent();
        }
    }
}