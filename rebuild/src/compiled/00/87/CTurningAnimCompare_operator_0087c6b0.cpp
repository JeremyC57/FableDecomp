// CTurningAnimCompare::operator()(const CAnimationEntry* entry) const -- is the entry's rotation within 45 degrees of the wanted rotation (dot of the two unit directions, clamped, against cos(pi/4))
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- statement-order dot product in C2DVector::Dot (`float d = Y * o.Y; d += X * o.X;`): VC7.1 then multiplies the two register-held cosines first and adds the spilled sines' product, as retail
// Leaf: ??RCTurningAnimCompare@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087C6B0, length 146 bytes (`ret 4`).
// Identity: Ego_r.pdb `??RCTurningAnimCompare@?A0x37766ae9@@QBE_NPBVCAnimationEntry@@@Z` -- the unary overload of the
//   CCreatureAction_InterruptableMidAttackAutoTurn TU's anonymous-namespace functor, used through
//   `not1(CTurningAnimCompare(angle))` by remove_if (__find_if 0x0087DB50, remove_copy_if 0x0087DDD0 call it).
//   The catalog label "Animation_GetRotationDotProduct" is descriptive.
// Callee: CAnimationEntry::GetRotationInAnimation(float*) const @ 0x005DBFD0 (landed).
// Constants: 0x0124F2B8 = (double)TWO_PI (6.2831855f), 0x0122DEE0 = -1.0f, 0x0122DED8 = 1.0f,
//   0x01261E50 = (double)QUARTER_PI (0.78539819f).
// Shape: rotations are fractions of a turn; each angle is `fraction * (double)TWO_PI` feeding the sin/cos intrinsics
//   (the product shared by both), and a direction is (sin, cos) -- the sine is spilled to the 16-byte frame, the
//   cosine stays on the x87 stack. The rotation itself is homed in the dead `entry` slot.
extern "C" double __cdecl sin(double x);
extern "C" double __cdecl cos(double x);
#pragma intrinsic(sin, cos)

static const float TWO_PI = 6.2831855f;
static const float QUARTER_PI = 0.78539819f;

class CAnimationEntry {
public:
    void GetRotationInAnimation(float* rotation) const; // 0x005DBFD0
};

struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    float Dot(const C2DVector& o) const { float d = Y * o.Y; d += X * o.X; return d; }
};

inline float Clamp(float v, float lo, float hi) { return v < lo ? lo : (v > hi ? hi : v); }

namespace {
class CTurningAnimCompare {                             // Ego_r.pdb, 4 bytes (a class: mangled V)
public:
    float RotationAngle;                                // +0x00
    bool operator()(const CAnimationEntry* entry) const;
};

bool CTurningAnimCompare::operator()(const CAnimationEntry* entry) const
{
    float rotation;
    entry->GetRotationInAnimation(&rotation);
    double entryAngle = rotation * (double)TWO_PI;
    C2DVector entryDirection((float)sin(entryAngle), (float)cos(entryAngle));
    double wantedAngle = RotationAngle * (double)TWO_PI;
    C2DVector wantedDirection((float)sin(wantedAngle), (float)cos(wantedAngle));
    float dot = Clamp(wantedDirection.Dot(entryDirection), -1.0f, 1.0f);
    if (dot > cos(QUARTER_PI))
        return true;
    return false;
}
}