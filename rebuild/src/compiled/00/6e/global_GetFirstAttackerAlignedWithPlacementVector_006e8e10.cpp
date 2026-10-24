// GetFirstAttackerAlignedWithPlacementVector(const list<const CThingCreatureBase*>& attackers, const C3DVector& defenderPos, const C3DVector& placementVector) -- the first attacker whose ground-plane direction from the defender lies within 60 degrees of the placement vector (dot of the two flattened unit vectors above cos 60), else null
// Leaf: ?GetFirstAttackerAlignedWithPlacementVector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E8E10, 197 bytes (208 with int3 padding), file-static `__fastcall`
//   (ecx = attackers, edx = defenderPos), `ret 4`.
// Identity: Ego_r.pdb name set `?GetFirstAttackerAlignedWithPlacementVector@?A0xd8b789b9@@YIPBVCThingCreatureBase@@ABV?$list@
//   PBVCThingCreatureBase@@...@std@@ABVC3DVector@@1@Z` (anonymous namespace of the combat-manager TU). No direct call
//   site in the image (an inlined/dead helper kept out of line). The catalog name is the PDB name.
// Callees: C3DVector::Normalise() @ 0x00A14440 (in place, twice per attacker), CThing::GetPosition() const @ 0x004C73D0.
// Globals: the double 1.0471976 (60 degrees, `fld qword ; fcos`) @ 0x01261DC8.
// Layout: STLport list<const CThingCreatureBase*> (4 bytes: the sentinel node; node {next, prev, data +8}).
// Shape: `(float)cos(60 degrees)` through the cos intrinsic into a float local; inside the loop the attacker is read
//   first, the placement copy (X, Y, 0) is rebuilt and normalised each time, the attacker offset is built through the
//   (x, y, z) constructor (arguments right to left: dy before dx); `dot > minDot` is `test ah,41h ; je`.
extern "C" double __cdecl cos(double);
#pragma intrinsic(cos)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void Normalise();                                       // 0x00A14440
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

const CThingCreatureBase* __fastcall GetFirstAttackerAlignedWithPlacementVector(
    const std::list<const CThingCreatureBase*>& attackers, const C3DVector& defenderPos, const C3DVector& placementVector)
{
    float minDot = (float)cos(1.0471976115464443);
    for (std::list<const CThingCreatureBase*>::const_iterator it = attackers.begin(); it != attackers.end(); ++it) {
        const CThingCreatureBase* attacker = *it;
        C3DVector flatPlacement(placementVector.X, placementVector.Y, 0.0f);
        flatPlacement.Normalise();
        const C3DVector& attackerPos = attacker->GetPosition();
        C3DVector toAttacker(attackerPos.X - defenderPos.X, attackerPos.Y - defenderPos.Y, 0.0f);
        toAttacker.Normalise();
        if (toAttacker * flatPlacement > minDot)
            return attacker;
    }
    return 0;
}