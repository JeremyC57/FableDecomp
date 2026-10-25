// std::__uninitialized_fill_n(C3DAnimationStateAlpha* first, unsigned int n, const C3DAnimationStateAlpha& x, const __false_type&) -- STLport placement copy of x into n raw slots; returns the end of the run
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe @ 0x00A3CBE0, 72 bytes (+ int3 padding), `ret 8`. /Gr TU written as explicit __fastcall: first in
//   ecx (moved to eax as `cur`), n in edx counted down, &x and the empty __false_type tag reference on the stack.
// Callers: the uninitialized_fill_n forwarder 0x00A3CCB0 and vector<C3DAnimationStateAlpha>::_M_insert_overflow /
//   _M_fill_insert (0x00A3D11D, 0x00A3D26A).
// Label: catalog "fill<CTextureManagerPoolInfo*,CTextureManagerPoolInfo>" is a propagated Ego_r label.
// Shape: STLport _uninitialized.h `for (; n > 0; --n, ++cur) _Construct(&*cur, x); return cur;` (unsigned size_t:
//   `test edx,edx ; jbe`), placement-new null test, implicit copy constructor (three vtable stores).

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
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
}

template C3DAnimationStateAlpha* __fastcall std::__uninitialized_fill_n<C3DAnimationStateAlpha*, unsigned int, C3DAnimationStateAlpha>(
    C3DAnimationStateAlpha*, unsigned int, const C3DAnimationStateAlpha&, const std::__false_type&);