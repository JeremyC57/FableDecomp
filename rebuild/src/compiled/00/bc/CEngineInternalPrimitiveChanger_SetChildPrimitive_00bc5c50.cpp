// CEngineInternalPrimitiveChanger::SetChildPrimitive(unsigned long index, const CEnginePrimitive& primitive, unsigned long flags) -- put a new internal primitive (created by the descriptor type's manager) into child slot `index` (growing the slot vector), release the old one, apply the descriptor, set the child's Enable bit from flag 0x20 being clear, then refresh the render-layer mask
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC5C50, length 270 bytes (`ret 0Ch`).
// Callees: vector<CEnginePrimitiveHandle>::Resize(long) @ 0x00443F90 (manifest label "CEnginePrimitiveRenderer::AddPrimitive" is
// propagated; landed callers name it Resize), the type manager's virtual slot 1 (+0x04, CreateInternalPrimitive(type)), the old
// child's slots 3/1 (OnLastHandleReleased, Destroy), the new child's slot 5 (+0x14, Update(const CEnginePrimitive&, flags)),
// UpdatePosition @ 0x00B91840 (`__fastcall`). Global: the primitive type info @ 0x01436E84 (engine/CEnginePrimitiveTypeInfo.h:
// PrimitiveManagers[] at +0x10).
// Layout/shape: the landed CEngineInternalPrimitiveChanger::ClearChildPrimitive 0x00BC58E0 model (handle vector +0x48, current
// index +0x58, flags +0x60; inline Clear and the mask helper); the slot is filled by an inline Attach (release the old object,
// store the new one without an add-ref -- the creator's reference is adopted); the Enable bit is computed before the manager
// test (`shr 5 ; not ; and 1`); `this` is parked in the `push ecx` slot.
#include "engine/CEnginePrimitiveTypeInfo.h"
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
    unsigned char Enable : 1;                      // +0x34 bit 0
    unsigned char _pad35[0x13];
    virtual void v004();
    virtual void Update(const class CEnginePrimitive& primitive, unsigned long flags);   // slot 5 (+0x14)
};

class CIVCountedPointer {                          // CIVCountedPointer<CEngineInternalPrimitiveBase>
public:
    CEngineInternalPrimitiveBase* Object;
    CEngineInternalPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    __forceinline void Attach(CEngineInternalPrimitiveBase* object)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = object;
    }
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
    void Resize(long count);                       // 0x00443F90
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
    void SetChildPrimitive(unsigned long index, const CEnginePrimitive& primitive, unsigned long flags);
};

class CEnginePrimitiveManager { public:
    virtual void v00();
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);   // slot 1 (+0x04)
};
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;   // 0x01436E84

void CEngineInternalPrimitiveChanger::SetChildPrimitive(unsigned long index, const CEnginePrimitive& primitive, unsigned long flags)
{
    if (index >= Primitives.GetCount())
        Primitives.Resize(index + 1);
    CEnginePrimitiveHandle& slot = Primitives.m_pFirst[index];
    CEnginePrimitiveManager* manager = g_pEnginePrimitiveTypeInfo->PrimitiveManagers[primitive.Type];
    bool enable = (flags & 0x20) == 0;
    if (!manager)
        return;
    CEngineInternalPrimitiveBase* child = manager->CreateInternalPrimitive(primitive.Type);
    slot.Clear();
    slot.Data.Attach(child);
    child->Update(primitive, flags);
    child->Enable = enable;
    UpdateRenderLayerMask();
}