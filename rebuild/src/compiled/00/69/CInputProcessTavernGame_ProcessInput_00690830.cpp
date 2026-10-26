// CInputProcessTavernGame::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- hand the input to the hero's current tavern game (game-player component 0xD0), then translate press/held of actions 0x19/0x4c and mouse capture 0x47 into front-end UI events
// Leaf: ProcessInput
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00690830, length 359 bytes, `ret 8`.
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970 (player manager at CMainGameComponent +0x1C, the component
//   global @ 0x013B86A0), CPlayer::GetHeroCreature @ 0x00487DC0, the component map's LowerBound @ 0x0040F020,
//   CTCGamePlayer::GetGame @ 0x005EDC90 (called twice: test, then use), the tavern game's virtual slot 28 (+0x70)
//   ProcessInput(event, processed, process), CInputProcessGameBase::IsEventButtonPressed @ 0x00687A90,
//   IsEventButtonHeld @ 0x00687BD0, IsEventGameAction @ 0x00687CF0, NUISystem::CFrontEndManager::GetInstance
//   @ 0x0041E5F2 and the manager's virtual slot 0 (send a UI event).
// Layout: `this` is engine/CInputProcessGameBase.h (engine/CInputProcessTavernGame.h is the same 0x28 bytes flattened);
//   component bitset at CThing +0x20 (kind 0xD0 = word 6 bit 16, `test ecx,10000h` on +0x38) and the map at +0x44, as in
//   the landed CInputProcessBetting::ProcessInput 0x00691560; mouse movement event +0x0C -> manager +0xB0 as in the
//   landed CInputProcessPhotojournalCapture::ProcessInput 0x00691AE0.
// Shape: `CTCGamePlayer* player = 0; if (HasTC) player = GetTC;` gives the `xor ebx,ebx` preload; each UI arm returns
//   on its own (no SetHandled in this process).
#include "engine/CInputProcessGameBase.h"
class CProcessedInput;
class C2DVector { public:
    float X, Y;
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {}
};
class CInputEvent {
public:
    unsigned char _pad00[0x0c];
    C2DVector     MouseMovementXY;                 // +0x0C
};
namespace NUISystem {
class CFrontEndManager {
public:
    virtual void SendEvent(long event);            // slot 0
    unsigned char _pad04[0xac];
    C2DVector     MouseMovement;                   // +0xB0
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
};
}
class CInputProcessTavernGame;
class CTCTavernGame { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed, CInputProcessTavernGame* process);   // slot 28 (+0x70)
};
class CTCGamePlayer { public:
    CTCTavernGame* GetGame();                      // 0x005EDC90
};
struct STCEntry {                                  // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long           m_kind;
    CTCGamePlayer* m_pTC;
};
struct CTCMap {                                    // the thing's component map at +0x44
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind);   // 0x0040F020
    __forceinline CTCGamePlayer* Find(long kind)
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_tcMask[8];                     // +0x20: which component kinds the thing holds
    unsigned char _pad40[4];
    CTCMap        m_tcs;                           // +0x44
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCGamePlayer* GetTC(long kind) { return m_tcs.Find(kind); }
};
class CPlayer { public:
    CThing* GetHeroCreature();                     // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer();                      // 0x00449970
};
struct CMainGameComponentView {
    unsigned char   _pad[0x1c];
    CPlayerManager* PlayerManager;                 // +0x1C
};
extern CMainGameComponentView* g_pMainGameComponent;   // 0x013B86A0
class CInputProcessTavernGame : public CInputProcessGameBase {
public:
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90
    bool IsEventButtonHeld(const CInputEvent& event);                 // 0x00687BD0
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessTavernGame::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroCreature();
    CTCGamePlayer* player = 0;
    if (hero->HasTC(0xd0))
        player = hero->GetTC(0xd0);
    if (player->GetGame())
        player->GetGame()->ProcessInput(event, processed, this);
    if (IsEventButtonPressed(event)) {
        if (IsEventGameAction(0x19, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(0x1a);
        else if (IsEventGameAction(0x4c, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(0x1b);
    } else if (IsEventButtonHeld(event)) {
        if (IsEventGameAction(0x19, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(0x1c);
        else if (IsEventGameAction(0x4c, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(0x1d);
    } else if (IsEventGameAction(0x47, event)) {
        C2DVector movement(event.MouseMovementXY);
        NUISystem::CFrontEndManager::GetInstance()->MouseMovement = movement;
        NUISystem::CFrontEndManager::GetInstance()->SendEvent(0x19);
    }
}