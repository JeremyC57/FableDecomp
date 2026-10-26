// CEngineInternalPrimitiveChanger::IsInsideFrustum(const CEngineCamera& camera, bool flag) -- forward to the active child primitive (index ActiveIndex in Primitives), false when there is none
// Leaf: ?IsInsideFrustum@CEngineInternalPrimitiveChanger@@UAE_NABVCEngineCamera@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC5790, 44 bytes (+ int3 padding to 48), thiscall, `ret 8` (or the tail jump).
// Identity (proven): CEngineInternalPrimitiveChanger::IsInsideFrustum(const CEngineCamera& camera, bool flag); evidence: the address is slot 20 of vtable 0x012A662C (RTTI .?AVCEngineInternalPrimitiveChanger@@), whose base CEngineInternalPrimitiveBase declares IsInsideFrustum at the same slot (PDB ?IsInsideFrustum@CEngineInternalPrimitiveChanger@@UAE_NABVCEngineCamera@@_N@Z and ?IsInsideFrustum@CEngineInternalPrimitiveBase@@...); the body tail-jumps to slot 20 of the active child (`jmp [edx+0x50]`, `ret 8` on the fall-through)
// Label: the manifest's "CEngineInternalPrimitiveChanger::IsInsideFrustum" names the right member (family J028: six CEngineInternalPrimitiveChanger forwarders,
//   vtable slots 15..20, one shape).
// Callees: the active child's own IsInsideFrustum (virtual slot 20, `jmp [edx+0x50]`).
// Layout: CEngineInternalPrimitiveChanger (Ego_r.pdb, rebased to retail's 12-byte STLport vector): Primitives
//   CArray<CEnginePrimitiveHandle> +0x48, MeshEffect +0x54, ActiveIndex +0x58, PreloadAll +0x5C, PrimitiveFlags +0x60 (retail
//   0x64); CEnginePrimitiveHandle {vptr, CIVCountedPointer<CEnginePersistentPrimitiveBase> Data +4} (8 bytes). The virtual slot
//   order of CIVCountedPointeeBase / CEnginePersistentPrimitiveBase / CEngineInternalPrimitiveBase is retail's vtable 0x012A662C
//   (slot names from the PDB overrides); which base introduces slots 5..20 is not recorded, so they are declared in
//   CEngineInternalPrimitiveBase, and the slots after 20 are not modelled.
// Shape: `if (HasActivePrimitive()) return GetActivePrimitive().IsInsideFrustum(...); return false;` -- both PDB helpers
//   (?HasActivePrimitive@CEngineInternalPrimitiveChanger@@QBE_NXZ, ?GetActivePrimitive@...@@QAEAAVCEngineInternalPrimitiveBase@@XZ)
//   inline: `cmp eax,-1`, the unsigned index test against size(), the handle's null test, then the sibling tail jump.
class C3DVector;
class C3DBoundingBox;
class C3DBoundingSphere;
class CMatrix3x4;
class CEngineCamera;
class CEnginePrimitive;
class CEnginePrimitiveHandle;
class CEngineSceneGridCell;
class CEngineInternalPrimitiveOcclusionBase;
enum EEngineInterpolationSet { };
enum EEnginePrimitiveStatIndex { };

namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(this->_M_start + __n); }
    const _Tp& operator[](size_type __n) const { return *(this->_M_start + __n); }
};
template <class _Key> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > > class map;
} // namespace std

template <class T> class CArray : public std::vector<T> { };

class CIVCountedPointeeBase {                               // Ego_r.pdb 8 bytes
public:
    virtual ~CIVCountedPointeeBase();                       // slot 0
    virtual void Delete() const;                            // slot 1 (0x0042D930)
    unsigned long IVRefCount;                               // +0x04
};

template <class T> class CIVCountedPointer {                // Ego_r.pdb 4 bytes
public:
    CIVCountedPointeeBase* Object;                          // +0x00
    bool IsNull() const { return Object == 0; }
    T* Get() const { return static_cast<T*>(Object); }
    T& operator*() const { return *Get(); }
    T* operator->() const { return Get(); }
};

class CEnginePersistentPrimitiveBase : public CIVCountedPointeeBase {   // Ego_r.pdb 0xC bytes
public:
    virtual void CollectStats(std::map<unsigned long, unsigned long>& stats, EEnginePrimitiveStatIndex index, unsigned long count) const = 0;   // slot 2
protected:
    virtual void NotifyReleased(const CEnginePrimitiveHandle& handle);   // slot 3
    virtual bool WasRendered() const;                                    // slot 4
public:
    long Type;                                              // +0x08
};

class CEngineFadeDistance {
public:
    float FadeBeginDistance;
    float MaxDrawDistance;
};

class CEngineInternalPrimitiveBase : public CEnginePersistentPrimitiveBase {   // Ego_r.pdb 0x48 bytes (= retail)
public:
    virtual void Update(const CEnginePrimitive& primitive, unsigned long flags);                                               // slot 5
    virtual unsigned long AddChildPrimitive(const CEnginePrimitive& primitive, unsigned long flags);                           // slot 6
    virtual void SetChildPrimitive(unsigned long index, CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive, unsigned long flags);   // slot 7
    virtual void SetChildPrimitive(unsigned long index, const CEnginePrimitive& primitive, unsigned long flags);                // slot 8
    virtual void GetChildPrimitiveHandle(unsigned long index, CEnginePrimitiveHandle& handle);                                 // slot 9
    virtual void ClearChildPrimitive(unsigned long index);                                                                     // slot 10
    virtual void ClearAllChildPrimitives();                                                                                    // slot 11
    virtual CEnginePersistentPrimitiveBase* GetChildPrimitive(unsigned long index);                                            // slot 12
    virtual CEngineFadeDistance GetFadeDistance() const;                                                                       // slot 13
    virtual void PostRender();                                                                                                 // slot 14
    virtual bool GetWorldPosition(C3DVector& position);                                                                        // slot 15
    virtual bool GetBoundingBoxTransform(CMatrix3x4& transform);                                                               // slot 16
    virtual bool GetBoundingBoxWorldSpace(C3DBoundingBox& box);                                                                // slot 17
    virtual bool GetBoundingSphereWorldSpace(C3DBoundingSphere& sphere);                                                       // slot 18
    virtual bool GetBoundingSphereRadiusWorldSpace(float& radius);                                                             // slot 19
    virtual bool IsInsideFrustum(const CEngineCamera& camera, bool flag);                                                      // slot 20
    unsigned long           EnginePrimitiveFlags;           // +0x0c
    float                   AlphaMipBias;                   // +0x10
    CEngineFadeDistance     FadeDistance;                   // +0x14
    unsigned long           OcclusionLayerBitMask;          // +0x1c
    CEngineSceneGridCell*   GridCell;                       // +0x20
    signed char             GridPos_Level;                  // +0x24 CEngineSceneGridPos::Level
    unsigned char           GridPos_X;                      // +0x25
    unsigned char           GridPos_Y;                      // +0x26
    unsigned char           Alpha;                          // +0x27
    unsigned char           AlphaRef;                       // +0x28
    unsigned char           PreloadStat;                    // +0x29
    EEngineInterpolationSet InterpolationSet;               // +0x2c
    unsigned long           RenderLayerMask;                // +0x30
    unsigned char           Enable : 1;                     // +0x34
    unsigned char           NonPersistent : 1;
    unsigned char           OverrideFadeDistance : 1;
    unsigned char           PostRenderEnable : 1;
    unsigned short          LastFrameRendered;              // +0x36
    CEngineInternalPrimitiveBase*  NextPrimitive;           // +0x38
    CEngineInternalPrimitiveBase** RefPrimitive;            // +0x3c
    CEngineInternalPrimitiveBase*  NextLayerMask;           // +0x40
    CEngineInternalPrimitiveBase** RefLayerMask;            // +0x44
};

class CEnginePrimitiveHandle {                              // Ego_r.pdb 8 bytes
public:
    virtual ~CEnginePrimitiveHandle();
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data; // +0x04
};

class CEngineMeshEffect;

class CEngineInternalPrimitiveChanger : public CEngineInternalPrimitiveBase {   // RTTI vtable 0x012A662C, retail 0x64 bytes
public:
    virtual bool IsInsideFrustum(const CEngineCamera& camera, bool flag);      // slot 20
    bool HasActivePrimitive() const
    {
        return ActiveIndex != -1 && (unsigned long)ActiveIndex < Primitives.size() && !Primitives[ActiveIndex].Data.IsNull();
    }
    CEngineInternalPrimitiveBase& GetActivePrimitive()
    {
        return static_cast<CEngineInternalPrimitiveBase&>(*Primitives[ActiveIndex].Data);
    }
    CArray<CEnginePrimitiveHandle>        Primitives;       // +0x48
    CIVCountedPointer<CEngineMeshEffect>  MeshEffect;       // +0x54
    long                                  ActiveIndex;      // +0x58
    bool                                  PreloadAll;       // +0x5c
    unsigned long                         PrimitiveFlags;   // +0x60
};

bool CEngineInternalPrimitiveChanger::IsInsideFrustum(const CEngineCamera& camera, bool flag)
{
    if (HasActivePrimitive())
        return GetActivePrimitive().IsInsideFrustum(camera, flag);
    return false;
}