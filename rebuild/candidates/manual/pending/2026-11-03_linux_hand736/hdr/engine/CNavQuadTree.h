#ifndef FABLE_ENGINE_CNAVQUADTREE_H
#define FABLE_ENGINE_CNAVQUADTREE_H
#ifdef FABLE_RETAIL_CNAVQUADTREE
// Retail opt-in (2026-11-03, chunk 736 rows 0x00A78E20 IsAreaClear / 0x00A7A0A0 SetUpRegions): the polymorphic retail class.
// RTTI .?AVCNavQuadTree@@ : CBaseClassNonCopyable, vtable 0x0129DE64; sizeof 0x88 (ctor 0x00A7A760, `new(0x88)` at 0x00A15748).
// Members: Ego_r.pdb names in PDB order at STLport sizes. MapPos sits at +7 because C2DVector is byte-packed in retail
// (FABLE_RETAIL_C2DVECTOR; IsAreaClear reads `fsub [edi+7]` / `[edi+0Bh]`, MapWidth +0x10). NavigationLayers elements are
// 0x28 bytes (`lea ecx,[eax+eax*4] ; [edx+ecx*8+0Ch]`): Nodes +0, LineLists +0xC, SwitchableLineLists +0x18 (16-byte entries,
// list at +8). Methods: Ego_r.pdb spellings (IsAreaClear QBE, IsAreaBlockedByLines ABE, SetUpRegions AAE, GetNodeAt QBE,
// LoadFromFile QAE); the static BlockedNode is ?BlockedNode@CNavQuadTree@@2VCNavBlockedQuadTreeNode@@A (Ego_r dynamic
// initializer; 0x013D2D2C).
#ifndef FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#endif
#ifndef FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF
#endif
#ifndef FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CANAVQUADTREENODE
#endif
#include <stddef.h>
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/C2DLineF.h"
#include "engine/CArray.h"
#include "engine/CVectorMap.h"
#include "engine/CCountedPointer.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CANavQuadTreeNode.h"
#include "engine/CNavRegion.h"
#include "engine/CNavigationPosition.h"
class CNavBlockedQuadTreeNode;
class CNavConnectorNode;
class CAFile;
class C2DBoxI;
class CASuspendableProcess;
class CNavigationTransitionPoint {                                       // Ego_r.pdb: Position +0, Layer1 +8, Layer2 +0xC
public:
    C2DVector Position;
    long      Layer1;
    long      Layer2;
};
class CNavQuadTree : public CBaseClassNonCopyable {
public:
    class CNavigationLayer {                                             // Ego_r.pdb CNavQuadTree::CNavigationLayer, 0x28 in retail
    public:
        CArray<CANavQuadTreeNode*> Nodes;                                                    // +0x00
        std::vector<std::list<C2DLineF> > LineLists;                                         // +0x0c
        CVectorMap<unsigned __int64, std::list<C2DLineF> > SwitchableLineLists;              // +0x18
    };
    bool          Initialised;                                           // +0x04
    bool          Activated;                                             // +0x05
    bool          Initialising;                                          // +0x06
    C2DVector     MapPos;                                                // +0x07
    float         MapWidth;                                              // +0x10
    float         MapHeight;                                             // +0x14
    long          MapIndex;                                              // +0x18
    std::vector<CNavigationLayer> NavigationLayers;                      // +0x1c
    std::vector<CNavigationTransitionPoint> TransitionPoints;            // +0x28
    std::vector<unsigned char> TopologyWeights;                          // +0x34
    CVectorMap<unsigned __int64, CANavQuadTreeNode*> SwitchableNodes;    // +0x40
    std::vector<CNavigationPosition> ActionPoints;                       // +0x50
    std::vector<C2DBoxF> DetailedAreas;                                  // +0x5c
    std::vector<CNavigationPosition> RegionSeeds;                        // +0x68
    std::list<CNavRegion> Regions;                                       // +0x74
    CVectorMap<long, CCountedPointer<CNavConnectorNode> > ConnectorNodes;   // +0x78
    static CNavBlockedQuadTreeNode BlockedNode;                          // 0x013D2D2C
    virtual ~CNavQuadTree();                                             // slot 0
    const CANavQuadTreeNode* GetNodeAt(const C2DVector& pos, long layer) const;          // 0x00A770B0
    bool IsAreaClear(const CANavQuadTreeNode* node, bool& switchable, std::vector<unsigned __int64>& ids) const;  // 0x00A78E20
    bool LoadFromFile(CAFile& file, long mapIndex, const C2DBoxI& area, CASuspendableProcess& process);           // 0x00A7AF60
private:
    bool IsAreaBlockedByLines(const C2DBoxF& area, const std::vector<const std::list<C2DLineF>*>& lineLists) const;  // 0x00A76F30
    void SetUpRegions(long mapIndex);                                    // 0x00A7A0A0
};
FABLE_STATIC_ASSERT(sizeof(CNavQuadTree) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, MapPos) == 0x7);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, MapWidth) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, NavigationLayers) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, TopologyWeights) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, RegionSeeds) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, Regions) == 0x74);
FABLE_STATIC_ASSERT(sizeof(CNavQuadTree::CNavigationLayer) == 0x28);
#else // FABLE_RETAIL_CNAVQUADTREE: the generated flattened layout (the landed default, unchanged)
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CNavQuadTree = 0xb0.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CNavQuadTree = 0x88.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

#pragma pack(push, 1)
class CNavQuadTree {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    bool          Initialised;                               // +0x04
    bool          Activated;                                 // +0x05
    bool          Initialising;                              // +0x06
    unsigned char MapPos[0x9];                               // +0x07 C2DVector
    float         MapWidth;                                  // +0x10
    float         MapHeight;                                 // +0x14
    long          MapIndex;                                  // +0x18
    unsigned char NavigationLayers[0xc];                     // +0x1c vector<CNavQuadTree::CNavigationLayer,std::allocator<CNavQuadTree::CNavigationLayer>_>
    unsigned char TransitionPoints[0xc];                     // +0x28 vector<CNavigationTransitionPoint,std::allocator<CNavigationTransitionPoint>_>
    unsigned char TopologyWeights[0xc];                      // +0x34 vector<unsigned_char,std::allocator<unsigned_char>_>
    unsigned char SwitchableNodes[0x10];                     // +0x40 CVectorMap<unsigned___int64,CANavQuadTreeNode_*,CKeyPairCompareLess<unsigned___int64,CANavQuadTreeNode_*>_>
    unsigned char ActionPoints[0xc];                         // +0x50 vector<CNavigationPosition,std::allocator<CNavigationPosition>_>
    unsigned char DetailedAreas[0xc];                        // +0x5c vector<C2DBoxF,std::allocator<C2DBoxF>_>
    unsigned char RegionSeeds[0xc];                          // +0x68 vector<CNavigationPosition,std::allocator<CNavigationPosition>_>
    unsigned char Regions[0x4];                              // +0x74 list<CNavRegion,std::allocator<CNavRegion>_>
    unsigned char ConnectorNodes[0x10];                      // +0x78 CVectorMap<long,CCountedPointer<CNavConnectorNode>,CKeyPairCompareLess<long,CCountedPointer<CNavConnectorNode>_>_>
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CNavQuadTree) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, Initialised) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, Activated) == 0x5);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, Initialising) == 0x6);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, MapPos) == 0x7);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, MapWidth) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, MapHeight) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, MapIndex) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, NavigationLayers) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, TransitionPoints) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, TopologyWeights) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, SwitchableNodes) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, ActionPoints) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, DetailedAreas) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, RegionSeeds) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, Regions) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CNavQuadTree, ConnectorNodes) == 0x78);

#endif // FABLE_RETAIL_CNAVQUADTREE
#endif // FABLE_ENGINE_CNAVQUADTREE_H
