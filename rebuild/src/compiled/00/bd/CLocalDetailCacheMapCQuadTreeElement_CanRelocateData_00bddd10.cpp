// CLocalDetailCacheMap::CQuadTreeElement::CanRelocateData(long blockType) const -- the element's memory may be moved unless its cache map is locked against relocation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDDD10, length 17 bytes.
// Leaf: CanRelocateData
// No callees. Name: vtable 0x012A7DF0 slot 2; FableWin.pdb gives the element's CMovableResource overrides
// ?RelocateData@CQuadTreeElement@CLocalDetailCacheMap@@UAEXJPAX0J@Z and
// ?CanRelocateData@CQuadTreeElement@CLocalDetailCacheMap@@UBE_NJ@Z (const, one long argument: `ret 4`, unused).
// Layout: Ego_r.pdb CQuadTreeElement (as the landed OnPostRelocation 0x00BDDCB0): Map (CLocalDetailCacheMap*)
// +0x28; the map's first member is bool LockedFromMemoryRelocation (engine/CLocalDetailCacheMap.h).
// Shape: `return !Map->LockedFromMemoryRelocation;` -- the byte loaded into dl, `xor ecx,ecx ; test ; sete cl`,
// copied to al.
#include "engine/CLocalDetailCacheMap.h"

struct CObjectCacheGroupCollection;

struct CQuadTreeElement {                          // CLocalDetailCacheMap::CQuadTreeElement (76 bytes)
    virtual ~CQuadTreeElement();                   // slot 0
    CObjectCacheGroupCollection* ObjectCacheGroupCollections;   // +0x04
    CQuadTreeElement*     Children[4];             // +0x08
    unsigned char         BoundingSphere[0x10];    // +0x18 C3DBoundingSphere
    CLocalDetailCacheMap* Map;                     // +0x28
    bool CanRelocateData(long blockType) const;
};

bool CQuadTreeElement::CanRelocateData(long blockType) const
{
    return !Map->LockedFromMemoryRelocation;
}