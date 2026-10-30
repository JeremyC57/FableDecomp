// C3DAnimationManager::C3DAnimationManager() -- no mesh bank, empty bone-index buffer, two 4 KB simple memory pools
// Leaf: ??0C3DAnimationManager@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA0F60, 62 bytes, thiscall, plain `ret`, returns this.
// Identity (proven): stores vtable 0x0129E134 (RTTI .?AVC3DAnimationManager@@ : CBaseClassNonCopyable : CBaseClass);
//   Ego_r.pdb ??0C3DAnimationManager@@QAE@XZ; members MeshBank, BoneIndicesBuffer, MemoryPool, MemoryPool2.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable() @ 0x0099A2F0, CSimpleMemoryPool::CSimpleMemoryPool(unsigned
//   long, unsigned long) @ 0x00A5C570 (twice, (0, 0x1000) as in the landed 0x0098AB00).
// Layout (retail, 12-byte STLport vector; Ego_r.pdb's 16-byte one puts the pools 4 higher): MeshBank +0x04,
//   BoneIndicesBuffer +0x08, MemoryPool +0x14, MemoryPool2 +0x30 (0x1C bytes each), size 0x4C.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                                 // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                        // 0x0099A300 (Ego_r.pdb ??1CBaseClassNonCopyable@@MAE@XZ)
};
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base();
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
};
}

template <class T> class CArray : public std::vector<T> { };
class CMeshDataBank;
class CSimpleMemoryPool {
public:
    CSimpleMemoryPool(unsigned long count, unsigned long size);   // 0x00A5C570
    virtual ~CSimpleMemoryPool();
    unsigned char _body[0x18];                               // FirstMemoryBlock, Capacity, GrowthGranularity, Root
};

class C3DAnimationManager : public CBaseClassNonCopyable {
public:
    C3DAnimationManager();
    virtual ~C3DAnimationManager();
    CMeshDataBank*          MeshBank;                       // +0x04
    CArray<unsigned long>   BoneIndicesBuffer;              // +0x08
    CSimpleMemoryPool       MemoryPool;                     // +0x14
    CSimpleMemoryPool       MemoryPool2;                    // +0x30
};

C3DAnimationManager::C3DAnimationManager()
    : MeshBank(0), MemoryPool(0, 0x1000), MemoryPool2(0, 0x1000)
{
}