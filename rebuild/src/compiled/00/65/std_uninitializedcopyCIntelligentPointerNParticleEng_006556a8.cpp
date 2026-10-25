// std::__uninitialized_copy<CIntelligentPointer<NParticleEngine::CParticleEmitter>*,
// CIntelligentPointer<NParticleEngine::CParticleEmitter>*>(first, last, result, const __false_type&) -- STLport:
// copy-construct [first, last) at result, return the advanced destination
// Leaf: ??$__uninitialized_copy@PAV?$CIntelligentPointer@VCParticleEmitter@NParticleEngine@@@@PAV1@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006556A8, 43 bytes, `__fastcall` (first in ecx, last in edx, result and the empty
//   __false_type reference on the stack, `ret 8`); placement new keeps the destination null test; the copy constructor is
//   out of line (0x00654DAF).
// Identity (FINDINGS chunk 383: the landed comment called it Dinkumware `_Uninit_copy` with a by-value allocator; the
//   retail STL is STLport and the second stack argument is the tag): its only callers are the landed
//   vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> >::_M_insert_overflow 0x006566C3 (which declares this
//   exact __fastcall __uninitialized_copy(first, last, result, const __false_type&)) and _M_fill_insert 0x006568F2; the
//   element's copy constructor 0x00654DAF pins the type.
// Shape: STLport 4.x verbatim `for (; first != last; ++first, ++cur) _Copy_Construct(&*cur, *first); return cur;`.
#pragma optimize("s", on)

#include <new>

class CBaseObject;
namespace NParticleEngine { class CParticleEmitter; }

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();               // 0x00A01C10
    CBaseObject* PItem;                               // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(const CIntelligentPointer<T>& other);   // 0x00654DAF
};

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Copy_Construct(&*__cur, *__first);
    return __cur;
}

template CIntelligentPointer<NParticleEngine::CParticleEmitter>* __fastcall __uninitialized_copy(
    CIntelligentPointer<NParticleEngine::CParticleEmitter>*, CIntelligentPointer<NParticleEngine::CParticleEmitter>*,
    CIntelligentPointer<NParticleEngine::CParticleEmitter>*, const __false_type&);
}
