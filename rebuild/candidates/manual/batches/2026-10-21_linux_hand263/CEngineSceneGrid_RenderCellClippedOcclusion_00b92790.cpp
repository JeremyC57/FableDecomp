// CEngineSceneGrid::RenderCellClippedOcclusion(unsigned long x, unsigned long y, EEngineRenderLayer layer, unsigned long occlusionLayers, const CEngineCamera& camera, bool clipped, CEngineSceneRenderStats& stats) -- render one grid cell's occlusion primitives against the camera: skip empty or already-occluded cells and cells without the layer, clip the cell box unless already inside, render the primitives of the layer and occlusion layers that pass their own camera test, recurse into the four child cells, mark the cell rendered and report whether the cell (or any child) produced occlusion results
// Leaf: RenderCellClippedOcclusion
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B92790, length 435 bytes (+ int3 padding), `ret 0x1c`.
// Callees: as the landed sibling CEngineSceneGrid::RenderCellClipped 0x00B92500 (GetCellRenderMask 0x00B91DC0,
//   GetCellBoundingBoxAndDrawDistance 0x00B919C0, CEngineCamera::ClipWorldSpaceBoundingBox 0x00B308E0, the primitive's camera
//   test slot 20, the manager's RenderPrimitive slot 7 on the type table 0x01436E84, the frame counter 0x0143B8D8) plus
//   itself on LevelDown.
// Layout: the sibling's model, plus Ego_r.pdb CEngineSceneGridCell OccludedLayers +0x40 and HasOcclusionResults (bit 4 of
//   +0x44), CEngineInternalPrimitiveBase OcclusionLayerBitMask +0x1c.
// Shape: the null-cell and occluded-cell tests are two separate `return false` statements (one `||` lets VC7.1 move the
//   shared exit to the end of the function); the clip failures return the cell flag; `rendered` is a long OR of the results.
class CEngineCamera;
class CEngineSceneRenderStats;
class CEngineSubPrimitiveBase;

struct C3DBoundingBox { float MinX, MinY, MinZ, MaxX, MaxY, MaxZ; };

class CEngineCamera {
public:
    bool ClipWorldSpaceBoundingBox(const C3DBoundingBox& box, float drawDistance, bool& clipped) const;   // 0x00B308E0
};

struct CEngineSubPrimitiveChain {
    CEngineSubPrimitiveBase* LastOpaque;
    CEngineSubPrimitiveBase* LastAlpha;
    CEngineSubPrimitiveChain() : LastOpaque(0), LastAlpha(0) {}
};

class CEngineInternalPrimitiveBase;
struct CEnginePrimitiveManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06();
    virtual long RenderPrimitive(CEngineInternalPrimitiveBase& primitive, long layer, CEngineSubPrimitiveChain& chain);   // slot 7
};
struct CEnginePrimitiveTypeInfo {
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
    unsigned char                 _pad0c[0x1c - 0x0c];
    unsigned long                 OcclusionLayerBitMask;   // +0x1c
    unsigned char                 _pad20[0x30 - 0x20];
    unsigned long                 RenderLayerMask;  // +0x30
    unsigned char                 Flags;            // +0x34
    unsigned short                LastFrameRendered;   // +0x36
    CEngineInternalPrimitiveBase* NextPrimitive;    // +0x38
    CEngineInternalPrimitiveBase** RefPrimitive;    // +0x3c
    CEngineInternalPrimitiveBase* NextLayerMask;    // +0x40
};

struct CEngineSceneGridCell {
    CEngineSceneGridCell*         Parent;           // +0x00
    CEngineInternalPrimitiveBase* FirstPrimitive;   // +0x04
    unsigned char                 _pad08[0x40 - 0x08];
    unsigned long                 OccludedLayers;   // +0x40
    unsigned char                 BoundingBoxValid : 1;   // +0x44
    unsigned char                 TotalCountValid : 1;
    unsigned char                 RenderMaskValid : 1;
    unsigned char                 WasRendered : 1;
    unsigned char                 HasOcclusionResults : 1;
};

template <class T> struct CArray {
    T* m_pBegin;
    T* m_pEnd;
    T* m_pCapacity;
    T& operator[](unsigned long i) { return m_pBegin[i]; }
};

struct CEngineSceneGrid {
    void*                         _vptr;         // +0x00
    unsigned long                 Level;         // +0x04
    CEngineSceneGrid*             LevelUp;       // +0x08
    CEngineSceneGrid*             LevelDown;     // +0x0c
    unsigned long                 Size;          // +0x10
    CArray<CEngineSceneGridCell*> GridCells;     // +0x14
    unsigned long GetCellRenderMask(unsigned long x, unsigned long y);   // 0x00B91DC0
    bool GetCellBoundingBoxAndDrawDistance(C3DBoundingBox& box, float& drawDistance, unsigned long x, unsigned long y);   // 0x00B919C0
    bool RenderCellClippedOcclusion(unsigned long x, unsigned long y, long layer, unsigned long occlusionLayers,
                                    const CEngineCamera& camera, bool clipped, CEngineSceneRenderStats& stats);
};

bool CEngineSceneGrid::RenderCellClippedOcclusion(unsigned long x, unsigned long y, long layer, unsigned long occlusionLayers,
                                                  const CEngineCamera& camera, bool clipped, CEngineSceneRenderStats& stats)
{
    CEngineSceneGridCell* cell = GridCells[Size * y + x];
    if (!cell)
        return false;
    if (cell->OccludedLayers & occlusionLayers)
        return false;
    if (!(layer & GetCellRenderMask(x, y)))
        return false;
    bool inside = clipped;
    if (!inside) {
        C3DBoundingBox box;
        float drawDistance;
        if (!GetCellBoundingBoxAndDrawDistance(box, drawDistance, x, y))
            return cell->HasOcclusionResults;
        if (!camera.ClipWorldSpaceBoundingBox(box, drawDistance, inside))
            return cell->HasOcclusionResults;
    }
    long rendered = 0;
    CEngineInternalPrimitiveBase* primitive = cell->FirstPrimitive;
    while (primitive) {
        if (primitive->RenderLayerMask & layer) {
            if ((primitive->OcclusionLayerBitMask & occlusionLayers) && primitive->IsVisibleToCamera(camera, inside)) {
                CEnginePrimitiveManager* manager = g_pEnginePrimitiveTypeInfo->PrimitiveManagers[primitive->Type];
                CEngineSubPrimitiveChain chain;
                long result = manager->RenderPrimitive(*primitive, layer, chain);
                if (result == 2)
                    primitive->LastFrameRendered = g_engineFrameNumber;
                rendered |= result;
            }
            primitive = primitive->NextPrimitive;
        }
        else
            primitive = primitive->NextLayerMask;
    }
    if (rendered)
        cell->HasOcclusionResults = 1;
    if (LevelDown) {
        for (unsigned long j = 0; j < 2; ++j)
            for (unsigned long i = 0; i < 2; ++i)
                cell->HasOcclusionResults |= LevelDown->RenderCellClippedOcclusion(x * 2 + i, y * 2 + j, layer, occlusionLayers,
                                                                                 camera, inside, stats);
    }
    cell->WasRendered = 1;
    return cell->HasOcclusionResults;
}
