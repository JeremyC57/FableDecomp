// NGraphicAppearance::CTCGraphicAppearanceMultipleStaticMeshes::~CTCGraphicAppearanceMultipleStaticMeshes() -- the multiple-static-mesh appearance component's destructor: its two vtables, unless the parent thing is dead unregister the component from the parent's graphic appearance TC, release the def pointer, free the PrimHandles vector, then the CAGraphicAppearanceComponent part and ~CTCBase
// Leaf: ??1CTCGraphicAppearanceMultipleStaticMeshes@NGraphicAppearance@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E1510, 95 bytes, thiscall, tail `jmp ~CTCBase` 0x00686830.
// Identity (proven, two sources): it stores vtable 0x0126FFCC first and 0x0126FFB4 at +0xC, both RTTI
//   .?AVCTCGraphicAppearanceMultipleStaticMeshes@NGraphicAppearance@@ (rtti-vtables.tsv; rtti_bases: CTCBase +0,
//   NGraphicAppearance::CAGraphicAppearanceComponent +0xC); its only caller is the scalar deleting dtor 0x007E1AA0, slot 0 of
//   0x0126FFCC. Ego_r.pdb ??1CTCGraphicAppearanceMultipleStaticMeshes@NGraphicAppearance@@UAE@XZ (0x007DCE90, right after ??0
//   0x007DCE40) and FableWin.pdb (0x02781F70). The catalog label (CTCGraphicAppearanceReplaceableMesh) is propagated from the masked
//   twin 0x007A66F0 and wrong for this copy.
// Callees: ?UnregisterComponent@CTCGraphicAppearance@NGraphicAppearance@@QAEXAAVCAGraphicAppearanceComponent@2@@Z 0x004C0000 (the
//   CAGraphicAppearanceComponent subobject: `push edi` = this+0xC), the def's virtual slot 1 Delete on its last release (CDefPointer<T>
//   inline Release), _free 0x00BFEA14 (the STLport vector block: allocator<T>::deallocate over the malloc allocator),
//   ??1CTCBase@@UAE@XZ 0x00686830 (tail). The CAGraphicAppearanceComponent dtor is inline and empty (FableWin
//   ??1CAGraphicAppearanceComponent@NGraphicAppearance@@QAE@XZ, non-virtual): only its vtable store 0x0125A170 remains.
// Layout: Ego_r.pdb CTCGraphicAppearanceMultipleStaticMeshes (0x24 with a 16-byte Dinkumware vector; retail 0x20 with the 12-byte
//   STLport one): the component base +0xC, PrimHandles (vector<NGraphicAppearance::CPrimitive*>) +0x10, PDef
//   (CDefPointer<const CMultiStaticMeshDef>) +0x1C (retail `mov ecx,[esi+1Ch]` release, `mov eax,[esi+10h]` free). CThing (retail
//   0x98 = Ego_r less 0x10 past Components): PTCGraphicAppearance +0x64, Dead bit 0 of +0x91, the rest opaque. engine/CTCBase.h
//   (FABLE_RETAIL_CTCBASE) and engine/CDefPointer.h (FABLE_RETAIL_DEF_CLASS_BASE); no header exists for the appearance classes.
// Shape: the landed CTCLook destructor body (`if (!ParentThing->IsDead()) ...UnregisterComponent(*this);`), then the members in
//   reverse order: the def pointer's inline release, the STLport vector (trivial elements: only `_Vector_base`'s null-tested free).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                                 // CTCBase : CBaseClassNonCopyable (vtable 0x0125BE0C)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"                             // CDefPointer<T> (inline Release)

extern "C" void __cdecl free(void* block);                  // 0x00BFEA14 _free (MSVCR71 import thunk)

namespace std {                                             // STLport 4.6 (_alloc.h / _vector.h), the members this body expands
template <class _Tp> class allocator { public:
    typedef unsigned int size_type;
    void deallocate(_Tp* __p, size_type) { if (__p != 0) free((void*)__p); }   // __sgi_alloc = the malloc allocator
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x8
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> { public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}

namespace NGraphicAppearance {
class CTCGraphicAppearance;
class CGraphicAppearanceDrawInfo;
class CMeshInfo;
class CPrimitive;
class CAGraphicAppearanceComponent { public:                // vtable 0x0125A170 (5 slots)
    virtual bool OnAppearanceDraw(CTCGraphicAppearance& appearance, const CGraphicAppearanceDrawInfo& info) = 0;   // slot 0
    virtual void OnAppearanceClear() = 0;                                                         // slot 1
    virtual void GetMeshesUsed(std::vector<CMeshInfo, std::allocator<CMeshInfo> >& meshes) const;  // slot 2 0x006407B0
    virtual void GetTexturesUsed(std::vector<long, std::allocator<long> >& textures) const;        // slot 3 0x006407C0
    virtual bool IsAppearanceSentEveryRenderFrame();                                              // slot 4 0x006407D0
    ~CAGraphicAppearanceComponent() { }                     // FableWin ??1CAGraphicAppearanceComponent@NGraphicAppearance@@QAE@XZ
};
class CTCGraphicAppearance { public:
    void UnregisterComponent(CAGraphicAppearanceComponent& component);   // 0x004C0000
};
}

class CThing { public:                                      // retail offsets (Ego_r minus 0x10 past Components), sizeof 0x98
    unsigned long                               Members00_[0x64 / 4];   // +0x00 vptr .. PhysicsTC (not modelled)
    NGraphicAppearance::CTCGraphicAppearance*   PTCGraphicAppearance;   // +0x64
    unsigned long                               Members68_[(0x90 - 0x68) / 4];   // +0x68 WorldFrameCreatedAt .. PlayerNumber
    unsigned char                               Members90_;             // +0x90 PlayerNumber
    unsigned char                               Dead : 1;               // +0x91 bit 0
    unsigned char                               Flags91_ : 7;           // +0x91 CreatedByOwnerThingCreation .. CreatedBySerialisation
    unsigned char                               Members92_[6];          // +0x92 the other flag bytes
    bool IsDead() const { return Dead; }                    // FableWin ?IsDead@CThing@@QBE_NXZ (inline)
};

FABLE_STATIC_ASSERT(sizeof(CThing) == 0x98);

class CMultiStaticMeshDef;

namespace NGraphicAppearance {
class CTCGraphicAppearanceMultipleStaticMeshes : public CTCBase, public CAGraphicAppearanceComponent {   // vtables 0x0126FFCC / 0x0126FFB4
public:
    CTCGraphicAppearanceMultipleStaticMeshes(CThing& thing);   // ??0CTCGraphicAppearanceMultipleStaticMeshes@NGraphicAppearance@@QAE@AAVCThing@@@Z
    virtual ~CTCGraphicAppearanceMultipleStaticMeshes();
    virtual void OnCreate();
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
    virtual bool OnAppearanceDraw(CTCGraphicAppearance& appearance, const CGraphicAppearanceDrawInfo& info);
    virtual void OnAppearanceClear();
    virtual void GetMeshesUsed(std::vector<CMeshInfo, std::allocator<CMeshInfo> >& meshes) const;
    std::vector<CPrimitive*>                PrimHandles;    // +0x10
    CDefPointer<const CMultiStaticMeshDef>  PDef;           // +0x1C
};
FABLE_STATIC_ASSERT(sizeof(CTCGraphicAppearanceMultipleStaticMeshes) == 0x20);

CTCGraphicAppearanceMultipleStaticMeshes::~CTCGraphicAppearanceMultipleStaticMeshes()
{
    if (!ParentThing->IsDead())
        ParentThing->PTCGraphicAppearance->UnregisterComponent(*this);
}
}