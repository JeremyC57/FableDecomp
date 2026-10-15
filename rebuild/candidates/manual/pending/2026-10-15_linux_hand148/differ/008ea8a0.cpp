// NCreatureMode::CModeWalkInAllDirections::GetMovementSpeed(const C3DVector&, const C3DVector&, EMovementType) -- project the direction onto the facing and its perpendicular, find the hover state of that type matching the heading and return its speed (0.0f when none)
// Grade: DIFFER(146v139) -- retail keeps the perpendicular's x in a frame word and its y (-facing.x) in the dead facing-argument slot, then reuses that slot as the predicate's out float; the dot products load facing.z*dir.z first; the lane builds the predicate through a register copy and orders the terms differently
// Leaf: GetMovementSpeed
// Retail: Fable.exe (TLC, VC7.1) @ 0x008EA8A0, 137 bytes (`ret 0Ch`). Vtable slot 7 of NCreatureMode::CModeWalkInAllDirections
// (0x012687AC); the catalog's CModeHoverBase class is propagated.
// Callees: the SGI find_if over the 0x18-byte hover states with the typed heading predicate @ 0x008EBB50 (`__fastcall`
// first/last in ecx/edx, the 16-byte predicate by value), 0.0f @ 0x0122DEDC (both the folded z term and the miss result).
// Layout: the states vector first/last at +0x1c / +0x20; CHoverState::Speed +0x14.
// Shape: as 0x008EA930 plus the movement type in the predicate's third word.
struct C3DVector {
    float x, y, z;
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) { }
    float operator*(const C3DVector& o) const { return x * o.x + y * o.y + z * o.z; }
};
struct C2DVector {
    float x, y;
    C2DVector(float ax, float ay) : x(ax), y(ay) { }
};
struct CHoverState {                        // 0x18 bytes (the 0x2AAAAAAB / sar 2 divide in the finders)
    unsigned char _pad00[0x10];
    long          MovementType;             // +0x10
    float         Speed;                    // +0x14
};

struct STypedHeadingMatch {
    C2DVector m_heading;
    long      m_type;
    float*    m_pScore;
    STypedHeadingMatch(const C2DVector& heading, long type, float* score) : m_heading(heading), m_type(type), m_pScore(score) { }
};
CHoverState* __fastcall FindTypedHoverState(CHoverState* first, CHoverState* last, STypedHeadingMatch match);   // 0x008EBB50

namespace NCreatureMode {
struct CModeWalkInAllDirections {
    unsigned char _pad04[0x18];
    CHoverState*  m_pStatesFirst;           // +0x1c
    CHoverState*  m_pStatesLast;            // +0x20
    virtual float GetMovementSpeed(const C3DVector& direction, const C3DVector& facing, long type);
};

float CModeWalkInAllDirections::GetMovementSpeed(const C3DVector& direction, const C3DVector& facing, long type)
{
    C3DVector side(facing.y, -facing.x, 0.0f);
    float forward = facing * direction;
    float sideways = side * direction;
    float score;
    CHoverState* found = FindTypedHoverState(m_pStatesFirst, m_pStatesLast, STypedHeadingMatch(C2DVector(sideways, forward), type, &score));
    if (found == m_pStatesLast)
        return 0.0f;
    return found->Speed;
}
}
