// CInputProcessBoastUI::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- translate boast-screen buttons into front-end UI events (press/held/release of actions 0x19/0x4c/0x58/0x59, mouse capture on 0x47) and mark the input handled
// Leaf: ?ProcessInput@CInputProcessBoastUI@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// NF478 (r467b, NAME_FIXES_QUEUE nf477, 2026-11-26): was the non-virtual ?ProcessInput@CInputProcessBoastUI@@QAE... on a local class
//   model; respelt onto engine/CInputProcessBoastUI.h FABLE_RETAIL_INPUTPROCESS_VTABLE (Ego_r.pdb / FableWin.pdb public virtual, retail
//   vftable slot 4), so the family vftables 0x00488D10 emits name this row. The base queries come from the retail view of
//   engine/CInputProcessGameBase.h (the same PDB spellings). No body change.
// NF64 (round 7, q10652): the local IsEventButtonPressed / Held / Released redeclarations on the derived class are gone; the calls bind
//   the Ego_r.pdb + FableWin.pdb protected const CInputProcessGameBase members (overlay opt-in FABLE_RETAIL_CINPUTPROCESSGAMEBASE_ISEVENTBUTTON).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00690E70, length 323 bytes, `ret 8`.
// Callees: CInputProcessGameBase::IsEventButtonPressed @ 0x00687A90, IsEventButtonReleased @ 0x00687BD0,
//   IsEventButtonHeld @ 0x00687B30, IsEventGameAction @ 0x00687CF0, NUISystem::CFrontEndManager::GetInstance
//   @ 0x0041E5F2 (`__fastcall`, no arguments), the manager's virtual slot 0 (send a UI event), CProcessedInput::SetAsPerformedEvent
//   @ 0x00A0D390 (manifest "CWomanToAttract::Init" is propagated).
// Layout: `this` is engine/CInputProcessGameBase.h (engine/CInputProcessBoastUI.h is the same 0x28 bytes flattened);
//   the event's mouse movement C2DVector sits at +0x0C and the front-end manager's at +0xB0 (as in the landed
//   CInputProcessPhotojournalCapture::ProcessInput 0x00691AE0, which reaches the same members).
// Shape: every arm ends in `Send(x)` + the shared SetAsPerformedEvent tail; the press and release arms end in the same
//   0x58/0x59 tests, which VC7.1 cross-jumps into one block. The mouse vector is copied into a named local through
//   C2DVector's user copy constructor before GetInstance runs.
#include "engine/EGameAction.h"
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#define FABLE_RETAIL_INPUTPROCESS_ZTARGET
#define FABLE_RETAIL_INPUTPROCESS_MOVEMENT
#include "engine/CInputProcessGameBase.h"
class CProcessedInput {
public:
    void SetAsPerformedEvent();                             // 0x00A0D390 (Ego_r 0x00B74CA0 + FableWin 0x0318F830 ?SetAsPerformedEvent@CProcessedInput@@QAEXXZ; NF33, was SetAsPerformedEvent)
};
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
class CManager {
public:
    virtual void SendEvent(long event);            // slot 0
    unsigned char _pad04[0xac];
    C2DVector     MouseMovement;                   // +0xB0
    static CManager* __fastcall GetInstance();   // 0x0041E5F2
};
}
#include "engine/CInputProcessBoastUI.h"

void CInputProcessBoastUI::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (IsEventButtonPressed(event)) {
        if (IsEventGameAction(GAME_ACTION_INVENTORY_SELECT, event))
            NUISystem::CManager::GetInstance()->SendEvent(0x1a);
        else if (IsEventGameAction(GAME_ACTION_INVENTORY_UNSELECT, event))
            NUISystem::CManager::GetInstance()->SendEvent(5);
        else if (IsEventGameAction(GAME_ACTION_SCROLL_DESCRIPTION_UP, event))
            NUISystem::CManager::GetInstance()->SendEvent(6);
        else if (IsEventGameAction(GAME_ACTION_SCROLL_DESCRIPTION_DOWN, event))
            NUISystem::CManager::GetInstance()->SendEvent(7);
        else
            return;
    } else if (IsEventButtonReleased(event)) {
        if (IsEventGameAction(GAME_ACTION_INVENTORY_SELECT, event))
            NUISystem::CManager::GetInstance()->SendEvent(0x1c);
        else if (IsEventGameAction(GAME_ACTION_INVENTORY_UNSELECT, event))
            NUISystem::CManager::GetInstance()->SendEvent(0x1d);
        else
            return;
    } else if (IsEventButtonHeld(event)) {
        if (IsEventGameAction(GAME_ACTION_INVENTORY_SELECT, event))
            NUISystem::CManager::GetInstance()->SendEvent(0x1f);
        else if (IsEventGameAction(GAME_ACTION_INVENTORY_UNSELECT, event))
            NUISystem::CManager::GetInstance()->SendEvent(0x20);
        else if (IsEventGameAction(GAME_ACTION_SCROLL_DESCRIPTION_UP, event))
            NUISystem::CManager::GetInstance()->SendEvent(6);
        else if (IsEventGameAction(GAME_ACTION_SCROLL_DESCRIPTION_DOWN, event))
            NUISystem::CManager::GetInstance()->SendEvent(7);
        else
            return;
    } else if (IsEventGameAction(GAME_ACTION_MOVE_MOUSE_ON_GUI, event)) {
        C2DVector movement(event.MouseMovementXY);
        NUISystem::CManager::GetInstance()->MouseMovement = movement;
        NUISystem::CManager::GetInstance()->SendEvent(0x19);
    } else
        return;
    processed.SetAsPerformedEvent();
}