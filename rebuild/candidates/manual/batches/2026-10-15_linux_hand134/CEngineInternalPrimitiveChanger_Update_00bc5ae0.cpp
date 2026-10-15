// CEngineInternalPrimitiveChanger::Update(const CEnginePrimitive& primitive, unsigned long flags) -- applies a changer descriptor: reserves the child slots, shares its mesh effect, takes the active index / preload flag / update flags, then refreshes the render-layer mask
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC5AE0, length 137 bytes (`ret 8`).
// Callees: vector<CEnginePrimitiveHandle>::reserve @ 0x00BC5E90 (manifest "std_vector_reserve_with_copy", on the vector at +0x48 with
// InitialSize), the old mesh effect's virtual slot 1 (Destroy at zero count), UpdatePosition @ 0x00B91840 (`__fastcall`, flags in edx).
// Layout: the descriptor is the PDB CEnginePrimitiveChanger (28 bytes over the 12-byte CEnginePrimitive): ActiveIndex +0x0C,
// InitialSize +0x10, PreloadAll +0x14, MeshEffect (CIVCountedPointer<CEngineMeshEffect>) +0x18; the changer keeps them at +0x58,
// +0x5C, +0x54 and the flags at +0x60 (see 0x00BC58E0).
// Shape: the counted copy keeps the old pointer in a local (`mov ecx,[esi+54h] ; cmp ecx,[edi+18h]`) with the release on the
// pointee; the vector address stays in ebx from the reserve call, so the inline mask helper reads `[ebx]`/`[ebx+4]`; the flags
// argument is still in edx for the `__fastcall` UpdatePosition (no reload of +0x60).
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

void CEngineInternalPrimitiveChanger::Update(const CEnginePrimitive& primitive, unsigned long flags)
{
    const CEnginePrimitiveChanger& desc = static_cast<const CEnginePrimitiveChanger&>(primitive);
    Primitives.reserve(desc.InitialSize);
    MeshEffect = desc.MeshEffect;
    m_currentIndex = desc.ActiveIndex;
    PreloadAll = desc.PreloadAll;
    m_flags = flags;
    UpdateRenderLayerMask();
}
