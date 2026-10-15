// CEngineInternalPrimitiveChanger::ClearChildPrimitive(unsigned long index) -- releases child slot `index` (last-handle notification + counted release), then recomputes the render-layer mask from the active child and re-registers the primitive when it changed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC58E0, length 142 bytes (`ret 4`).
// Callees: the child's virtual slots 3 (+0x0C, OnLastHandleReleased(handle)) and 1 (+0x04, Destroy at zero count),
// UpdatePosition @ 0x00B91840 (`__fastcall`: primitive in ecx, the flags at +0x60 in edx).
// Layout: no PDB layout for the changer; the handle vector (STLport, 8-byte CEnginePrimitiveHandle = vptr + counted Data) at +0x48,
// the mesh effect counted pointer +0x54, the active index +0x58 (-1 = none), PreloadAll +0x5C, flags +0x60 (from the descriptor copy
// in Update @ 0x00BC5AE0 and the landed ClearAllChildPrimitives @ 0x00BC5C00, whose ComputeRenderLayerMask shape is reused).
// Shape: no validity test before the handle Clear (unlike the viewport/group bodies); `m_pFirst[index].Clear()` re-reads the
// first pointer after the range check and forms the slot address in edi; the mask helper is inline.
class CEnginePrimitiveHandle;

class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    long RefCount;                                                   // +0x04
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};

class CEngineInternalPrimitiveBase : public CEnginePersistentPrimitiveBase {   // engine/CEngineInternalPrimitiveBase.h (PDB, 0x48 bytes)
public:
    unsigned char _pad08[0x28];
    unsigned long RenderLayerMask;                 // +0x30
    unsigned char _pad34[0x14];
};

class CIVCountedPointer {                          // CIVCountedPointer<CEngineInternalPrimitiveBase>
public:
    CEngineInternalPrimitiveBase* Object;
    CEngineInternalPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    __forceinline CIVCountedPointer& operator=(int)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = 0;
        return *this;
    }
    __forceinline CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        CEngineInternalPrimitiveBase* old = Object;
        if (old != other.Object) {
            if (old) {
                old->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};

class CEnginePrimitiveHandle {                     // 8 bytes: vptr (0x0122F598) + counted Data
public:
    virtual void v000();
    CIVCountedPointer Data;                        // +0x04
    __forceinline void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
};

struct CRetailHandleVector {                       // STLport vector<CEnginePrimitiveHandle>: first, last, end
    CEnginePrimitiveHandle* m_pFirst;
    CEnginePrimitiveHandle* m_pLast;
    CEnginePrimitiveHandle* m_pEnd;
    unsigned long GetCount() const { return m_pLast - m_pFirst; }
    void reserve(unsigned long count);             // 0x00BC5E90
};

class CEnginePrimitive {                           // 12 bytes: Type, InterpolationSet, ThemeChannelDesc
public:
    long          Type;
    long          InterpolationSet;
    unsigned long ThemeChannelDesc;
};

class CEnginePrimitiveChanger : public CEnginePrimitive {   // PDB, 28 bytes
public:
    long              ActiveIndex;                 // +0x0c
    long              InitialSize;                 // +0x10
    bool              PreloadAll;                  // +0x14
    CIVCountedPointer MeshEffect;                  // +0x18
};

void __fastcall UpdatePosition_00b91840(CEngineInternalPrimitiveBase* primitive, unsigned long flags);   // 0x00B91840

class CEngineInternalPrimitiveChanger : public CEngineInternalPrimitiveBase {   // no PDB layout: handle vector +0x48, current index +0x58, flags +0x60
public:
    CRetailHandleVector Primitives;                // +0x48
    CIVCountedPointer   MeshEffect;                // +0x54 (CIVCountedPointer<CEngineMeshEffect>)
    long                m_currentIndex;            // +0x58 ActiveIndex (-1 = none)
    bool                PreloadAll;                // +0x5c
    unsigned long       m_flags;                   // +0x60
    unsigned long ComputeRenderLayerMask()
    {
        unsigned long mask = 0;
        long index = m_currentIndex;
        if (index != -1 && (unsigned long)index < Primitives.GetCount()) {
            CEngineInternalPrimitiveBase* child = Primitives.m_pFirst[index].Data.Object;
            if (child)
                mask = child->RenderLayerMask;
        }
        return mask;
    }
    void UpdateRenderLayerMask()
    {
        unsigned long mask = ComputeRenderLayerMask();
        if (RenderLayerMask != mask) {
            RenderLayerMask = mask;
            UpdatePosition_00b91840(this, m_flags);
        }
    }
    virtual void ClearChildPrimitive(unsigned long index);
    virtual void Update(const CEnginePrimitive& primitive, unsigned long flags);
};

void CEngineInternalPrimitiveChanger::ClearChildPrimitive(unsigned long index)
{
    if (index < Primitives.GetCount())
        Primitives.m_pFirst[index].Clear();
    UpdateRenderLayerMask();
}
