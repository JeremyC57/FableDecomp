// NCameraModes::CScriptModuleDolly::Update() -- put the owning camera at PosAround + DollyVector looking at PosAround, then turn DollyVector about Z by one frame's share of a full turn
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008522B0, length 160 bytes (153 + int3 padding).
// Callees: C3DVector::Rotate(const C3DVector& axis, float angle) @ 0x00A13C60.
// Globals: the frames-per-second count @ 0x01375550 (a long, 15 at rest).
// Shape: `PosAround + DollyVector` through the inline member operator+ of the landed GoToLookInDirection
// model, the look-at a plain copy; the frame rate read into a long local FIRST (the owner stores could
// alias the global, so the value is kept in the frame and `fild`ed from there), the Z axis a `C3DVector(0,0,1)` temporary, the angle `1.0f / (fps * TimeFor360Turn)`.
// Layout: PDB NCameraModes::CScriptModuleDolly (0x68 bytes): PosAround +0x4c, DollyVector +0x58,
// TimeFor360Turn +0x64, over the 0x4c-byte CScriptModuleBase (POwner +0x38).
extern long g_FramesPerSecond;                       // 0x01375550

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& other) const
    {
        C3DVector result;
        result.X = X + other.X;
        result.Y = Y + other.Y;
        result.Z = Z + other.Z;
        return result;
    }
    void Rotate(const C3DVector& axis, float angle); // 0x00A13C60
};

struct CCameraState {
    unsigned char _pad00[0x10];
    C3DVector     LookAt;                            // +0x10
    unsigned char _pad1c[0x38];
    C3DVector     Position;                          // +0x54
};

namespace NCameraModes {

struct CScriptModuleDolly {
    virtual void v00();
    unsigned char _pad04[0x34];
    CCameraState* POwner;                            // +0x38
    unsigned char _pad3c[0x10];
    C3DVector     PosAround;                         // +0x4c
    C3DVector     DollyVector;                       // +0x58
    float         TimeFor360Turn;                    // +0x64
    void Update();
};

void CScriptModuleDolly::Update()
{
    long fps = g_FramesPerSecond;
    POwner->Position = PosAround + DollyVector;
    POwner->LookAt = PosAround;
    DollyVector.Rotate(C3DVector(0.0f, 0.0f, 1.0f), 1.0f / (fps * TimeFor360Turn));
}

}
