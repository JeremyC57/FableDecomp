// C3DAnimationManager::~C3DAnimationManager() -- implicit destructor of the 3D animation manager: MemoryPool2 then MemoryPool (each CSimpleMemoryPool's inline virtual dtor), the BoneIndicesBuffer block, then ~CBaseClassNonCopyable
// Leaf: ??1C3DAnimationManager@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA4170, 75 bytes, thiscall, tail `jmp ~CBaseClassNonCopyable` 0x0099A300.
// Identity (proven, two sources): its only caller is the scalar deleting dtor 0x00AA4150, slot 0 of vtable 0x0129E134 (rtti-vtables.tsv:
//   RTTI .?AVC3DAnimationManager@@ : CBaseClassNonCopyable : CBaseClass), the vtable the ctor 0x00AA0F60 stores (reached from `push 4Ch`
//   at 0x00A272D6 = sizeof); Ego_r.pdb ??1C3DAnimationManager@@UAE@XZ (0x009DC910, emitted far from the class's own functions, as a
//   compiler-generated dtor is). The two vtable stores are CSimpleMemoryPool's 0x0129A064 (its members at +0x30 / +0x14), not the
//   class's own: the compiler-generated destructor.
// Callees: ?ClearMemoryBlocks@CSimpleMemoryPool@@AAEXXZ 0x00A5C600 (x2), ??1CBaseClassNonCopyable@@MAE@XZ 0x0099A300 (x2 as each
//   pool's base, then the tail), _free 0x00BFEA14 (the BoneIndicesBuffer block, STLport allocator over malloc).
// Layout: Ego_r.pdb C3DAnimationManager (0x50 with a 16-byte Dinkumware vector; retail 0x4C): MeshBank +0x4, BoneIndicesBuffer
//   (CArray<unsigned long>) +0x8, MemoryPool +0x14, MemoryPool2 +0x30. engine/CSimpleMemoryPool.h retail opt-in
//   (FABLE_RETAIL_CSIMPLEMEMORYPOOL, hdr/ overlay: the polymorphic class with its inline dtor); engine/CBaseClassNonCopyable.h.
// TU: retail's own ctor ??0C3DAnimationManager@@QAE@XZ (0x00AA0F60: MeshBank 0, the empty vector, both pools (0, 0x1000)) is
//   defined, which emits the vtable, the ??_G and this implicit destructor.
#include "engine/CBaseClassNonCopyable.h"                  // CBaseClassNonCopyable : CBaseClass (~ 0x0099A300)
#define FABLE_RETAIL_CSIMPLEMEMORYPOOL
#include "engine/CSimpleMemoryPool.h"                       // CSimpleMemoryPool : CBaseClassNonCopyable (vtable 0x0129A064)

extern "C" void __cdecl free(void* block);                  // 0x00BFEA14 _free (MSVCR71 import thunk)

namespace std {                                             // STLport 4.6 (_alloc.h / _construct.h / _vector.h), the members this body expands
template <class _Tp> class allocator { public:
    typedef unsigned int size_type;
    void deallocate(_Tp* __p, size_type) { if (__p != 0) free((void*)__p); }   // __sgi_alloc = the malloc allocator
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) {}
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x8
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { public:
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) {}
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };   // Fable's CArray over the STLport vector

class CMeshDataBank;

class C3DAnimationManager : public CBaseClassNonCopyable {  // vtable 0x0129E134
public:
    C3DAnimationManager();                                  // ??0C3DAnimationManager@@QAE@XZ (0x00AA0F60)
    CMeshDataBank*         MeshBank;                        // +0x04
    CArray<unsigned long>  BoneIndicesBuffer;               // +0x08
    CSimpleMemoryPool      MemoryPool;                      // +0x14
    CSimpleMemoryPool      MemoryPool2;                     // +0x30
};
FABLE_STATIC_ASSERT(sizeof(C3DAnimationManager) == 0x4C);

C3DAnimationManager::C3DAnimationManager()
    : MeshBank(0), MemoryPool(0, 0x1000), MemoryPool2(0, 0x1000)
{
}