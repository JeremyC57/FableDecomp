// NParticleEngine::CPSCAttractor::~CPSCAttractor() -- the compiler-generated destructor: free the AttractorUserPointsArray block, then the inline CPSCBase / CParticleUpdateParamCallback destructors and ~CBaseClassNonCopyable
// Leaf: ??1CPSCAttractor@NParticleEngine@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B18780, 27 bytes, thiscall, tail `jmp ~CBaseClassNonCopyable`.
// Identity (proven): the sole caller is the slot-0 `??_G` 0x00B18760 of vtable 0x012A04AC (RTTI
//   .?AVCPSCAttractor@NParticleEngine@@ : CPSCBase : CParticleUpdateParamCallback<KK> : CBaseClassNonCopyable : CBaseClass);
//   Ego_r.pdb and FableWin.pdb both spell ??1CPSCAttractor@NParticleEngine@@UAE@XZ. Ego_r.pdb's only non-trivial member is
//   AttractorUserPointsArray (CArray<C3DVector>) at +0x24 -- the landed ctor 0x00B18020 builds it there. No 0x012A04AC
//   store: the destructor is the implicit one. The catalog label "CTCInventoryItem::OnDie" is propagated.
// Callees: free 0x00BFEA14 (~_Vector_base's deallocate), CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300 (MAE).
// Shape: the chunk 699 CPSCSingleSprite 0x00B0A160 model (STLport vector under CArray, implicit CPSCBase chain); the
//   CPSCBase / CParticleUpdateParamCallback destructors are implicit and empty, so the base call is ~CBaseClassNonCopyable.
//   DestroyOne (a destruction by name, ungraded) makes VC7.1 emit this out-of-line implicit `??1`.
extern "C" void __cdecl free(void* block);                      // 0x00BFEA14
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}   // namespace std
#include "engine/CBaseClassNonCopyable.h"           // ~CBaseClassNonCopyable 0x0099A300 (protected virtual)

#pragma pack(push, 1)
class C3DVector {                                       // retail C3DVector is 1-aligned
public:
    float X, Y, Z;
};
#pragma pack(pop)
template <class T> class CArray : public std::vector<T> { };   // Lionhead CArray over STLport vector (0xc)

namespace NParticleEngine {
class CParticleSystem;
template <class T1, class T2> class CParticleUpdateParameters;
template <class T1, class T2> class CParticleUpdateParamCallback : public CBaseClassNonCopyable {
public:
    virtual bool GetUpdateParam(C3DVector& param, unsigned long index, float time, bool flag,
                                const CParticleUpdateParameters<T1, T2>& parameters, unsigned long extra);
};
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {   // vtable 0x0129F3F4
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool             UpdateStarted : 1;                 // +0x08
    bool             Initialised : 1;                   // +0x08
};
class CPSCAttractor : public CPSCBase {                 // vtable 0x012A04AC, 0x30 bytes in retail
public:
    CPSCAttractor();                                    // 0x00B18020
    bool              AttractorEnabled : 1;             // +0x0c
    bool              AttractorUseParamPosition : 1;    // +0x0c
    long              AttractorInfluenceFallOff;        // +0x10
    unsigned long     AttractorPositionParam;           // +0x14
    unsigned long     AttractorPositionParamName;       // +0x18
    float             AttractorInfluenceRadius;         // +0x1c
    float             AttractorInfluenceForce;          // +0x20
    CArray<C3DVector> AttractorUserPointsArray;         // +0x24
};
} // namespace NParticleEngine

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(NParticleEngine::CPSCAttractor* component)
{
    component->NParticleEngine::CPSCAttractor::~CPSCAttractor();
}