// CTCCameraPointTrack::CTCCameraPointTrack(CThing&) -- the CTCCameraPointShapeActivationBase base, this class's vtable, then CameraStringLength = 7.0f, CameraCageRadius = 2.0f
// Leaf: ??0CTCCameraPointTrack@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3AF4, 38 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): stores vtable 0x0123B0E4 (RTTI .?AVCTCCameraPointTrack@@ : CTCCameraPointShapeActivationBase ...); FableWin.pdb ??0CTCCameraPointTrack@@QAE@AAVCThing@@@Z; only caller
//   CTCCameraPointTrack::Alloc 0x004D8264 (landed); the stores are Ego_r.pdb's CTCCameraPointTrack members (retail = Ego_r.pdb).
// Callees: CTCCameraPointShapeActivationBase::CTCCameraPointShapeActivationBase(CThing&) 0x004D3A03.
// Shape (family K091, representative 0x0044C4D5): the argument forwarded to the base, the vtable store, the two member
//   initialisers in declaration order; size TU (the pass-through `push dword ptr [esp+8]`).
#pragma optimize("s", on)
class CThing;
class C3DVector { public: float X, Y, Z; };
class CTCCameraPointShapeActivationBase {                                            // retail 0x40 bytes (members not touched here)
public:
    CTCCameraPointShapeActivationBase(CThing& arg);                                   // 0x004D3A03
    virtual ~CTCCameraPointShapeActivationBase();
    unsigned char _body[0x40 - 4];
};
class CTCCameraPointTrack : public CTCCameraPointShapeActivationBase {
public:
    CTCCameraPointTrack(CThing& arg);
    float     CameraStringLength;                  // +0x40
    float     CameraCageRadius;                    // +0x44
    C3DVector DragCamPos;                          // +0x48
};
CTCCameraPointTrack::CTCCameraPointTrack(CThing& arg)
    : CTCCameraPointShapeActivationBase(arg), CameraStringLength(7.0f), CameraCageRadius(2.0f)
{
}