// _STL::__uninitialized_copy(CParticleEmitterInfo* first, CParticleEmitterInfo* last, CParticleEmitterInfo* result, const __false_type&) -- copy-construct the particle emitter info records of [first, last) into raw storage at result and return the end of the copies
// Leaf: ??$__uninitialized_copy@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF1C10, 57 bytes, __fastcall (first ecx, last edx; result and the empty tag on the
//   stack, `ret 8`).
// Callers: vector<NParticleEngine::CParticleEmitterInfo>::_M_insert_overflow 0x00AF1E90 (0x00AF1EE9 / 0x00AF1F45: the prefix
//   and suffix copies out of _M_start, so the iterators are plain non-const pointers) and the _M_fill_insert path at
//   0x00AF209A / 0x00AF20F1.
// Element: NParticleEngine::CParticleEmitterInfo (Ego_r.pdb, 12 bytes): CIVCountedPointer<CEmitterData> Data +0 (the copy
//   bumps CIVCountedPointeeBase::IVRefCount +4), CParticleDataBank* Bank +4, unsigned long Index +8.
// Label: catalog "_Copy_impl<CNavigationPosition*,CNavigationPosition*>" is propagated.
// Shape: STLport 4.6 __uninitialized_copy verbatim (`for (; __first != __last; ++__first, ++__cur) _Construct(&*__cur,
//   *__first);`), the placement new keeping its null test.
inline void* operator new(unsigned int, void* p) { return p; }

class CIVCountedPointeeBase {
public:
    void*         VTable;                                   // +0x0
    unsigned long IVRefCount;                               // +0x4
};

template <class T> class CIVCountedPointer {
public:
    CIVCountedPointeeBase* Object;                          // +0x0
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
};

namespace NParticleEngine {
class CParticleDataBank;
class CParticleEmitterInfo {
public:
    class CEmitterData;
    CIVCountedPointer<CEmitterData> Data;                   // +0x0
    CParticleDataBank*              Bank;                   // +0x4
    unsigned long                   Index;                  // +0x8
};
}

namespace _STL {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
inline _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                    const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template NParticleEngine::CParticleEmitterInfo* __fastcall
__uninitialized_copy(NParticleEngine::CParticleEmitterInfo*, NParticleEngine::CParticleEmitterInfo*,
                     NParticleEngine::CParticleEmitterInfo*, const __false_type&);
}