// CTextureManager::~CTextureManager() -- uninitialise, then destroy the 16 preallocated texture pools and the CBaseClass base
// Leaf: ??1CTextureManager@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A6A440, 136 bytes. __fastcall; called by the deleting dtor 0x00A6C000 (vtable
//         0x0129DC5C slot 0). RENDER LAYER (texture pools).
// Callees: free 0x00BFEA14 (vector storage); CResourceList::~CResourceList() 0x009FC5C0 (vtable 0x0129C808, the pool base;
//          catalog "Stack<unsigned long,64>::~Stack" is wrong); CBaseClass::~CBaseClass() 0x0099A300 (tail jump); each
//          CManagedTextureData release through its vtable slot 1.
// Shape: vtable store, the inlined Uninitialise() (`if (TextureBuffer) TextureBuffer = 0; Initialised = false;`), then the
//        member array destroyed last-to-first (counter 16 in the frame, `sub edi,54h`): each pool's STLport vector of
//        CIVCountedPointer<CManagedTextureData> releases every element (null-tested release, pointer zeroed), frees its
//        storage, then the pool's CResourceList base dtor runs. The element release goes through an inline
//        `CManagedTextureData::DecRef()` on the POINTEE (`if (--RefCount == 0) Destroy();`): it keeps the object in ecx,
//        reuses the `dec [ecx+4]` flags (`jne`) and leaves `this` in ebx; the release spelled on the holder re-reads [esi].
// Layout: retail CTextureManager (PDB - 0x40, see landed Uninitialise 0x00A696D0): Initialised +0x04, PreallocPools +0x4C
//         (16 x 0x54), FailedAllocations +0x58C, BufferSize +0x5CC, TextureBuffer +0x5D0; CPreallocTexturePool: CResourceList
//         base 0x3C, Count +0x3C, MaxSize +0x40, PoolIndex +0x44, Textures +0x48 (12-byte STLport vector; PDB 16).
extern "C" void __cdecl free(void* p);           // 0x00BFEA14
namespace N_0099a300 { class CBaseClass {                               // vtable 0x01231710
public:
    virtual void v000();
    CBaseClass();                                // 0x0099A2D0
    ~CBaseClass();                               // 0x0099A300
}; } using N_0099a300::CBaseClass;
class CManagedTextureData { public:
    virtual void v00();
    virtual void Destroy();                      // slot 1
    long RefCount;                               // +0x04
    __forceinline void DecRef() { if (--RefCount == 0) Destroy(); }
};
template <class T> class CIVCountedPointer { public:
    T* Object;
    __forceinline ~CIVCountedPointer()
    {
        if (Object) {
            Object->DecRef();
            Object = 0;
        }
    }
};
template <class T> struct CRetailVector {        // STLport vector over a free()-based allocator
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    __forceinline ~CRetailVector()
    {
        T* last = _M_finish;
        for (T* p = _M_start; p != last; ++p)
            p->~T();
        if (_M_start)
            free(_M_start);
    }
};
class CResourceList { public:                           // vtable 0x0129C808, 0x3C bytes
    virtual void v00();
    unsigned char _pad04[0x38];
    ~CResourceList();                            // 0x009FC5C0
};
struct __declspec(novtable) CPreallocTexturePool : CResourceList {
    unsigned long Count;                         // +0x3c
    unsigned long MaxSize;                       // +0x40
    long PoolIndex;                              // +0x44
    CRetailVector<CIVCountedPointer<CManagedTextureData> > Textures;   // +0x48
    __forceinline ~CPreallocTexturePool() {}
};
class CTextureManager : public CBaseClass {      // vtable 0x0129DC5C
public:
    bool                 Initialised;            // +0x04
    unsigned long        PoolCount;              // +0x08
    unsigned long        PreallocSizes[16];      // +0x0c
    CPreallocTexturePool PreallocPools[16];      // +0x4c
    unsigned long        FailedAllocations[16];  // +0x58c
    long                 BufferSize;             // +0x5cc
    void*                TextureBuffer;          // +0x5d0
    virtual ~CTextureManager();
    void Uninitialise()
    {
        if (TextureBuffer)
            TextureBuffer = 0;
        Initialised = false;
    }
};
CTextureManager::~CTextureManager()
{
    Uninitialise();
}

// ---- test harness ----
#include <stdio.h>
extern "C" void* __cdecl malloc(unsigned int size);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_baseDtor = 0, g_destroyed = 0, g_nlist = 0;
static void* g_listOrder[32];
static CManagedTextureData* g_destroyedWhat[8];
void CBaseClass::v000() { }
CBaseClass::CBaseClass() { }
CBaseClass::~CBaseClass() { ++g_baseDtor; }
void CResourceList::v00() { }
CResourceList::~CResourceList() { g_listOrder[g_nlist++ & 31] = this; }
void CManagedTextureData::v00() { }
void CManagedTextureData::Destroy() { g_destroyedWhat[g_destroyed++ & 7] = this; }

static void fill(CPreallocTexturePool& pool, CManagedTextureData** objs, int n)
{
    CIVCountedPointer<CManagedTextureData>* p = (CIVCountedPointer<CManagedTextureData>*)malloc(n * 4 + 4);
    for (int i = 0; i < n; ++i) p[i].Object = objs[i];
    pool.Textures._M_start = p;
    pool.Textures._M_finish = p + n;
    pool.Textures._M_end_of_storage = p + n + 1;
}

int main()
{
    CTextureManager* tm = new CTextureManager;
    for (int i = 0; i < 16; ++i) {
        tm->PreallocPools[i].Textures._M_start = 0;
        tm->PreallocPools[i].Textures._M_finish = 0;
        tm->PreallocPools[i].Textures._M_end_of_storage = 0;
    }
    tm->Initialised = true;
    tm->TextureBuffer = (void*)0x1234;
    static CManagedTextureData a, b, c;
    a.RefCount = 1; b.RefCount = 2; c.RefCount = 1;
    CManagedTextureData* pool0[3] = { &a, &b, 0 };
    CManagedTextureData* pool5[1] = { &c };
    fill(tm->PreallocPools[0], pool0, 3);
    fill(tm->PreallocPools[5], pool5, 1);
    tm->~CTextureManager();
    CHECK(tm->TextureBuffer == 0);
    CHECK(tm->Initialised == false);
    CHECK(g_baseDtor == 1);
    CHECK(g_nlist == 16);
    for (int i = 0; i < 16; ++i)
        CHECK(g_listOrder[i] == (void*)&tm->PreallocPools[15 - i]);
    CHECK(g_destroyed == 2);
    CHECK(g_destroyedWhat[0] == &c && g_destroyedWhat[1] == &a);   // pool 5 before pool 0
    CHECK(a.RefCount == 0 && b.RefCount == 1 && c.RefCount == 0);
    if (fails == 0) printf("00a6a440_TEST PASS\n");
    return fails;
}