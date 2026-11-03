#ifndef FABLE_ENGINE_CANAVQUADTREENODE_H
#define FABLE_ENGINE_CANAVQUADTREENODE_H
#ifdef FABLE_RETAIL_CANAVQUADTREENODE
#ifndef FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF
#endif
// Retail opt-in (2026-11-03, chunk 721 row 0x00A1CEB0 CNavigatorAStar::Initialise; extended by chunk 736): the polymorphic retail class. RTTI
// .?AVCANavQuadTreeNode@@ vtable 0x0129CBBC. Slots as the landed CNavNavigableLeafNode rows (0x00A76490, 0x00A76B90): 0 dtor,
// 1/2 GetNeighbours (overloads in reverse order), 3 HasNeighbours, 4 IsNeighbourOf, 5 GetRegionID (+0x14), 6 GetNodeType
// (+0x18; Ego_r ?GetNodeType@...@@UBE?AW4ENavQuadTreeNodeType@@XZ), 7 GetRegion, 8 GetPreferabilityCost. Members: Ego_r names
// and offsets (Position +4, QuadTreeLevel +0xc, NavLayer +0xd, LeafNode +0xe, Blocked +0xf), sizeof 0x10.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "stlport/fable_containers.h"
class CNavRegion;
class CAFile;
template <class T, int N> class CSmallVector;
// chunk 736: the per-level node side table GetSize reads (float[] @ 0x0129DCAC: 32, 16, 8, 4, 2, 1, 0.5); no PDB data symbol
// (descriptive; the spelling of the landed 0x00A76640 / 0x00A76CC0 rows).
extern const float g_QuadTreeLevelSizes[];
enum ENavQuadTreeNodeType {                          // Ego_r enum; enumerator names descriptive (values from the landed rows)
    NAV_NODE_QUADTREE = 0, NAV_NODE_NAVIGABLE = 1, NAV_NODE_BLOCKED = 2, NAV_NODE_SWITCHABLE = 3
};
class CANavQuadTreeNode {
public:
    virtual ~CANavQuadTreeNode();                                                        // slot 0
    virtual const CSmallVector<const CANavQuadTreeNode*, 8>* GetNeighbours() const;      // slot 2
    virtual CSmallVector<const CANavQuadTreeNode*, 8>* GetNeighbours();                  // slot 1
    virtual bool HasNeighbours() const;                                                  // slot 3
    virtual bool IsNeighbourOf(const CANavQuadTreeNode*) const;                          // slot 4
    virtual long GetRegionID() const;                                                    // slot 5 (+0x14)
    virtual ENavQuadTreeNodeType GetNodeType() const;                                    // slot 6 (+0x18)
    virtual const CNavRegion* GetRegion() const;                                         // slot 7
    virtual unsigned char GetPreferabilityCost() const;                                  // slot 8
    // chunk 736 (rows 0x00A78E20 / 0x00A7A0A0): slots 9..22 of 0x0129CBBC / 0x0129DCD4 in PDB order (Ego_r CNavQuadTreeNode
    // MakeRegion 0xBBBCD0, AddNavigable... 0xBBBD30, RemoveRegionZero 0xBBBD90 = retail 0x00A75FC0 / 0x00A76010 / 0x00A76060,
    // slots 12 / 13 / 14: SetUpRegions calls [vtbl+30h] / [vtbl+34h] / [vtbl+38h]). Ego_r spells the protected ones `M`.
    virtual bool IsSwitchable() const;                                                   // slot 9
    virtual bool IsSwitchableOn(unsigned __int64) const;                                 // slot 10
protected:
    virtual const CANavQuadTreeNode* GetLeafNodeAt(const C2DVector&) const;              // slot 11
public:
    virtual bool MakeRegion(const CNavRegion*, std::list<CANavQuadTreeNode*>&);          // slot 12 (+0x30)
    virtual bool AddNavigableNodeNeighbouringSwitchableNodeToRegion(const CNavRegion*, std::list<CANavQuadTreeNode*>&);  // slot 13
    virtual bool RemoveRegionZero();                                                     // slot 14 (+0x38)
    virtual unsigned long GetMemoryUsage(unsigned long&, unsigned long&, unsigned long&, unsigned long&) const;   // slot 15
    // slots 16..22 (SetUpNeighboursWithLeaf, SetUpNeighbours, CalculateChildrenNeighbours, RemoveNeighbours, AddToIndexTable,
    // SaveToFile, ConvertIndexesToPointers) are not called through this model; not declared.
    // Ego_r ?GetSize@CANavQuadTreeNode@@QBEMXZ (out of line in Ego_r, inline in retail) and ?GetAreaCovering@CANavQuadTreeNode@@
    // QBE?AVC2DBoxF@@XZ (retail 0x00A763B0; CNavQuadTree::IsAreaClear 0x00A78E20 expands it: `fld size[level] ; fmul 0.5 ; fst`).
    float GetSize() const { return g_QuadTreeLevelSizes[QuadTreeLevel]; }
    C2DBoxF GetAreaCovering() const
    {
        float half = GetSize() * 0.5f;
        return C2DBoxF(Position.X - half, Position.Y - half, half + Position.X, half + Position.Y);
    }
    bool operator==(const CANavQuadTreeNode&) const;                                     // 0x00A763F0 ??8CANavQuadTreeNode@@QBE_NABV0@@Z
    const C2DVector& GetPosition() const { return Position; }                            // Ego_r ?GetPosition@CANavQuadTreeNode@@QBEABVC2DVector@@XZ
    C2DVector     Position;                                                              // +0x04
    unsigned char QuadTreeLevel;                                                         // +0x0c
    unsigned char NavLayer;                                                              // +0x0d
    bool          LeafNode;                                                              // +0x0e
    bool          Blocked;                                                               // +0x0f
};
FABLE_STATIC_ASSERT(sizeof(CANavQuadTreeNode) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CANavQuadTreeNode, NavLayer) == 0xd);
#else // flattened Ego_r layout (the landed default, unchanged)
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CANavQuadTreeNode = 0x10.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

#pragma pack(push, 1)
class CANavQuadTreeNode {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    float         Position_X;                                // +0x04 C2DVector::X
    float         Position_Y;                                // +0x08 C2DVector::Y
    unsigned char QuadTreeLevel;                             // +0x0c
    unsigned char NavLayer;                                  // +0x0d
    bool          LeafNode;                                  // +0x0e
    bool          Blocked;                                   // +0x0f
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CANavQuadTreeNode) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CANavQuadTreeNode, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CANavQuadTreeNode, Position_X) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CANavQuadTreeNode, Position_Y) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CANavQuadTreeNode, QuadTreeLevel) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CANavQuadTreeNode, NavLayer) == 0xd);
FABLE_STATIC_ASSERT(offsetof(CANavQuadTreeNode, LeafNode) == 0xe);
FABLE_STATIC_ASSERT(offsetof(CANavQuadTreeNode, Blocked) == 0xf);

#endif // FABLE_RETAIL_CANAVQUADTREENODE
#endif // FABLE_ENGINE_CANAVQUADTREENODE_H
