// CEngineScene::RenderEntireScene(EEngineRenderLayer layer) -- render one layer without culling: every scene map's top grid level renders all its cells into the layer's stats, then each moving primitive of the layer is rendered through its type's primitive manager and stamped with the frame number when it drew
// Leaf: ?RenderEntireScene@CEngineScene@@QAEXW4EEngineRenderLayer@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B943F0, length 336 bytes (with int3 padding; body 0x14a), `ret 4`.
// PDB: ?RenderEntireScene@CEngineScene@@QAEXW4EEngineRenderLayer@@@Z (catalogued "CEngineSceneGrid::RenderAll", propagated).
// Callees: map<long, CEngineSceneRenderStats>::operator[](const long&) @ 0x00B97B60 (on +0x2c), CEngineSceneGrid::
//   RenderCellAll(unsigned long, unsigned long, EEngineRenderLayer, CEngineSceneRenderStats&) @ 0x00B92C20, the primitive
//   manager's virtual RenderPrimitive(CEngineInternalPrimitiveBase&, EEngineRenderLayer, CEngineSubPrimitiveChain&)
//   (slot 7, +0x1c; see 0x00BC6340) on the type table @ 0x01436E84 (PrimitiveManagers +0x10); the engine frame counter
//   @ 0x0143B8D8.
// Layout: the landed sibling CEngineScene::RenderLayerCells 0x00B93F40 (MapList header +0x04, grid-layer mask +0x1c, moving
//   list +0x20, stats map +0x2c, occlusion marker +0x44; outer/inner STLport tree nodes, scene map GridLevels.first +0x18);
//   CEngineInternalPrimitiveBase (PDB): Type +0x08, RenderLayerMask +0x30, LastFrameRendered +0x36, NextPrimitive +0x38,
//   NextLayerMask +0x40. CEngineSubPrimitiveChain (PDB, 8 bytes): LastOpaque, LastAlpha.
// Shape: as 0x00B93F40, including its same-TU operator[] stand-in under auto_inline(off) (keeps `layer` in ebp); the chain
//   local reuses the dead stats slot and is zeroed per primitive.
class CEngineSceneRenderStats;
enum EEngineRenderLayer { };
enum EEngineOcclusionLayer { };
class CEngineInternalPrimitiveBase;
class CEngineSubPrimitiveBase;

class CEngineSubPrimitiveChain { public:
    CEngineSubPrimitiveBase* LastOpaque;
    CEngineSubPrimitiveBase* LastAlpha;
    CEngineSubPrimitiveChain() : LastOpaque(0), LastAlpha(0) {}
};

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
    void*                         _vptr;            // +0x00
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

class CEngineSceneGrid { public:
    void RenderCellAll(unsigned long x, unsigned long y, EEngineRenderLayer layer, CEngineSceneRenderStats& stats);   // 0x00B92C20
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

namespace std {
template <class T> struct less;
template <class T> class allocator;
template <class T1, class T2> struct pair;
template <class Key, class T, class Compare = less<Key>, class Alloc = allocator<pair<const Key, T> > > class map {
public:
    void* m_header; unsigned long m_count;
    T& operator[](const Key& key);              // 0x00B97B60 (stand-in body below)
    T& FindOrInsert(Key key);                   // descriptive: the tree lookup/insert the real body performs
};
}
typedef std::map<EEngineRenderLayer, CEngineSceneRenderStats> CRenderStatsMap;

// Stand-in for the out-of-line map operator[] @ 0x00B97B60 (as in the landed 0x00B93F40), compiled above the graded function
// under auto_inline(off) so VC7.1 sees that the key reference is only read. Only RenderEntireScene is graded.
#pragma auto_inline(off)
namespace std {
template <> CEngineSceneRenderStats& map<EEngineRenderLayer, CEngineSceneRenderStats>::operator[](const EEngineRenderLayer& key)
{
    return FindOrInsert(key);
}
}
#pragma auto_inline(on)

class CEngineScene { public:
    unsigned long                 Base;           // +0x00
    COuterNode*                   MapListHead;    // +0x04
    unsigned long                 MapListCount;   // +0x08
    unsigned char                 _pad0c[0x10];
    unsigned long                 GridLayerMask;  // +0x1c
    CEngineInternalPrimitiveBase* PrimitiveListA; // +0x20 FirstMovingPrimitive
    CEngineInternalPrimitiveBase* PrimitiveListB; // +0x24
    unsigned char                 _pad28[4];
    CRenderStatsMap               LayerStats;     // +0x2c
    unsigned char                 _pad34[0x10];
    long                          CurrentCell;    // +0x44
public:
    void RenderEntireScene(EEngineRenderLayer layer);
};

void CEngineScene::RenderEntireScene(EEngineRenderLayer layer)
{
    CEngineSceneRenderStats& stats = LayerStats[layer];
    CurrentCell = -1;
    if (GridLayerMask & layer) {
        for (COuterNode* outer = MapListHead->Left; outer != MapListHead; outer = Next(outer))
            for (CMapNode* inner = outer->Maps.Head->Left; inner != outer->Maps.Head; inner = Next(inner))
                inner->Data->GridLevelsFirst->m_pObject->RenderCellAll(0, 0, layer, stats);
    }
    CEngineInternalPrimitiveBase* primitive = PrimitiveListA;
    while (primitive) {
        if (primitive->RenderLayerMask & layer) {
            CEnginePrimitiveManager* manager = g_pEnginePrimitiveTypeInfo->PrimitiveManagers[primitive->Type];
            CEngineSubPrimitiveChain chain;
            if (manager->RenderPrimitive(*primitive, layer, chain) == 2)
                primitive->LastFrameRendered = g_engineFrameNumber;
            primitive = primitive->NextPrimitive;
        }
        else
            primitive = primitive->NextLayerMask;
    }
}