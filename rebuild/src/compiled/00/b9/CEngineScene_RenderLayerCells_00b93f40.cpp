// CEngineScene::RenderLayerCells(EEngineRenderLayer layer, const CEngineCamera& camera) -- render one layer: when any primitive of the layer lives in the grid, every scene map's top grid level renders its cells clipped to the camera; then the two loose primitive lists are rendered; all into the layer's render-stats record
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B93F40, length 283 bytes (`ret 8`).
// Callees: map<long, CEngineSceneRenderStats>::operator[](const long&) @ 0x00B97B60 (on +0x2c), CEngineSceneGrid::RenderCellClipped
// (unsigned long, unsigned long, EEngineRenderLayer, const CEngineCamera&, bool, CEngineSceneRenderStats&) @ 0x00B92500,
// CEngineScene::RenderPrimitiveList(CEngineInternalPrimitiveBase*, EEngineRenderLayer, const CEngineCamera&,
// CEngineSceneRenderStats&) @ 0x00B91760 (landed).
// Layout: as the landed CEngineScene::ForEachUpdate 0x00B948E0 (MapList tree header at +0x04; outer node value = an inner
// tree whose nodes hold the CEngineSceneMap at +0x14; VC6-era node colour/parent/left/right/key/value) and the landed
// CEngineSceneMap models (GridLevels.first at +0x18, counted pointer object first). Scene fields (descriptive): the layer
// mask of grid-held primitives +0x1c, the two loose primitive lists +0x20/+0x24, the per-layer stats map +0x2c, the
// current-cell marker +0x44 (reset to -1).
// Shape: the map operator[] is a same-TU stand-in under auto_inline(off) (see below; without it `layer` is re-read from its slot
// after every call instead of living in ebp, and the camera is not cached in edi). The stats reference is taken first (the key's address = the `layer` argument slot), the marker reset follows the
// call; `this` is parked in the `push ecx` frame slot while esi walks the inner trees; both ends are re-read every step.
class CEngineCamera;
class CEngineSceneRenderStats;
class CEngineInternalPrimitiveBase;

struct CEngineSceneGrid {
    void RenderCellClipped(unsigned long x, unsigned long y, long layer, const CEngineCamera& camera, bool clipped,
                           CEngineSceneRenderStats& stats);   // 0x00B92500
};
struct CGridPointer { CEngineSceneGrid* m_pObject; long* m_pCount; };
struct CEngineSceneMap {
    unsigned char  _pad[0x18];
    CGridPointer*  GridLevelsFirst;               // +0x18 (GridLevels.first)
};

struct CMapNode { unsigned long Colour; CMapNode *Parent, *Left, *Right; long Key; CEngineSceneMap* Data; void* Info; };
struct CMapTree { CMapNode* Head; unsigned long Count; };
struct COuterNode { unsigned long Colour; COuterNode *Parent, *Left, *Right; long Key; CMapTree Maps; };
template <class T> static T* Next(T* node)
{
    if (node->Right) { node = node->Right; while (node->Left) node = node->Left; }
    else { T* parent = node->Parent; while (node == parent->Right) { node = parent; parent = parent->Parent; } if (node->Right != parent) node = parent; }
    return node;
}

struct CRenderStatsMap {
    void* m_header; unsigned long m_count;
    CEngineSceneRenderStats& operator[](const long& key);   // 0x00B97B60 (stand-in body below)
    CEngineSceneRenderStats& FindOrInsert(long key);         // descriptive: the tree lookup/insert the real body performs
};

// Stand-in for the out-of-line map operator[] @ 0x00B97B60, compiled above the graded function under auto_inline(off) so
// VC7.1 sees (as retail's whole-program build did) that the key reference is only read: the `layer` argument then stays
// in ebp across the calls although its address is passed. Only RenderLayerCells is graded.
#pragma auto_inline(off)
CEngineSceneRenderStats& CRenderStatsMap::operator[](const long& key)
{
    return FindOrInsert(key);
}
#pragma auto_inline(on)

struct CEngineScene {
    unsigned long                 Base;           // +0x00
    COuterNode*                   MapListHead;    // +0x04
    unsigned long                 MapListCount;   // +0x08
    unsigned char                 _pad0c[0x10];
    unsigned long                 GridLayerMask;  // +0x1c
    CEngineInternalPrimitiveBase* PrimitiveListA; // +0x20
    CEngineInternalPrimitiveBase* PrimitiveListB; // +0x24
    unsigned char                 _pad28[4];
    CRenderStatsMap               LayerStats;     // +0x2c
    unsigned char                 _pad34[0x10];
    long                          CurrentCell;    // +0x44
    void RenderPrimitiveList(CEngineInternalPrimitiveBase* list, long layer, const CEngineCamera& camera,
                             CEngineSceneRenderStats& stats);   // 0x00B91760
    void RenderLayerCells(long layer, const CEngineCamera& camera);
};

void CEngineScene::RenderLayerCells(long layer, const CEngineCamera& camera)
{
    CEngineSceneRenderStats& stats = LayerStats[layer];
    CurrentCell = -1;
    if (GridLayerMask & layer) {
        for (COuterNode* outer = MapListHead->Left; outer != MapListHead; outer = Next(outer))
            for (CMapNode* inner = outer->Maps.Head->Left; inner != outer->Maps.Head; inner = Next(inner))
                inner->Data->GridLevelsFirst->m_pObject->RenderCellClipped(0, 0, layer, camera, false, stats);
    }
    RenderPrimitiveList(PrimitiveListA, layer, camera, stats);
    RenderPrimitiveList(PrimitiveListB, layer, camera, stats);
}