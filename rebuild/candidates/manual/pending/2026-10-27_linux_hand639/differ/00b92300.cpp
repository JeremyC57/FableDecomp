// CEngineSceneGridCell::CreateOcclusionGroup(EEngineOcclusionLayer layer, CEngineSceneGrid* grid, unsigned long x, unsigned long y) -- when the cell has no occlusion group for the layer yet: build one, hold it in the layer's primitive handle, give it the layer's occlusion info and mark that occluded, set its layer mask, add it to the parent cell's primitives and flag the layer as occluded in this cell
// Grade: DIFFER(335v335) -- register tie: retail pushes ebx/ebp/edi first and keeps layer in edi and the handle reference in ebx; ours keeps layer in ebx and the handle in edi (pushes interleaved); every other instruction matches
// Leaf: ?CreateOcclusionGroup@CEngineSceneGridCell@@AAEXW4EEngineOcclusionLayer@@PAVCEngineSceneGrid@@KK@Z
// Tried (a-e): __forceinline PDB helpers (Clear/SetData/Reset/AddPrimitive), null-safe GetRefCount, early return vs if-block (layer lands in edi but the handle is no longer held), handle reference vs re-indexing.
#include "rebuild_abi.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CIVCountedPointeeBase.h"
class CEnginePrimitiveHandle;
class CEngineSceneGrid;
class CEngineSceneGridCell;
enum EEngineOcclusionLayer { };
template <class T> class CIVCountedPointer { public:
    T* Object;
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object ? Object->GetRefCount() : 0; }
    T* operator->() const { return Object; }
    T& operator*() const { return *Object; }
    T* Get() const { return Object; }
    __forceinline void Release()
    {
        T* object = Object;
        if (object) {
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    __forceinline void Reset(T* object) { Release(); Object = object; }
};
class CEnginePersistentPrimitiveBase : public CIVCountedPointeeBase { public:
    long Type;                                         // +0x08
    virtual void CollectStats() const;                 // slot 2 (signature not modelled)
    virtual void NotifyReleased(const CEnginePrimitiveHandle& handle);   // slot 3 (+0x0c)
    virtual bool WasRendered() const;                  // slot 4
};
#include "engine/CBaseClass.h"
class CEnginePrimitiveHandle : public CBaseClass { public:   // vtable 0x0122F598
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;  // +0x04
    bool IsNull() const { return Data.IsNull(); }
    CEnginePersistentPrimitiveBase& GetData() const { return *Data; }
    __forceinline void Clear()
    {
        if (Data.GetRefCount() == 1)
            Data->NotifyReleased(*this);
        Data.Reset(0);
    }
    __forceinline void SetData(CEnginePersistentPrimitiveBase* object) { Clear(); Data.Reset(object); }
};
class CEnginePrimitiveOcclusionInfo : public CIVCountedPointeeBase { public:   // 0x20 bytes
    unsigned char _body[0x20 - 8];
    CEnginePrimitiveOcclusionInfo();                   // 0x00B2FA90
};
class CEnginePrimitiveOcclusionManager { public:
    void SetAsOccluded(CEnginePrimitiveOcclusionInfo& info) const;   // 0x00B2F9F0
};
extern CEnginePrimitiveOcclusionManager* g_pPrimitiveOcclusionManager;   // 0x01436E58
class CEngineInternalPrimitiveBase : public CEnginePersistentPrimitiveBase { public:
    unsigned long         EnginePrimitiveFlags;        // +0x0c
    float                 AlphaMipBias;                // +0x10
    float                 FadeDistance[2];             // +0x14
    unsigned long         OcclusionLayerBitMask;       // +0x1c
    CEngineSceneGridCell* GridCell;                    // +0x20
    unsigned char         _pad24[0x48 - 0x24];
    void SetGridCell(CEngineSceneGridCell* cell) { GridCell = cell; }
    void AddToList(CEngineInternalPrimitiveBase** list);   // 0x00B8FDF0
};
class CEngineInternalPrimitiveOcclusionBase : public CEngineInternalPrimitiveBase { public:
    CIVCountedPointer<CEnginePrimitiveOcclusionInfo> OcclusionInfo[2];   // +0x48
};
class CEngineInternalPrimitiveOcclusionGroup : public CEngineInternalPrimitiveOcclusionBase { public:   // 0x64 bytes
    unsigned char _pad50[0x64 - 0x50];
    CEngineInternalPrimitiveOcclusionGroup(CEngineSceneGridCell* cell, CEngineSceneGrid* grid, unsigned long x, unsigned long y);   // 0x00BACC00
};
class CEngineSceneGridCell { public:
    CEngineSceneGridCell*         Parent;              // +0x00
    CEngineInternalPrimitiveBase* FirstPrimitive;      // +0x04
    float                         BoundingBox[6];      // +0x08 C3DBoundingBox
    float                         DrawDistance;        // +0x20
    CEnginePrimitiveHandle        GroupPrimitiveHandle[2];   // +0x24
    long                          LocalCount;          // +0x34
    unsigned long                 TotalCount;          // +0x38
    unsigned long                 RenderMask;          // +0x3c
    unsigned long                 OccludedLayers;      // +0x40
    unsigned char                 BoundingBoxValid : 1;     // +0x44
    unsigned char                 TotalCountValid : 1;
    unsigned char                 RenderMaskValid : 1;
    unsigned char                 WasRendered : 1;
    unsigned char                 HasOcclusionResults : 1;
    bool IsBoundingBoxValid() const { return BoundingBoxValid; }
private:
    bool IsTotalCountValid() const { return TotalCountValid; }
    bool IsRenderMaskValid() const { return RenderMaskValid; }
    void InvalidateAll();                              // 0x00B91710
    CEngineInternalPrimitiveOcclusionGroup& GetGroupPrimitive(EEngineOcclusionLayer layer) const
    { return static_cast<CEngineInternalPrimitiveOcclusionGroup&>(GroupPrimitiveHandle[layer].GetData()); }
    __forceinline void AddPrimitive(CEngineInternalPrimitiveBase& primitive)
    {
        primitive.SetGridCell(this);
        primitive.AddToList(&FirstPrimitive);
        ++LocalCount;
        if (IsBoundingBoxValid() || IsTotalCountValid() || IsRenderMaskValid()) {
            BoundingBoxValid = 0;
            TotalCountValid = 0;
            RenderMaskValid = 0;
            if (Parent)
                Parent->InvalidateAll();
        }
    }
    void CreateOcclusionGroup(EEngineOcclusionLayer layer, CEngineSceneGrid* grid, unsigned long x, unsigned long y);
};

void CEngineSceneGridCell::CreateOcclusionGroup(EEngineOcclusionLayer layer, CEngineSceneGrid* grid, unsigned long x, unsigned long y)
{
    CEnginePrimitiveHandle& handle = GroupPrimitiveHandle[layer];
    if (!handle.IsNull())
        return;
    unsigned long mask = 1 << (layer + 1);
    handle.SetData(new CEngineInternalPrimitiveOcclusionGroup(this, grid, x, y));
    CEngineInternalPrimitiveOcclusionGroup& group = GetGroupPrimitive(layer);
    if (layer != -1) {
        if (!(group.EnginePrimitiveFlags & 0x4000) && group.OcclusionInfo[layer].IsNull())
            group.OcclusionInfo[layer].Reset(new CEnginePrimitiveOcclusionInfo);
        g_pPrimitiveOcclusionManager->SetAsOccluded(*group.OcclusionInfo[layer]);
    }
    group.OcclusionLayerBitMask = mask;
    Parent->AddPrimitive(static_cast<CEngineInternalPrimitiveBase&>(handle.GetData()));
    OccludedLayers |= mask;
}
