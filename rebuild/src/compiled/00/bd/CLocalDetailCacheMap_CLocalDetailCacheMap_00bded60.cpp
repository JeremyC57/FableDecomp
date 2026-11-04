// CLocalDetailCacheMap::~CLocalDetailCacheMap() -- free the collection-type palette's entry array and the area update list, then destroy the QuadTree root element
// Leaf: ??1CLocalDetailCacheMap@@QAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDED60, 44 bytes, thiscall, tail `jmp ~CQuadTreeElement` on this+0xC.
// Identity (proven): the three callers are the landed CEngineLocalDetailGenerator::DeleteMap 0x00B59840 and
//   ~CEngineLocalDetailGenerator 0x00B59BE0 (both `delete map->LocalDetailCacheMap`, binding ??1CLocalDetailCacheMap@@QAE@XZ)
//   and the deleting destructor 0x00B59D10; non-polymorphic (no vtable store, no base call). Ego_r.pdb (0x00A92180, after the
//   ctor 0x00A920F0) and FableWin.pdb (0x02E3AE50) spell ??1CLocalDetailCacheMap@@QAE@XZ. The catalog label
//   "~CPatchTesselationEdgeStrip" is propagated.
// Callees: free 0x00BFEA14 (STLport allocator deallocate, twice), CLocalDetailCacheMap::CQuadTreeElement::~CQuadTreeElement
//   0x00BDDFB0 (stores the nested class's vtable 0x012A7DF0, RTTI .?AVCQuadTreeElement@CLocalDetailCacheMap@@; Ego_r 0x00A8F770 /
//   FableWin 0x02E384B0 ??1CQuadTreeElement@CLocalDetailCacheMap@@UAE@XZ).
// Shape: an empty user destructor; members in reverse order: CollectionTypePalette (+0x64; its Entries CArray, the STLport
//   vector's _Vector_base deallocate = `if (start) free(start)`; trivially destructible pointer elements), AreaUpdateList
//   (+0x58, vector<C2DBoxI>), then the QuadTree element's out-of-line virtual dtor called directly as the tail jump.
// Layout: Ego_r.pdb CLocalDetailCacheMap with retail container sizes (engine/CLocalDetailCacheMap.h, 0x70): LockedFromMemoryRelocation
//   +0, FileBlockManager +4, Map +8, QuadTree (CQuadTreeElement, 0x4C) +0xC, AreaUpdateList +0x58, CollectionTypePalette +0x64.
//   The engine header is a flattened record with opaque members (no element types, no nested classes), so the class is a local
//   model with the header's offsets (header debt); STLport vector as in the landed caller 0x00B59BE0.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

#include "engine/CArray.h"


class CEngineMap;
class CEngineStaticMapFileBlockManager;
class CLocalDetailObjectCollectionType;

class C2DBoxI {
public:
    long TLX, TLY, BRX, BRY;
};

class CLocalDetailCacheMap {                                // 0x70 bytes (retail)
public:
    class CQuadTreeElement {                                // vtable 0x012A7DF0, 0x4C bytes
    public:
        CQuadTreeElement(CQuadTreeElement* parent, long x, long y, long size, long level);
        virtual ~CQuadTreeElement();                        // 0x00BDDFB0
        unsigned char _body[0x4c - 0x4];
    };
    class CObjectTypeCollectionPalette {
    public:
        CArray<const CLocalDetailObjectCollectionType*> Entries;   // +0x00
    };
    CLocalDetailCacheMap(CEngineMap* map);
    ~CLocalDetailCacheMap();
    bool LockedFromMemoryRelocation;                        // +0x00
    CEngineStaticMapFileBlockManager* FileBlockManager;     // +0x04
    CEngineMap* Map;                                        // +0x08
    CQuadTreeElement QuadTree;                              // +0x0C
    std::vector<C2DBoxI> AreaUpdateList;                    // +0x58
    CObjectTypeCollectionPalette CollectionTypePalette;     // +0x64
};

CLocalDetailCacheMap::~CLocalDetailCacheMap()
{
}