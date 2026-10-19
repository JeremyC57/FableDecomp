// Behaviour test for NParticleEngine::CEmitterData::BeginLoading (0x00AF1360): unloaded data reserves space, starts a
// read of the whole entry, keeps the data and sets the resource size; loaded data is left alone; temporaries released.
#include <cstdio>
static int g_destroyed = 0;
static void DestroyCountedObject(void*) { ++g_destroyed; }
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
template <class T> struct CCountedPointer {
    T* Object; SCountBlock* Info;
    CCountedPointer() : Object(0), Info(0) {}
    CCountedPointer(T* o) : Object(o), Info(0) { if (o) { Info = new SCountBlock; Info->m_references = 1; Info->m_pfnDestroy = DestroyCountedObject; Info->m_pObject = o; } }
    CCountedPointer(const CCountedPointer& o) : Object(o.Object), Info(o.Info) { if (Info) ++Info->m_references; }
    ~CCountedPointer() { Release(); }
    void Release() { if (Info) { --Info->m_references; if (Info->m_references == 0) { Info->m_pfnDestroy(Info->m_pObject); delete Info; } } }
    CCountedPointer& operator=(const CCountedPointer& o) { if (o.Info) ++o.Info->m_references; Release(); Object = o.Object; Info = o.Info; return *this; }
    T* operator->() const { return Object; }
};
enum EThreadedFilePriority { PRIORITY_NORMAL = 2 };
static EThreadedFilePriority g_ParticleReadPriority = PRIORITY_NORMAL;
struct CBankFileAsyncData { unsigned long bytes; unsigned long Sizeof() const { return bytes + 16; } };
static CBankFileAsyncData g_data;
static unsigned long g_readOffset = 99, g_readSize = 0; static int g_readPriority = 0;
struct CBankFileAsyncEntry {
    CCountedPointer<CBankFileAsyncData> BeginReadDataAutoAlloc(unsigned long offset, unsigned long size, EThreadedFilePriority p)
    { g_readOffset = offset; g_readSize = size; g_readPriority = p; g_data.bytes = size; return CCountedPointer<CBankFileAsyncData>(&g_data); }
};
static CBankFileAsyncEntry g_entry;
struct CResourceListMethods { unsigned long freed; bool MakeFreeSpace(unsigned long s) { freed = s; return true; } };
struct CBankFileAsync {
    CResourceListMethods ResourceList; unsigned long index;
    unsigned long GetEntryDataSize(unsigned long i) { index = i; return 1000; }
    CCountedPointer<CBankFileAsyncEntry> CreateEntry(unsigned long) { return CCountedPointer<CBankFileAsyncEntry>(&g_entry); }
};
struct CParticleEmitterInfo { CBankFileAsync* Bank; unsigned long EntryIndex; };
struct CResourceMethods { unsigned long ResourceSize; void SetResourceSize(unsigned long s) { ResourceSize = s; } };
struct CEmitterData : CResourceMethods {
    CCountedPointer<CBankFileAsyncData> FileData; CParticleEmitterInfo* Owner;
    void BeginLoading();
};
void CEmitterData::BeginLoading()
{
    if (FileData.Object)
        return;
    unsigned long size = Owner->Bank->GetEntryDataSize(Owner->EntryIndex);
    Owner->Bank->ResourceList.MakeFreeSpace(size);
    CCountedPointer<CBankFileAsyncEntry> entry = Owner->Bank->CreateEntry(Owner->EntryIndex);
    FileData = entry->BeginReadDataAutoAlloc(0, size, g_ParticleReadPriority);
    unsigned long dataSize = FileData.Object ? FileData->Sizeof() + 8 : 8;
    SetResourceSize(dataSize + 12);
}
int main()
{
    CBankFileAsync bank; CParticleEmitterInfo info = { &bank, 7 };
    CEmitterData d; d.Owner = &info; d.ResourceSize = 0;
    int bad = 0;
    d.BeginLoading();
    if (bank.index != 7 || bank.ResourceList.freed != 1000 || g_readOffset != 0 || g_readSize != 1000 || g_readPriority != 2) bad = 1;
    if (d.FileData.Object != &g_data || d.FileData.Info->m_references != 1) bad = 2;
    if (d.ResourceSize != 1016 + 8 + 12) bad = 3;
    if (g_destroyed != 1) bad = 4;   // the entry died with its local, the data is still held
    d.ResourceSize = 0; d.BeginLoading();
    if (d.ResourceSize != 0) bad = 5;
    if (bad) { std::printf("00af1360_TEST FAIL\n"); return 1; }
    std::printf("00af1360_TEST PASS\n");
    return 0;
}
