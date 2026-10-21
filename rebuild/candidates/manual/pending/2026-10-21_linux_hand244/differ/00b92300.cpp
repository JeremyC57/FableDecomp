// Grade: DIFFER(335v335) -- register swap only: retail keeps the layer in edi (loaded after the four pushes) and the handle address in ebx; ours loads the layer into ebx after push ebx and the handle into edi
// CEngineSceneGridCell::CreateOcclusionGroup(EEngineOcclusionLayer layer, CEngineSceneGrid* grid, ulong x, ulong y) -- when the cell has no occlusion group for the layer yet: build one, hold it in the layer's primitive handle, give it the layer's occlusion info and mark it occluded, then add it to the parent cell's primitives and flag the layer as occluded in this cell
// Leaf: CreateOcclusionGroup
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B92300, length 336 bytes (with int3 padding; body 0x14f), `ret 10h`.
// PDB: ?CreateOcclusionGroup@CEngineSceneGridCell@@AAEXW4EEngineOcclusionLayer@@PAVCEngineSceneGrid@@KK@Z.
// Callees: operator new @ 0x00BFEA1A, CEngineInternalPrimitiveOcclusionGroup::CEngineInternalPrimitiveOcclusionGroup
//   (CEngineSceneGridCell*, CEngineSceneGrid*, ulong, ulong) @ 0x00BACC00 (100 bytes), CEnginePrimitiveOcclusionInfo ctor
//   @ 0x00B2FA90 (0x20 bytes), CEnginePrimitiveOcclusionManager::SetAsOccluded(CEnginePrimitiveOcclusionInfo*) @ 0x00B2F9F0
//   (global @ 0x01436E58), CEngineInternalPrimitiveBase::AddToList @ 0x00B8FDF0, CEngineSceneGridCell::InvalidateAll
//   @ 0x00B91710; the counted primitive's virtual slots 1 (Destroy) / 3 (OnLastHandleReleased).
// Layout: CEngineSceneGridCell (PDB): Parent +0x00, FirstPrimitive +0x04, GroupPrimitiveHandle[2] +0x24 (8-byte handles),
//   LocalCount +0x34, OccludedLayers +0x40, validity bits 0-2 of +0x44; the group's primitive base: RefCount +0x04,
//   EnginePrimitiveFlags +0x0c, OcclusionLayerBitMask +0x1c, GridCell +0x20, per-layer occlusion info pointers +0x48.
// Shape: the handle and counted-pointer helpers of the landed CEngineInternalPrimitiveParticleGroup::SetChildPrimitive
//   0x00BBC310 (Clear() = unique-owner callback + release), the cell AddPrimitive inline of this chunk's MovePrimitivesDown.
class CEnginePrimitiveHandle;
class CEngineSceneGrid;
struct CEngineSceneGridCell;

class CEnginePrimitiveOcclusionInfo {
public:
    virtual void v000();
    virtual void Destroy();                        // slot 1
    long RefCount;                                 // +0x04
    unsigned char _pad08[0x18];
    CEnginePrimitiveOcclusionInfo();               // 0x00B2FA90
    void Release() { if (--RefCount == 0) Destroy(); }
};
class CEnginePrimitiveOcclusionManager {
public:
    void SetAsOccluded(CEnginePrimitiveOcclusionInfo* info);   // 0x00B2F9F0
};
extern CEnginePrimitiveOcclusionManager* g_pPrimitiveOcclusionManager;   // 0x01436E58

template <class T> class CIVCountedPointer {
public:
    T* Object;
    T* operator->() const { return Object; }
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
    __forceinline void Assign(T* object)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = object;
    }
};

class CEngineInternalPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    long                          RefCount;         // +0x04
    unsigned long                 _pad08;
    unsigned long                 EnginePrimitiveFlags;   // +0x0c
    unsigned char                 _pad10[0x0c];
    unsigned long                 OcclusionLayerBitMask;  // +0x1c
    CEngineSceneGridCell*         GridCell;         // +0x20
    unsigned char                 _pad24[0x48 - 0x24];
    CIVCountedPointer<CEnginePrimitiveOcclusionInfo> OcclusionInfo[2];   // +0x48
    void Release() { if (--RefCount == 0) Destroy(); }
    void AddToList(CEngineInternalPrimitiveBase** list);   // 0x00B8FDF0
};

class CEngineInternalPrimitiveOcclusionGroup : public CEngineInternalPrimitiveBase {
public:
    unsigned char _pad50[0x14];
    CEngineInternalPrimitiveOcclusionGroup(CEngineSceneGridCell* cell, CEngineSceneGrid* grid, unsigned long x, unsigned long y);   // 0x00BACC00
};

class CEnginePrimitiveHandle {                     // 8 bytes: vptr + counted Data
public:
    void* _vptr;
    CIVCountedPointer<CEngineInternalPrimitiveBase> Data;   // +0x04
    __forceinline void Clear()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
    __forceinline void Set(CEngineInternalPrimitiveBase* primitive)
    {
        Clear();
        Data.Assign(primitive);
    }
};

struct CEngineSceneGridCell {
    CEngineSceneGridCell*         Parent;           // +0x00
    CEngineInternalPrimitiveBase* FirstPrimitive;   // +0x04
    unsigned char                 _pad08[0x1c];
    CEnginePrimitiveHandle        GroupPrimitiveHandle[2];   // +0x24
    long                          LocalCount;       // +0x34
    unsigned long                 TotalCount;       // +0x38
    unsigned long                 RenderMask;       // +0x3c
    unsigned long                 OccludedLayers;   // +0x40
    unsigned char                 BoundingBoxValid : 1;   // +0x44
    unsigned char                 TotalCountValid : 1;
    unsigned char                 RenderMaskValid : 1;
    unsigned char                 WasRendered : 1;
    unsigned char                 HasOcclusionResults : 1;
    void InvalidateAll();                           // 0x00B91710
    __forceinline void AddPrimitive(CEngineInternalPrimitiveBase& primitive)
    {
        primitive.AddToList(&FirstPrimitive);
        ++LocalCount;
        if (BoundingBoxValid || TotalCountValid || RenderMaskValid) {
            BoundingBoxValid = 0;
            TotalCountValid = 0;
            RenderMaskValid = 0;
            if (Parent)
                Parent->InvalidateAll();
        }
    }
    void CreateOcclusionGroup(long layer, CEngineSceneGrid* grid, unsigned long x, unsigned long y);
};

void CEngineSceneGridCell::CreateOcclusionGroup(long layer, CEngineSceneGrid* grid, unsigned long x, unsigned long y)
{
    CEnginePrimitiveHandle& handle = GroupPrimitiveHandle[layer];
    if (handle.Data.Object)
        return;
    unsigned long mask = 1 << (layer + 1);
    handle.Set(new CEngineInternalPrimitiveOcclusionGroup(this, grid, x, y));
    CEngineInternalPrimitiveBase* group = GroupPrimitiveHandle[layer].Data.Object;
    if (layer != -1) {
        if (!(group->EnginePrimitiveFlags & 0x4000) && !group->OcclusionInfo[layer].Object)
            group->OcclusionInfo[layer].Assign(new CEnginePrimitiveOcclusionInfo);
        g_pPrimitiveOcclusionManager->SetAsOccluded(group->OcclusionInfo[layer].Object);
    }
    group->OcclusionLayerBitMask = mask;
    CEngineSceneGridCell* parent = Parent;
    CEngineInternalPrimitiveBase& primitive = *handle.Data.Object;
    primitive.GridCell = parent;
    parent->AddPrimitive(primitive);
    OccludedLayers |= mask;
}
