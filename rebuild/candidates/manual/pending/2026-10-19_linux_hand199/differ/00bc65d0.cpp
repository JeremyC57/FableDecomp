// CEngineInternalPrimitiveGroup::SetChildPrimitive(unsigned long index, const CEnginePrimitive& primitive, unsigned long flags) -- grow the child array to cover the slot, replace the slot's internal primitive with a new one created by the primitive type's manager, initialise it from the primitive and flags (enabled unless flag bit 5), then recompute the group
// Grade: DIFFER(126v210) -- draft (one grade): the retail handle replacement inlines the CIVCountedPointer assignment twice (detach when singly referenced, release, two null stores) and keeps `this` in a frame slot; ours collapses it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC65D0, length 210 bytes (ret 0Ch).
// Callees: the child CArray's resize 0x00443F90 (manifest "CEnginePrimitiveRenderer::AddPrimitive" is a propagation), the type
// manager's virtual slot 1 CreateInternalPrimitive(type) (PrimitiveManagers[] of the type info 0x01436E84), the old child's
// slot 3 (detach, when singly referenced) and slot 1 (destroy, on the last reference), the new child's slot 5 Set(primitive,
// flags), the group's 0x00BC6180 (manifest "ClearAllChildPrimitives"; called after the change, purpose not proven).
// Layout: Primitives +0x50 (CArray of 8-byte CEnginePrimitiveHandle {info, CIVCountedPointer data +4}); the internal primitive's
// reference count +4, Enable bit 0 of +0x34.
// Shape: draft (one grade); see the Grade line.
#include "engine/CEnginePrimitiveTypeInfo.h"

class CEnginePrimitive {
public:
    long Type;                                  // +0x00
};

class CEngineInternalPrimitiveBase {
public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    virtual void v02();
    virtual void Detach(void* handle);          // slot 3
    virtual void v04();
    virtual void Set(const CEnginePrimitive& primitive, unsigned long flags);   // slot 5
    long          RefCount;                     // +0x04
    unsigned char _pad08[0x34 - 8];
    unsigned char Enable : 1;                   // +0x34 bit 0
};

struct CEnginePrimitiveManager {
    virtual void v00();
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(long type);   // slot 1
};
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;   // 0x01436E84

struct CEnginePrimitiveHandle {                 // 8 bytes
    void*                         Info;         // +0x00
    CEngineInternalPrimitiveBase* Data;         // +0x04 CIVCountedPointer
    void Release()
    {
        if (Data) {
            if (--Data->RefCount == 0)
                Data->Destroy();
            Data = 0;
        }
    }
    void Reset(CEngineInternalPrimitiveBase* primitive)
    {
        if (Data && Data->RefCount == 1)
            Data->Detach(this);
        Release();
        Data = 0;
        Release();
        Data = primitive;
    }
};

struct CPrimitiveHandleArray {                  // CArray<CEnginePrimitiveHandle>
    CEnginePrimitiveHandle* first;
    CEnginePrimitiveHandle* last;
    CEnginePrimitiveHandle* end;
    unsigned long size() const { return last - first; }
    void resize(unsigned long size);            // 0x00443F90
};

class CEngineInternalPrimitiveGroup {
public:
    unsigned char         _pad00[0x50];
    CPrimitiveHandleArray Primitives;           // +0x50
    void OnChildPrimitivesChanged_00BC6180();                           // 0x00BC6180
    void SetChildPrimitive(unsigned long index, const CEnginePrimitive& primitive, unsigned long flags);
};

void CEngineInternalPrimitiveGroup::SetChildPrimitive(unsigned long index, const CEnginePrimitive& primitive, unsigned long flags)
{
    if (index >= Primitives.size())
        Primitives.resize(index + 1);
    bool enable = !((flags >> 5) & 1);
    CEnginePrimitiveManager* manager = g_pEnginePrimitiveTypeInfo->PrimitiveManagers[primitive.Type];
    CEnginePrimitiveHandle& handle = Primitives.first[index];
    if (manager) {
        CEngineInternalPrimitiveBase* child = manager->CreateInternalPrimitive(primitive.Type);
        handle.Reset(child);
        child->Set(primitive, flags);
        child->Enable = enable;
        OnChildPrimitivesChanged_00BC6180();
    }
}
