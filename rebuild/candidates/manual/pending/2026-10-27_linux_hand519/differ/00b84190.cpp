// CEngineClothSimulationAnimatedMesh::`scalar deleting destructor'(unsigned int) -- the implicit destructor (the inlined ~CEngineClothSimulationMeshBase: ClearParticleSims, destroy and free ParticleSims, ~CBaseClassNonCopyable), then free the object on flag bit 0
// Grade: DIFFER(36v75) -- the base destructor is not inlined into the derived ??_G: an implicit derived dtor gives `call ??1CEngineClothSimulationAnimatedMesh`; an empty inline user dtor gives its own vptr store + `call ??1CEngineClothSimulationMeshBase` (retail inlines the whole base dtor body); inline/__forceinline on the base dtor are inert
// Leaf: ??_GCEngineClothSimulationAnimatedMesh@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B84190, 75 bytes, thiscall, `ret 4`.
// Identity (proven): slot 0 of vtable 0x012A39C0 (RTTI .?AVCEngineClothSimulationAnimatedMesh@@ :
//   CEngineClothSimulationMeshBase). Its only vptr store is the base's 0x012A3924: CEngineClothSimulationAnimatedMesh's
//   destructor is an empty inline one (no out-of-line ??1 in Ego_r.pdb / FableWin.pdb); its own vptr store is dead under
//   the inlined base destructor's store and is dropped. (An implicit destructor makes VC7.1 call ??1 out of line, the
//   shape of the StaticMesh sibling 0x00B84170.)
//   The catalog label "~CEngineClothSimulationMeshBase_Alt" is a propagated twin name.
// Callees: as 0x00B84120 (ClearParticleSims 0x00B81D60, __destroy_aux 0x00B833F0, free 0x00BFEA14, ~CBaseClassNonCopyable 0x0099A300,
//   operator delete 0x00BFE9BC).
// Emission: the vtable (and with it this ??_G) is emitted by the class's real constructor, written exactly as landed
//   0x00B81ED0 (`??0CEngineClothSimulationAnimatedMesh@@QAE@PBVC3DMeshInfo@@KPAVCEngineInternalPrimitiveAnimatedMesh@@@Z`).
extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
void __cdecl operator delete(void* p);                       // 0x00BFE9BC

class CBaseClass {
public:
    CBaseClass();                                            // 0x0099A2D0
    virtual ~CBaseClass();                                   // 0x0099A2E0
};
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

inline CEngineClothSimulationMeshBase::~CEngineClothSimulationMeshBase()
{
    ClearParticleSims();
}

class CEngineClothSimulationAnimatedMesh : public CEngineClothSimulationMeshBase {
public:
    CEngineClothSimulationAnimatedMesh(const C3DMeshInfo* meshInfo, unsigned long index, CEngineInternalPrimitiveAnimatedMesh* primitive);
    virtual ~CEngineClothSimulationAnimatedMesh() { }
};

CEngineClothSimulationAnimatedMesh::CEngineClothSimulationAnimatedMesh(const C3DMeshInfo* meshInfo, unsigned long index, CEngineInternalPrimitiveAnimatedMesh* primitive)
    : CEngineClothSimulationMeshBase(meshInfo, index, primitive)
{
}
