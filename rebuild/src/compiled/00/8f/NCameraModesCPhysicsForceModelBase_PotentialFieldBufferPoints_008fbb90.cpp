// NCameraModes::CPhysicsForceModelBase::PotentialFieldBufferPoints(const C3DVector& pos, const list<C3DVector>& points, float strength, float radius) const -- the gradient of the summed potential field of the buffer points at pos, by forward differences over the four sample offsets (pos itself and one 0.1 step along each axis)
// Leaf: ?PotentialFieldBufferPoints@CPhysicsForceModelBase@NCameraModes@@IBE?AVC3DVector@@ABV3@ABV?$list@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@MM@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLength respelled GetMagnitude (the header / FableWin.pdb method, same computation).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008FBB90, 288 bytes (+ int3 padding), thiscall (`this` unused) with a
//   hidden result pointer, `ret 14h` (result, pos, points, strength, radius); returns the result pointer in eax.
// Identity: the catalog's "Light_ComputeContributionKernel" is descriptive. FableWin.pdb names
//   `?PotentialFieldBufferPoints@CPhysicsForceModelBase@NCameraModes@@IBE?AVC3DVector@@ABV3@ABV?$list@VC3DVector@@..@MM@Z`
//   and the inline `?PotentialField@CPhysicsForceModelBase@NCameraModes@@IBEMABVC3DVector@@0MM@Z`; the TLC camera
//   physics carries its own copy of the CTools helper (landed twin NCameraModes::CTools::PotentialFieldBufferPoints
//   0x00876E10). No direct callers (reached through the force model's evasion code).
// Globals: the four sample offsets 0x013893AC ({0,0,0}, {1,0,0}, {0,1,0}, {0,0,1}; .data, statically initialised;
//   descriptive name). Constants: 0.1f 0x0122EE58 (the sample step and the strength scale), 10.0f 0x01237F0C.
// Shape: the 0x00876E10 model with the step a literal 0.1f instead of a global: four zeroed float potentials; for each
//   point (STLport list: sentinel _M_node, node data at +8) and each offset `potential[i] += PotentialField(pos +
//   offset * 0.1f, point, strength, radius)` with the `strength * 0.1f` product hoisted out of both loops (kept on the
//   x87 stack); the forward differences are divided by the literal step, which VC7.1 folds into a multiply by 10.0f.
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

#include "engine/C3DVector.h"

namespace std {
template <class _Tp> class allocator { };
struct _List_node_base {
    _List_node_base* _M_next;                               // +0x00
    _List_node_base* _M_prev;                               // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x08
};
template <class _Tp> struct _List_const_iterator {
    _List_node_base* _M_node;
    _List_const_iterator(_List_node_base* __x) : _M_node(__x) { }
    const _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_const_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_const_iterator& __x) const { return _M_node != __x._M_node; }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class list {
public:
    typedef _List_const_iterator<_Tp> const_iterator;
    _List_node<_Tp>* _M_node;                               // the sentinel
    const_iterator begin() const { return const_iterator(_M_node->_M_next); }
    const_iterator end() const { return const_iterator(_M_node); }
};
} // namespace std

extern C3DVector g_ForceModelPotentialSampleOffsets[4];     // 0x013893AC

namespace NCameraModes {
class CPhysicsForceModelBase {
protected:
    float PotentialField(const C3DVector& pos, const C3DVector& source, float strength, float radius) const
    {
        return (strength * 0.1f) / ((pos - source).GetMagnitude() - radius);
    }
    C3DVector PotentialFieldBufferPoints(const C3DVector& pos, const std::list<C3DVector>& points, float strength,
                                         float radius) const;
};
}

C3DVector NCameraModes::CPhysicsForceModelBase::PotentialFieldBufferPoints(const C3DVector& pos,
                                                                          const std::list<C3DVector>& points,
                                                                          float strength, float radius) const
{
    float potential[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    for (std::list<C3DVector>::const_iterator it = points.begin(); it != points.end(); ++it) {
        for (int i = 0; i < 4; ++i)
            potential[i] += PotentialField(pos + g_ForceModelPotentialSampleOffsets[i] * 0.1f, *it, strength, radius);
    }
    return C3DVector((potential[1] - potential[0]) / 0.1f,
                     (potential[2] - potential[0]) / 0.1f,
                     (potential[3] - potential[0]) / 0.1f);
}