// CInputProcessCameraLookAround::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- turn the right-stick look-around input into the camera manager's look vector: the analogue vector of game action 0x4d or 0x3d, else +1 / -1 along x for the digital actions 0x3e / 0x3f; nothing for other events
// Leaf: ProcessInput
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068D790, length 155 bytes.
// Callees: CInputProcessGameBase::GetEventGameActionVector(long, const CInputEvent&, C2DVector*) @ 0x00687D00 (forwards to
// the game player interface at +0x24; manifest label propagated), IsEventGameAction @ 0x00687CF0, the current game
// camera's (CWorld +0x34) virtual slot 9 GetCameraManager(CGameCameraManager**) (as the landed 0x007CF460),
// CGameCameraManager::SetRightStickLookAroundVector(const C2DVector&) @ 0x00697B10.
// Shape: the vector is a zeroed frame local; the camera manager out-pointer is null-initialised and VC homes it in the
// dead `event` argument slot; the camera source is read into a local BEFORE the out-pointer is declared (else the
// zero store is scheduled above the source load).
class CInputEvent;
class CProcessedInput;
class C2DVector { public:
    float x, y;
    C2DVector(float x_, float y_) : x(x_), y(y_) {}
};
class CGameCameraManager { public:
    void SetRightStickLookAroundVector(const C2DVector& look);   // 0x00697B10
};
class CAIGameCameraBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool GetCameraManager(CGameCameraManager** camera);   // slot 9 (+0x24)
};
class CWorld { public:
    unsigned char      _pad[0x34];
    CAIGameCameraBase* PCurrentGameCamera;         // +0x34
    CAIGameCameraBase* GetCurrentGameCamera() { return PCurrentGameCamera; }
};
class CPlayer; class CGameDefinitionManager; class CMainGameComponent; class CGamePlayerInterface;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00();
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    bool IsEventGameAction(long action, const CInputEvent& event);                          // 0x00687CF0
    bool GetEventGameActionVector(long action, const CInputEvent& event, C2DVector* vector); // 0x00687D00
};
class CInputProcessCameraLookAround : public CInputProcessGameBase {
public:
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessCameraLookAround::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    C2DVector look(0.0f, 0.0f);
    if (!GetEventGameActionVector(0x4d, event, &look) && !GetEventGameActionVector(0x3d, event, &look)) {
        if (IsEventGameAction(0x3e, event))
            look.x = 1.0f;
        else if (IsEventGameAction(0x3f, event))
            look.x = -1.0f;
        else
            return;
    }
    CAIGameCameraBase* source = World->GetCurrentGameCamera();
    CGameCameraManager* camera = 0;
    source->GetCameraManager(&camera);
    camera->SetRightStickLookAroundVector(look);
}