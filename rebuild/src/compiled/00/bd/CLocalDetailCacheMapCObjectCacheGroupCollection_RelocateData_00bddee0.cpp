// CLocalDetailCacheMap::CObjectCacheGroupCollection::RelocateData(long blockType, void* oldBase, void* newBase, long size) -- one of this group's object-type collections was moved: find it, re-base the pointer, let it re-register its primitives and relink it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDDEE0, length 78 bytes (80 with int3 padding).
// Leaf: RelocateData
// Callees: CLocalDetailCacheMap::CObjectTypeCollection::OnPostRelocation() @ 0x00BDD8E0 (thiscall, non-virtual:
// re-binds every primitive's block owner with the landscape memory manager 0x00BD9AB0).
// Name: vtable 0x012A7DDC slot 1; FableWin.pdb ?RelocateData@CObjectCacheGroupCollection@CLocalDetailCacheMap@@UAEXJPAX0J@Z
// (void; long, void*, void*, long: `ret 0x10`, the first and last arguments unused); OnPostRelocation is QAEXXZ.
// Layout: Ego_r.pdb CObjectCacheGroupCollection: ObjectTypeCollections (CObjectTypeCollection*) +0x20; the type
// collections are chained through +0xc (Next), as walked by the landed RenderUpdate caller 0x00BDD930.
// Shape: the sibling of 0x00BDDDE0 (same walk with a trailing `prev`, the in-place integer re-base under its own
// null test, the relink through `prev` or the head) with a direct call instead of the virtual fix-up.
template <class T> inline void RelocatePointer(T*& p, void* oldBase, void* newBase)
{
    if (p)
        p = (T*)((long)p + ((long)newBase - (long)oldBase));
}

struct CObjectTypeCollection {                              // CLocalDetailCacheMap::CObjectTypeCollection
    virtual ~CObjectTypeCollection();                       // slot 0
    void*                  m_pType;                         // +0x04
    void*                  m_pFirstPrimitive;               // +0x08
    CObjectTypeCollection* Next;                            // +0x0c
    void OnPostRelocation();                                // 0x00BDD8E0
};

struct CObjectCacheGroupCollection {                        // CLocalDetailCacheMap::CObjectCacheGroupCollection
    virtual ~CObjectCacheGroupCollection();                 // slot 0
    CObjectCacheGroupCollection* Next;                      // +0x04
    unsigned char                BoundingSphere[0x10];      // +0x08 C3DBoundingSphere
    float                        MaxFadeDistance;           // +0x18
    unsigned long                PrimitiveTypeMask;         // +0x1c
    CObjectTypeCollection*       ObjectTypeCollections;     // +0x20
    void RelocateData(long blockType, void* oldBase, void* newBase, long size);
};

void CObjectCacheGroupCollection::RelocateData(long blockType, void* oldBase, void* newBase, long size)
{
    CObjectTypeCollection* prev = 0;
    for (CObjectTypeCollection* p = ObjectTypeCollections; p; p = p->Next) {
        if (p == oldBase) {
            RelocatePointer(p, oldBase, newBase);
            p->OnPostRelocation();
            if (prev)
                prev->Next = p;
            else
                ObjectTypeCollections = p;
            return;
        }
        prev = p;
    }
}