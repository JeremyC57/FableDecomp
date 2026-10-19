#include <cstdio>
static int g_objDtors = 0, g_freed = 0, g_notified = 0, g_deleted = 0;
static void* g_freedPtrs[4];
struct CPrimitive {
    unsigned long IVRefCount;
    virtual void Delete() const { ++g_deleted; }
    virtual void NotifyReleased() { ++g_notified; }
    void RemoveReference() { --IVRefCount; if (IVRefCount == 0) Delete(); }
};
struct CEnginePrimitiveHandle {
    CPrimitive* Object;
    void Clear() { if (Object && Object->IVRefCount == 1) Object->NotifyReleased(); if (Object) { Object->RemoveReference(); Object = 0; } Object = 0; }
    ~CEnginePrimitiveHandle() { Clear(); }
};
struct LandscapeMemoryMethods { void FreeVirtual(void* p) { g_freedPtrs[g_freed++] = p; } };
struct LocalDetailEngine { LandscapeMemoryMethods* LandscapeMemory; };
static LandscapeMemoryMethods g_mm; static LocalDetailEngine g_e = { &g_mm };
static LocalDetailEngine* g_LocalDetailEngine = &g_e;
struct CLocalDetailPrimitiveMeshZSpriteBatch {
    struct CObject { float Scale; ~CObject() { ++g_objDtors; } };
    CObject* Objects; void* ZSpriteEntries; long ObjectCount; bool HasMeshPrimitiveHandles;
    CEnginePrimitiveHandle ZSpriteHandle;
    ~CLocalDetailPrimitiveMeshZSpriteBatch();
};
CLocalDetailPrimitiveMeshZSpriteBatch::~CLocalDetailPrimitiveMeshZSpriteBatch()
{
 CObject* objects=Objects;
 LandscapeMemoryMethods* manager=g_LocalDetailEngine->LandscapeMemory;
 long count=ObjectCount;
 if(objects){
  for(long i=0;i<count;++i) objects[i].~CObject();
  manager->FreeVirtual(objects);
 }
 if(ZSpriteEntries) manager->FreeVirtual(ZSpriteEntries);
}
static int fail() { std::printf("00bfb590_TEST FAIL\n"); return 1; }
int main()
{
    static unsigned char objs[sizeof(CLocalDetailPrimitiveMeshZSpriteBatch::CObject) * 3];
    int entries;
    CPrimitive prim; prim.IVRefCount = 1;
    {
        CLocalDetailPrimitiveMeshZSpriteBatch b;
        b.Objects = (CLocalDetailPrimitiveMeshZSpriteBatch::CObject*)objs; b.ObjectCount = 3; b.ZSpriteEntries = &entries; b.ZSpriteHandle.Object = &prim;
    }
    if (g_objDtors != 3 || g_freed != 2 || g_freedPtrs[0] != objs || g_freedPtrs[1] != &entries || g_notified != 1 || g_deleted != 1) return fail();
    {
        CLocalDetailPrimitiveMeshZSpriteBatch b;
        b.Objects = 0; b.ObjectCount = 5; b.ZSpriteEntries = 0; b.ZSpriteHandle.Object = 0;
    }
    if (g_objDtors != 3 || g_freed != 2) return fail();
    std::printf("00bfb590_TEST PASS\n");
    return 0;
}