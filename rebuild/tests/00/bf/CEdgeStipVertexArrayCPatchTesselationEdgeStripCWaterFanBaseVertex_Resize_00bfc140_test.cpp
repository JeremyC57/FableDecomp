// Behaviour test for 00bfc140 CEdgeStipVertexArray<CWaterFanBaseVertex>::Resize (copy of the source model,
// spy memory manager; the second renderer view checks that the free re-reads the global).
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct CMovableResource;
struct CMemoryManagerSpy {
    int id;
    void* AllocVirtual(int bytes, CMovableResource* owner);
    void FreeVirtual(void* block);
};
struct NativeLandscapeRenderer {
    unsigned char RendererState[0x6b0];
    CMemoryManagerSpy* MemoryManager;
};
NativeLandscapeRenderer* g_LandscapeRenderer;

struct CWaterFanBaseVertex {
    float          Z;
    float          ShoreLookUpArray[12];
    unsigned short VertexBufferIndex;
};

struct EdgeArray_CWaterFanBaseVertex {
    CWaterFanBaseVertex* Data;
    int                  Count;
    bool Resize(int count, CMovableResource* resource);
};

bool EdgeArray_CWaterFanBaseVertex::Resize(int count, CMovableResource* resource)
{
    if (Count != count) {
        if (count) {
            int copied = count < Count ? count : Count;
            CMemoryManagerSpy* manager = g_LandscapeRenderer->MemoryManager;
            CWaterFanBaseVertex* data = static_cast<CWaterFanBaseVertex*>(manager->AllocVirtual(count * sizeof(CWaterFanBaseVertex), resource));
            if (!data)
                return false;
            if (copied > 0)
                memcpy(data, Data, copied * sizeof(CWaterFanBaseVertex));
            if (Data)
                g_LandscapeRenderer->MemoryManager->FreeVirtual(Data);
            Count = count;
            Data = data;
        } else {
            g_LandscapeRenderer->MemoryManager->FreeVirtual(Data);
            Data = 0;
            Count = 0;
        }
    }
    return true;
}

static int bad = 0, allocs = 0, frees = 0, failNext = 0, lastBytes = 0, lastAllocId = 0, lastFreeId = 0;
static CMovableResource* lastOwner = 0;
static void* lastFreed = 0;
static NativeLandscapeRenderer r1, r2;
static CMemoryManagerSpy m1, m2;

void* CMemoryManagerSpy::AllocVirtual(int bytes, CMovableResource* owner)
{
    ++allocs; lastBytes = bytes; lastOwner = owner; lastAllocId = id;
    g_LandscapeRenderer = &r2;                    // the free must re-read the global
    if (failNext) return 0;
    void* p = malloc(bytes);
    memset(p, 0xA5, bytes);
    return p;
}
void CMemoryManagerSpy::FreeVirtual(void* block) { ++frees; lastFreed = block; lastFreeId = id; free(block); }

static void Check(bool b) { if (!b) ++bad; }

int main()
{
    m1.id = 1; m2.id = 2; r1.MemoryManager = &m1; r2.MemoryManager = &m2;
    int ownerWord = 0;
    CMovableResource* owner = reinterpret_cast<CMovableResource*>(&ownerWord);
    EdgeArray_CWaterFanBaseVertex a; a.Data = 0; a.Count = 0;
    g_LandscapeRenderer = &r1;
    Check(a.Resize(0, owner) && allocs == 0 && frees == 0);                    // same count: nothing
    Check(a.Resize(3, owner) && a.Count == 3 && a.Data != 0 && allocs == 1 && frees == 0);
    Check(lastBytes == 3 * 56 && lastOwner == owner && lastAllocId == 1);
    for (int i = 0; i < 3; ++i) { a.Data[i].Z = (float)i; a.Data[i].VertexBufferIndex = (unsigned short)(10 + i); }
    CWaterFanBaseVertex* old = a.Data;
    g_LandscapeRenderer = &r1; failNext = 1;
    Check(!a.Resize(5, owner) && a.Data == old && a.Count == 3 && frees == 0);  // failed alloc keeps state
    failNext = 0; g_LandscapeRenderer = &r1;
    Check(a.Resize(5, owner) && a.Count == 5 && frees == 1 && lastFreed == old && lastFreeId == 2);
    Check(a.Data[2].Z == 2.0f && a.Data[2].VertexBufferIndex == 12);
    unsigned char* tail = reinterpret_cast<unsigned char*>(&a.Data[3]);
    Check(tail[0] == 0xA5 && tail[2 * 56 - 1] == 0xA5);
    g_LandscapeRenderer = &r1;
    Check(a.Resize(1, owner) && a.Count == 1 && a.Data[0].VertexBufferIndex == 10 && frees == 2);
    g_LandscapeRenderer = &r1;
    Check(a.Resize(0, owner) && a.Data == 0 && a.Count == 0 && frees == 3 && lastFreeId == 1);
    if (bad) { std::printf("00bfc140_TEST FAIL\n"); return 1; }
    std::printf("00bfc140_TEST PASS\n");
    return 0;
}