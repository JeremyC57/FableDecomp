// CCard::FrameUpdate(const C3DVector& position, const CRightHandedSet& orientation) -- advance a tavern-game card one frame along its move (lerp LocalPos to NewLocalPos) and its turn (lerp the rotation, lift and tilt the card), then place its mesh on the table
// Leaf: ?FrameUpdate@CCard@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F06F0, length 553 bytes (ret 8). Name: Ego_r.pdb CCard::FrameUpdate; the
// catalog label "CTCTavernGame_UpdateCardTilt" is descriptive only. Caller: CTCTavernGameCardBase::FrameUpdate @ 0x008F1DE4.
// Callee: PositionMesh(CThingObject*, const C3DVector& pos, const CRightHandedSet& rh, const C3DVector& offset, float tilt,
//         float scale) @ 0x005EABC0 (__fastcall global; the landed CTCTavernGameCoinBase::UpdateMeshes declares the 5th
//         parameter `long` and passes 0 -- here it receives a float bit pattern straight from a float slot, so it is a float).
// Globals: g_framesPerSecond 0x01375550 (long). Float pool: 1.0f 0x0122DED8, 0.0f 0x0122DEDC, 0.5f 0x0122F59C; double pool:
//          (double)TWO_PI 0x0124F2B8, 1.0 0x01237F00, 0.25 0x01238010.
// Layout (Ego_r.pdb CCard, 88 bytes, byte-packed vectors, as the landed CCard::Move @ 0x008F0660 / Turn @ 0x008F0620 model
// it): InitialRotation +0x0c, FinalRotation +0x10, TurnSpeed +0x14, TurnFraction +0x18, MoveSpeed +0x1c, MoveFraction +0x20,
// Turning +0x24, Moving +0x26, LocalPos +0x27, NewLocalPos +0x33, CurrentPos +0x3f, Width +0x4c. CRightHandedSet: Up +0x00.
// Shape: dt = 1.0f / fps stays on the x87 stack for both updates (fld st(0) for the move, consumed by the turn). Each
// fraction is `+=` then compared with fcom/fst. The interpolated position is (NewLocalPos - LocalPos) * fraction +
// LocalPos through by-value C3DVector operators (the fraction is spilled to a float slot as the inlined operator's
// parameter); the three paths merge with the position in esi/edi/eax. The turn angle is a double (turn * 2*pi literal):
// the tilt is (1.0 - cos) * 0.25 negated for a negative turn and lives in the dead `orientation` argument slot; the lift
// is sin clamped at 0.0f and CurrentPos = pos + Up * lift * Width * 0.5f, left to right through C3DVector temporaries.
// cos/sin are the /Oi intrinsics (fcos/fsin).
class CThingObject;
#pragma pack(push, 1)
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
};
class CRightHandedSet {
public:
    C3DVector Up;                             // +0x00
    C3DVector Forward;                        // +0x0c
};
class CCard {
public:
    CThingObject* Thing;                      // +0x00
    long      Name;                           // +0x04
    long      Value;                          // +0x08
    float     InitialRotation;                // +0x0c
    float     FinalRotation;                  // +0x10
    float     TurnSpeed;                      // +0x14
    float     TurnFraction;                   // +0x18
    float     MoveSpeed;                      // +0x1c
    float     MoveFraction;                   // +0x20
    bool      Turning;                        // +0x24
    bool      FaceDown;                       // +0x25
    bool      Moving;                         // +0x26
    C3DVector LocalPos;                       // +0x27
    C3DVector NewLocalPos;                    // +0x33
    C3DVector CurrentPos;                     // +0x3f
    unsigned char _pad4b;                     // +0x4b
    float     Width;                          // +0x4c
    float     Height;                         // +0x50
    long      Index;                          // +0x54
    void FrameUpdate(const C3DVector& position, const CRightHandedSet& orientation);
};
#pragma pack(pop)

extern long g_framesPerSecond;                // 0x01375550
static const float TWO_PI = 6.2831855f;       // double pool 0x0124F2B8 = (double)6.2831855f
extern "C" double __cdecl cos(double);
extern "C" double __cdecl sin(double);
#pragma intrinsic(cos, sin)
void __fastcall PositionMesh(CThingObject* piece, const C3DVector& pos, const CRightHandedSet& rh,
                             const C3DVector& offset, float tilt, float scale);   // 0x005EABC0

void CCard::FrameUpdate(const C3DVector& position, const CRightHandedSet& orientation)
{
    float dt = 1.0f / g_framesPerSecond;
    C3DVector pos;
    if (Moving) {
        MoveFraction += dt * MoveSpeed;
        if (MoveFraction >= 1.0f) {
            LocalPos = NewLocalPos;
            pos = NewLocalPos;
            MoveFraction = 0.0f;
            Moving = false;
        } else
            pos = (NewLocalPos - LocalPos) * MoveFraction + LocalPos;
    } else
        pos = LocalPos;
    float tilt;
    if (Turning) {
        TurnFraction += dt * TurnSpeed;
        float turn;
        if (TurnFraction >= 1.0f) {
            turn = FinalRotation;
            InitialRotation = FinalRotation;
            Turning = false;
            TurnFraction = 0.0f;
        } else
            turn = InitialRotation + (FinalRotation - InitialRotation) * TurnFraction;
        double angle = turn * (double)TWO_PI;
        tilt = (float)((1.0 - cos(angle)) * 0.25);
        if (turn < 0.0f)
            tilt = -tilt;
        float lift = (float)sin(angle);
        if (lift < 0.0f)
            lift = 0.0f;
        CurrentPos = pos + orientation.Up * lift * Width * 0.5f;
    } else {
        tilt = FinalRotation;
        CurrentPos = pos;
    }
    PositionMesh(Thing, position, orientation, CurrentPos, tilt, 0.5f);
}