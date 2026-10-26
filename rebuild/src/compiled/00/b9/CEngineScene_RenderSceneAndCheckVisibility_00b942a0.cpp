// CEngineScene::RenderSceneAndCheckVisibility(EEngineRenderLayer layer, const CEngineCamera& camera, EEngineOcclusionLayer occlusion, bool flag) -- render one layer with occlusion queries: set the occlusion-layer bit for this pass, let every scene map's top grid level render its clipped cells and check their visibility, then render the two loose primitive lists
// Leaf: RenderSceneAndCheckVisibility
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B942A0, length 336 bytes (with int3 padding; body 0x14d), `ret 10h`.
// PDB: ?RenderSceneAndCheckVisibility@CEngineScene@@QAEXW4EEngineRenderLayer@@ABVCEngineCamera@@W4EEngineOcclusionLayer@@_N@Z
//   (catalogued "CEngineScene::RenderLayered", descriptive).
// Callees: map<long, CEngineSceneRenderStats>::operator[](const long&) @ 0x00B97B60, CEngineSceneGrid::
//   RenderCellClippedAndCheckVisibility(ulong, ulong, EEngineRenderLayer, ulong, const CEngineCamera&, bool,
//   CEngineSceneRenderStats&, EEngineOcclusionLayer, bool, bool&) @ 0x00B92950 (manifest signature), CEngineScene::
//   RenderPrimitiveList(CEngineInternalPrimitiveBase*, EEngineRenderLayer, const CEngineCamera&, CEngineSceneRenderStats&)
//   @ 0x00B91760 (landed).
// Layout: as the landed sibling CEngineScene::RenderLayerCells 0x00B93F40 (the +0x44 word is the current occlusion mask,
//   PDB CurrentOcclusionMask, here set to `1 << (occlusion + 1)`).
// Shape: the landed sibling's same-TU operator[] stand-in under auto_inline(off); the per-cell visibility bool is a block
//   local of an inline grid wrapper (its zero store then lands after all ten pushes, as in retail; a loop-body local is
//   stored between the pushes); the occlusion mask is re-read for every cell.
class CEngineCamera;
class CEngineSceneRenderStats;
class CEngineInternalPrimitiveBase;

class CEngineSceneGrid { public:
    bool RenderCellClippedAndCheckVisibility(unsigned long x, unsigned long y, long layer, unsigned long occlusionMask,
                                             const CEngineCamera& camera, bool clipped, CEngineSceneRenderStats& stats,
                                             long occlusion, bool flag, bool& visible);   // 0x00B92950
    void RenderCellAndCheck(long layer, unsigned long occlusionMask, const CEngineCamera& camera, CEngineSceneRenderStats& stats,
                            long occlusion, bool flag)
    {
        bool visible = false;
        RenderCellClippedAndCheckVisibility(0, 0, layer, occlusionMask, camera, false, stats, occlusion, flag, visible);
    }
};
struct CGridPointer { CEngineSceneGrid* m_pObject; long* m_pCount; };
class CEngineSceneMap { public:
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

// Stand-in for the out-of-line map operator[] @ 0x00B97B60 (as in the landed 0x00B93F40), compiled above the graded function
// under auto_inline(off) so VC7.1 sees that the key reference is only read. Only RenderSceneAndCheckVisibility is graded.
#pragma auto_inline(off)
CEngineSceneRenderStats& CRenderStatsMap::operator[](const long& key)
{
    return FindOrInsert(key);
}
#pragma auto_inline(on)

class CEngineScene { public:
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
    unsigned long                 CurrentOcclusionMask;   // +0x44
    void RenderPrimitiveList(CEngineInternalPrimitiveBase* list, long layer, const CEngineCamera& camera,
                             CEngineSceneRenderStats& stats);   // 0x00B91760
    void RenderSceneAndCheckVisibility(long layer, const CEngineCamera& camera, long occlusion, bool flag);
};

void CEngineScene::RenderSceneAndCheckVisibility(long layer, const CEngineCamera& camera, long occlusion, bool flag)
{
    CEngineSceneRenderStats& stats = LayerStats[layer];
    CurrentOcclusionMask = 1 << (occlusion + 1);
    if (GridLayerMask & layer) {
        for (COuterNode* outer = MapListHead->Left; outer != MapListHead; outer = Next(outer)) {
            for (CMapNode* inner = outer->Maps.Head->Left; inner != outer->Maps.Head; inner = Next(inner)) {
                inner->Data->GridLevelsFirst->m_pObject->RenderCellAndCheck(layer, CurrentOcclusionMask, camera, stats, occlusion, flag);
            }
        }
    }
    RenderPrimitiveList(PrimitiveListA, layer, camera, stats);
    RenderPrimitiveList(PrimitiveListB, layer, camera, stats);
}