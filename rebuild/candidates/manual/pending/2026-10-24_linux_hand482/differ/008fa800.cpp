// NCameraModes::CPhysicsForceModelBase::DoAttractToPoint() -- the camera's attraction force: each incidental attractor pulls along the unit direction with a Gaussian-derivative profile (peak 5 at distance 10: dir * (sqrt(0.01) * 5 / exp(-0.5)) * d * exp(-0.005 * d^2)) times its weight, skipping attractors closer than 0.0001; each plain attractor adds its offset times its weight
// Grade: DIFFER(496v496) -- the x87 instruction stream matches (both exp expansions, the accumulators on the FP stack, the per-component K/d/e/weight products) except that the frame is 0x5C instead of 0x50 (one more 12-byte vector temporary, so every spill slot sits 0xC higher) and the falloff exp result is stored twice (`fst` + `fstp dword [esp+8]`) where retail stores it once. tried: float locals with casts (560v496), user copy ctor (fixes the enregistered accumulator), a named pull vector, scalar-on-left operator* (both 496v496, same residue)
// Leaf: ?DoAttractToPoint@
// Retail: Fable.exe (TLC, VC7.1) @ 0x008FA800, 496 bytes (incl. int3 padding), virtual thiscall returning C3DVector
//   through the hidden pointer (`ret 4`). FableWin.pdb `?DoAttractToPoint@CPhysicsForceModelBase@NCameraModes@@UAE?AVC3DVector@@XZ`;
//   caller 0x008FCBD7 (FrameUpdate). No callees: exp() is VC7.1's inline fldl2e/f2xm1/fscale expansion.
// Layout: CPhysicsForceModelBase::LinkedState +4 (CPhysicsState*); retail CPhysicsState: Position +0, Velocity +0xC,
//   Acceleration +0x18, Attractors +0x24 and IncidentalAttractors +0x28 (4-byte STLport lists of pair<C3DVector, float>;
//   the PDB's 12-byte lists put them at 0x24/0x30).
#include <math.h>
#pragma intrinsic(sqrt, exp)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector& operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; return *this; }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};
inline C3DVector operator*(float s, const C3DVector& v) { return C3DVector(v.X * s, v.Y * s, v.Z * s); }

namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
class _List_node_base {
public:
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> class _List_node : public _List_node_base {
public:
    _Tp _M_data;
};
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    _List_node<_Tp>* _M_node;
    class const_iterator {
    public:
        _List_node<_Tp>* _M_node;
        const_iterator(_List_node<_Tp>* n) : _M_node(n) { }
        const _Tp& operator*() const { return _M_node->_M_data; }
        const _Tp* operator->() const { return &_M_node->_M_data; }
        const_iterator& operator++() { _M_node = (_List_node<_Tp>*)_M_node->_M_next; return *this; }
        bool operator!=(const const_iterator& o) const { return _M_node != o._M_node; }
    };
    const_iterator begin() const { return const_iterator((_List_node<_Tp>*)_M_node->_M_next); }
    const_iterator end() const { return const_iterator(_M_node); }
};
} // namespace std

namespace NCameraModes {
typedef std::list<std::pair<C3DVector, float> > CAttractorList;
class CPhysicsState {
public:
    C3DVector      Position;                        // +0x00
    C3DVector      Velocity;                        // +0x0C
    C3DVector      Acceleration;                    // +0x18
    CAttractorList Attractors;                      // +0x24
    CAttractorList IncidentalAttractors;            // +0x28
};
class CPhysicsForceModelBase {
public:
    virtual C3DVector DoAttractToPoint();
    CPhysicsState* LinkedState;                     // +0x04
    C3DVector GetPosition() { return LinkedState->Position; }
    const CAttractorList& GetAttractors() const { return LinkedState->Attractors; }
    const CAttractorList& GetIncidentalAttractors() const { return LinkedState->IncidentalAttractors; }
};

C3DVector CPhysicsForceModelBase::DoAttractToPoint()
{
    C3DVector force(0.0f, 0.0f, 0.0f);
    C3DVector position = GetPosition();
    for (CAttractorList::const_iterator it = GetIncidentalAttractors().begin(); it != GetIncidentalAttractors().end(); ++it) {
        C3DVector delta = it->first - position;
        float distance = delta.GetLength();
        if (distance > 0.0001f) {
            C3DVector direction = delta * (1.0f / distance);
            float invSigma = (float)sqrt(0.01);
            float scale = invSigma * 5.0f / exp(-0.5);
            force += exp(distance * distance * -0.005f) * (direction * scale * distance) * it->second;
        }
    }
    for (CAttractorList::const_iterator it2 = GetAttractors().begin(); it2 != GetAttractors().end(); ++it2)
        force += (it2->first - position) * it2->second;
    return force;
}
} // namespace NCameraModes
