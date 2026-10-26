// CInputProcessInventoryStatsScreen::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- refresh the process, then page the stats screen: analogue events 0x11/0x12 past +-0.6 on the stick's Y axis send front-end UI events 1/0 and 7/6, presses of actions 0x25/0x26/0x21/0x22 send 0/1/4/5 and releases of 0x25/0x26 send 0/1; a sent event marks the input handled
// Leaf: ProcessInput
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00689C40, length 335 bytes, `ret 8`.
// Callees: the process's own virtual slot 9 (+0x24, no arguments), CInputEvent::GetEventType @ 0x00A03B40,
//   CInputEvent::GetAnalogueVector @ 0x00A043A0 (C2DVector by hidden pointer; manifest "GetDefaultVal<CEngineGraphic>"
//   is propagated; names as in the landed CInputProcessFirstPerson::ProcessInput 0x0068CAC0),
//   CInputProcessGameBase::IsEventButtonPressed @ 0x00687A90, IsEventButtonReleased @ 0x00687B30, IsEventGameAction
//   @ 0x00687CF0, NUISystem::CFrontEndManager::GetInstance @ 0x0041E5F2 + slot 0, CProcessedInput::SetHandled
//   @ 0x00A0D390. Float pool: 0.6f @ 0x0125BEC8, -0.6f @ 0x0125BED4.
// Layout: engine/CInputProcessInventoryStatsScreen.h (0x28 bytes, flattened over CInputProcessGameBase) -- modelled
//   locally because the process's own virtual is called (the header's vptr is a plain word).
// Shape: `y > 0.6f` gives `test ah,41h`, `y < -0.6f` gives `test ah,5 ; jp`; the release arm's sends and the press
//   arm's 0x25/0x26 sends cross-jump into the analogue arms' Send(0)/Send(1) blocks.
class C2DVector { public:
    float x, y;
};
class CInputEvent {
public:
    long      GetEventType() const;                // 0x00A03B40
    C2DVector GetAnalogueVector() const;           // 0x00A043A0
};
class CProcessedInput {
public:
    void SetHandled();                             // 0x00A0D390
};
namespace NUISystem {
class CFrontEndManager {
public:
    virtual void SendEvent(long event);            // slot 0
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
};
}
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Refresh();                        // slot 9 (+0x24)
    unsigned char _pad04[0x24];                    // +0x04 CAInputProcess .. PGamePlayerInterface
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90
    bool IsEventButtonReleased(const CInputEvent& event);             // 0x00687B30
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
};
class CInputProcessInventoryStatsScreen : public CInputProcessGameBase {
public:
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessInventoryStatsScreen::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    Refresh();
    if (event.GetEventType() == 0x11) {
        C2DVector stick = event.GetAnalogueVector();
        if (stick.y > 0.6f)
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(1);
        else if (stick.y < -0.6f)
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(0);
        else
            return;
    } else if (event.GetEventType() == 0x12) {
        C2DVector stick = event.GetAnalogueVector();
        if (stick.y > 0.6f)
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(7);
        else if (stick.y < -0.6f)
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(6);
        else
            return;
    } else if (IsEventButtonPressed(event)) {
        if (IsEventGameAction(0x25, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(0);
        else if (IsEventGameAction(0x26, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(1);
        else if (IsEventGameAction(0x21, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(4);
        else if (IsEventGameAction(0x22, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(5);
        else
            return;
    } else if (IsEventButtonReleased(event)) {
        if (IsEventGameAction(0x25, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(0);
        else if (IsEventGameAction(0x26, event))
            NUISystem::CFrontEndManager::GetInstance()->SendEvent(1);
        else
            return;
    } else
        return;
    processed.SetHandled();
}