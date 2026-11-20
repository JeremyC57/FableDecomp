// CInputProcessRebootGame::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- for an event of kind 0x15 (0x00A03B40), consume it (0x00A03B90) and mark the processed input performed (CProcessedInput::SetAsPerformedEvent 0x00A0D390)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00690360, length 48 bytes.
// Leaf: ?ProcessInput@CInputProcessRebootGame@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// The callee labels ("CWorld::DrawGetEnvironment", "GetCanItemBeQuickAssigned", "CWomanToAttract::Init") are propagated. Shape: the event in esi, `cmp eax,15h`, `ret 8`.
// Naming (NF160, 2026-11-20, q4300): the local CInputEvent model replaced by the shared engine/CInputEvent.h (Ego_r.pdb / FableWin.pdb
//   spellings): 0x00A03B40 GetKind -> GetType; 0x00A03B90 MarkAsUsed -> GetJoystickButton.
#define FABLE_RETAIL_CINPUTEVENT_ACCESSORS
#include "engine/CInputEvent.h"
class CProcessedInput { public:
    void SetAsPerformedEvent();                    // 0x00A0D390 (Ego_r 0x00B74CA0 + FableWin 0x0318F830; NF33, was the made-up RequestReboot)
};
class CInputProcessRebootGame { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessRebootGame::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (event.GetType() == 0x15) {
        event.GetJoystickButton();
        processed.SetAsPerformedEvent();
    }
}