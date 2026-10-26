// CGameCameraManager::UpdateCreatureControlAngleXY() -- refresh the hero control angle from the camera unless an angle change is latched (a change in control direction clears the latch), start/clear the no-control-movement frame stamp, then clear the per-frame ChangeInControlDirection / ControlNoControlMovement flags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00697760, length 131 bytes (+ int3 padding).
// Callees: CCamera::GetAngleXY @ 0x00A0C380 (on Camera +4, float result; catalog "Math_ComputeAngleXY"), the global frame-counter getter
// @ 0x0049D870 (`mov eax,[0x013B89BC] ; ret`; catalog label "GFGetPreMainMemoryUsed" is wrong).
// Layout: engine/CGameCameraManager.h is the PDB layout; retail is 0xc bytes narrower from AvailableCamPoints on (4-byte retail list),
// so `this` is modelled locally with PDB names at retail offsets: ChangeInControlDirection +0xb1 (landed setter 0x00697750),
// ControlAngleXY +0xb8 (landed getter 0x00697700), AngleChange +0xbc, ControlNoControlMovement +0xbd, ControlNoControlMovementStartFrame +0xc0.
// Shape: one zero register (ebx) for every byte compare and store; each flag re-read after a store or call.
class CCamera { public:
    unsigned char _body[0x38];
    float GetAngleXY();                                     // 0x00A0C380
};

long GetGameFrame();                                        // 0x0049D870

class CGameCameraManager { public:
    void*         __vftable;                                // +0x00
    CCamera       Camera;                                   // +0x04
    unsigned char _pad3c[0x74];                             // +0x3c
    bool          SingleCut;                                // +0xb0
    bool          ChangeInControlDirection;                 // +0xb1
    unsigned char _padb2[2];
    long          InsideHeroCount;                          // +0xb4
    float         ControlAngleXY;                           // +0xb8
    bool          AngleChange;                              // +0xbc
    bool          ControlNoControlMovement;                 // +0xbd
    unsigned char _padbe[2];
    long          ControlNoControlMovementStartFrame;       // +0xc0
    void UpdateCreatureControlAngleXY();
};

void CGameCameraManager::UpdateCreatureControlAngleXY()
{
    if (AngleChange) {
        if (ChangeInControlDirection)
            AngleChange = false;
    }
    if (!AngleChange)
        ControlAngleXY = Camera.GetAngleXY();
    if (ControlNoControlMovement) {
        if (ControlNoControlMovementStartFrame == 0)
            ControlNoControlMovementStartFrame = GetGameFrame();
    }
    if (!ControlNoControlMovement) {
        if (ControlNoControlMovementStartFrame != 0)
            ControlNoControlMovementStartFrame = 0;
    }
    ChangeInControlDirection = false;
    ControlNoControlMovement = false;
}