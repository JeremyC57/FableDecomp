// CInputProcessControlCreatureRightStick::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- right stick
//   camera control: with the stick vector (game action 0x3d, Y flipped; dead zone 0.2, 0.02 for mouse movement) a push within
//   10 degrees of straight up/down past 0.8 steps the manual camera list up/down once per push, a push within 10 degrees of
//   sideways starts look-around, releasing the stick ends look-around; otherwise (not in ranged-weapon mode) game actions
//   0x40 / 0x41 (pressed, or event type 0xe) step the list up/down and mark the input handled
// Retail: Fable.exe (TLC, VC7.1) @ 0x0068D890, 789 bytes (+ int3 padding), `ret 8`.
// Callees: the current game camera (CWorld +0x34) slot 9 GetCameraManager (out pointer, as the landed Update 0x0068D840),
//   CInputEvent::GetDeviceType 0x00A03B50 / GetType 0x00A03B40, CInputProcessGameBase::GetEventGameActionVector 0x00687D00 /
//   IsEventGameAction 0x00687CF0 / IsEventButtonPressed 0x00687A90, CGameCameraManager PushCurrentManualMode 0x006988B0,
//   PushUpManualCameraList 0x00698B50, PushDownManualCameraList 0x00698C30, PushLookAround 0x00699970 (FableWin.pdb names
//   for the class, addresses by order), CProcessedInput::SetHandled 0x00A0D390. Constants -1.0f 0x0122DEE0, 0.0f 0x0122DEDC,
//   0.0001f 0x0129BA3C, 0.8f 0x01254DA8, 10 / 80 degrees in radians (double, PI/180.0*deg) 0x0125BF10 / 0x0125BF08.
// Layout: engine/CInputProcessControlCreatureRightStick.h (PushingCameraModeUp +0x28, PushingCameraModeDown +0x29,
//   LookingAround +0x2a); the game base modelled locally as in the landed Update; CPlayer +0x218 as the landed 0x0068EF50 row.
// Shape: the mouse flag is `bool x = false; if (...) x = true;` (kept in bl); the angle tests use a float Abs (a double fabs
//   compares in double with fcompp); the stick magnitude and the up component live in the dead argument slots.
// Leaf: ProcessInput
extern "C" double __cdecl cos(double);
extern "C" double __cdecl sqrt(double);
extern "C" double __cdecl fabs(double);
#pragma intrinsic(cos, sqrt, fabs)
const float PI = 3.1415927f;
class C2DVector { public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};
inline float Abs(float x) { return (float)fabs(x); }
inline float DotProduct(const C2DVector& a, const C2DVector& b) { return a.X * b.X + a.Y * b.Y; }
class CInputEvent {
public:
    long GetType() const;                          // 0x00A03B40
    long GetDeviceType() const;                    // 0x00A03B50
};
class CProcessedInput {
public:
    void SetHandled();                             // 0x00A0D390
};
class CGameCameraManager { public:
    void PushCurrentManualMode();                  // 0x006988B0
    void PushUpManualCameraList();                 // 0x00698B50
    void PushDownManualCameraList();               // 0x00698C30
    void PushLookAround();                         // 0x00699970
};
class CAIGameCameraBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08();
    virtual void GetCameraManager(CGameCameraManager** ppManager);   // slot 9
};
class CWorld { public:
    unsigned char      _pad[0x34];
    CAIGameCameraBase* PCurrentGameCamera;         // +0x34
};
class CPlayer { public:
    unsigned char _pad00[0x218];
    bool          InRangedWeaponMode;              // +0x218 (descriptive)
};
struct CInputProcessGameBase {                     // engine/CInputProcessGameBase.h layout
    virtual void v00();
    unsigned char _pad04[0x10];                    // +0x04 CAInputProcess
    CWorld*       World;                           // +0x14
    CPlayer*      MyPlayer;                        // +0x18
    void*         DefinitionManager;               // +0x1c
    void*         Component;                       // +0x20
    void*         PGamePlayerInterface;            // +0x24
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
    bool GetEventGameActionVector(long action, const CInputEvent& event, C2DVector* vector);   // 0x00687D00
};
class CInputProcessControlCreatureRightStick : public CInputProcessGameBase { public:   // vtable 0x01237A40
    bool PushingCameraModeUp;                      // +0x28
    bool PushingCameraModeDown;                    // +0x29
    bool LookingAround;                            // +0x2a
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessControlCreatureRightStick::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    CGameCameraManager* cameraManager = 0;
    World->PCurrentGameCamera->GetCameraManager(&cameraManager);
    bool isMouse = false;
    if (event.GetDeviceType() == 3)
        isMouse = true;
    C2DVector stick;
    if (GetEventGameActionVector(0x3d, event, &stick)) {
        stick.Y *= -1.0f;
        float deadZone = 0.2f;
        float magnitude = (float)sqrt(stick.X * stick.X + stick.Y * stick.Y);
        if (isMouse && event.GetType() == 0xd)
            deadZone = 0.02f;
        C2DVector direction(0.0f, 0.0f);
        if (magnitude > deadZone)
            direction = stick;
        float cosTen = (float)cos(PI / 180.0 * 10.0);
        float cosEighty = (float)cos(PI / 180.0 * 80.0);
        float up = magnitude > 0.0001f ? DotProduct(direction, C2DVector(0.0f, 1.0f)) / magnitude : 0.0f;
        float right = magnitude > 0.0001f ? DotProduct(direction, C2DVector(1.0f, 0.0f)) / magnitude : 0.0f;
        if (up < 0.0f && Abs(up) > cosTen) {
            if (isMouse) {
                LookingAround = false;
                return;
            }
            if (magnitude > 0.8f) {
                if (!PushingCameraModeUp) {
                    cameraManager->PushUpManualCameraList();
                    LookingAround = isMouse;
                    PushingCameraModeUp = true;
                }
                return;
            }
            PushingCameraModeUp = false;
            return;
        }
        if (up > 0.0f && Abs(up) > cosTen) {
            if (isMouse) {
                LookingAround = false;
                return;
            }
            if (magnitude > 0.8f) {
                if (!PushingCameraModeDown) {
                    cameraManager->PushDownManualCameraList();
                    LookingAround = isMouse;
                    PushingCameraModeDown = true;
                }
                return;
            }
            PushingCameraModeDown = false;
            return;
        }
        if (Abs(right) > cosEighty && magnitude > deadZone) {
            if (!LookingAround) {
                cameraManager->PushLookAround();
                LookingAround = true;
            }
            return;
        }
        if (LookingAround && magnitude > deadZone)
            return;
        if (LookingAround) {
            cameraManager->PushCurrentManualMode();
            LookingAround = false;
            return;
        }
        PushingCameraModeDown = false;
        PushingCameraModeUp = false;
        return;
    }
    if (MyPlayer->InRangedWeaponMode)
        return;
    if (IsEventGameAction(0x40, event)) {
        if (IsEventButtonPressed(event) || event.GetType() == 0xe) {
            cameraManager->PushUpManualCameraList();
            processed.SetHandled();
        }
    } else if (IsEventGameAction(0x41, event)) {
        if (IsEventButtonPressed(event) || event.GetType() == 0xe) {
            cameraManager->PushDownManualCameraList();
            processed.SetHandled();
        }
    }
}