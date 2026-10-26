// NCameraModes::CScriptModuleFullTransition::Update() -- until EndFrame, step the owning camera's position by Increment and its look-at by IncrementForLookAtPoints
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00852220, length 144 bytes (133 + int3 padding).
// Callees: GFGetCurrentFrame @ 0x0049D870 (`__fastcall`, no arguments).
// Shape: `if (frame < EndFrame)`, then two `X = X + step` updates through the inline member operator+
// in its CONSTRUCTOR form (`return C3DVector(X + o.X, ...)`: right operand loaded first on all three terms,
// the sum in a frame temporary copied by integer moves; the owner re-read for the second update). The
// named-result body (`result.X = X + o.X; ...`) commutes the X term only.
// Layout: PDB NCameraModes::CScriptModuleFullTransition (0x80 bytes): IncrementForLookAtPoints +0x4c,
// Increment +0x58, LookAtPos +0x64, StartPos +0x70, EndFrame +0x7c, over the 0x4c-byte CScriptModuleBase
// (POwner +0x38: the camera state with LookAt +0x10, Position +0x54).
long __fastcall GFGetCurrentFrame();                 // 0x0049D870

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& other) const { return C3DVector(X + other.X, Y + other.Y, Z + other.Z); }
};

struct CCameraState {
    unsigned char _pad00[0x10];
    C3DVector     LookAt;                            // +0x10
    unsigned char _pad1c[0x38];
    C3DVector     Position;                          // +0x54
};

namespace NCameraModes {

struct CScriptModuleFullTransition {
    virtual void v00();
    unsigned char _pad04[0x34];
    CCameraState* POwner;                            // +0x38
    unsigned char _pad3c[0x10];
    C3DVector     IncrementForLookAtPoints;          // +0x4c
    C3DVector     Increment;                         // +0x58
    C3DVector     LookAtPos;                         // +0x64
    C3DVector     StartPos;                          // +0x70
    long          EndFrame;                          // +0x7c
    void Update();
};

void CScriptModuleFullTransition::Update()
{
    if (GFGetCurrentFrame() < EndFrame) {
        POwner->Position = POwner->Position + Increment;
        POwner->LookAt = POwner->LookAt + IncrementForLookAtPoints;
    }
}

}