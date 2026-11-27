#ifndef FABLE_ENGINE_CNAVNAVIGABLELEAFNODE_H
#define FABLE_ENGINE_CNAVNAVIGABLELEAFNODE_H
#ifdef FABLE_RETAIL_CNAVNAVIGABLELEAFNODE
// Retail opt-in (chunk 2054, row 0x00A7AF60 CNavQuadTree::LoadFromFile): RTTI .?AVCNavNavigableLeafNode@@ : CANavQuadTreeNode, vtable
// 0x0129DDA4, sizeof 0x20 (`push 20h ; call operator new` at 0x00A7B653). Members: Ego_r.pdb PRegion +0x10, Neighbours +0x14
// (CSmallVector<const CANavQuadTreeNode*, 8>: Start +0, Size +4 schar), PreferabilityCost +0x1C. Ctor: Ego_r.pdb
// ??0CNavNavigableLeafNode@@QAE@ABVC2DVector@@EEAAVCAFile@@ABV?$list@VCNavRegion@@V?$allocator@VCNavRegion@@@std@@@std@@_N@Z (0x00A772D0).
#ifndef FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CANAVQUADTREENODE
#endif
#include "engine/CANavQuadTreeNode.h"
template <class T, int N> class CSmallVector {
#ifdef FABLE_RETAIL_FLYER_RANDOM_PASS29
public:
    long size() const { return Size; }
    const T& operator[](unsigned long index) const { return Start[index]; }
#endif
                                    // Ego_r.pdb CSmallVector<T,N>: Start +0, Size +4
#ifdef FABLE_RETAIL_CSMALLVECTOR_ITERATORS_PASS23
public:
 typedef T* iterator;typedef const T* const_iterator;
 CSmallVector();~CSmallVector();
 iterator begin(){return Start;}const_iterator begin() const{return Start;}
 iterator end(){return Start+Size;}const_iterator end() const{return Start+Size;}
protected:
#else
public:
#ifdef FABLE_RETAIL_CNAVNAVIGABLELEAFNODE_GENERATOR_CTOR
    // NF76 opt-in (row 0x00A79930): the inline default ctor (no out-of-line copy in Ego_r.pdb; retail clears Start and the Size byte
    // in place, `mov [esi+14h],ebp ; mov byte ptr [esi+18h],0` at 0x00A79972, `mov [ebx],ebp ; mov byte ptr [ebx+4],0` at 0x00A77322).
    CSmallVector() : Start(0), Size(0) { }
#endif
#endif
#ifdef FABLE_RETAIL_CSMALLVECTOR_RESIZE
    // Re-port opt-in (2026-11-19, rows 0x00A78A90 / 0x00A780B0 CNavConnectorNode ctor / dtor): Ego_r.pdb
    // ?resize@?$CSmallVector@PBVCANavQuadTreeNode@@$07@@QAEXJABQBVCANavQuadTreeNode@@@Z (0x00A7DB10, out of line) and FableWin.pdb's
    // ?push_back@..@@QAEXABQBVCANavQuadTreeNode@@@Z / ?pop_back@..@@QAEXXZ / ?begin@ / ?end@..@@QAEPAPBVCANavQuadTreeNode@@XZ (debug only:
    // inline in release). Retail expands push_back as resize(Size + 1) with the default fill stored per call, then the element store
    // through the re-read size (0x00A78AA5..0x00A78AC5), and pop_back as resize(Size - 1) (the call at 0x00A78160). Inferred: the default argument
    // `fill = T()` (the landed rows' spelling; the zero temporary is built at every call site). Not with FABLE_RETAIL_CSMALLVECTOR_ITERATORS_PASS23.
    void resize(long size, const T& fill = T());                         // 0x00A7DB10
    void push_back(const T& value) { resize(Size + 1); Start[Size - 1] = value; }
    void pop_back() { resize(Size - 1); }
    T* begin() { return Start; }
    T* end() { return Start + Size; }
#endif
#ifdef FABLE_RETAIL_CSMALLVECTOR_PUSHBACK_OUTOFLINE
    // NF161 opt-in (q4782, row 0x00A77480 CNavQuadTree::JoinNavigationLayers): that TU calls push_back out of line, 0x00A7DBF0
    // ?push_back@?$CSmallVector@PBVCANavQuadTreeNode@@$07@@QAEXABQBVCANavQuadTreeNode@@@Z (FableWin.pdb name set; the landed binding of
    // 0x00A77480), `lea ecx,[node+14h] ; push &value ; call 0x00A7DBF0` twice. Declaration only; not with FABLE_RETAIL_CSMALLVECTOR_RESIZE.
    void push_back(const T& value);                                      // 0x00A7DBF0
#endif
    T*          Start;
    signed char Size;
};
class CNavNavigableLeafNode : public CANavQuadTreeNode {
public:
    CNavNavigableLeafNode(const C2DVector& position, unsigned char quadTreeLevel, unsigned char navLayer, CAFile& file,
                          const std::list<CNavRegion>& regions, bool readPreferability);           // 0x00A772D0
#ifdef FABLE_RETAIL_CNAVNAVIGABLELEAFNODE_GENERATOR_CTOR
    // NF76 opt-in (row 0x00A79930): Ego_r.pdb ??0CNavNavigableLeafNode@@QAE@ABVC2DVector@@EEE@Z, inline in retail's generator
    // CNavSwitchableLeafNode ctor (PRegion, Neighbours, PreferabilityCost stores at 0x00A7996F..0x00A79979; its vtable store is dead).
    // Needs FABLE_RETAIL_CANAVQUADTREENODE_GENERATOR_CTOR. Parameter names Ego_d.pdb.
    CNavNavigableLeafNode(const C2DVector& pos, unsigned char quad_tree_level, unsigned char nav_layer, unsigned char preferability_cost)
        : CANavQuadTreeNode(pos, quad_tree_level, nav_layer), PRegion(0), PreferabilityCost(preferability_cost) { }
#endif
#ifdef FABLE_RETAIL_CNAVNAVIGABLELEAFNODE_REGION
    // Opt-in (followup, row 0x00A78800 CNavSwitchableLeafNode::SetAsBlocked): Ego_r.pdb / FableWin.pdb ?GetRegion@CNavNavigableLeafNode@@
    // UBEPBVCNavRegion@@XZ, the class-body inline of the header (Ego_d.pdb: one line, 221, in the header's file record, not the .cpp's;
    // retail out of line only through the vtable) and ?ForceRegionTo@CNavNavigableLeafNode@@IAEXPBVCNavRegion@@@Z (protected, defined in
    // lib_nav_quad_tree.cpp: Ego_d lines 412..414, parameter pregion; retail 0x00A75B60 between RemoveRegionZero 0x00A75B50 and
    // SetUpNeighbours 0x00A75B70, as FableWin.pdb orders them). Declarations and the one-line accessor only.
    virtual const CNavRegion* GetRegion() const { return PRegion; }
protected:
    void ForceRegionTo(const CNavRegion* pregion);                                 // 0x00A75B60
public:
#endif
    CNavRegion*                               PRegion;                           // +0x10
    CSmallVector<const CANavQuadTreeNode*, 8> Neighbours;                        // +0x14
    unsigned char                             PreferabilityCost;                 // +0x1c
#ifdef FABLE_RETAIL_CNAVNAVIGABLELEAFNODE_RTTI_DECLS
    // Opt-in (NF319, 2026-11-23): declarations only, no layout change. Each member below is the one the retail RTTI vftable placement
    // (ghidra_out/labels_rtti_port.tsv) names at its address, in the PDB spelling (access, const, types); each overrides a virtual
    // its base CANavQuadTreeNode chain declares at the same slot (no new slot).
    // 0x00A75A90 vtable 0x0129DDA4 slot 8: ?GetPreferabilityCost@CNavNavigableLeafNode@@UBEEXZ (Ego_r.pdb + FableWin.pdb)
public:
    virtual unsigned char GetPreferabilityCost() const;               // 0x00A75A90
public:
#endif
#ifdef FABLE_RETAIL_CNAVNAVIGABLELEAFNODE_REMOVEREGIONZERO_DECL
    // Opt-in (NF321, 2026-11-23, row 0x00A75B50): Ego_r.pdb / FableWin.pdb ?RemoveRegionZero@CNavNavigableLeafNode@@MAE_NXZ, the override
    // the retail RTTI vftable placement names at vtable 0x0129DDA4 slot 14 (CANavQuadTreeNode slot 14). Declaration only.
protected:
    virtual bool RemoveRegionZero();                                  // 0x00A75B50
public:
#endif
};
FABLE_STATIC_ASSERT_AT(25, sizeof(CNavNavigableLeafNode) == 0x20);
#else // flattened Ego_r layout (the landed default, unchanged)
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CNavNavigableLeafNode = 0x20.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CNavRegion;

#pragma pack(push, 1)
class CNavNavigableLeafNode {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _pad_0x04[0x4];                            // +0x04
    unsigned char _pad_0x08[0x4];                            // +0x08
    unsigned char _pad_0x0c[0x4];                            // +0x0c
    CNavRegion*   PRegion;                                   // +0x10
    unsigned char Neighbours[0x8];                           // +0x14 CSmallVector<CANavQuadTreeNode_const_*,8>
    unsigned char PreferabilityCost;                         // +0x1c
    unsigned char _pad_0x1d[0x3];                            // +0x1d
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(51, sizeof(CNavNavigableLeafNode) == 0x20);
FABLE_STATIC_ASSERT_AT(52, offsetof(CNavNavigableLeafNode, __vftable) == 0x0);
FABLE_STATIC_ASSERT_AT(53, offsetof(CNavNavigableLeafNode, PRegion) == 0x10);
FABLE_STATIC_ASSERT_AT(54, offsetof(CNavNavigableLeafNode, Neighbours) == 0x14);
FABLE_STATIC_ASSERT_AT(55, offsetof(CNavNavigableLeafNode, PreferabilityCost) == 0x1c);

#endif // retail opt-in
#endif // FABLE_ENGINE_CNAVNAVIGABLELEAFNODE_H
