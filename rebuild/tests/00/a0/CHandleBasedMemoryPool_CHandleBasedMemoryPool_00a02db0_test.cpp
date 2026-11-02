// Behaviour test for 0x00A02DB0 CHandleBasedMemoryPool::~CHandleBasedMemoryPool:
// Uninitialise first, then Name, then the Handles block freed, then the base destructor.
#include <cstdio>
#include <cstdlib>
static int g_log[8];
static int g_n = 0;
static void Note(int e) { if (g_n < 8) g_log[g_n++] = e; }
static void FreeBlock(void* p) { if (p) { Note(3); std::free(p); } }
struct HandleArray {
    void** Start; void** Finish; void** EndOfStorage;
    HandleArray() : Start(0), Finish(0), EndOfStorage(0) { }
    ~HandleArray() { FreeBlock(Start); }
};
struct CharString { void* PStringData; CharString() : PStringData(0) { } ~CharString() { Note(2); } };
struct BaseClassNonCopyable { virtual ~BaseClassNonCopyable() { Note(4); } };
struct HandleBasedMemoryPool : BaseClassNonCopyable {
    void* Buffer; long PoolSize;
    HandleArray Handles; long FirstUnusedHandle; CharString Name;
    HandleBasedMemoryPool() : Buffer(0), PoolSize(0), FirstUnusedHandle(0) { }
    void Uninitialise() { Note(1); Buffer = 0; PoolSize = 0; }
    virtual ~HandleBasedMemoryPool() { Uninitialise(); }
};
int main()
{
    HandleBasedMemoryPool* pool = new HandleBasedMemoryPool;
    pool->Handles.Start = (void**)std::malloc(4 * sizeof(void*));
    BaseClassNonCopyable* base = pool;
    delete base;
    const int expected[4] = { 1, 2, 3, 4 };
    if (g_n != 4) { std::printf("00a02db0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i)
        if (g_log[i] != expected[i]) { std::printf("00a02db0_TEST FAIL\n"); return 1; }
    std::printf("00a02db0_TEST PASS\n");
    return 0;
}