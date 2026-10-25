// std::fill(C3DAnimationStateAlpha* first, C3DAnimationStateAlpha* last, const C3DAnimationStateAlpha& val) -- STLport fill: assign val to every element of [first, last)
// Leaf: ??$fill@
// Retail: Fable.exe @ 0x00A3CEE0, 51 bytes (+ int3 padding), `ret 4`. /Gr TU written as explicit __fastcall: first in
//   ecx, last in edx, &val on the stack.
// Callers: 0x00A3D24C / 0x00A3D2A9 in vector<C3DAnimationStateAlpha>::_M_fill_insert.
// Label: catalog "fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo>" names the right algorithm but a propagated
//   element type; the element is C3DAnimationStateAlpha (20-byte stride, vptr left alone).
// Shape: STLport 4.6 _algobase.h `for (; first != last; ++first) *first = val;` -- the implicit assignment copies the
//   four data dwords +4..+0x10 through esi.

inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

// Local polymorphic model (engine/C3DAnimationStateAlpha.h flattens the vptr into a data member, which cannot produce
// the per-level vtable stores of the implicit copy constructor). Layout per Ego_r.pdb: CIVCountedPointeeBase is
// {vptr, IVRefCount} (8 bytes), C3DAnimationState adds AnimationIndex +0x08 / AnimationTime +0x0C, and
// C3DAnimationStateAlpha adds AlphaWeight +0x10 (0x14 bytes). Copy constructor and assignment are implicit: the copy
// stores the three vtables 0x012354D0 / 0x01249F50 / 0x01265128 in base-to-derived order, the assignment skips them.
class CIVCountedPointeeBase {                               // vtable 0x012354D0
public:
    virtual ~CIVCountedPointeeBase();                       // slot 0
    unsigned long IVRefCount;                               // +0x04
};

class C3DAnimationState : public CIVCountedPointeeBase {    // vtable 0x01249F50
public:
    virtual ~C3DAnimationState();
    virtual long GetType() const;
    virtual void Validate();
    long  AnimationIndex;                                   // +0x08
    float AnimationTime;                                    // +0x0C
};

class C3DAnimationStateAlpha : public C3DAnimationState {   // vtable 0x01265128, sizeof 0x14
public:
    virtual long GetType() const;                           // 0x0070FE20
    virtual void Validate();                                // 0x00A3BE20
    float AlphaWeight;                                      // +0x10
};

namespace std {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}
}

template void __fastcall std::fill<C3DAnimationStateAlpha*, C3DAnimationStateAlpha>(C3DAnimationStateAlpha*,
    C3DAnimationStateAlpha*, const C3DAnimationStateAlpha&);