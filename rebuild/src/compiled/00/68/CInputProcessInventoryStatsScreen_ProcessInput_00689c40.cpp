// CInputProcessInventoryStatsScreen::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- refresh the process, then page the stats screen: analogue events 0x11/0x12 past +-0.6 on the stick's Y axis send front-end UI events 1/0 and 7/6, presses of actions 0x25/0x26/0x21/0x22 send 0/1/4/5 and releases of 0x25/0x26 send 0/1; a sent event marks the input handled
// Leaf: ProcessInput
// NF179 (PRIORITY q11981): IsEventButtonReleased 0x00687B30 respelled in place on the row's existing local CInputProcessGameBase model to
//   Ego_r.pdb / FableWin.pdb ?IsEventButtonReleased@CInputProcessGameBase@@IBE_NABVCInputEvent@@@Z (protected const; the self row and 14
//   other landed referencers bind it; engine/CInputProcessGameBase.h); was the public non-const QAE form.
// NF33 (2026-11-16): IsEventGameAction / GetControlMovementFromGameActionEvent respelled to the Ego_r.pdb + FableWin.pdb
//   CInputProcessGameBase spellings (EGameAction, const; the movement vector by reference).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00689C40, length 335 bytes, `ret 8`.
// Callees: the process's own virtual slot 9 (+0x24, no arguments), CInputEvent::GetEventType @ 0x00A03B40,
//   CInputEvent::GetAnalogueVector @ 0x00A043A0 (C2DVector by hidden pointer; manifest "GetDefaultVal<CEngineGraphic>"
//   is propagated; names as in the landed CInputProcessFirstPerson::ProcessInput 0x0068CAC0),
//   CInputProcessGameBase::IsEventButtonPressed @ 0x00687A90, IsEventButtonReleased @ 0x00687B30, IsEventGameAction
//   @ 0x00687CF0, NUISystem::CManager::GetInstance @ 0x0041E5F2 + slot 0, CProcessedInput::SetAsPerformedEvent
//   @ 0x00A0D390. Float pool: 0.6f @ 0x0125BEC8, -0.6f @ 0x0125BED4.
// Layout: engine/CInputProcessInventoryStatsScreen.h (0x28 bytes, flattened over CInputProcessGameBase) -- modelled
//   locally because the process's own virtual is called (the header's vptr is a plain word).
// Shape: `y > 0.6f` gives `test ah,41h`, `y < -0.6f` gives `test ah,5 ; jp`; the release arm's sends and the press
//   arm's 0x25/0x26 sends cross-jump into the analogue arms' Send(0)/Send(1) blocks.
class C2DVector { public:
    float x, y;
};
// Naming (NF160, 2026-11-20, q4300): the local CInputEvent model replaced by the shared engine/CInputEvent.h (Ego_r.pdb / FableWin.pdb
//   spellings): 0x00A03B40 GetEventType -> GetType; 0x00A043A0 GetAnalogueVector -> GetJoystickPosition.
#define FABLE_RETAIL_CINPUTEVENT_ACCESSORS
#include "engine/CInputEvent.h"
class CProcessedInput {
public:
    void SetAsPerformedEvent();                             // 0x00A0D390 (Ego_r 0x00B74CA0 + FableWin 0x0318F830 ?SetAsPerformedEvent@CProcessedInput@@QAEXXZ; NF33, was SetAsPerformedEvent)
};
namespace NUISystem {
class CManager {
public:
    virtual void SendEvent(long event);            // slot 0
    static CManager* __fastcall GetInstance();   // 0x0041E5F2
};
}
#include "engine/EGameAction.h"
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Refresh();                        // slot 9 (+0x24)
    unsigned char _pad04[0x24];                    // +0x04 CAInputProcess .. PGamePlayerInterface
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90
protected:
    bool IsEventButtonReleased(const CInputEvent& event) const;       // 0x00687B30
public:
    bool IsEventGameAction(EGameAction action, const CInputEvent& event) const;    // 0x00687CF0
};
class CInputProcessInventoryStatsScreen : public CInputProcessGameBase {
public:
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessInventoryStatsScreen::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    Refresh();
    if (event.GetType() == 0x11) {
        C2DVector stick = event.GetJoystickPosition();
        if (stick.y > 0.6f)
            NUISystem::CManager::GetInstance()->SendEvent(1);
        else if (stick.y < -0.6f)
            NUISystem::CManager::GetInstance()->SendEvent(0);
        else
            return;
    } else if (event.GetType() == 0x12) {
        C2DVector stick = event.GetJoystickPosition();
        if (stick.y > 0.6f)
            NUISystem::CManager::GetInstance()->SendEvent(7);
        else if (stick.y < -0.6f)
            NUISystem::CManager::GetInstance()->SendEvent(6);
        else
            return;
    } else if (IsEventButtonPressed(event)) {
        if (IsEventGameAction((EGameAction)(0x25), event))
            NUISystem::CManager::GetInstance()->SendEvent(0);
        else if (IsEventGameAction((EGameAction)(0x26), event))
            NUISystem::CManager::GetInstance()->SendEvent(1);
        else if (IsEventGameAction((EGameAction)(0x21), event))
            NUISystem::CManager::GetInstance()->SendEvent(4);
        else if (IsEventGameAction((EGameAction)(0x22), event))
            NUISystem::CManager::GetInstance()->SendEvent(5);
        else
            return;
    } else if (IsEventButtonReleased(event)) {
        if (IsEventGameAction((EGameAction)(0x25), event))
            NUISystem::CManager::GetInstance()->SendEvent(0);
        else if (IsEventGameAction((EGameAction)(0x26), event))
            NUISystem::CManager::GetInstance()->SendEvent(1);
        else
            return;
    } else
        return;
    processed.SetAsPerformedEvent();
}