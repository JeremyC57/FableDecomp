// GetBestAttackerForPlacementVector(const list<const CThingCreatureBase*>& attackers, const C3DVector& defenderPos, const C3DVector& placementVector) -- the attacker whose ground-plane direction from the defender best lines up with the placement vector (largest dot of the two flattened unit vectors, clamped to [-1, 1]), else null
// Grade: DIFFER(298v294) 13 difflines (was 17): stepwise GetMagnitude (l = Z*Z; l += Y*Y; l += X*X) gives retail's Y-before-X squaring; left: retail keeps inv on the stack (fst [esp+38h]; fmul st(2) ... fmul [esp+38h]), ours fld inv; fmul st(1) ... fstp st(0) (retry wave 2 sec2b)
// Was: DIFFER(298v294) -- x87 scheduling of the first inlined Normalise (retail squares y before x and keeps inv on the stack: `fst [esp+38h] ; fmul st(2)`, ours `fstp` + reload) and the start-up `fchs ; fstp` (fixed by declaring `best` before `bestDot`, grade 4). 4 grades: X*X+Y*Y+Z*Z, Z*Z+Y*Y+X*X (no change), `*this *= 1.0f / GetMagnitude()` (fixed the second Normalise), copy-constructed flat vector + declaration order (worse dot schedule; this file is grade 3 plus the grade-4 declaration order, ungraded as a combination)
// Leaf: ?GetBestAttackerForPlacementVector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E8CE0, 294 bytes (304 with int3 padding), file-static `__fastcall`
//   (ecx = attackers, edx = defenderPos), `ret 4`.
// Identity: Ego_r.pdb / FableWin.pdb name set `?GetBestAttackerForPlacementVector@?A0xd8b789b9@@YIPBVCThingCreatureBase@@
//   ABV?$list@PBVCThingCreatureBase@@...@std@@ABVC3DVector@@1@Z` (anonymous namespace of the combat-manager TU, beside
//   the landed GetFirstAttackerAlignedWithPlacementVector 0x006E8E10). No direct call site in the image.
// Callees: CThing::GetPosition() const @ 0x004C73D0. Globals: 1e-4f @ 0x0129BA3C (a non-constant float, `fld ; fadd 1.0 ;
//   fchs`), 1.0f 0x0122DED8, 0.0f 0x0122DEDC, -1.0f 0x0122DEE0.
// Layout: STLport list<const CThingCreatureBase*> (4 bytes: the sentinel node; node {next, prev, data +8}).
// Shape: as the sibling, but C3DVector::Normalise (out of line at 0x00A14440, z*z + y*y + x*x) is inlined both times, so
//   the flat Z (0) folds out of the magnitude and survives only as `inv * 0.0f`; the offset is `pos - defenderPos` with its
//   Z zeroed (dx before dy); the dot clamped to [-1, 1] before `dot > bestDot`.
#include <math.h>
#pragma intrinsic(sqrt)

extern const float g_Epsilon;                               // 0x0129BA3C (1e-4f)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float GetMagnitude() const { float l = Z * Z; l += Y * Y; l += X * X; return (float)sqrt(l); }
    C3DVector& operator*=(float f)
    {
        X *= f;
        Y *= f;
        Z *= f;
        return *this;
    }
    __forceinline C3DVector& Normalise()
    {
        return *this *= 1.0f / GetMagnitude();
    }
};
inline float operator*(const C3DVector& a, const C3DVector& b) { return a.Z * b.Z + (a.X * b.X + a.Y * b.Y); }

class CThing {
public:
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};
class CThingCreatureBase : public CThing { };

namespace std {
struct _List_node_base {
    _List_node_base* _M_next;                               // +0x00
    _List_node_base* _M_prev;                               // +0x04
};
template <class T> struct _List_node : public _List_node_base {
    T _M_data;                                              // +0x08
};
template <class T> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* x) : _M_node(x) { }
    T& operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& y) const { return _M_node != y._M_node; }
};
template <class T> class allocator { };
template <class T, class A = allocator<T> > class list {
public:
    typedef _List_iterator<T> const_iterator;
    _List_node_base* _M_node;                               // the sentinel
    const_iterator begin() const { return const_iterator(_M_node->_M_next); }
    const_iterator end() const { return const_iterator(_M_node); }
};
}

namespace {

const CThingCreatureBase* __fastcall GetBestAttackerForPlacementVector(
    const std::list<const CThingCreatureBase*>& attackers, const C3DVector& defenderPos, const C3DVector& placementVector)
{
    const CThingCreatureBase* best = 0;
    float bestDot = -(1.0f + g_Epsilon);
    for (std::list<const CThingCreatureBase*>::const_iterator it = attackers.begin(); it != attackers.end(); ++it) {
        const CThingCreatureBase* attacker = *it;
        C3DVector flatPlacement(placementVector.X, placementVector.Y, 0.0f);
        flatPlacement.Normalise();
        const C3DVector& attackerPos = attacker->GetPosition();
        C3DVector toAttacker(attackerPos.X - defenderPos.X, attackerPos.Y - defenderPos.Y, 0.0f);
        toAttacker.Normalise();
        float dot = toAttacker * flatPlacement;
        if (dot < -1.0f)
            dot = -1.0f;
        else if (dot > 1.0f)
            dot = 1.0f;
        if (dot > bestDot) {
            bestDot = dot;
            best = attacker;
        }
    }
    return best;
}

}

const CThingCreatureBase* (__fastcall* g_pGetBestAttackerForPlacementVector)(
    const std::list<const CThingCreatureBase*>&, const C3DVector&, const C3DVector&) = &GetBestAttackerForPlacementVector;   // keeps the internal-linkage body emitted; not part of the retail function
