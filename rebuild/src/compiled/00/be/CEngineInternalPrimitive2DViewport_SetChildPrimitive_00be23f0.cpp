// CEngineInternalPrimitive2DViewport::SetChildPrimitive(unsigned long, const CEnginePrimitive&, unsigned long) -- grow the child-handle vector to cover `index`, have the primitive type's manager create the internal primitive, store it in the child handle, update it from the description, enable it unless flag 0x20 is set and recompute the viewport's render-layer mask
// Leaf: SetChildPrimitive
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE23F0, 216 bytes (+ int3 padding), `ret 0Ch`.
// Callees: vector<CEnginePrimitiveHandle>::Resize(long) 0x00443F90 (on the child handles +0x94),
//   CEngineInternalPrimitive2DViewport::UpdateRenderLayerMask 0x00BE1D80 (landed); the primitive-type manager table
//   (global 0x01436E84, managers at +0x10 indexed by CEnginePrimitive::Type) slot 1 CreateInternalPrimitive(type); the new
//   primitive's slots 3 OnLastHandleReleased(handle), 1 Destroy, 5 Update(const CEnginePrimitive&, flags).
// Layout: CEngineInternalPrimitiveBase (root CIVCountedPointeeBase {vptr, RefCount +4}; Enable = bit 0 of the +0x34 bit-field
//   byte, Ego_r.pdb agrees); the viewport's child handles at +0x94 (engine/CEngineInternalPrimitive2DViewport.h: Primitives
//   +0x94 as a 16-byte CArray; retail reads a 12-byte first/last/end vector). The header flattens the primitive base, whose
//   virtuals and bit field this body needs, so `this` is a local model with the header's member name.
// Shape: the child handle is a reference local into the vector; storing the new primitive is the landed
//   CEnginePrimitiveHandle model (`Clear()` = last-handle notice + `Data = 0`, then `Data = p`, every helper inline, so
//   two release sequences precede the store); the handle reference is taken BEFORE the manager lookup (taken after it, the
//   table global lands in edx instead of ecx: the parked group twin 0x00BC65D0); the enable bit is a bool local
//   `(flags & 0x20) ? false : true` computed before the null test (bl).
// Naming (NF160, 2026-11-20, q6213): the local CHandleVector stand-in replaced by the shared STLport 4.6 std::vector<CEnginePrimitiveHandle>
//   (stlport/fable_containers.h, FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL; the landed 0x0064E1E4 / 0x0064ED06 recipe), so its Resize call binds
//   0x00443F90 as ?resize@?$vector@VCEnginePrimitiveHandle@@V?$allocator@VCEnginePrimitiveHandle@@@std@@@std@@QAEXI@Z (STLport one-argument
//   resize; the self row and 29 landed callers bind it).
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#include "stlport/fable_containers.h"
class CEnginePrimitive { public:
    long Type;                                         // +0x00
};

class CEnginePrimitiveHandle;
class CEngineInternalPrimitiveBase { public:
    virtual void v00();
    virtual void Destroy();                            // slot 1 (+0x04)
    virtual void v02();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle);   // slot 3 (+0x0c)
    virtual void v04();
    virtual void Update(const CEnginePrimitive& primitive, unsigned long flags);   // slot 5 (+0x14)
    long          RefCount;                            // +0x04
    unsigned char _pad08[0x34 - 8];
    unsigned char Enable : 1;                          // +0x34 bit 0
    unsigned char NonPersistent : 1;
    unsigned char OverrideFadeDistance : 1;
    unsigned char PostRenderEnable : 1;
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};

class CIVCountedPointer {                              // CIVCountedPointer<CEngineInternalPrimitiveBase>
public:
    CEngineInternalPrimitiveBase* Object;
    CEngineInternalPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    CIVCountedPointer& operator=(CEngineInternalPrimitiveBase* p)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = p;
        return *this;
    }
};

class CEnginePrimitiveHandle { public:                        // 8 bytes: vptr (0x0122F598) + counted Data
    virtual void v00();
    CIVCountedPointer Data;                            // +0x04
    void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
};


// NF46 (round 7): the 0x01436E84 global is NEngineInternal::GPPrimitiveTypeInfo (Ego_r.pdb / FableWin.pdb
// ?GPPrimitiveTypeInfo@NEngineInternal@@3PAVCEnginePrimitiveTypeInfo@@A); the manager is its FableWin.pdb inline GetPrimitiveManager(type)
// (PrimitiveManagers +0x10, engine/CEnginePrimitiveTypeInfo.h, overlay opt-in _GETPRIMITIVEMANAGER), a CEnginePrimitiveManager
// (engine/CEnginePrimitiveManager.h: the Ego_r.pdb virtuals in retail slot order). Were the CEnginePrimitiveManagerTable / g_pPrimitiveManagers and local CEnginePrimitiveManagerBase slot-view stand-ins.
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO
#define FABLE_RETAIL_CENGINEPRIMITIVETYPEINFO_GETPRIMITIVEMANAGER
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEnginePrimitiveManager.h"
namespace NEngineInternal { extern CEnginePrimitiveTypeInfo* GPPrimitiveTypeInfo; }   // 0x01436E84

class CEngineInternalPrimitive2DViewport : public CEngineInternalPrimitiveBase { public:
    unsigned char _pad38[0x94 - 0x38];
    std::vector<CEnginePrimitiveHandle> Primitives;                          // +0x94
    void UpdateRenderLayerMask();                      // 0x00BE1D80
    void SetChildPrimitive(unsigned long index, const CEnginePrimitive& primitive, unsigned long flags);
};

void CEngineInternalPrimitive2DViewport::SetChildPrimitive(unsigned long index, const CEnginePrimitive& primitive, unsigned long flags)
{
    if (index >= Primitives.size())
        Primitives.resize(index + 1);
    CEnginePrimitiveHandle& handle = Primitives[index];
    CEnginePrimitiveManager* manager = NEngineInternal::GPPrimitiveTypeInfo->GetPrimitiveManager(primitive.Type);
    bool enable = (flags & 0x20) ? false : true;
    if (manager) {
        CEngineInternalPrimitiveBase* child = manager->CreateInternalPrimitive((EEnginePrimitiveType)primitive.Type);
        handle.Clear();
        handle.Data = child;
        child->Update(primitive, flags);
        child->Enable = enable;
        UpdateRenderLayerMask();
    }
}