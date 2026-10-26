// CEngineSceneGrid::RenderCellClipped(ulong x, ulong y, EEngineRenderLayer layer, const CEngineCamera& camera, bool clipped, CEngineSceneRenderStats& stats) -- render one grid cell against the camera: skip empty cells and cells without the layer, clip the cell box unless already inside, render the cell's primitives of the layer that pass their own camera test, recurse into the four child cells and mark the cell rendered
// Leaf: RenderCellClipped
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B92500, length 352 bytes (with int3 padding; body 0x157), `ret 18h`.
// PDB: ?RenderCellClipped@CEngineSceneGrid@@QAEXKKW4EEngineRenderLayer@@ABVCEngineCamera@@_NAAVCEngineSceneRenderStats@@@Z.
// Callees: CEngineSceneGrid::GetCellRenderMask(ulong, ulong) @ 0x00B91DC0, GetCellBoundingBoxAndDrawDistance(C3DBoundingBox&,
//   float&, ulong, ulong) @ 0x00B919C0, CEngineCamera::ClipWorldSpaceBoundingBox(const C3DBoundingBox&, float, bool&) const
//   @ 0x00B308E0, the primitive's virtual camera test (slot 20, +0x50; name unknown), the primitive manager's
//   RenderPrimitive (slot 7) on the type table @ 0x01436E84, the frame counter @ 0x0143B8D8, itself on LevelDown.
// Layout: CEngineSceneGrid (PDB: LevelDown +0x0c, Size +0x10, GridCells +0x14), CEngineSceneGridCell (FirstPrimitive +0x04,
//   WasRendered = bit 3 of +0x44), CEngineInternalPrimitiveBase (Type +0x08, RenderLayerMask +0x30, LastFrameRendered +0x36,
//   NextPrimitive +0x38, NextLayerMask +0x40), as the sibling CEngineScene::RenderEntireScene 0x00B943F0 in this chunk.
// Shape: `clipped` is copied to a frame bool (its slot then holds the draw distance and the child row counter); the cell
//   box/distance come back through out-parameters; the chain is zeroed after the manager load.
class CEngineCamera;
class CEngineSceneRenderStats;
class CEngineSubPrimitiveBase;

class C3DBoundingBox { public: float MinX, MinY, MinZ, MaxX, MaxY, MaxZ; };

class CEngineCamera {
public:
    bool ClipWorldSpaceBoundingBox(const C3DBoundingBox& box, float drawDistance, bool& clipped) const;   // 0x00B308E0
};

class CEngineSubPrimitiveChain { public:
    CEngineSubPrimitiveBase* LastOpaque;
    CEngineSubPrimitiveBase* LastAlpha;
    CEngineSubPrimitiveChain() : LastOpaque(0), LastAlpha(0) {}
};

class CEngineInternalPrimitiveBase;
class CEnginePrimitiveManager { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06();
    virtual long RenderPrimitive(CEngineInternalPrimitiveBase& primitive, long layer, CEngineSubPrimitiveChain& chain);   // slot 7
};
class CEnginePrimitiveTypeInfo { public:
    unsigned char            _pad[0x10];
    CEnginePrimitiveManager* PrimitiveManagers[47];   // +0x10
};
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;   // 0x01436E84
extern unsigned short g_engineFrameNumber;                     // 0x0143B8D8

class CEngineInternalPrimitiveBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual bool IsVisibleToCamera(const CEngineCamera& camera, bool clipped);   // slot 20 (+0x50)
    unsigned long                 _pad04;
    long                          Type;             // +0x08
    unsigned char                 _pad0c[0x30 - 0x0c];
    unsigned long                 RenderLayerMask;  // +0x30
    unsigned char                 Flags;            // +0x34
    unsigned short                LastFrameRendered;   // +0x36
    CEngineInternalPrimitiveBase* NextPrimitive;    // +0x38
    CEngineInternalPrimitiveBase** RefPrimitive;    // +0x3c
    CEngineInternalPrimitiveBase* NextLayerMask;    // +0x40
};

class CEngineSceneGridCell { public:
    CEngineSceneGridCell*         Parent;           // +0x00
    CEngineInternalPrimitiveBase* FirstPrimitive;   // +0x04
    unsigned char                 _pad08[0x3c];
    unsigned char                 BoundingBoxValid : 1;   // +0x44
    unsigned char                 TotalCountValid : 1;
    unsigned char                 RenderMaskValid : 1;
    unsigned char                 WasRendered : 1;
    unsigned char                 HasOcclusionResults : 1;
};

template <class T> class CArray { public:
    T* m_pBegin;
    T* m_pEnd;
    T* m_pCapacity;
    T& operator[](unsigned long i) { return m_pBegin[i]; }
};

class CEngineSceneGrid { public:
    void*                         _vptr;         // +0x00
    unsigned long                 Level;         // +0x04
    CEngineSceneGrid*             LevelUp;       // +0x08
    CEngineSceneGrid*             LevelDown;     // +0x0c
    unsigned long                 Size;          // +0x10
    CArray<CEngineSceneGridCell*> GridCells;     // +0x14
    unsigned long GetCellRenderMask(unsigned long x, unsigned long y);   // 0x00B91DC0
    bool GetCellBoundingBoxAndDrawDistance(C3DBoundingBox& box, float& drawDistance, unsigned long x, unsigned long y);   // 0x00B919C0
    void RenderCellClipped(unsigned long x, unsigned long y, long layer, const CEngineCamera& camera, bool clipped,
                           CEngineSceneRenderStats& stats);
};

void CEngineSceneGrid::RenderCellClipped(unsigned long x, unsigned long y, long layer, const CEngineCamera& camera, bool clipped,
                                         CEngineSceneRenderStats& stats)
{
    CEngineSceneGridCell* cell = GridCells[Size * y + x];
    if (!cell)
        return;
    if (!(layer & GetCellRenderMask(x, y)))
        return;
    bool inside = clipped;
    if (!inside) {
        C3DBoundingBox box;
        float drawDistance;
        if (!GetCellBoundingBoxAndDrawDistance(box, drawDistance, x, y))
            return;
        if (!camera.ClipWorldSpaceBoundingBox(box, drawDistance, inside))
            return;
    }
    CEngineInternalPrimitiveBase* primitive = cell->FirstPrimitive;
    while (primitive) {
        if (primitive->RenderLayerMask & layer) {
            if (primitive->IsVisibleToCamera(camera, inside)) {
                CEnginePrimitiveManager* manager = g_pEnginePrimitiveTypeInfo->PrimitiveManagers[primitive->Type];
                CEngineSubPrimitiveChain chain;
                if (manager->RenderPrimitive(*primitive, layer, chain) == 2)
                    primitive->LastFrameRendered = g_engineFrameNumber;
            }
            primitive = primitive->NextPrimitive;
        }
        else
            primitive = primitive->NextLayerMask;
    }
    if (LevelDown) {
        for (unsigned long j = 0; j < 2; ++j)
            for (unsigned long i = 0; i < 2; ++i)
                LevelDown->RenderCellClipped(x * 2 + i, y * 2 + j, layer, camera, inside, stats);
    }
    cell->WasRendered = 1;
}