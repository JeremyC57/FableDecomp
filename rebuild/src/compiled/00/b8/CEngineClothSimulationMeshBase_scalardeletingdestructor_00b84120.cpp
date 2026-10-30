// CEngineClothSimulationMeshBase::`scalar deleting destructor'(unsigned int) -- drop the particle simulations, destroy and free the ParticleSims array, chain to ~CBaseClassNonCopyable, free the object on flag bit 0
// Leaf: ??_GCEngineClothSimulationMeshBase@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B84120, 75 bytes, thiscall, `ret 4`.
// Identity (proven): slot 0 of vtable 0x012A3924 (RTTI .?AVCEngineClothSimulationMeshBase@@ : CEngineSimulationBase :
//   CParticleContext : CBaseClassNonCopyable : CBaseClass), and it stores that vtable first. Ego_r.pdb has the user
//   destructor ??1CEngineClothSimulationMeshBase@@UAE@XZ (inlined here) and ClearParticleSims.
// Callees: CEngineClothSimulationMeshBase::ClearParticleSims 0x00B81D60; std::__destroy_aux<CCountedPointer<CParticleSimulation>*>
//   0x00B833F0 (`__fastcall`, the __false_type tag homed in the dead flags slot); free 0x00BFEA14; CBaseClassNonCopyable::~CBaseClassNonCopyable
//   0x0099A300 (protected virtual, Ego_r.pdb MAE; the CParticleContext base between has an implicit destructor); operator delete 0x00BFE9BC.
// Layout: CEngineSimulationBase {vptr, RunState +4, SimStepOffset +8}, ParticleSims +0x0C (12-byte STLport vector).
// Shape: the user destructor `{ ClearParticleSims(); }`, then the inline ~vector (_Destroy -> out-of-line __destroy_aux,
//   a same-TU noinline body as in landed 0x00B83DF0) and ~_Vector_base (deallocate behind its null test).
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
void __cdecl operator delete(void* p);                       // 0x00BFE9BC

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                                 // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                                 // 0x0099A300 (protected virtual: Ego_r.pdb ??1CBaseClassNonCopyable@@MAE@XZ)
};
class CParticleContext : public CBaseClassNonCopyable { };

class CCPPointerInfo;
template <class T> class CCountedPointer {             // 8 bytes
public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer();                                // inline in retail (in __destroy_aux)
};

namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
#pragma auto_inline(off)
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)
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
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}
template <class T> class CArray : public std::vector<T> { };

class CParticleSimulation;
class C3DMeshInfo;
class CEngineInternalPrimitiveMeshBase { };
class CEngineInternalPrimitiveAnimatedMesh : public CEngineInternalPrimitiveMeshBase { };

class CEngineSimulationBase : public CParticleContext {
public:
    enum ERunState { };
    ERunState RunState;                                      // +0x04
    unsigned char SimStepOffset;                             // +0x08
};

class CEngineClothSimulationMeshBase : public CEngineSimulationBase {
public:
    CEngineClothSimulationMeshBase(const C3DMeshInfo* meshInfo, unsigned long index, CEngineInternalPrimitiveMeshBase* owner);   // 0x00B81AD0
    virtual ~CEngineClothSimulationMeshBase();
    void ClearParticleSims();                                // 0x00B81D60
    CArray<CCountedPointer<CParticleSimulation> > ParticleSims;   // +0x0C
};

CEngineClothSimulationMeshBase::~CEngineClothSimulationMeshBase()
{
    ClearParticleSims();
}