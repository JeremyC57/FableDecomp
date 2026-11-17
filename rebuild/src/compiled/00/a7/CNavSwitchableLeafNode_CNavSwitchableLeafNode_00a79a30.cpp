// CNavSwitchableLeafNode::CNavSwitchableLeafNode(const C2DVector& position, unsigned char quadTreeLevel, unsigned char navLayer, CAFile& file, const list<CNavRegion>& regions, bool readPreferability) -- load a switchable leaf node: the navigable leaf part, then the list of 64-bit unique ids of the switches that open it (all read as closed), blocked by default, no region, the id map sorted
// Leaf: ??0CNavSwitchableLeafNode@@QAE@ABVC2DVector@@EEAAVCAFile@@ABV?$list@VCNavRegion@@V?$allocator@VCNavRegion@@@std@@@std@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A79A30, 245 bytes (+ int3 padding), `ret 18h`.
// Identity: Ego_r.pdb ??0CNavSwitchableLeafNode@@QAE@ABVC2DVector@@EEAAVCAFile@@ABV?$list@VCNavRegion@@..@std@@_N@Z (engine/CNavSwitchableLeafNode.h
//   FABLE_RETAIL_CNAVSWITCHABLELEAFNODE); vtable 0x0129DE04.
// Port (NF68, round 7): the local C2DVector / CAFile / CNavNavigableLeafNode / CRegionList / SUniqueIDOpen / CKeyPairCompareLess /
//   CUniqueIDVector / CUniqueIDMap models and the made-up GrowVectorStorage_s7 stand-in are replaced by the shared headers on the macro
//   block of the landed CNavQuadTree::LoadFromFile 0x00A7AF60 (which constructs this class): the base ctor 0x00A772D0 now takes the PDB
//   const std::list<CNavRegion>&, the ids go through CVectorMap::Insert(key, value) (push_back -> vector<pair<unsigned __int64, bool> >
//   ::_M_insert_overflow 0x00A82380) and Update (std::sort, __fastcall, 0x00A84160).
// Callees: CNavNavigableLeafNode(file ctor) 0x00A772D0 (out of line), CAFile slot Read(void*, long, CASuspendableProcess*), the id
//   vector's _M_insert_overflow 0x00A82380, std::sort 0x00A84160 (first, last, comparator by value).
// Layout: Ego_r.pdb CNavSwitchableLeafNode: UniqueIDsOpen +0x20 (CVectorMap<unsigned __int64, bool>: STLport vector of 16-byte pairs,
//   comparator byte +0x2c, Dirty +0x2d); the leaf base's Blocked +0x0f and PRegion +0x10.
// Shape: the count and the loop index are homed in the dead `regions` / `readPreferability` slots; each id is added through the vector
//   map's inline Insert (push_back, then Dirty), the map is sorted once through the inline Update().
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CANAVQUADTREENODE
#define FABLE_RETAIL_CNAVNAVIGABLELEAFNODE
#define FABLE_RETAIL_CNAVSWITCHABLELEAFNODE
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
#include "engine/CAFile.h"
#include "engine/CNavSwitchableLeafNode.h"

CNavSwitchableLeafNode::CNavSwitchableLeafNode(const C2DVector& position, unsigned char quadTreeLevel, unsigned char navLayer, CAFile& file,
                                               const std::list<CNavRegion>& regions, bool readPreferability)
    : CNavNavigableLeafNode(position, quadTreeLevel, navLayer, file, regions, readPreferability)
{
    Blocked = true;
    unsigned long count;
    file.Read(&count, 4, 0);
    for (unsigned long i = 0; i < count; ++i) {
        unsigned __int64 id;
        file.Read(&id, 8, 0);
        UniqueIDsOpen.Insert(id, false);
    }
    PRegion = 0;
    UniqueIDsOpen.Update();
}
