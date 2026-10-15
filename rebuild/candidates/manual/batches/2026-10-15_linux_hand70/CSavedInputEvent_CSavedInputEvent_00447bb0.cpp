// CSavedInputEvent::CSavedInputEvent() -- construct the recorded input event (out-of-line CInputEvent constructor) and clear the camera's two lock flags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00447BB0, length 23 bytes. Callees: CInputEvent::CInputEvent @ 0x00A04410
// (labelled "CDefaultEngineMemoryInit"; the landed CInputProcessTargetLockCycleTargets::Update identifies it as the
// CInputEvent constructor).
// Label correction: the manifest calls this a CRegister constructor (a label propagated over four unrelated bodies;
// the PDB CRegister is {SymbolName, Name, Free}, 12 bytes). The bytes fit Ego_r.pdb's CSavedInputEvent (0x78 bytes):
// Frame +0x00 (left alone), CInputEvent InputEvent +0x04 (the call on `esi+4`), C2DVector MousePos +0x38 (trivial),
// CCamera Camera +0x40 whose inline constructor clears HeightLocked (+0x24 -> +0x64) and ViewVecZLocked (+0x25 -> +0x65).
// No direct caller or address reference exists in the image (an unreferenced inline-constructor copy).
// Shape: the member constructor call, then the two bool initialisers through a cleared al.
// Leaf: ??0CSavedInputEvent
struct CInputEvent {                                    // 0x34 bytes: UInputEventData Data, DeviceType, DeviceNumber, Type, CurrentTime, StartTime
    unsigned char _body[0x34];
    CInputEvent();                                      // 0x00A04410
};

struct C2DVector { float X, Y; };
struct C3DVector { float X, Y, Z; };

struct CCamera {
    C3DVector       WorldPosition;                      // +0x00
    unsigned char   RHSet[0x18];                        // +0x0c CRightHandedSet (opaque here)
    bool            HeightLocked;                       // +0x24
    bool            ViewVecZLocked;                     // +0x25
    unsigned long   FOVFlags;                           // +0x28
    float           HorizontalFOV;                      // +0x2c
    float           VerticalFOV;                        // +0x30
    float           Zoom;                               // +0x34
    CCamera() : HeightLocked(false), ViewVecZLocked(false) {}
};

struct CSavedInputEvent {
    unsigned long Frame;                                // +0x00
    CInputEvent   InputEvent;                           // +0x04
    C2DVector     MousePos;                             // +0x38
    CCamera       Camera;                               // +0x40
    CSavedInputEvent();
};

CSavedInputEvent::CSavedInputEvent()
{
}
