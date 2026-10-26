// CLocalDetailCacheMap::CQuadTreeElement::RelocateData(long blockType, void* oldBase, void* newBase, long size) -- a block owned by this quad-tree element was moved: a child element is re-based and re-registered, an object-cache group collection is re-based, relinked and has its type collections re-registered
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDE0C0, length 155 bytes (160 with int3 padding).
// Leaf: ?RelocateData@CQuadTreeElement
// Callees: CQuadTreeElement::OnPostRelocation() @ 0x00BDDCB0 (landed, on the moved child), the landscape memory
// manager's RebindBlockOwner(void* block, CMovableResource* owner) @ 0x00BD9AB0 through the engine global
// 0x01436EA8 (+0x6b0), as in the landed OnPostRelocation bodies.
// Name: vtable 0x012A7DF0 slot 1; FableWin.pdb ?RelocateData@CQuadTreeElement@CLocalDetailCacheMap@@UAEXJPAX0J@Z
// (void; long, void*, void*, long: `ret 0x10`, the first and last arguments unused).
// Layout: Ego_r.pdb CQuadTreeElement (as the landed OnPostRelocation 0x00BDDCB0): ObjectCacheGroupCollections +4
// (chained through +4), Children[4] +8; a group collection's ObjectTypeCollections at +0x20 (chained through +0xc).
// Shape: the four children are an index loop (`cmp oldBase,[eax]`, the slot re-read by index when found) whose hit
// re-bases the slot in place and calls the child's OnPostRelocation; then the sibling-style walk of the group
// chain with a trailing `prev`, the in-place re-base, the relink, and the group's inline OnPostRelocation
// (manager read into ebx after the chain's null test is scheduled, a bottom-tested loop over the type
// collections passing the group as owner). The in-place integer re-base helper is the one of 0x00BDDDE0/0x00BDDEE0.
// CMovableResource (engine/CMovableResource.h: the vptr word only; its virtuals are slot 0 the destructor, slot 1
// RelocateData, slot 2 CanRelocateData) is the base of the element and of the group collections.
#include "engine/CMovableResource.h"

class CEngineLandscapeMemoryManager { public:
    void RebindBlockOwner(void* block, CMovableResource* owner);   // 0x00BD9AB0
};
struct SEngine_01436ea8 {
    unsigned char                  _pad[0x6b0];
    CEngineLandscapeMemoryManager* m_pLandscapeMemory;   // +0x6b0
};
extern SEngine_01436ea8* g_pEngine_01436ea8;      // 0x01436EA8

template <class T> inline void RelocatePointer(T*& p, void* oldBase, void* newBase)
{
    if (p)
        p = (T*)((long)p + ((long)newBase - (long)oldBase));
}

struct CObjectTypeCollection {                    // CLocalDetailCacheMap::CObjectTypeCollection (retail view)
    void*                  __vftable;             // +0x00
    void*                  m_pType;               // +0x04
    void*                  m_pFirstPrimitive;     // +0x08
    CObjectTypeCollection* Next;                  // +0x0c
};

struct CObjectCacheGroupCollection : CMovableResource {   // CLocalDetailCacheMap::CObjectCacheGroupCollection
    CObjectCacheGroupCollection* Next;                  // +0x04
    unsigned char                BoundingSphere[0x10];  // +0x08 C3DBoundingSphere
    float                        MaxFadeDistance;       // +0x18
    unsigned long                PrimitiveTypeMask;     // +0x1c
    CObjectTypeCollection*       ObjectTypeCollections; // +0x20
    void OnPostRelocation()
    {
        CEngineLandscapeMemoryManager* manager = g_pEngine_01436ea8->m_pLandscapeMemory;
        for (CObjectTypeCollection* collection = ObjectTypeCollections; collection; collection = collection->Next)
            manager->RebindBlockOwner(collection, this);
    }
};

struct CQuadTreeElement : CMovableResource {      // CLocalDetailCacheMap::CQuadTreeElement (76 bytes)
    CObjectCacheGroupCollection* ObjectCacheGroupCollections;   // +0x04
    CQuadTreeElement*            Children[4];                   // +0x08
    void OnPostRelocation();                                    // 0x00BDDCB0
    void RelocateData(long blockType, void* oldBase, void* newBase, long size);
};

void CQuadTreeElement::RelocateData(long blockType, void* oldBase, void* newBase, long size)
{
    for (long i = 0; i < 4; ++i) {
        if (oldBase == Children[i]) {
            RelocatePointer(Children[i], oldBase, newBase);
            Children[i]->OnPostRelocation();
            return;
        }
    }
    CObjectCacheGroupCollection* prev = 0;
    for (CObjectCacheGroupCollection* p = ObjectCacheGroupCollections; p; p = p->Next) {
        if (p == oldBase) {
            RelocatePointer(p, oldBase, newBase);
            if (prev)
                prev->Next = p;
            else
                ObjectCacheGroupCollections = p;
            p->OnPostRelocation();
            return;
        }
        prev = p;
    }
}