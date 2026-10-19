// Behaviour test for NGameText::CDataBankGroupEntry::BuildGroup (0x009C8700): MRU touch, wait on unfinished async
// data, parse from a stream over the data, then release the counted file data (deleter runs on the last reference).
#include <cstdio>
struct CResourceList;
struct CResource {
    CResourceList* ResourceList; CResource* PrevResource; CResource* NextResource; unsigned long LastUsedFrame;
    void ListRemove() { PrevResource->NextResource = NextResource; NextResource->PrevResource = PrevResource; }
    void ListInsert(CResource* prev, CResource* next) { PrevResource = prev; NextResource = next; prev->NextResource = this; next->PrevResource = this; }
};
struct CResourceList { CResource Head; unsigned long CurrentFrame; };
struct CResourceMethods : CResource {
    static CResource* GetHead(CResourceList* list) { return &list->Head; }
    void MarkAsUsed()
    {
        if (ResourceList && LastUsedFrame != ResourceList->CurrentFrame) {
            ListRemove();
            ListInsert(GetHead(ResourceList)->PrevResource, GetHead(ResourceList));
            LastUsedFrame = ResourceList->CurrentFrame;
        }
    }
};
static int g_waits = 0, g_destroyed = 0, g_freed = 0;
struct CBankFileAsyncData {
    bool finished; long DataSize; const char* data;
    long GetDataSize() const { return DataSize; }
    bool IsFinished() { return finished; }
    bool WaitUntilFinished() { ++g_waits; finished = true; return true; }
    const void* PeekData() const { return data; }
};
static void DestroyData(void* p) { ++g_destroyed; (void)p; }
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
template <class T> struct CCountedPointer {
    T* Object; SCountBlock* Info;
    T* operator->() const { return Object; }
    void Clear()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) { Info->m_pfnDestroy(Info->m_pObject); ++g_freed; }
        }
        Info = 0;
        Object = 0;
    }
};
struct CMemoryDataInputStream { const char* data; long size; CMemoryDataInputStream(const void* d, long s) : data((const char*)d), size(s) {} };
struct CGroupEntry : CResourceMethods {
    CCountedPointer<CBankFileAsyncData> PFileData; long readSize; char first;
    void ReadFromDataStream(CMemoryDataInputStream* s) { readSize = s->size; first = s->data[0]; }
};
struct CDataBankGroupEntry {
    CGroupEntry* PGroup;
    void BuildGroup() const;
};
void CDataBankGroupEntry::BuildGroup() const
{
    PGroup->MarkAsUsed();
    if (PGroup->PFileData.Object) {
        if (!PGroup->PFileData->IsFinished())
            PGroup->PFileData->WaitUntilFinished();
        CMemoryDataInputStream stream(PGroup->PFileData->PeekData(), PGroup->PFileData->GetDataSize());
        PGroup->ReadFromDataStream(&stream);
        PGroup->PFileData.Clear();
    }
}
int main()
{
    CResourceList list; list.Head.PrevResource = list.Head.NextResource = &list.Head; list.CurrentFrame = 7;
    CGroupEntry a, b;
    a.ResourceList = b.ResourceList = &list; a.LastUsedFrame = 3; b.LastUsedFrame = 7;
    a.ListInsert(list.Head.PrevResource, &list.Head); b.ListInsert(list.Head.PrevResource, &list.Head);   // order: a, b
    CBankFileAsyncData async; async.finished = false; async.DataSize = 5; async.data = "Hello";
    SCountBlock block; block.m_references = 1; block.m_pfnDestroy = DestroyData; block.m_pObject = &async;
    a.PFileData.Object = &async; a.PFileData.Info = &block; a.readSize = 0; a.first = 0;
    CDataBankGroupEntry e; e.PGroup = &a;
    e.BuildGroup();
    int bad = 0;
    if (list.Head.PrevResource != &a || list.Head.NextResource != &b || a.LastUsedFrame != 7) bad = 1;   // a moved to the tail
    if (g_waits != 1 || a.readSize != 5 || a.first != 'H') bad = 2;
    if (g_destroyed != 1 || g_freed != 1 || a.PFileData.Object || a.PFileData.Info) bad = 3;
    e.BuildGroup();   // no file data, frame current: nothing happens
    if (g_waits != 1 || g_destroyed != 1 || list.Head.PrevResource != &a) bad = 4;
    if (bad) { std::printf("009c8700_TEST FAIL\n"); return 1; }
    std::printf("009c8700_TEST PASS\n");
    return 0;
}