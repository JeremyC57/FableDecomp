// NCameraModes::CScriptModuleCameraOnAStick::Update() -- look at the thing's position plus LocalRel and put the owning camera RelVector away from that point
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00852400, length 144 bytes (133 + int3 padding).
// Callees: CThing::GetPosition @ 0x004C73D0 (out of line here; returns `[this+60h]+0Ch`).
// Shape: the look-at point is a named local filled field by field from the position reference
// (`pos.X + LocalRel.X` loads the right operand first, straight into the frame -- `pos + LocalRel`
// through operator+ adds a temporary copy); the camera position is `RelVector + lookAt` through the
// inline member operator+ of the landed GoToLookInDirection model (0x00851E70), stored through integer
// registers to the owner's Position (+0x54), then the look-at copied to the owner's LookAt (+0x10).
// Layout: PDB NCameraModes::CScriptModuleCameraOnAStick (0x68 bytes): PThing (CThing*) +0x4c,
// LocalRel +0x50, RelVector +0x5c, over the 0x4c-byte CScriptModuleBase (POwner +0x38).
struct C3DVector {
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

struct CThing {
    const C3DVector& GetPosition() const;            // 0x004C73D0
};

struct CCameraState {
    unsigned char _pad00[0x10];
    C3DVector     LookAt;                            // +0x10
    unsigned char _pad1c[0x38];
    C3DVector     Position;                          // +0x54
};

namespace NCameraModes {

struct CScriptModuleCameraOnAStick {
    virtual void v00();
    unsigned char _pad04[0x34];
    CCameraState* POwner;                            // +0x38
    unsigned char _pad3c[0x10];
    CThing*       PThing;                            // +0x4c
    C3DVector     LocalRel;                          // +0x50
    C3DVector     RelVector;                         // +0x5c
    void Update();
};

void CScriptModuleCameraOnAStick::Update()
{
    const C3DVector& pos = PThing->GetPosition();
    C3DVector lookAt;
    lookAt.X = pos.X + LocalRel.X;
    lookAt.Y = pos.Y + LocalRel.Y;
    lookAt.Z = pos.Z + LocalRel.Z;
    POwner->Position = RelVector + lookAt;
    POwner->LookAt = lookAt;
}

}