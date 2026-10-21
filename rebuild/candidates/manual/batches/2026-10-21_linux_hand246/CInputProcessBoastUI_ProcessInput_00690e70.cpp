// CInputProcessBoastUI::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- translate boast-screen buttons into front-end UI events (press/held/release of actions 0x19/0x4c/0x58/0x59, mouse capture on 0x47) and mark the input handled
// Leaf: ProcessInput
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00690E70, length 323 bytes, `ret 8`.
// Callees: CInputProcessGameBase::IsEventButtonPressed @ 0x00687A90, IsEventButtonHeld @ 0x00687BD0,
//   IsEventButtonReleased @ 0x00687B30, IsEventGameAction @ 0x00687CF0, NUISystem::CFrontEndManager::GetInstance
//   @ 0x0041E5F2 (`__fastcall`, no arguments), the manager's virtual slot 0 (send a UI event), CProcessedInput::SetHandled
//   @ 0x00A0D390 (manifest "CWomanToAttract::Init" is propagated).
// Layout: `this` is engine/CInputProcessGameBase.h (engine/CInputProcessBoastUI.h is the same 0x28 bytes flattened);
//   the event's mouse movement C2DVector sits at +0x0C and the front-end manager's at +0xB0 (as in the landed
//   CInputProcessPhotojournalCapture::ProcessInput 0x00691AE0, which reaches the same members).
// Shape: every arm ends in `Send(x)` + the shared SetHandled tail; the press and release arms end in the same
//   0x58/0x59 tests, which VC7.1 cross-jumps into one block. The mouse vector is copied into a named local through
//   C2DVector's user copy constructor before GetInstance runs.
#include "engine/CInputProcessGameBase.h"
class CProcessedInput {
public:
    void SetHandled();                             // 0x00A0D390
};
struct C2DVector {
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
class CInputProcessBoastUI : public CInputProcessGameBase {
public:
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90
    bool IsEventButtonReleased(const CInputEvent& event);             // 0x00687B30
    bool IsEventButtonHeld(const CInputEvent& event);                 // 0x00687BD0
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessBoastUI::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (IsEventButtonPressed(event)) {
        if (IsEventGameAction(0x19, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(0x1a);
        else if (IsEventGameAction(0x4c, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(5);
        else if (IsEventGameAction(0x58, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(6);
        else if (IsEventGameAction(0x59, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(7);
        else
            return;
    } else if (IsEventButtonHeld(event)) {
        if (IsEventGameAction(0x19, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(0x1c);
        else if (IsEventGameAction(0x4c, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(0x1d);
        else
            return;
    } else if (IsEventButtonReleased(event)) {
        if (IsEventGameAction(0x19, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(0x1f);
        else if (IsEventGameAction(0x4c, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(0x20);
        else if (IsEventGameAction(0x58, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(6);
        else if (IsEventGameAction(0x59, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(7);
        else
            return;
    } else if (IsEventGameAction(0x47, event)) {
        C2DVector movement(event.MouseMovementXY);
        NUISystem::CFrontEndManager::GetInstance()->MouseMovement = movement;
        NUISystem::CFrontEndManager::GetInstance()->SendEvent(0x19);
    } else
        return;
    processed.SetHandled();
}
