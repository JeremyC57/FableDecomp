// std::__uninitialized_copy(CActiveSoundInfo* first, CActiveSoundInfo* last, CActiveSoundInfo* result, const __false_type&) -- copy-construct the sound records [first, last) into raw storage at result and return the end of the copies
// Leaf: ??$__uninitialized_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AACB10, length 58 bytes (+6 int3), __fastcall (first in ecx, last in
//   edx, result and the empty tag reference on the stack, `ret 8`), returns the end of the copies in eax.
// Identity: both callers (0x00AACDFB, 0x00AACE54) are in vector<CActiveSoundInfo>::_M_insert_overflow 0x00AACDA0 (the
//   push_back 0x00AACEF0 calls it); the element is 16 bytes with vtable 0x0129E674 = CActiveSoundInfo (rtti-vtables.tsv,
//   landed constructor 0x00AAB810). The manifest's "_Copy_backward<C3DVector*,C3DVector*>" is propagated and wrong.
// Shape: STLport 4.x `for (; first != last; ++first, ++cur) _Construct(&*cur, *first)`: the placement new keeps VC's
//   null test, the implicit copy constructor of the polymorphic record stores the vtable then copies the three words.
inline void* operator new(unsigned int, void* p) { return p; }

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

class CActiveSoundInfo : public CBaseClass {       // vtable 0x0129E674, 16 bytes
public:
    void* m_pSound;                                // +0x04
    void* m_pOwner;                                // +0x08
    void* m_pInfo;                                 // +0x0c
    virtual ~CActiveSoundInfo();
};

namespace std {
struct __false_type { __false_type() {} };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template CActiveSoundInfo* __fastcall __uninitialized_copy(CActiveSoundInfo*, CActiveSoundInfo*, CActiveSoundInfo*,
                                                           const __false_type&);
}