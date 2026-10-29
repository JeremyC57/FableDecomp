// CInputProcessInventoryWeapons::ProcessInput(const CInputEvent& event, CProcessedInput& input) -- the stick (Y past +-0.6, X past +-0.9) and the pressed (0x25..0x28, 0x21/0x22) or released (0x25/0x26) D-pad game actions become front-end events 0..3, each marking the input performed
// Leaf: ?ProcessInput@CInputProcessInventoryWeapons@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00689820, 326 bytes, thiscall, `ret 8`.
// Identity (proven): slot 4 of vtable 0x01237750 (RTTI .?AVCInputProcessInventoryWeapons@@; slot 9 is its GetPInventory 0x00689810);
//   Ego_r.pdb ?ProcessInput@CInputProcessInventoryWeapons@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z.
// Callees: CInputEvent::GetType 0x00A03B40, CInputEvent::GetJoystickPosition 0x00A043A0 (hidden C2DVector),
//   NUISystem::CManager::GetInstance 0x0041E5F2 (__fastcall) + its slot 0 (ProcessEvent),
//   CInputProcessGameBase::IsEventButtonPressed 0x00687A90 / IsEventButtonReleased 0x00687B30 / IsEventGameAction 0x00687CF0,
//   CProcessedInput::SetAsPerformedEvent 0x00A0D390; the virtual GetPInventory (slot 9) whose result is unused.
// Shape (family M192, representative 0x006896C0): the landed CInputProcessInGameMenu::ProcessInput 0x00690480 pattern -- one
//   `GetInstance()->ProcessEvent(n); input.SetAsPerformedEvent();` per arm, which VC tail-merges into `call GetInstance; push n;
//   jmp common`. Speed TU.
// Layout: engine/CInputProcessGameBase.h and engine/CInputProcessInventoryWeapons.h are flattened data views without the
//   base chain or methods; the base chain is modelled locally to give the callees their Ego_r.pdb owners.
class CInputEvent;
class CProcessedInput;
class CInputProcessManager;
namespace NInventory { class CTCInventoryBase; }
enum EInputEventType { INPUT_EVENT_TYPE_JOYSTICK_POSITION = 0x11 };
enum EGameAction { GAME_ACTION_NONE = 0 };
class C2DVector {
public:
    C2DVector(float x, float y) : X(x), Y(y) { }
    float X;                                              // +0x00
    float Y;                                              // +0x04
};
class CInputEvent {
public:
    EInputEventType GetType() const;                      // 0x00A03B40
    C2DVector GetJoystickPosition() const;                // 0x00A043A0
};
class CProcessedInput {
public:
    void SetAsPerformedEvent();                           // 0x00A0D390
};
namespace NUISystem {
enum EEvent { EVENT_NONE = 0 };
class CManager {
public:
    virtual void ProcessEvent(EEvent event);              // slot 0
    static CManager* __fastcall GetInstance();    // 0x0041E5F2
};
}
class CAInputProcess {
public:
    virtual ~CAInputProcess();
    virtual bool AddToInterface(CInputProcessManager& manager);
    virtual void Init();
    virtual void Cleanup();
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& input);
    virtual void PreparePrimitivesForEngine();
    virtual void Update();
    virtual bool ProcessWhilePaused();
};
class CInputProcessGameBase : public CAInputProcess {
public:
    bool IsEventGameAction(EGameAction action, const CInputEvent& event) const;    // 0x00687CF0
protected:
    bool IsEventButtonPressed(const CInputEvent& event) const;                     // 0x00687A90
    bool IsEventButtonReleased(const CInputEvent& event) const;                    // 0x00687B30
    virtual bool IsInputEventToBeSentToThisInputProcess(const CInputEvent& event) const;   // slot 8
};
class CInputProcessInventoryBase : public CInputProcessGameBase {
protected:
    virtual NInventory::CTCInventoryBase* GetPInventory() const = 0;               // slot 9
};
class CInputProcessInventoryWeapons : public CInputProcessInventoryBase {
public:
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& input);
protected:
    virtual NInventory::CTCInventoryBase* GetPInventory() const;                   // 0x00689810
};
void CInputProcessInventoryWeapons::ProcessInput(const CInputEvent& event, CProcessedInput& input)
{
    GetPInventory();
    if (event.GetType() == INPUT_EVENT_TYPE_JOYSTICK_POSITION) {
        C2DVector stick = event.GetJoystickPosition();
        if (stick.Y > 0.6f) {
            NUISystem::CManager::GetInstance()->ProcessEvent((NUISystem::EEvent)1);
            input.SetAsPerformedEvent();
        } else if (stick.Y < -0.6f) {
            NUISystem::CManager::GetInstance()->ProcessEvent((NUISystem::EEvent)0);
            input.SetAsPerformedEvent();
        } else if (stick.X < -0.9f) {
            NUISystem::CManager::GetInstance()->ProcessEvent((NUISystem::EEvent)2);
            input.SetAsPerformedEvent();
        } else if (stick.X > 0.9f) {
            NUISystem::CManager::GetInstance()->ProcessEvent((NUISystem::EEvent)3);
            input.SetAsPerformedEvent();
        }
    } else if (IsEventButtonPressed(event)) {
        if (IsEventGameAction((EGameAction)0x25, event)) {
            NUISystem::CManager::GetInstance()->ProcessEvent((NUISystem::EEvent)0);
            input.SetAsPerformedEvent();
        } else if (IsEventGameAction((EGameAction)0x26, event)) {
            NUISystem::CManager::GetInstance()->ProcessEvent((NUISystem::EEvent)1);
            input.SetAsPerformedEvent();
        } else if (IsEventGameAction((EGameAction)0x27, event) || IsEventGameAction((EGameAction)0x22, event)) {
            NUISystem::CManager::GetInstance()->ProcessEvent((NUISystem::EEvent)2);
            input.SetAsPerformedEvent();
        } else if (IsEventGameAction((EGameAction)0x28, event) || IsEventGameAction((EGameAction)0x21, event)) {
            NUISystem::CManager::GetInstance()->ProcessEvent((NUISystem::EEvent)3);
            input.SetAsPerformedEvent();
        }
    } else if (IsEventButtonReleased(event)) {
        if (IsEventGameAction((EGameAction)0x25, event)) {
            NUISystem::CManager::GetInstance()->ProcessEvent((NUISystem::EEvent)0);
            input.SetAsPerformedEvent();
        } else if (IsEventGameAction((EGameAction)0x26, event)) {
            NUISystem::CManager::GetInstance()->ProcessEvent((NUISystem::EEvent)1);
            input.SetAsPerformedEvent();
        }
    }
}