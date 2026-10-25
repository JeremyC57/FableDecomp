// std::__uninitialized_fill_n<CIntelligentPointer<NParticleEngine::CParticleEmitter>*,unsigned int,
// CIntelligentPointer<NParticleEngine::CParticleEmitter> >(first, n, x, const __false_type&) -- STLport: copy-construct n
// copies of x from first, return the advanced pointer
// Leaf: ??$__uninitialized_fill_n@PAV?$CIntelligentPointer@VCParticleEmitter@NParticleEngine@@@@IV1@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00654E90, 38 bytes, `__fastcall` (first in ecx, n in edx, the value reference and the
//   empty __false_type reference on the stack, `ret 8`); placement new keeps the destination null test; the copy
//   constructor is out of line (0x00654DAF, landed CIntelligentPointer<NParticleEngine::CParticleEmitter> copy ctor).
// Identity (FINDINGS chunk 383: the landed comment called it Dinkumware `_Uninit_fill_n` with a by-value allocator; the
//   retail STL is STLport and the second stack argument is the tag): callers are the landed
//   vector<CIntelligentPointer<NParticleEngine::CParticleEmitter> >::_M_insert_overflow 0x006566C3 (which declares this
//   exact __fastcall __uninitialized_fill_n(first, n, x, const __false_type&)) and _M_fill_insert 0x006568F2, plus the
//   uncalled uninitialized_fill_n front end 0x006556F2; the element's copy constructor 0x00654DAF pins the type.
// Shape: STLport 4.x verbatim `for (; n > 0; --n, ++cur) _Copy_Construct(&*cur, x); return cur;`.
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

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Copy_Construct(&*__cur, __x);
    return __cur;
}

template CIntelligentPointer<NParticleEngine::CParticleEmitter>* __fastcall __uninitialized_fill_n(
    CIntelligentPointer<NParticleEngine::CParticleEmitter>*, unsigned int, const CIntelligentPointer<NParticleEngine::CParticleEmitter>&,
    const __false_type&);
}
