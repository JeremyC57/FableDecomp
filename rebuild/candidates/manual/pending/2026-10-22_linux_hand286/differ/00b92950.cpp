// Grade: DIFFER(719v707) -- first-pass model: retail places the early return-false epilogue right after the render-mask test and tests the mask from memory (test [esp+0x48],eax); ours shares the tail; this/inside frame slots swapped; the primitive loop and child recursion schedule differently.
// CEngineSceneGrid::RenderCellClippedAndCheckVisibility(x, y, renderMask, layerMask, camera, insideFrustum, passThrough, occlusionIndex, ignoreOcclusion, anyVisible) -- render one cell: reject by layer/render masks, clip its cached box against the camera unless the parent was fully inside, render (or retire occlusion groups of) its primitives, recurse into the four child cells, then track how long the cell has been invisible and build an occlusion group once it stays hidden long enough; returns whether anything in the cell was visible
// Leaf: RenderCellClippedAndCheckVisibility
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B92950, length 720 bytes (body 0x2c3 + int3 padding), `ret 0x28`, recursive.
// Callees: GetCellRenderMask 0x00B91DC0, GetCellBoundingBoxAndDrawDistance 0x00B919C0, CEngineCamera::ClipWorldSpaceBoundingBox
//   0x00B308E0, CEnginePrimitiveOcclusionManager::GetOccludedFrames 0x00B2FAC0 (0x01436E58), RemoveFromList 0x00B8FE60,
//   CEngineSceneGridCell::DeleteOcclusionGroup 0x00B92450 / CreateOcclusionGroup 0x00B92300, GetCellTotalPrimitiveCount 0x00B91D20,
//   primitive-manager table 0x01436E84 (virtual RenderPrimitive slot 0x1c), primitive virtuals 0x50 / 0x6c.
// Layout: local model.
struct C3DVector { float X, Y, Z; };
struct CAABox { C3DVector Min, Max; };
struct CEngineCamera {
    bool ClipWorldSpaceBoundingBox(const CAABox* box, float drawDistance, bool* inside);   // 0x00B308E0
};
struct COcclusionQuery;
struct CEnginePrimitiveOcclusionManager {
    unsigned char _pad[0xc];
    unsigned long MaxOccludedFrames;               // +0x0c
    unsigned long GetOccludedFrames(COcclusionQuery* query);   // 0x00B2FAC0
};
extern CEnginePrimitiveOcclusionManager* g_pPrimitiveOcclusionManager;   // 0x01436E58
struct CPrimitiveRenderData {
    unsigned char    _pad00[0xc];
    unsigned long    Flags;                        // +0x0c
    unsigned char    _pad10[0x48 - 0x10];
    COcclusionQuery* Queries[8];                   // +0x48
};
struct CEngineSceneGridCell;
struct SPrimitiveRenderInfo { long A, B; };
struct CEngineInternalPrimitiveBase {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14();
    virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c(); virtual void v40(); virtual void v44();
    virtual void v48(); virtual void v4c();
    virtual bool IsVisible(CEngineCamera* camera, bool inside);                 // +0x50
    virtual void v54(); virtual void v58(); virtual void v5c(); virtual void v60(); virtual void v64(); virtual void v68();
    virtual CPrimitiveRenderData* GetRenderData();                            // +0x6c
    long                          ManagerType;     // +0x08
    unsigned char                 _pad0c[0x1c - 0xc];
    unsigned long                 LayerMask;       // +0x1c
    unsigned char                 _pad20[0x30 - 0x20];
    unsigned long                 RenderMask;      // +0x30
    unsigned short                _pad34;
    unsigned short                RenderedFrame;   // +0x36
    CEngineInternalPrimitiveBase* Next;            // +0x38
    unsigned char                 _pad3c[0x50 - 0x3c];
    CEngineSceneGridCell*         OwnerCell;       // +0x50
    unsigned char                 _pad54[0x60 - 0x54];
    unsigned long                 GroupSize;       // +0x60
    void RemoveFromList();                         // 0x00B8FE60
};
struct CEnginePrimitiveManagerBase {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10(); virtual void v14();
    virtual void v18();
    virtual long RenderPrimitive(CEngineInternalPrimitiveBase* primitive, unsigned long renderMask, SPrimitiveRenderInfo* info);   // +0x1c
};
struct CEnginePrimitiveManagers {
    unsigned char                _pad00[0x10];
    CEnginePrimitiveManagerBase* Managers[64];     // +0x10
};
extern CEnginePrimitiveManagers* g_pPrimitiveManagers;   // 0x01436E84
extern unsigned short g_renderFrame_0143b8d8;            // 0x0143B8D8
extern unsigned long g_occlusionHiddenFrames_0139ac60;   // 0x0139AC60
extern unsigned long g_occlusionMinPrimitives_0139ac64;  // 0x0139AC64

struct CEngineSceneGrid;
struct CEngineSceneGridCell {
    void*                         OcclusionGroup;  // +0x00
    CEngineInternalPrimitiveBase* Primitives;      // +0x04
    unsigned char                 _pad08[0x40 - 8];
    unsigned long                 LayerMask;       // +0x40
    unsigned char                 Cached : 1;      // +0x44 bit 0
    unsigned char                 Bit1 : 1;
    unsigned char                 Bit2 : 1;
    unsigned char                 Processed : 1;   // bit 3
    unsigned char                 Visible : 1;     // bit 4
    unsigned char                 HiddenFrames;    // +0x45
    void DeleteOcclusionGroup(long index, bool large);                                  // 0x00B92450
    void CreateOcclusionGroup(long index, CEngineSceneGrid* grid, long x, long y);      // 0x00B92300
};
struct CEngineSceneGrid {
    void*                  _vptr;                  // +0x00
    long                   Level;                  // +0x04
    CEngineSceneGrid*      Parent;                 // +0x08
    CEngineSceneGrid*      Child;                  // +0x0c
    long                   Side;                   // +0x10
    CEngineSceneGridCell** Cells;                  // +0x14
    unsigned long GetCellRenderMask(long x, long y);                                    // 0x00B91DC0
    bool GetCellBoundingBoxAndDrawDistance(CAABox& box, float& drawDistance, long x, long y);   // 0x00B919C0
    unsigned long GetCellTotalPrimitiveCount(long x, long y);                           // 0x00B91D20
    bool RenderCellClippedAndCheckVisibility(long x, long y, unsigned long renderMask, unsigned long layerMask,
                                             CEngineCamera* camera, bool insideFrustum, long passThrough,
                                             long occlusionIndex, bool ignoreOcclusion, bool* anyVisible);
};

bool CEngineSceneGrid::RenderCellClippedAndCheckVisibility(long x, long y, unsigned long renderMask, unsigned long layerMask,
                                                           CEngineCamera* camera, bool insideFrustum, long passThrough,
                                                           long occlusionIndex, bool ignoreOcclusion, bool* anyVisible)
{
    CEngineSceneGridCell* cell = Cells[x + Side * y];
    if (!cell || (cell->LayerMask & layerMask))
        return false;
    bool wasVisible = cell->Visible;
    cell->Visible = 0;
    if (!(GetCellRenderMask(x, y) & renderMask))
        return false;
    bool inside = insideFrustum;
    if (!inside) {
        CAABox box;
        float drawDistance;
        if (!GetCellBoundingBoxAndDrawDistance(box, drawDistance, x, y))
            return cell->Visible;
        if (!camera->ClipWorldSpaceBoundingBox(&box, drawDistance, &inside))
            return cell->Visible;
    }
    long rendered = 0;
    for (CEngineInternalPrimitiveBase* p = cell->Primitives; p; ) {
        CPrimitiveRenderData* data = p->GetRenderData();
        if (data && (p->LayerMask & layerMask)) {
            if (ignoreOcclusion || (data->Flags & 0x4000)
                || (occlusionIndex != -1 && data->Queries[occlusionIndex]
                    && g_pPrimitiveOcclusionManager->GetOccludedFrames(data->Queries[occlusionIndex]) <= g_pPrimitiveOcclusionManager->MaxOccludedFrames)) {
                *anyVisible = true;
                if (p->ManagerType == 0x29) {
                    CEngineInternalPrimitiveBase* next = p->Next;
                    CEngineSceneGridCell* owner = p->OwnerCell;
                    p->RemoveFromList();
                    owner->DeleteOcclusionGroup(occlusionIndex, p->GroupSize > 1);
                    p = next;
                    continue;
                }
            }
        }
        if ((p->RenderMask & renderMask) && (p->LayerMask & layerMask) && p->IsVisible(camera, inside)) {
            SPrimitiveRenderInfo info;
            info.A = 0;
            info.B = 0;
            long result = g_pPrimitiveManagers->Managers[p->ManagerType]->RenderPrimitive(p, renderMask, &info);
            if (result == 2)
                p->RenderedFrame = g_renderFrame_0143b8d8;
            rendered |= result;
        }
        p = p->Next;
    }
    if (rendered)
        cell->Visible = 1;
    if (Child) {
        for (unsigned long j = 0; j < 2; ++j) {
            for (unsigned long i = 0; i < 2; ++i) {
                bool childVisible = false;
                bool result = Child->RenderCellClippedAndCheckVisibility(i + x * 2, j + y * 2, renderMask, layerMask, camera, inside,
                                                                         passThrough, occlusionIndex, ignoreOcclusion, &childVisible);
                *anyVisible |= childVisible;
                cell->Visible |= result;
            }
        }
    }
    if (wasVisible && !*anyVisible) {
        if (++cell->HiddenFrames < g_occlusionHiddenFrames_0139ac60)
            goto counted;
        if (cell->OcclusionGroup && GetCellTotalPrimitiveCount(x, y) >= g_occlusionMinPrimitives_0139ac64)
            cell->CreateOcclusionGroup(occlusionIndex, this, x, y);
    }
    cell->HiddenFrames = 0;
counted:
    cell->Processed = 1;
    return cell->Visible;
}
