// CInputProcessFirstPersonLookAround::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- feed the analogue vector of game action 0x4b to the camera manager's first-person look vector: doubled for event kind 0xd, run through the stick dead-zone curve for kind 0x11
// Leaf: ProcessInput
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068D700, length 129 bytes.
// Callees: CInputProcessGameBase::GetEventGameActionVector(long, const CInputEvent&, C2DVector*) @ 0x00687D00,
// CInputEvent::GetEventType @ 0x00A03B40, PreProcessStickVector(C2DVector*) @ 0x0068D5F0 (__fastcall), the current game
// camera's (CWorld +0x34) virtual slot 9 GetCameraManager(CGameCameraManager**), CGameCameraManager's look-vector
// setter @ 0x00697B60 (sibling of SetRightStickLookAroundVector @ 0x00697B10; manifest label propagated).
// Model: as the sibling CInputProcessCameraLookAround::ProcessInput 0x0068D790 (agent 143): the vector is an
// uninitialised frame local, the camera source is read into a local BEFORE the null-initialised out-pointer (which VC
// homes in the dead `event` argument slot). `v.x += v.x` gives `fadd st(0),st(0)`.
class CInputEvent;
class CProcessedInput;
struct C2DVector {
    float x, y;
};
void __fastcall PreProcessStickVector(C2DVector* stick);   // 0x0068D5F0
class CInputEvent {
public:
    long GetEventType() const;                     // 0x00A03B40
};
struct CGameCameraManager {
    void SetFirstPersonLookAroundVector(const C2DVector& look);   // 0x00697B60
};
struct CAIGameCameraBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** camera);   // slot 9 (+0x24)
};
struct CWorld {
    unsigned char      _pad[0x34];
    CAIGameCameraBase* PCurrentGameCamera;         // +0x34
    CAIGameCameraBase* GetCurrentGameCamera() { return PCurrentGameCamera; }
};
struct CPlayer; struct CGameDefinitionManager; struct CMainGameComponent; struct CGamePlayerInterface;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00();
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    bool GetEventGameActionVector(long action, const CInputEvent& event, C2DVector* vector); // 0x00687D00
};
class CInputProcessFirstPersonLookAround : public CInputProcessGameBase {
public:
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessFirstPersonLookAround::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    C2DVector look;
    if (!GetEventGameActionVector(0x4b, event, &look))
        return;
    if (event.GetEventType() == 0xd) {
        look.x += look.x;
        look.y += look.y;
    } else if (event.GetEventType() == 0x11) {
        PreProcessStickVector(&look);
    }
    CAIGameCameraBase* source = World->GetCurrentGameCamera();
    CGameCameraManager* camera = 0;
    source->GetCameraManager(&camera);
    camera->SetFirstPersonLookAroundVector(look);
}