// NCameraModes::CScriptModuleFOVZoom::Update() -- until EndFrame, step the owning camera's position by Increment, aim it at LookAtPos and add FOVIncrement to its field of view
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00852160, length 128 bytes (120 + int3 padding).
// Callees: GFGetCurrentFrame @ 0x0049D870 (`__fastcall`, no arguments).
// Shape: `if (frame < EndFrame)`; the position update is `Position = Position + Increment` through the
// inline member operator+ of the landed GoToLookInDirection model (right operand loaded first, the sum
// in a frame temporary copied by integer moves), the look-at is a plain record copy, the FOV a float
// `+=` on the owner (+0x84) re-read after the copies.
// Layout: PDB NCameraModes::CScriptModuleFOVZoom (see 0x008520F0); the owner (POwner +0x38) is the
// camera state with LookAt +0x10, Position +0x54 and the FOV at +0x84.
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};                 // 0x0049D870

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector operator+(const C3DVector& other) const
    {
        C3DVector result;
        result.X = X + other.X;
        result.Y = Y + other.Y;
        result.Z = Z + other.Z;
        return result;
    }
};

struct CCameraState {
    unsigned char _pad00[0x10];
    C3DVector     LookAt;                            // +0x10
    unsigned char _pad1c[0x38];
    C3DVector     Position;                          // +0x54
    unsigned char _pad60[0x24];
    float         FOV;                               // +0x84
};

namespace NCameraModes {

struct CScriptModuleFOVZoom {
    virtual void v00();
    unsigned char _pad04[0x34];
    CCameraState* POwner;                            // +0x38
    unsigned char _pad3c[0x10];
    C3DVector     LookAtPos;                         // +0x4c
    long          EndFrame;                          // +0x58
    long          WorldFrames;                       // +0x5c
    float         FOVIncrement;                      // +0x60
    float         FOVFraction;                       // +0x64
    C3DVector     Increment;                         // +0x68
    C3DVector     StartPos;                          // +0x74
    bool          ShortenOnOcclusion;                // +0x80
    bool          ResetCamera;                       // +0x81
    void Update();
};

void CScriptModuleFOVZoom::Update()
{
    if (CWorld::GetFrame() < EndFrame) {
        POwner->Position = POwner->Position + Increment;
        POwner->LookAt = LookAtPos;
        POwner->FOV += FOVIncrement;
    }
}

}