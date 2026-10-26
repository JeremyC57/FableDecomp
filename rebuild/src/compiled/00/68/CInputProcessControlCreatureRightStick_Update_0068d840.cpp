// CInputProcessControlCreatureRightStick::Update(float) -- unless the right-stick game action (0x3d, any button event) is queued, ask the current game camera for its manual-mode owner (slot 9, out pointer), push the current manual mode when the stick was looking around, then clear the three stick flags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068D840, length 66 bytes (+ int3 padding). Vtable 0x01237A40 slot 6; the float argument is unused.
// Callees: CInputProcessGameBase::IsGameActionEventInQueue(long action, long buttonEventType) @ 0x00687D50 (the two-argument overload),
// CGameCameraManager::PushCurrentManualMode @ 0x006988B0 (on the out pointer), the camera's vtable slot 9 (CWorld::PCurrentGameCamera +0x34).
// Layout: engine/CInputProcessControlCreatureRightStick.h (PushingCameraModeUp +0x28, PushingCameraModeDown +0x29, LookingAround +0x2a);
// World is CInputProcessGameBase::World (+0x14); the base is modelled locally for the names.
// Shape: one `push ecx` frame slot for the out pointer, zeroed from ebx (the shared zero that also feeds the second argument and the
// three byte clears); the LookingAround test is `cmp byte ptr [esi+2Ah],bl`.
class CGameCameraManager { public:
    void PushCurrentManualMode();                   // 0x006988B0
};
class CAIGameCameraBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08();
    virtual void GetCameraManager(CGameCameraManager** ppManager);   // slot 9
};
class CWorld { public:
    unsigned char      _pad[0x34];
    CAIGameCameraBase* PCurrentGameCamera;          // +0x34
};
struct CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
    virtual void v00();
    unsigned char _pad04[0x10];                     // +0x04 CAInputProcess
    CWorld*       World;                            // +0x14
    void*         MyPlayer;                         // +0x18
    void*         DefinitionManager;                // +0x1c
    void*         Component;                        // +0x20
    void*         PGamePlayerInterface;             // +0x24
    bool IsGameActionEventInQueue(long action, long buttonEventType);   // 0x00687D50
};
class CInputProcessControlCreatureRightStick : public CInputProcessGameBase { public:   // vtable 0x01237A40
    bool PushingCameraModeUp;                       // +0x28
    bool PushingCameraModeDown;                     // +0x29
    bool LookingAround;                             // +0x2a
    void Update(float deltaTime);
};

void CInputProcessControlCreatureRightStick::Update(float deltaTime)
{
    if (!IsGameActionEventInQueue(0x3d, 0)) {
        CGameCameraManager* cameraManager = 0;
        World->PCurrentGameCamera->GetCameraManager(&cameraManager);
        if (LookingAround)
            cameraManager->PushCurrentManualMode();
        PushingCameraModeUp = false;
        PushingCameraModeDown = false;
        LookingAround = false;
    }
}