// std::vector::resize(unsigned int new_size) -- STLport vector<C3DAnimationStateAlpha>::resize(n): resize the alpha-state list to n entries, filling with a default-constructed alpha state (one reference, no animation, full weight)
// Leaf: ?resize@?$vector@VC3DAnimationStateAlpha@@V?$allocator@VC3DAnimationStateAlpha@@@std@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A3D570, 55 bytes (+9 int3), thiscall on the vector, `ret 4`.
// Identity: FableWin.pdb ?resize@?$vector@VC3DAnimationStateAlpha@@...@@QAEXI@Z; no direct call site in the image
//   (xref: 0 calls / 0 references); its callee is the two-argument resize that C3DAnimationBlendState::SetInterpolate
//   0x00A3C3B0 calls on the same vector.
// Callees: vector<C3DAnimationStateAlpha>::resize(n, const C3DAnimationStateAlpha&) 0x00A3D500 (out of line).
// Layout: RTTI of vtable 0x01265128: C3DAnimationStateAlpha : C3DAnimationState : C3DAnimationStateBase :
//   CIVCountedPointeeBase. Ego_r.pdb: vptr +0, IVRefCount +4, AnimationIndex +8, AnimationTime +0xC, AlphaWeight +0x10
//   (20 bytes). Default constructors (FableWin.pdb lists each): IVRefCount 1, AnimationIndex -1, AlphaWeight 1.0f,
//   AnimationTime left unset.
// Shape: STLport 4.x `void resize(size_type n) { resize(n, _Tp()); }` with the two-argument resize out of line: the
//   20-byte temporary is built in the frame after the pushes (count 1, index -1, the derived vtable -- the base vtable
//   stores are dead and dropped -- then the 1.0f weight); the inline virtual destructor of the temporary leaves no
//   code (the two-argument resize carries a same-TU noinline stand-in body, as the retail TU holds the real one).
// Label correction: catalogued as `vector<EWeaponClass>::_Umove<EWeaponClass*>` (propagated); the body is
//   vector<C3DAnimationStateAlpha>::resize(n).
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase() { }                            // slot 0 (??_G)
    unsigned long IVRefCount;                                       // +0x04
    CIVCountedPointeeBase() : IVRefCount(1) { }
};

class C3DAnimationStateBase : public CIVCountedPointeeBase {
public:
    C3DAnimationStateBase() { }
};

class C3DAnimationState : public C3DAnimationStateBase {
public:
    long  AnimationIndex;                                           // +0x08
    float AnimationTime;                                            // +0x0C
    C3DAnimationState() : AnimationIndex(-1) { }
};

class C3DAnimationStateAlpha : public C3DAnimationState {          // vtable 0x01265128
public:
    float AlphaWeight;                                              // +0x10
    C3DAnimationStateAlpha() : AlphaWeight(1.0f) { }
};

namespace std {
template <class _Tp> class allocator {
public:
    allocator() {}
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;                                                  // +0x00
    _Tp* _M_finish;                                                 // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;         // +0x08

    void resize(size_type __new_size, const _Tp& __x);              // 0x00A3D500
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
};
}

// 0x00A3D500 resize(n, x) (out of line in retail): a same-TU noinline stand-in that, like the retail body, never
// writes through the value reference (not graded)
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void std::vector<_Tp, _Alloc>::resize(unsigned int __new_size, const _Tp& __x)
{
    _Tp* __last = this->_M_start + __new_size;
    for (_Tp* __p = this->_M_finish; __p < __last; ++__p)
        __p->AlphaWeight = __x.AlphaWeight;
    this->_M_finish = __last;
}
#pragma auto_inline(on)

template void std::vector<C3DAnimationStateAlpha>::resize(unsigned int);