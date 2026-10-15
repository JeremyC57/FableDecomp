// NCreatureMode::CModeWalkInAllDirections::GetHoverState(const C3DVector&, const C3DVector&) -- project the direction onto the facing and its perpendicular and return the first hover state matching that 2D heading (0 when none)
// Grade: DIFFER(127v121) -- same dead-slot sharing as 008ea8a0 (12-byte predicate, `sete ; dec ; and` null-on-miss tail)
// Leaf: GetHoverState
// Retail: Fable.exe (TLC, VC7.1) @ 0x008EA930, 120 bytes (`ret 8`).
// Label: the catalog's "CModeHoverBase::GetMovementSpeed" is propagated; the twin 0x008EA8A0 is vtable slot 7 of
// NCreatureMode::CModeWalkInAllDirections (0x012687AC); this body returns the found record, so it is named descriptively.
// Callees: the SGI find_if over the 0x18-byte hover states with the heading predicate @ 0x008EBC90 (`__fastcall`
// first/last in ecx/edx, the 12-byte predicate by value), 0.0f @ 0x0122DEDC.
// Layout: the states vector first/last at +0x1c / +0x20 (retail 12-byte vector).
// Shape: the perpendicular (facing.y, -facing.x, 0) is a named local whose y is homed in the dead facing slot; the
// predicate's third word is that slot's address (a match-score out pointer).
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

struct SHeadingMatch {
    C2DVector m_heading;
    float*    m_pScore;
    SHeadingMatch(const C2DVector& heading, float* score) : m_heading(heading), m_pScore(score) { }
};
CHoverState* __fastcall FindHoverState(CHoverState* first, CHoverState* last, SHeadingMatch match);   // 0x008EBC90

namespace NCreatureMode {
struct CModeWalkInAllDirections {
    unsigned char _pad00[0x1c];
    CHoverState*  m_pStatesFirst;           // +0x1c
    CHoverState*  m_pStatesLast;            // +0x20
    CHoverState* GetHoverState(const C3DVector& direction, const C3DVector& facing);
};

CHoverState* CModeWalkInAllDirections::GetHoverState(const C3DVector& direction, const C3DVector& facing)
{
    C3DVector side(facing.y, -facing.x, 0.0f);
    float forward = facing * direction;
    float sideways = side * direction;
    float score;
    CHoverState* found = FindHoverState(m_pStatesFirst, m_pStatesLast, SHeadingMatch(C2DVector(sideways, forward), &score));
    return found == m_pStatesLast ? 0 : found;
}
}
