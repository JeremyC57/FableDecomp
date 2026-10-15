// CLocalDetailCacheMap::CObjectTypeCollection::CreatePrimitiveOfType(LOCAL_DETAIL_PRIMITIVE_TYPE) -- allocate (from this movable collection's pool) and construct the local-detail primitive class for the type: mesh, repeated mesh, or mesh/z-sprite batch
// Leaf: ?CreatePrimitiveOfType@CObjectTypeCollection
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDE600, 120 bytes (+ int3 padding; `ret 4`).
// Callees: the class allocators `static void* __fastcall operator new(size_t, CMovableResource*)` of CLocalDetailPrimitiveMesh
//   @ 0x00BF9900 (0x74 bytes), CLocalDetailPrimitiveRepeatedMesh @ 0x00BF9980 (0x60) and CLocalDetailPrimitiveMeshZSpriteBatch
//   @ 0x00BF9A00 (0x50) (labels "_Cons_val<...CAnimationEntry...>" are propagated), and their constructors
//   (const CLocalDetailObjectCollectionType*, CLocalDetailCacheMap*) @ 0x00BDFB50 / 0x00BFAA80 / 0x00BFAC70 (the 0x00BDFB50 label
//   "CLocalDetailPrimitiveRepeatedMesh" is a byte twin: the 0x74-byte allocation is the Mesh class).
// Layout (retail): object type +4, owning cache map +0x10; the collection itself is the CMovableResource owner.
// Shape: `switch` with case 0, case 1 and default; `this` is loaded into edx for the allocator once before the dispatch; each arm is
//   a null-checked `new (this) T(m_pType, m_pMap)` returning the constructor's result.
struct CMovableResource {
    virtual void v00();
};
struct CLocalDetailObjectCollectionType;
struct CLocalDetailCacheMap;
enum LOCAL_DETAIL_PRIMITIVE_TYPE { LOCAL_DETAIL_PRIMITIVE_MESH = 0, LOCAL_DETAIL_PRIMITIVE_REPEATED_MESH = 1, LOCAL_DETAIL_PRIMITIVE_MESH_ZSPRITE_BATCH = 2 };
struct CLocalDetailPrimitiveBase {
    virtual void v00();
};
struct CLocalDetailPrimitiveMesh : CLocalDetailPrimitiveBase {
    unsigned char _data[0x70];
    CLocalDetailPrimitiveMesh(const CLocalDetailObjectCollectionType* type, CLocalDetailCacheMap* map);              // 0x00BDFB50
    static void* __fastcall operator new(unsigned int size, CMovableResource* owner);                                 // 0x00BF9900
    static void __fastcall operator delete(void* p, CMovableResource* owner);
};
struct CLocalDetailPrimitiveRepeatedMesh : CLocalDetailPrimitiveBase {
    unsigned char _data[0x5c];
    CLocalDetailPrimitiveRepeatedMesh(const CLocalDetailObjectCollectionType* type, CLocalDetailCacheMap* map);      // 0x00BFAA80
    static void* __fastcall operator new(unsigned int size, CMovableResource* owner);                                 // 0x00BF9980
    static void __fastcall operator delete(void* p, CMovableResource* owner);
};
struct CLocalDetailPrimitiveMeshZSpriteBatch : CLocalDetailPrimitiveBase {
    unsigned char _data[0x4c];
    CLocalDetailPrimitiveMeshZSpriteBatch(const CLocalDetailObjectCollectionType* type, CLocalDetailCacheMap* map);  // 0x00BFAC70
    static void* __fastcall operator new(unsigned int size, CMovableResource* owner);                                 // 0x00BF9A00
    static void __fastcall operator delete(void* p, CMovableResource* owner);
};
struct CLocalDetailCacheMap {
    struct CObjectTypeCollection : CMovableResource {
        CLocalDetailObjectCollectionType* m_pType;          // +0x04
        unsigned char                     _pad08[0x08];
        CLocalDetailCacheMap*             m_pMap;           // +0x10
        CLocalDetailPrimitiveBase* CreatePrimitiveOfType(LOCAL_DETAIL_PRIMITIVE_TYPE type);
    };
};

CLocalDetailPrimitiveBase* CLocalDetailCacheMap::CObjectTypeCollection::CreatePrimitiveOfType(LOCAL_DETAIL_PRIMITIVE_TYPE type)
{
    switch (type) {
    case LOCAL_DETAIL_PRIMITIVE_MESH:
        return new (this) CLocalDetailPrimitiveMesh(m_pType, m_pMap);
    case LOCAL_DETAIL_PRIMITIVE_REPEATED_MESH:
        return new (this) CLocalDetailPrimitiveRepeatedMesh(m_pType, m_pMap);
    default:
        return new (this) CLocalDetailPrimitiveMeshZSpriteBatch(m_pType, m_pMap);
    }
}