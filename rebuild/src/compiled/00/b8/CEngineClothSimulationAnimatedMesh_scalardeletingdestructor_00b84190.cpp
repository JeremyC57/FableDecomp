// CEngineClothSimulationAnimatedMesh::`scalar deleting destructor'(unsigned int) -- the empty destructor with the base CEngineClothSimulationMeshBase destructor inlined (drop the particle simulations, destroy and free ParticleSims, chain to ~CBaseClassNonCopyable), then free the object on flag bit 0
// Leaf: ??_GCEngineClothSimulationAnimatedMesh@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B84190, 75 bytes, thiscall, `ret 4`.
// Identity (proven): slot 0 of vtable 0x012A39C0 (rtti-vtables.tsv; RTTI .?AVCEngineClothSimulationAnimatedMesh@@ : CEngineClothSimulationMeshBase);
//   both PDB name sets hold CEngineClothSimulationAnimatedMesh (ctor, Render, RunSimulation, CalcAndSetRunState). Byte twin of the landed
//   0x00B84120 (the base's ??_G): the derived destructor is empty, so after the base destructor body is inlined the derived vptr store is
//   dead and only the base's 0x012A3924 store remains. Catalog label "~CEngineClothSimulationMeshBase_Alt" is a propagated twin name.
// Shape: the twin's TU (base destructor `{ ClearParticleSims(); }` defined here) plus the derived class with its empty destructor and
//   its constructor defined in the same TU, so VC7.1 emits the derived vtable and expands both destructors into ??_G. Callees as the twin: ClearParticleSims 0x00B81D60, __destroy_aux 0x00B833F0, free,
//   ~CBaseClassNonCopyable 0x0099A300, operator delete.
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
void __cdecl operator delete(void* p);                       // 0x00BFE9BC

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                                 // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                                 // 0x0099A300 (protected virtual: Ego_r.pdb ??1CBaseClassNonCopyable@@MAE@XZ)
};
class CParticleContext : public CBaseClassNonCopyable { };

#include "engine/CCountedPointer.h"

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

// The derived class adds no data. Its destructor is a user one defined out of class in this TU (empty): only that form lets VC7.1 expand
// it, and the base destructor inside it, into ??_G (measured: an implicit destructor gives the 30-byte `call ??1` shape of the StaticMesh /
// 2DAnimatedMesh ??_G 0x00B84170 / 0x00B841E0; an empty in-class one gives `call ??1CEngineClothSimulationMeshBase`, 36 bytes). The
// constructor (0x00B81ED0, ??0CEngineClothSimulationAnimatedMesh@@QAE@PBVC3DMeshInfo@@IPAVCEngineInternalPrimitiveAnimatedMesh@@@Z) is
// defined in this TU too: the TU that defines it emits the vtable 0x012A39C0 and with it this ??_G.
class CEngineClothSimulationAnimatedMesh : public CEngineClothSimulationMeshBase {
public:
    CEngineClothSimulationAnimatedMesh(const C3DMeshInfo* meshInfo, unsigned int index, CEngineInternalPrimitiveAnimatedMesh* owner);
    virtual ~CEngineClothSimulationAnimatedMesh();
};

CEngineClothSimulationAnimatedMesh::~CEngineClothSimulationAnimatedMesh()
{
}

CEngineClothSimulationAnimatedMesh::CEngineClothSimulationAnimatedMesh(const C3DMeshInfo* meshInfo, unsigned int index,
                                                                       CEngineInternalPrimitiveAnimatedMesh* owner)
    : CEngineClothSimulationMeshBase(meshInfo, index, owner)
{
}