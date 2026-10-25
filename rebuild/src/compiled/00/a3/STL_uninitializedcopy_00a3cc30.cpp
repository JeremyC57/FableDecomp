// std::__uninitialized_copy(const C3DAnimationStateAlpha* first, const C3DAnimationStateAlpha* last, C3DAnimationStateAlpha* result, const __false_type&) -- STLport placement copy of [first, last) into raw storage (const source); returns the end of the copies
// Leaf: ??$__uninitialized_copy@
// Retail: Fable.exe @ 0x00A3CC30, 76 bytes, `ret 8`. /Gr TU written as explicit __fastcall (first ecx, last edx, result
//   and the __false_type tag reference on the stack).
// Caller: 0x00A3CE28 in vector<C3DAnimationStateAlpha>::vector(const vector&) 0x00A3CDD0, which passes the source's
//   const_pointer range -- the second instantiation of the body at 0x00A3CB90 (byte-identical).
// Label: catalog "_Copy_backward<CDisplayModeDesc*,CDisplayModeDesc*>" is a propagated Ego_r label.
// Shape: STLport 4.6 non-POD __uninitialized_copy: placement-new null test, the implicit copy constructor with the
//   three vtable stores (base, mid, derived) interleaved with the dword copies.

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

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
}

template C3DAnimationStateAlpha* __fastcall std::__uninitialized_copy<const C3DAnimationStateAlpha*, C3DAnimationStateAlpha*>(
    const C3DAnimationStateAlpha*, const C3DAnimationStateAlpha*, C3DAnimationStateAlpha*, const std::__false_type&);