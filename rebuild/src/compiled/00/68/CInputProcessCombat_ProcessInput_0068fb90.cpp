// CInputProcessCombat::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- once per frame, pull this player's pending joystick event of kind 0x11 out of the input manager's queue into the combat input's own event queue; then run the combat input processing for this event
// Leaf: ProcessInput
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068FB90, length 112 bytes.
// Callees: GFGetCurrentFrame @ 0x0049D870 (static, twice), CInputEvent::CInputEvent() @ 0x00A04410 / ~CInputEvent @ 0x00A03B30
// (a scratch event, as the landed CInputProcessTargetLockCycleTargets::Update), CPlayer::GetPlayerNumber @ 0x00487510
// (manifest "CWorld::DrawGetEnvironment" propagated), CInputManager::IsJoystickEventInQueue(EInputEventType, long,
// CInputEvent*, long) @ 0x009F4C60 on the global input manager @ 0x013B8388, the queue push @ 0x00A0DFC0 on the member at
// +0x28, CInputProcessCombat::ProcessCombatInput @ 0x0068F470.
// Shape: the frame number is fetched again for the store (two calls); the scratch event lives in its own block.
class CInputEvent {
public:
    unsigned char _body[0x34];
    CInputEvent();                                 // 0x00A04410
    ~CInputEvent();                                // 0x00A03B30
};
class CProcessedInput;
long __cdecl GFGetCurrentFrame();                  // 0x0049D870
class CInputManager {
public:
    bool IsJoystickEventInQueue(long type, long device, CInputEvent* event, long player);   // 0x009F4C60
};
extern CInputManager* g_pInputManager;             // 0x013B8388
class CPlayer {
public:
    long GetPlayerNumber() const;                  // 0x00487510
};
struct CInputEventQueue {
    void Push(const CInputEvent& event);           // 0x00A0DFC0
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
};
class CInputProcessCombat : public CInputProcessGameBase {
public:
    CInputEventQueue m_joystickEvents;             // +0x28
    unsigned char    _pad2c[0x24];
    long             m_lastJoystickFrame;          // +0x50
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
    void ProcessCombatInput(const CInputEvent& event, CProcessedInput& processed);   // 0x0068F470
};

void CInputProcessCombat::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (m_lastJoystickFrame != GFGetCurrentFrame()) {
        m_lastJoystickFrame = GFGetCurrentFrame();
        CInputEvent joystickEvent;
        if (g_pInputManager->IsJoystickEventInQueue(0x11, 0, &joystickEvent, MyPlayer->GetPlayerNumber()))
            m_joystickEvents.Push(joystickEvent);
    }
    ProcessCombatInput(event, processed);
}