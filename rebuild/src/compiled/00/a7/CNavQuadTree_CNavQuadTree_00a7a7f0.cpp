// CNavQuadTree::~CNavQuadTree() -- free the quad-tree nodes, then tear down the navigation containers in reverse declaration order and the CBaseClassNonCopyable base
// Leaf: ??1CNavQuadTree@@UAE@XZ
// Naming (naming wave 24): ~CBaseClassNonCopyable 0x0099A300 is protected virtual (Ego_r.pdb / FableWin.pdb ??1CBaseClassNonCopyable@@MAE@XZ, engine/CBaseClassNonCopyable.h).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A7A7F0, 212 bytes (+ int3 padding). Vtable 0x0129DE64 (CNavQuadTree, RTTI); called
//   only from the scalar deleting destructor 0x00A85E20 (slot 0). The catalogued name "~CPatchTesselationEdgeStrip" is a
//   propagated label.
// Callees: CNavQuadTree::ClearNodes 0x00A792E0, _Destroy_range for the connector-node pairs 0x00A80E40 and for the
//   navigation layers 0x00A85930 (both `__fastcall` first, last, plus an empty STLport `__false_type` tag by reference),
//   CRetailList_ListOfCountedPointer::clear 0x00A84110 (the region list's clear, landed under that name), free 0x00BFEA14,
//   CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300.
// Shape: the AI navigation library's containers allocate with malloc/free (retail STLport-style 12-byte vectors, a
//   4-byte list holding its head node, CVectorMap = vector + 4-byte comparator). Retail layout (the PDB/header layout
//   is the debug one with 16-byte vectors, so this is a local model): NavigationLayers +0x1C, TransitionPoints +0x28,
//   TopologyWeights +0x34, SwitchableNodes +0x40, ActionPoints +0x50, DetailedAreas +0x5C, RegionSeeds +0x68,
//   Regions +0x74, ConnectorNodes +0x78. The user destructor stores the vtable and calls ClearNodes; each member's
//   inline destructor runs (destroy range with the tag temporary in the `push ecx` frame byte, null-tested free of the
//   start; the list clears and frees its head node, re-read through edi).
// NF61 (round 7, q10583): moved onto the shared header engine/CNavQuadTree.h (FABLE_RETAIL_CNAVQUADTREE: the retail-layout polymorphic
//   class with its STLport members; NF61 opt-in FABLE_RETAIL_CNAVQUADTREE_CLEARNODES declares the private ClearNodes) on
//   stlport/fable_containers.h: FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL (STLport's inline __destroy_aux in the /Gr convention: VC7.1
//   drops the trivial members' loops and calls the two non-trivial ranges 0x00A85930 / 0x00A80E40 as the COMDATs the landed
//   0x00A85EC0 / 0x00A838F0 bind) and FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE (the region
//   list's _List_base<CNavRegion>::clear 0x00A84110). The local CNavQuadTree / vector / list / CVectorMap models are gone; the member
//   destructors are the implicit ones.
#define FABLE_RETAIL_CNAVQUADTREE
#define FABLE_RETAIL_CNAVQUADTREE_CLEARNODES
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#include "engine/CNavQuadTree.h"

CNavQuadTree::~CNavQuadTree()
{
    ClearNodes();
}
