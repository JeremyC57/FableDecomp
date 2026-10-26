// Behaviour test for 009d6100: src/009d6100.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CBankFileAsyncEntry::BeginReadDataAutoAlloc(unsigned long start, unsigned long size, EThreadedFilePriority priority)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CBankFileAsyncEntry::BeginReadDataAutoAlloc(unsigned long start, unsigned long size, EThreadedFilePriority priority) -- start an async read of `size` entry bytes from `start` into pool memory: non-cached banks round the start down to the alignment (the skip is remembered), the read and allocation sizes are aligned; returns the counted data object
// Leaf: BeginReadDataAutoAlloc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D6100, length 235 bytes (+5 int3), ret 0x10 (hidden CCountedPointer result).
// Callees: CBankFile::GetAlignment() @ 0x009CBB80 (catalog "CWorld::DrawGetEnvironment", propagated), CBankFile::Align
// @ 0x009CBB90, operator new @ 0x00BFEA1A (the 0x48-byte data, the 12-byte count block), CBankFileAsyncData::CBankFileAsyncData
// (unsigned long, unsigned long, CMovableResourceMemoryPool*, CBankFileAsync&) @ 0x009D5DF0, the count block's destroy
// function @ 0x009A42B0, CBankFileAsyncEntry::SetupRead @ 0x009D5970, operator delete @ 0x00BFE9BC.
// Shape: the read size (skip + size) and the aligned size are homed in the dead size/start argument slots; the counted
// pointer is a NAMED local built from the new object (inline count block: 1, deleter, object) and returned by copy, so the
// copy's add-ref and the local's release survive as `inc ; dec` on the shared null test.
// Layout: CBankFileAsyncEntry (landed ctor 0x009D5260): m_pBank +4, m_index +8; retail CBankFileAsync: LoadingMemoryPool
// +0x118, NonCached +0x148.
void __cdecl operator delete(void* p);             // 0x00BFE9BC
void __fastcall Destroy_009a42b0(void* object);    // 0x009A42B0: the per-type deleter stored in the block

struct SCountBlock {
    long m_references;                             // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(Destroy_009a42b0), m_pObject(object) { }
};
template <class T> class CCountedPointer { public:
    T* Data;                                       // +0
    SCountBlock* Info;                             // +4
    T* GetPtr() const { return Data; }
    CCountedPointer(T* object) : Data(object), Info(object ? new SCountBlock(object) : 0) {}
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_references;
    }
    ~CCountedPointer()
    {
        if (Info) {
            if (--Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                operator delete(Info);
            }
        }
    }
};
enum EThreadedFilePriority { THREADED_FILE_PRIORITY_DEFAULT = 3 };
class CMovableResourceMemoryPool;
class CBankFileAsync;
class CBankFileAsyncData {
public:
    unsigned char _body[0x48];
    CBankFileAsyncData(unsigned long length, unsigned long skipBytes, CMovableResourceMemoryPool* pool, CBankFileAsync& bank);   // 0x009D5DF0
};
class CBankFileAsync {
public:
    unsigned char _pad[0x118];
    CCountedPointer<CMovableResourceMemoryPool> LoadingMemoryPool;   // +0x118
    unsigned char _pad120[0x28];                   // +0x120 .. +0x147
    bool NonCached;                                // +0x148
    bool DisableAsync;                             // +0x149
    unsigned long GetAlignment() const;            // 0x009CBB80 (CBankFile)
    unsigned long Align(unsigned long size) const; // 0x009CBB90 (CBankFile)
};

class CBankFileAsyncEntry {
public:
    virtual void v00();
    CBankFileAsync* m_pBank;                       // +0x04
    unsigned long   m_index;                       // +0x08
    unsigned long   m_alignedSize;                 // +0x0c
    void SetupRead(unsigned long start, CBankFileAsyncData* data, unsigned long readSize, unsigned long allocSize,
                   unsigned long skip, EThreadedFilePriority priority);   // 0x009D5970
    CCountedPointer<CBankFileAsyncData> BeginReadDataAutoAlloc(unsigned long start, unsigned long size, EThreadedFilePriority priority);
};

CCountedPointer<CBankFileAsyncData> CBankFileAsyncEntry::BeginReadDataAutoAlloc(unsigned long start, unsigned long size,
                                                                                EThreadedFilePriority priority)
{
    unsigned long alignedStart;
    unsigned long skip;
    if (m_pBank->NonCached) {
        unsigned long alignment = m_pBank->GetAlignment();
        alignedStart = start / alignment * alignment;
        skip = start - alignedStart;
    } else {
        alignedStart = start;
        skip = 0;
    }
    unsigned long readSize = skip + size;
    unsigned long allocSize = m_pBank->Align(readSize);
    CCountedPointer<CBankFileAsyncData> data(new CBankFileAsyncData(size, skip, m_pBank->LoadingMemoryPool.GetPtr(), *m_pBank));
    SetupRead(alignedStart, data.GetPtr(), readSize, allocSize, skip, priority);
    return data;
}

// ---- stubs and driver ----
static int g_destroyed = 0;
void __fastcall Destroy_009a42b0(void* object) { ++g_destroyed; }
static unsigned long g_len, g_skip; static CMovableResourceMemoryPool* g_pool; static CBankFileAsync* g_bank; static int g_datas = 0;
CBankFileAsyncData::CBankFileAsyncData(unsigned long length, unsigned long skipBytes, CMovableResourceMemoryPool* pool, CBankFileAsync& bank)
{ g_len = length; g_skip = skipBytes; g_pool = pool; g_bank = &bank; ++g_datas; }
unsigned long CBankFileAsync::GetAlignment() const { return 0x800; }
unsigned long CBankFileAsync::Align(unsigned long size) const { return (size + 0x7ff) & ~0x7ffUL; }
static unsigned long g_start, g_read, g_alloc, g_rskip; static CBankFileAsyncData* g_data; static EThreadedFilePriority g_prio;
void CBankFileAsyncEntry::SetupRead(unsigned long start, CBankFileAsyncData* data, unsigned long readSize, unsigned long allocSize, unsigned long skip, EThreadedFilePriority priority)
{ g_start = start; g_data = data; g_read = readSize; g_alloc = allocSize; g_rskip = skip; g_prio = priority; }
void CBankFileAsyncEntry::v00() {}
int main()
{
    static unsigned char bs[sizeof(CBankFileAsync)]; memset(bs, 0, sizeof bs); CBankFileAsync* bank = (CBankFileAsync*)bs;
    bank->LoadingMemoryPool.Data = (CMovableResourceMemoryPool*)0x1234;
    static unsigned char es[sizeof(CBankFileAsyncEntry)]; memset(es, 0, sizeof es); CBankFileAsyncEntry* e = (CBankFileAsyncEntry*)es; e->m_pBank = bank;
    bank->NonCached = true;
    {
        CCountedPointer<CBankFileAsyncData> d = e->BeginReadDataAutoAlloc(0x1234, 0x100, THREADED_FILE_PRIORITY_DEFAULT);
        CHECK(g_start == 0x1000); CHECK(g_rskip == 0x234); CHECK(g_read == 0x334); CHECK(g_alloc == 0x800); CHECK(g_len == 0x100); CHECK(g_skip == 0x234);
        CHECK(g_pool == (CMovableResourceMemoryPool*)0x1234); CHECK(g_bank == bank); CHECK(g_data == d.Data); CHECK(g_prio == THREADED_FILE_PRIORITY_DEFAULT);
        CHECK(d.Info && d.Info->m_references == 1);
    }
    CHECK(g_destroyed == 1);
    bank->NonCached = false;
    {
        CCountedPointer<CBankFileAsyncData> d = e->BeginReadDataAutoAlloc(0x1234, 0x100, (EThreadedFilePriority)1);
        CHECK(g_start == 0x1234); CHECK(g_rskip == 0); CHECK(g_read == 0x100); CHECK(g_alloc == 0x800); CHECK(g_prio == 1);
    }
    CHECK(g_destroyed == 2); CHECK(g_datas == 2);
    if (fails == 0) printf("009d6100_TEST PASS\n"); else printf("009d6100_TEST FAIL\n");
    return fails;
}