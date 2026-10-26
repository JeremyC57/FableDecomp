// NCameraModes::CZTargetNew::InitialiseTransitionVariables() -- measure the camera's current offset from the observed thing (XY distance, height, signed angle to the X axis) and set up the per-frame deltas that move it to the def's Z-target offsets over the transition
// Leaf: InitialiseTransitionVariables
// Retail: Fable.exe (TLC, VC7.1) @ 0x00872490, 405 bytes (+ int3 padding), plain `ret`.
// Callees: NCameraModes::CBase::GetObservedThing() const 0x0084F1B0, CThing::GetPosition 0x004C73D0 (const C3DVector&),
//   C3DVector::AccurateNormalise @ 0x00A14440, _CIacos 0x00BFEBA2 (acos intrinsic); constants 1e-4f @ 0x0129BA3C (squared at run
//   time), 0.0f @ 0x0122DEDC, -1.0f @ 0x0122DEE0, 1.0f @ 0x0122DED8, 1/(2*pi) @ 0x01237F08.
// Layout (retail NCameraModes::CBase is 0xA4 bytes, PDB 0xC8; as the landed OnInit 0x00872A20 / Update 0x00873030):
//   PCameraModeDef +0x44, GoTo.Position +0x54; CZTargetNew BaseOffsetXY +0xAC, BaseHeightOffset +0xB0, Theta +0xB4,
//   DeltaBaseOffsetXY +0xB8, DeltaBaseHeightOffset +0xBC, DeltaTheta +0xC0, TargetBaseOffsetXY +0xC4,
//   TargetBaseHeightOffset +0xC8, StringLengthXY +0xCC, CageRadius +0xD0, XAxis +0xD4, FramesForTransition +0xEC.
//   CCameraModeDef (retail = Ego_r.pdb - 4): StringLengthXY +0x40, CageRadius +0x44, HeightOffsetZ +0x48,
//   TransitionInFrames +0x58, ZTargetBaseOffset +0x64.
// Shape: the offset is a C3DVector difference (Z, Y, X order on the x87 stack); its XY length squared (accumulated
//   X*X then += Y*Y: x87 term order follows statement order) stays on the stack for the epsilon test after the def
//   copies; the sign is `1.0f` overwritten by `-1.0f` when `!(cross > 0)` (`test ah,41h ; je`); the dot is accumulated
//   Y, Z, X and clamped to [-1, 1]; `turn = acos * 1/2pi` is named so the sign multiplies last; dividing by the long
//   member each time keeps one fild and three fdivs (a float `frames` local becomes a reciprocal multiply).
extern "C" double __cdecl acos(double);
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(acos, sqrt)

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector& AccurateNormalise();                                   // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
};
extern const float g_epsilon_0129ba3c;                  // 0x0129BA3C (1e-4f)

class CThing { public:
    const C3DVector& GetPosition() const;               // 0x004C73D0
};

class CCameraModeDef { public:
    unsigned char _pad[0x40];
    float StringLengthXY;                               // +0x40
    float CageRadius;                                   // +0x44
    float HeightOffsetZ;                                // +0x48
    float LookOffsetZ;                                  // +0x4c
    float FOV;                                          // +0x50
    float RollAngle;                                    // +0x54
    long TransitionInFrames;                            // +0x58
    float LazyLookAtPosWeight;                          // +0x5c
    bool RaiseUpWhenClose;                              // +0x60
    bool StretchAroundHero;                             // +0x61
    float ZTargetBaseOffset;                            // +0x64
};
struct CPhysicsState {                                  // retail 0x30 bytes
    C3DVector Position;                                 // +0x00
    unsigned char _rest[0x24];
};

namespace NCameraModes {
class CBase {
public:
    virtual void v00();
    unsigned char _pad04[0x40];                         // +0x04..+0x43
    const CCameraModeDef* PCameraModeDef;               // +0x44 (CDefPointer data)
    void* SubComponents[3];                             // +0x48
    CPhysicsState GoTo;                                 // +0x54
    unsigned char _pad84[0x20];
    const CThing* GetObservedThing() const;             // 0x0084F1B0
};
class CZTargetNew : public CBase {
public:
    void* PTargetedThing[2];                            // +0xa4
    float BaseOffsetXY;                                 // +0xac
    float BaseHeightOffset;                             // +0xb0
    float Theta;                                        // +0xb4
    float DeltaBaseOffsetXY;                            // +0xb8
    float DeltaBaseHeightOffset;                        // +0xbc
    float DeltaTheta;                                   // +0xc0
    float TargetBaseOffsetXY;                           // +0xc4
    float TargetBaseHeightOffset;                       // +0xc8
    float StringLengthXY;                               // +0xcc
    float CageRadius;                                   // +0xd0
    C3DVector XAxis;                                    // +0xd4
    C3DVector YAxis;                                    // +0xe0
    long FramesForTransition;                           // +0xec
    void InitialiseTransitionVariables();
};
}

void NCameraModes::CZTargetNew::InitialiseTransitionVariables()
{
    FramesForTransition = PCameraModeDef->TransitionInFrames;
    C3DVector offset = GoTo.Position - GetObservedThing()->GetPosition();
    BaseHeightOffset = offset.Z;
    float lengthSquaredXY = offset.X * offset.X;
    lengthSquaredXY += offset.Y * offset.Y;
    BaseOffsetXY = (float)sqrt(lengthSquaredXY);
    TargetBaseHeightOffset = PCameraModeDef->HeightOffsetZ;
    TargetBaseOffsetXY = PCameraModeDef->ZTargetBaseOffset;
    StringLengthXY = PCameraModeDef->StringLengthXY;
    CageRadius = PCameraModeDef->CageRadius;
    offset.Z = 0.0f;
    if (lengthSquaredXY > g_epsilon_0129ba3c * g_epsilon_0129ba3c)
        offset.AccurateNormalise();
    float sign = 1.0f;
    if (!(offset.X * XAxis.Y - offset.Y * XAxis.X > 0.0f))
        sign = -1.0f;
    float dot = offset.Y * XAxis.Y;
    dot += offset.Z * XAxis.Z;
    dot += offset.X * XAxis.X;
    if (dot < -1.0f)
        dot = -1.0f;
    else if (dot > 1.0f)
        dot = 1.0f;
    float turn = (float)acos(dot) * 0.15915493667125702f;
    Theta = turn * sign;
    DeltaTheta = -(Theta / FramesForTransition);
    DeltaBaseOffsetXY = (TargetBaseOffsetXY - BaseOffsetXY) / FramesForTransition;
    DeltaBaseHeightOffset = (TargetBaseHeightOffset - BaseHeightOffset) / FramesForTransition;
}