// NCameraModes::CScriptModuleDollyThing::Update() -- put the owning camera at RotThing's position + DollyVector looking at RotThing, then turn DollyVector about Z by one frame's share of a full turn
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00852350, length 172 bytes (`ret`).
// Callees: CThing::GetPos() const @ 0x004C73D0 (twice), C3DVector::Rotate(const C3DVector& axis, float angle) @ 0x00A13C60.
// Globals: the frames-per-second count @ 0x01375550 (a long).
// Shape: the landed CScriptModuleDolly::Update 0x008522B0 with the thing's position for PosAround: the frame rate read
// into a long local first, `position + DollyVector` through the inline member operator+ (VC loads the right operand
// first, so retail's `fld DollyVector ; fadd position` needs the position on the left), the owner taken into a local
// before the second GetPos (kept in ebx across the call) for the look-at copy, the Z axis a C3DVector(0,0,1) temporary, angle 1 / (fps * TimeFor360Turn).
// Layout: Ego_r.pdb NCameraModes::CScriptModuleDollyThing (0x60 bytes): RotThing +0x4c, DollyVector +0x50,
// TimeFor360Turn +0x5c, over the 0x4c-byte CScriptModuleBase (POwner +0x38).
extern long g_FramesPerSecond;                       // 0x01375550

class C3DVector { public:
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

class CThing {
public:
    const C3DVector& GetPos() const;            // 0x004C73D0
};

struct CCameraState {
    unsigned char _pad00[0x10];
    C3DVector     LookAt;                            // +0x10
    unsigned char _pad1c[0x38];
    C3DVector     Position;                          // +0x54
};

namespace NCameraModes {

struct CScriptModuleDollyThing {
    virtual void v00();
    unsigned char _pad04[0x34];
    CCameraState* POwner;                            // +0x38
    unsigned char _pad3c[0x10];
    CThing*       RotThing;                          // +0x4c
    C3DVector     DollyVector;                       // +0x50
    float         TimeFor360Turn;                    // +0x5c
    void Update();
};

void CScriptModuleDollyThing::Update()
{
    long fps = g_FramesPerSecond;
    POwner->Position = RotThing->GetPos() + DollyVector;
    CCameraState* owner = POwner;
    owner->LookAt = RotThing->GetPos();
    DollyVector.Rotate(C3DVector(0.0f, 0.0f, 1.0f), 1.0f / (fps * TimeFor360Turn));
}

}