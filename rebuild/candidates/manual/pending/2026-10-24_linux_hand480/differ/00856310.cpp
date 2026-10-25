// Grade: DIFFER(594v600) -- x87 stackifier + frame layout: retail keeps the dying (start - pos).XY, |dir.XY|^2 and t on the x87 stack (fld st(i) / fmul st(i), then fstp st(3) / fstp st(0) clean-up) where VC7.1 here consumes them in place (fxch / fdivrp), and retail homes the XY temporaries at the top of a 0x60 frame (ours 0x58); the 3D tail, both end-replacement branches and the loop match. Tried: plain floats, C2DVector temporaries (two forms hit C1001 in fppeeps.c), named offset2D, operand orders, inline helper functions, scoped blocks, /Op /O1 /Os /Ot (1 grade)
// ProcessWeaponTrailPointsToIncludeCylinderCloseToCreatureAxis(vector<pair<C3DVector,C3DVector> >& trailPoints, const CThing& creature) -- for each weapon-trail segment, project the creature's position onto the segment's XY line; when the projected point lies beyond the end nearer the creature, move that end out to the 3D point on the segment's line, so the trail reaches the creature's axis
// Leaf: ?ProcessWeaponTrailPointsToIncludeCylinderCloseToCreatureAxis@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00856310, 600 bytes + 8 int3, /Gr `__fastcall` (ecx = trail points, edx = creature),
//   plain `ret`.
// Identity: Ego_r.pdb ?ProcessWeaponTrailPointsToIncludeCylinderCloseToCreatureAxis@?A0xad05d24c@@YIXAAV?$vector@U?$pair@
//   VC3DVector@@V1@@std@@...@std@@ABVCThing@@@Z (the CCombatActionBase TU's anonymous namespace, ahead of
//   IsThingWithHelperPoints 0x00856570); its only caller is 0x008594E7 in the same TU.
// Callees: CThing::GetPosition() const 0x004C73D0 (`return &PhysicsTC->Position`; catalogued "DoSizeof").
// Globals: the enabling bool 0x0137574B (initialised 1, read only here), the 1e-4f epsilon 0x0129BA3C, -1.0f 0x0122DEE0.
// Shape: dir = end - start (also kept as VC7.1's CSE copy for the far-end branch); the XY projection parameter
//   t = (dir.Y * (start - pos).Y * -1 - dir.X * (start - pos).X) / |dir.XY|^2 (skipped when |dir.XY|^2 <= epsilon), the
//   XY point, its parameter along the dominant axis, the 3D point dir * s + start; the end nearer the creature (XY) is
//   replaced when the point is farther from the other end than it is (MagSquared Z, Y, X order).
#include <math.h>
#pragma intrinsic(fabs)

extern bool g_IncludeCylinderCloseToCreatureAxis;       // 0x0137574B
extern const float g_epsilon_0129ba3c;                  // 0x0129BA3C (1e-4f)

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    float MagSquared() const { return Z * Z + Y * Y + X * X; }
};
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {}
    explicit C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) {}
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    C2DVector operator*(float f) const { return C2DVector(X * f, Y * f); }
    float MagSquared() const { return Y * Y + X * X; }
};

class CThing {
public:
    const C3DVector& GetPosition() const;               // 0x004C73D0
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* iterator;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
};
}

typedef std::vector<std::pair<C3DVector, C3DVector> > TTrailPoints;

namespace {
void __fastcall ProcessWeaponTrailPointsToIncludeCylinderCloseToCreatureAxis(TTrailPoints& trailPoints,
                                                                             const CThing& creature)
{
    if (!g_IncludeCylinderCloseToCreatureAxis)
        return;
    const C3DVector& creaturePos = creature.GetPosition();
    for (TTrailPoints::iterator it = trailPoints.begin(); it != trailPoints.end(); ++it) {
        C3DVector dir = it->second - it->first;
        C2DVector startFromCreature(it->first.X - creaturePos.X, it->first.Y - creaturePos.Y);
        float lengthSquared = dir.Y * dir.Y + dir.X * dir.X;
        if (lengthSquared > g_epsilon_0129ba3c) {
            float t = (dir.Y * startFromCreature.Y * -1.0f - dir.X * startFromCreature.X) / lengthSquared;
            C2DVector offset2D(dir.X * t, dir.Y * t);
            C2DVector closest2D(offset2D.X + it->first.X, offset2D.Y + it->first.Y);
            float s;
            if (fabs(dir.X) > fabs(dir.Y))
                s = (closest2D.X - it->first.X) / dir.X;
            else
                s = (closest2D.Y - it->first.Y) / dir.Y;
            C3DVector closest = dir * s + it->first;
            C2DVector creatureFromStart(creaturePos.X - it->first.X, creaturePos.Y - it->first.Y);
            C2DVector creatureFromEnd(creaturePos.X - it->second.X, creaturePos.Y - it->second.Y);
            if (creatureFromStart.MagSquared() < creatureFromEnd.MagSquared()) {
                C3DVector closestFromEnd = closest - it->second;
                C3DVector startFromEnd = it->first - it->second;
                if (closestFromEnd.MagSquared() > startFromEnd.MagSquared())
                    it->first = closest;
            }
            else {
                C3DVector closestFromStart = closest - it->first;
                C3DVector endFromStart = it->second - it->first;
                if (closestFromStart.MagSquared() > endFromStart.MagSquared())
                    it->second = closest;
            }
        }
    }
}
}

void (__fastcall* g_keepProcessWeaponTrail)(TTrailPoints&, const CThing&) =
    &ProcessWeaponTrailPointsToIncludeCylinderCloseToCreatureAxis;
