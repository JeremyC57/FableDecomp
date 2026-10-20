// CInputProcessCutScene::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- on a button press: game action 0x15 marks the input handled and queues game event 0x39; otherwise post message 0x4b (flags 0x32) to the world's message manager -- in cutscene action mode for a joystick (device 3) event of type 4, else for game action 0x16
// Leaf: ?ProcessInput@CInputProcessCutScene@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006909A0, length 288 bytes, `ret 8`.
// Callees: CInputProcessGameBase::IsEventButtonPressed @ 0x00687A90, IsEventGameAction @ 0x00687CF0,
//   CProcessedInput::SetHandled @ 0x00A0D390 and AddGameEvent(const CGameEvent&) @ 0x00A0D340, CInputEvent::GetType
//   @ 0x00A03B40 / GetDeviceType @ 0x00A03B50, CMessageEventManager::AddMessage(EMessageEventType, ulong, C3DVector*,
//   CThing*, CThing*, float, float, long) @ 0x00687540 (the world's manager at +0x60), __security_check_cookie @ 0x00BFE9F9.
// Globals: the player GUI singleton pointer @ 0x013B8790, byte +0xfa (inside its CSubtitleRenderer at +0x18: retail
//   +0xe2, most likely CutsceneActionMode -- the same byte CInputProcessCutScene::ProcessWhilePaused 0x00690AC0 returns).
// Layout: engine/CInputProcessGameBase.h (World +0x14), modelled locally as in the landed 0x00690C70; CGameEvent
//   (Ego_r.pdb, 40 bytes) as there.
// Shape: the handled arm returns on its own; the two message arms make the same call (retail tail-merges them).
class CInputEvent {
public:
    long GetType() const;                          // 0x00A03B40
    long GetDeviceType() const;                    // 0x00A03B50
};
struct CMemoryBuffer {
};
struct CGameEvent : CMemoryBuffer {
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
};
class CProcessedInput {
public:
    void SetHandled();                             // 0x00A0D390
    void AddGameEvent(const CGameEvent& event);    // 0x00A0D340
};
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class CThing;
struct CMessageEvent;
struct CMessageEventManager {
    CMessageEvent* AddMessage(long type, unsigned long flags, const C3DVector* position, CThing* thing1, CThing* thing2, float a, float b, long priority);   // 0x00687540
};
struct CWorld {
    unsigned char         _pad00[0x60];
    CMessageEventManager* PMessageEventManager;    // +0x60
};
struct CPlayerGui {
    unsigned char _pad00[0xfa];
    bool          CutsceneActionMode;              // +0xfa (SubtitleRenderer +0xe2)
};
extern CPlayerGui* g_pPlayerGui;                   // 0x013B8790
class CPlayer;
struct CGameDefinitionManager; struct CMainGameComponent; struct CGamePlayerInterface;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00();
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
};
class CInputProcessCutScene : public CInputProcessGameBase {
public:
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessCutScene::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (!IsEventButtonPressed(event))
        return;
    if (IsEventGameAction(0x15, event)) {
        CGameEvent gameEvent(0x39);
        processed.SetHandled();
        processed.AddGameEvent(gameEvent);
        return;
    }
    if (g_pPlayerGui->CutsceneActionMode) {
        if (event.GetDeviceType() == 3 && event.GetType() == 4)
            World->PMessageEventManager->AddMessage(0x4b, 0x32, &C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, 0);
    } else if (IsEventGameAction(0x16, event))
        World->PMessageEventManager->AddMessage(0x4b, 0x32, &C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, 0);
}
