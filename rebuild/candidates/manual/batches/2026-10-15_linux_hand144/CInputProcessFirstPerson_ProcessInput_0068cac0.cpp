// CInputProcessFirstPerson::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- leave first-person mode (player mode 0x10) and mark the input handled on a left-stick deflection longer than 0.0001, or on a held button that is not game action 5, 2 or 0x13
// Leaf: ProcessInput
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068CAC0, length 149 bytes.
// Callees: CInputEvent::GetEventType @ 0x00A03B40 (`mov eax,[ecx+28h]`), CInputEvent::GetAnalogueVector @ 0x00A043A0
// (C2DVector by hidden pointer; manifest label GetDefaultVal<CEngineGraphic> is propagated),
// CInputProcessGameBase::IsEventButtonHeld @ 0x00687BD0, IsEventGameAction @ 0x00687CF0, CPlayer::RemoveMode @ 0x00634190
// (MyPlayer +0x18), CProcessedInput::SetHandled @ 0x00A0D390. Float constant 0x0129BA3C = 0.0001f.
// Shape: `!(len > eps)` (not `len <= eps`, which adds a `jnp`); `x*x + y*y` in one expression squares y first; the length test and the action tests both end
// in one shared `jne` to the exit; the event rides in esi, `this` in edi.
#include <math.h>
#pragma intrinsic(sqrt)
class CInputEvent;
struct C2DVector {
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
class CPlayer {
public:
    void RemoveMode(long mode);                    // 0x00634190
};
struct CWorld; struct CGameDefinitionManager; struct CMainGameComponent; struct CGamePlayerInterface;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00();
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    bool IsEventButtonHeld(const CInputEvent& event);                 // 0x00687BD0
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
};
class CInputProcessFirstPerson : public CInputProcessGameBase {
public:
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessFirstPerson::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (event.GetEventType() == 0x11) {
        C2DVector stick = event.GetAnalogueVector();
        if (!(sqrt(stick.x * stick.x + stick.y * stick.y) > 0.0001f))
            return;
    } else {
        if (!IsEventButtonHeld(event) || IsEventGameAction(5, event) || IsEventGameAction(2, event)
            || IsEventGameAction(0x13, event))
            return;
    }
    MyPlayer->RemoveMode(0x10);
    processed.SetHandled();
}
