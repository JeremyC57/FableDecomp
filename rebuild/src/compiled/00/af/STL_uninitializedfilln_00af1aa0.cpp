// std::__uninitialized_fill_n(CParticleEmitterInfo* first, unsigned int n, const CParticleEmitterInfo& x, const __false_type&) -- copy-construct n copies of a particle emitter info record (counted emitter data + bank + index) into raw storage at first and return first + n
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF1AA0, 53 bytes, __fastcall (first ecx, n edx; x and the empty tag on the stack, `ret 8`).
// Callers: the vector<NParticleEngine::CParticleEmitterInfo> grow/fill paths at 0x00AF1CCA, 0x00AF1F29 and 0x00AF20CE
//   (the resize / _M_fill_insert family around 0x00AF2010 that also calls std::fill 0x00AF1C70, landed).
// Element: NParticleEngine::CParticleEmitterInfo (Ego_r.pdb, 12 bytes): CIVCountedPointer<CEmitterData> Data +0 (the copy
//   bumps CIVCountedPointeeBase::IVRefCount +4), CParticleDataBank* Bank +4, unsigned long Index +8.
// Label: catalog "_Fill_n<unsigned_long*,unsigned_int,unsigned_long>" is propagated.
// Shape: STLport 4.6 __uninitialized_fill_n verbatim (`for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x);`, unsigned n
//   so `jbe`), the placement new keeping its null test; the element's copy constructor is the implicit memberwise one over
//   the intrusive pointer's user copy constructor.
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

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template NParticleEngine::CParticleEmitterInfo* __fastcall
__uninitialized_fill_n(NParticleEngine::CParticleEmitterInfo*, unsigned int, const NParticleEngine::CParticleEmitterInfo&,
                       const __false_type&);
}