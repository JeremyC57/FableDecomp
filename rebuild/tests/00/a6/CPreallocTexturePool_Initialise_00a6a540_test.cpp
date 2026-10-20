// Behaviour test for CPreallocTexturePool::Initialise (0x00A6A540): the pool remembers index / max size / count, the
// slot array is resized, each slot gets a fresh zeroed managed texture (the old one released and destroyed when its
// count drops to zero) holding a new texture, the pool's size and index, and every texture is added to the resource list.
#include <cstdio>
#include <cstring>

static int g_deleted = 0, g_added = 0;
struct CResource {
    long RefCount;
    CResource() : RefCount(1) {}
    virtual ~CResource() {}
    virtual void DeleteThis() { ++g_deleted; delete this; }
    void Release() { if (--RefCount == 0) DeleteThis(); }
};
struct CTexture { void* a; void* b; CTexture() : a(0), b(0) {} };
template <class T> struct CCountedPointer { T* Data; CCountedPointer() : Data(0) {} void Reset(T* o) { delete Data; Data = o; } ~CCountedPointer() { delete Data; } };
struct CManagedTextureData : CResource {
    unsigned short Width, Height; CCountedPointer<CTexture> Texture; void* Owner; void* Data;
    unsigned long AllocSize, PoolIndex; bool Allocated, Locked, BusyFlag;
    CManagedTextureData() : Width(0), Height(0), Owner(0), Data(0), AllocSize(0), PoolIndex(0), Allocated(false), Locked(false), BusyFlag(false) {}
};
template <class T> struct CIVCountedPointer {
    T* Object;
    CIVCountedPointer() : Object(0) {}
    CIVCountedPointer& operator=(T* o) { if (Object) { Object->Release(); Object = 0; } Object = o; return *this; }
};
template <class T> struct CArray {
    T* _M_start; T* _M_finish; T storage[8];
    void resize(unsigned int n, const T& x) { _M_start = storage; for (unsigned int i = (unsigned int)(_M_finish - _M_start); i < n; ++i) storage[i] = x; _M_finish = storage + n; }
    T& operator[](unsigned long n) { return *(_M_start + n); }
};
struct CPreallocTexturePool {
    unsigned long Count, MaxSize; long PoolIndex;
    CArray<CIVCountedPointer<CManagedTextureData> > Textures;
    void AddResource(CResource* r) { if (r) ++g_added; }
    void Initialise(long poolIndex, unsigned long maxSize, unsigned long count);
};
void CPreallocTexturePool::Initialise(long poolIndex, unsigned long maxSize, unsigned long count)
{
    PoolIndex = poolIndex;
    MaxSize = maxSize;
    Count = count;
    Textures.resize(count, CIVCountedPointer<CManagedTextureData>());
    for (unsigned long i = 0; i < count; ++i) {
        Textures[i] = new CManagedTextureData();
        CManagedTextureData* texture = Textures[i].Object;
        texture->Texture.Reset(new CTexture());
        texture->AllocSize = maxSize;
        texture->PoolIndex = poolIndex;
        AddResource(Textures[i].Object);
    }
}
int main()
{
    int bad = 0;
    CPreallocTexturePool pool; pool.Textures._M_start = pool.Textures.storage; pool.Textures._M_finish = pool.Textures.storage;
    pool.Initialise(3, 0x4000, 2);
    if (pool.Count != 2 || pool.MaxSize != 0x4000 || pool.PoolIndex != 3 || g_added != 2 || g_deleted != 0) bad = 1;
    for (int i = 0; i < 2; ++i) {
        CManagedTextureData* t = pool.Textures[i].Object;
        if (!t || t->AllocSize != 0x4000 || t->PoolIndex != 3 || !t->Texture.Data || t->Width || t->Allocated) bad = 1;
    }
    // re-initialise: the old textures (count 1) are released and destroyed
    pool.Initialise(4, 0x8000, 2);
    if (g_deleted != 2 || g_added != 4 || pool.Textures[1].Object->PoolIndex != 4) bad = 1;
    if (bad) { std::printf("00a6a540_TEST FAIL\n"); return 1; }
    std::printf("00a6a540_TEST PASS\n");
    return 0;
}