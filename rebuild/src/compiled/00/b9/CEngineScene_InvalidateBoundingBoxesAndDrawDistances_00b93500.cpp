// CEngineScene::InvalidateBoundingBoxesAndDrawDistances() -- for every scene map of every map row, invalidate the bounding boxes and draw distances of its top grid level (cell 0,0, recursively)
// Leaf: InvalidateBoundingBoxesAndDrawDistances
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B93500, 181 bytes, thiscall, `ret`.
// Identity: Ego_r.pdb ?InvalidateBoundingBoxesAndDrawDistances@CEngineScene@@QAEXXZ; one caller 0x00B32072. Catalog
//   "InvalidateAllGridBoundingBoxes" is descriptive.
// Callees: CEngineSceneGrid::InvalidateBoundingBoxAndDrawDistance(0, 0, true) 0x00B91CA0 (landed).
// Layout: as the landed CEngineScene::RenderLayerCells 0x00B93F40 / ForEachUpdate 0x00B948E0 (MapList tree header at +0x04,
//   STLport node colour/parent/left/right/key/value; inner tree header at outer node +0x14; the CEngineSceneMap at inner
//   node +0x14; GridLevels.first at +0x18, counted pointer object first). Ego_r.pdb CEngineScene less 4 bytes after the
//   12-byte STLport MapList: FirstMovingPrimitive +0x20, OcclusionGroupDelay +0x48, OcclusionMinGroupSize +0x4c.
//   No engine header exists for CEngineScene, so `this` is a local model.
// Shape: two nested in-order tree walks with the STLport `_M_increment` inlined (the landed `Next` template); both
//   ends are re-read every step; the grid is the map's first (finest-level) counted grid pointer.
class CEngineInternalPrimitiveBase;
class CEngineSceneEnumerator;

class CEngineSceneGrid { public:
    void EnumeratePrimitives(unsigned long x, unsigned long y, CEngineSceneEnumerator& enumerator);   // 0x00B92250
    void InvalidateBoundingBoxAndDrawDistance(unsigned long x, unsigned long y, bool all);           // 0x00B91CA0
    bool CheckVisibilityResults(long layer, unsigned long x, unsigned long y, unsigned long groupDelay,
                                unsigned long minGroupSize, bool force);                           // 0x00B939E0
};
struct CGridPointer { CEngineSceneGrid* m_pObject; long* m_pCount; };   // CCountedPointer<CEngineSceneGrid>
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

class CEngineScene { public:
    unsigned long Base;                           // +0x00
    COuterNode*   MapListHead;                    // +0x04
    unsigned long MapListCount;                   // +0x08
    void InvalidateBoundingBoxesAndDrawDistances();
};

void CEngineScene::InvalidateBoundingBoxesAndDrawDistances()
{
    for (COuterNode* outer = MapListHead->Left; outer != MapListHead; outer = Next(outer))
        for (CMapNode* inner = outer->Maps.Head->Left; inner != outer->Maps.Head; inner = Next(inner))
            inner->Data->GridLevelsFirst->m_pObject->InvalidateBoundingBoxAndDrawDistance(0, 0, true);
}