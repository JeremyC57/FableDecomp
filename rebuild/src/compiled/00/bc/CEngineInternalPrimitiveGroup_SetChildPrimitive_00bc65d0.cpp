// CEngineInternalPrimitiveGroup::SetChildPrimitive(unsigned long, const CEnginePrimitive&, unsigned long) -- grow the child-handle vector to cover `index`, have the primitive type's manager create the internal primitive, store it in the child handle, update it from the description, enable it unless flag 0x20 is set and recompute the group's render-layer mask
// Grade: RELOCATION_MATCH -- the child handle reference is taken BEFORE the manager lookup (as in the viewport twin 0x00BE23F0): with the lookup first VC loaded the table global into edx ahead of primitive.Type; after the handle it lands in ecx after the type load like retail
// Leaf: SetChildPrimitive
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BC65D0, 210 bytes, `ret 0Ch`.
// Callees: vector<CEnginePrimitiveHandle>::Resize(long) 0x00443F90 (on Primitives +0x50, count index + 1),
//   CEngineInternalPrimitiveGroup::UpdateRenderLayerMask 0x00BC6180 (its own body; landed inline in 0x00BC6580); the
//   primitive-type manager table (global 0x01436E84, managers at +0x10 indexed by CEnginePrimitive::Type) slot 1
//   CreateInternalPrimitive(type); the new primitive's slots 3 OnLastHandleReleased(handle), 1 Destroy, 5
//   Update(const CEnginePrimitive&, flags).
// Layout (retail): CEngineInternalPrimitiveBase (root CIVCountedPointeeBase {vptr, RefCount +4}) with the Enable bit (bit 0 of
//   the +0x34 bit-field byte, Ego_r.pdb agrees); CEngineInternalPrimitiveGroup Primitives +0x50 (12-byte vector: first,
//   last, end), PrimitiveFlags +0x5c (engine/CEngineInternalPrimitiveGroup.h: a 16-byte CArray, PrimitiveFlags +0x60), so
//   `this` is a local model.
// Shape: the child handle is a reference local into the vector (`lea edi,[edx+edi*8]`); storing the new primitive is the
//   landed CEnginePrimitiveHandle model (`Clear()` = last-handle notice + `Data = 0`, then `Data = p`, every helper inline, so
//   two release sequences precede the store); the enable bit is `!(flags & 0x20)` (hoisted into bl).
// NF56 (round 7, queue q10427): Primitives is STLport 4.6.2 std::vector<CEnginePrimitiveHandle> (stlport/fable_containers.h,
//   FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL), so the call binds 0x00443F90 as
//   ?resize@?$vector@VCEnginePrimitiveHandle@@V?$allocator@VCEnginePrimitiveHandle@@@std@@@std@@QAEXI@Z (the self row's STLport spelling, as
//   0x00602E30 / 0x0064F659). Was the local stand-in CHandleVector (Resize(long)). The local CEnginePrimitiveHandle / CIVCountedPointer
//   models are pre-existing.
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

struct CEngineInternalPrimitiveGroup : CEngineInternalPrimitiveBase {
    unsigned char _pad38[0x50 - 0x38];
    std::vector<CEnginePrimitiveHandle> Primitives;    // +0x50
    unsigned long PrimitiveFlags;                      // +0x5c
    void UpdateRenderLayerMask();                      // 0x00BC6180
    void SetChildPrimitive(unsigned long index, const CEnginePrimitive& primitive, unsigned long flags);
};

void CEngineInternalPrimitiveGroup::SetChildPrimitive(unsigned long index, const CEnginePrimitive& primitive, unsigned long flags)
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