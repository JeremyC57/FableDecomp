// CInputProcessInGameMenu::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- drive the in-game menu while no sub-page (+0xC8) is open: analogue keys 0x11/0x12 past +-0.6 send front-end events, pressed actions 0x25..0x28/0x21/0x22 send navigation events (left/right go to an active slider first), actions 3/2 close the menu (2 also queues game event 0x1A for the live hero), released 0x25/0x26 send 0/1
// Leaf: ProcessInput
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00690480, 848 bytes, `ret 8` (/GS cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9).
#include "engine/CInputProcessGameBase.h"
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};
class CGameEvent : public CMemoryBuffer { public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddLong(long value) { Copy(sizeof(value), &value); }
};
class CProcessedInput {
public:
    void SetHandled();                             // 0x00A0D390
};
class C2DVector { public: float X, Y; };
class CInputEvent {
public:
    long GetKey_00a03b40() const;                           // 0x00A03B40
    void GetAnalogueVector(C2DVector& vector) const;   // 0x00A043A0
};
namespace NUISystem {
class CManager {
public:
    virtual void SendEvent(long event);            // slot 0
    static CManager* __fastcall GetInstance();   // 0x0041E5F2
};
}
class CThing { public:
};
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)   // 0x0043DBD0 (T = CThingPlayerCreature*: fed by GetPControlledCreature 0x00487DC0 at 25 of 32 retail call sites)
{
    return thing != 0 && !(reinterpret_cast<const unsigned char*>(thing)[0x91] & 1);   // CThing dead bit +0x91
}
class CThingCreatureBase : public CThing { public:
    unsigned char _pad_0x00[0x90];
    signed char   PlayerNumber;                    // +0x90 (as in the landed CTCInGameMenu::CloseInGameMenuViaGameEvent 0x0047D21C)
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
class CMainGameComponent { public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;                 // +0x1C
    void AddGameEvent(const CGameEvent& event);    // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
class CTCInGameMenu { public:
    unsigned char _pad00[0xc8];
    long          CurrentSubPage;                  // +0xC8 (descriptive; -1 = none)
    bool IsAnySliderReceivingInput();              // 0x0047D483
    bool IsQuitMenuReceivingInput();               // 0x0047D537
    bool IsListReceivingInputA();                  // 0x0047D6F2 (descriptive)
    bool IsListReceivingInputB();                  // 0x0047D648 (descriptive)
    void CloseInGameMenuViaGameEvent();            // 0x0047D21C
};
// Layout: engine/CInputProcessInGameMenu.h is a flattened view (PInGameMenu +0x28); the game-base methods need the
// CInputProcessGameBase base, so the derived member is declared here.
class CInputProcessInGameMenu : public CInputProcessGameBase {
public:
    CTCInGameMenu* PInGameMenu;                    // +0x28
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90
    bool IsEventButtonReleased(const CInputEvent& event);             // 0x00687B30
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessInGameMenu::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (PInGameMenu->CurrentSubPage != -1)
        return;
    if (event.GetKey_00a03b40() == 0x11) {
        C2DVector stick;
        event.GetAnalogueVector(stick);
        if (stick.Y > 0.6f) {
            NUISystem::CManager::GetInstance()->SendEvent(1);
            processed.SetHandled();
        } else if (stick.Y < -0.6f) {
            NUISystem::CManager::GetInstance()->SendEvent(0);
            processed.SetHandled();
        }
        if (stick.X > 0.6f) {
            NUISystem::CManager::GetInstance()->SendEvent(0x15);
            processed.SetHandled();
        } else if (stick.X < -0.6f) {
            NUISystem::CManager::GetInstance()->SendEvent(0x14);
            processed.SetHandled();
        }
    } else if (event.GetKey_00a03b40() == 0x12) {
        C2DVector stick;
        event.GetAnalogueVector(stick);
        if (stick.Y > 0.6f) {
            NUISystem::CManager::GetInstance()->SendEvent(7);
            processed.SetHandled();
        } else if (stick.Y < -0.6f) {
            NUISystem::CManager::GetInstance()->SendEvent(6);
            processed.SetHandled();
        }
    } else if (IsEventButtonPressed(event)) {
        if (IsEventGameAction(0x25, event)) {
            NUISystem::CManager::GetInstance()->SendEvent(0);
            processed.SetHandled();
        } else if (IsEventGameAction(0x26, event)) {
            NUISystem::CManager::GetInstance()->SendEvent(1);
            processed.SetHandled();
        } else if (IsEventGameAction(0x27, event)) {
            if (PInGameMenu->IsAnySliderReceivingInput())
                NUISystem::CManager::GetInstance()->SendEvent(0x14);
            else if (!PInGameMenu->IsQuitMenuReceivingInput() && !PInGameMenu->IsListReceivingInputA() && !PInGameMenu->IsListReceivingInputB())
                NUISystem::CManager::GetInstance()->SendEvent(5);
            processed.SetHandled();
        } else if (IsEventGameAction(0x28, event)) {
            if (PInGameMenu->IsAnySliderReceivingInput())
                NUISystem::CManager::GetInstance()->SendEvent(0x15);
            else if (!PInGameMenu->IsQuitMenuReceivingInput() && !PInGameMenu->IsListReceivingInputA() && !PInGameMenu->IsListReceivingInputB())
                NUISystem::CManager::GetInstance()->SendEvent(4);
            processed.SetHandled();
        } else if (IsEventGameAction(0x21, event)) {
            NUISystem::CManager::GetInstance()->SendEvent(4);
            processed.SetHandled();
        } else if (IsEventGameAction(0x22, event)) {
            NUISystem::CManager::GetInstance()->SendEvent(5);
            processed.SetHandled();
        } else if (IsEventGameAction(3, event)) {
            PInGameMenu->CloseInGameMenuViaGameEvent();
            processed.SetHandled();
        } else if (IsEventGameAction(2, event)) {
            PInGameMenu->CloseInGameMenuViaGameEvent();
            CThingPlayerCreature* const& hero = g_pMainGameComponent->PlayerManager->GetMainPlayer().GetPControlledCreature();
            if (IS_THING_ALIVE(hero)) {
                CGameEvent gameEvent(0x1a);
                gameEvent.Player = hero->PlayerNumber;
                gameEvent.AddLong(4);
                g_pMainGameComponent->AddGameEvent(gameEvent);
            }
            processed.SetHandled();
        }
    } else if (IsEventButtonReleased(event)) {
        if (IsEventGameAction(0x25, event)) {
            NUISystem::CManager::GetInstance()->SendEvent(0);
            processed.SetHandled();
        } else if (IsEventGameAction(0x26, event)) {
            NUISystem::CManager::GetInstance()->SendEvent(1);
            processed.SetHandled();
        }
    }
}