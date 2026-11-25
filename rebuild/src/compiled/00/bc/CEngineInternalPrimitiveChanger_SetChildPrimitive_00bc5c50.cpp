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
// Naming (NF160, 2026-11-20, q6213): the local CRetailHandleVector stand-in replaced by the shared STLport 4.6 std::vector<CEnginePrimitiveHandle>
//   (stlport/fable_containers.h, FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL; the landed 0x0064E1E4 / 0x0064ED06 recipe), so its Resize call binds
//   0x00443F90 as ?resize@?$vector@VCEnginePrimitiveHandle@@V?$allocator@VCEnginePrimitiveHandle@@@std@@@std@@QAEXI@Z (STLport one-argument
//   resize; the self row and 29 landed callers bind it).
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#include "stlport/fable_containers.h"
#include "fable_compiler.h"
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
    FABLE_FORCEINLINE void Attach(CEngineInternalPrimitiveBase* object)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = object;
    }
    FABLE_FORCEINLINE CIVCountedPointer& operator=(int)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = 0;
        return *this;
    }
    FABLE_FORCEINLINE CIVCountedPointer& operator=(const CIVCountedPointer& other)
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
    FABLE_FORCEINLINE void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
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


// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x00B91840 ?UpdatePosition@CEngineScene@@SIXAAVCEngineInternalPrimitiveBase@@K@Z (was ?UpdatePosition_00b91840@@YIXPAVCEngineInternalPrimitiveBase@@K@Z)
class CEngineScene { public: static void __fastcall UpdatePosition(CEngineInternalPrimitiveBase&, unsigned long); };
class CEngineInternalPrimitiveChanger : public CEngineInternalPrimitiveBase {   // no PDB layout: handle vector +0x48, current index +0x58, flags +0x60
public:
    std::vector<CEnginePrimitiveHandle> Primitives;                // +0x48
    CIVCountedPointer   MeshEffect;                // +0x54 (CIVCountedPointer<CEngineMeshEffect>)
    long                m_currentIndex;            // +0x58 ActiveIndex (-1 = none)
    bool                PreloadAll;                // +0x5c
    unsigned long       m_flags;                   // +0x60
    unsigned long ComputeRenderLayerMask()
    {
        unsigned long mask = 0;
        long index = m_currentIndex;
        if (index != -1 && (unsigned long)index < Primitives.size()) {
            CEngineInternalPrimitiveBase* child = Primitives[index].Data.Object;
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
            CEngineScene::UpdatePosition(*(this), m_flags);
        }
    }
    void SetChildPrimitive(unsigned long index, const CEnginePrimitive& primitive, unsigned long flags);
};

class CEnginePrimitiveManager { public:
    virtual void v00();
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);   // slot 1 (+0x04)
};
namespace NEngineInternal { extern CEnginePrimitiveTypeInfo* GPPrimitiveTypeInfo; }   // 0x01436E84

void CEngineInternalPrimitiveChanger::SetChildPrimitive(unsigned long index, const CEnginePrimitive& primitive, unsigned long flags)
{
    if (index >= Primitives.size())
        Primitives.resize(index + 1);
    CEnginePrimitiveHandle& slot = Primitives[index];
    CEnginePrimitiveManager* manager = NEngineInternal::GPPrimitiveTypeInfo->PrimitiveManagers[primitive.Type];
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