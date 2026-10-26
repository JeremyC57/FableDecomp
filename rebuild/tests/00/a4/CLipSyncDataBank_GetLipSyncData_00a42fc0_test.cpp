#include <cstdio>
static int g_loads = 0, g_builds = 0;
class CResourceList;
class CResource { public:
    CResourceList* ResourceList;
    CResource* PrevResource;
    CResource* NextResource;
    unsigned long LastUsedFrame;
    void ListRemove() { PrevResource->NextResource = NextResource; NextResource->PrevResource = PrevResource; }
    void ListInsert(CResource* prev, CResource* next) { PrevResource = prev; NextResource = next; prev->NextResource = this; NextResource->PrevResource = this; }
};
class CResourceList { public: CResource Head; unsigned long CurrentFrame; };
struct SCountBlock { long m_references; };
template <class T>
class CCountedPointer { public:
    T* m_pObject; SCountBlock* m_pCount;
    CCountedPointer(T* o, SCountBlock* b) : m_pObject(o), m_pCount(b) {}
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount) ++m_pCount->m_references;
    }
};
struct CLipSyncData { int v; };
static CLipSyncData g_lip;
static SCountBlock g_block;
struct CLipSyncEntryData : CResource {
    CCountedPointer<const CLipSyncData> LipSync;
    CLipSyncEntryData() : LipSync(0, 0) {}
    void BuildLipSync() { ++g_builds; LipSync.m_pObject = &g_lip; LipSync.m_pCount = &g_block; }
    void Touch()
    {
        if (ResourceList && LastUsedFrame != ResourceList->CurrentFrame) {
            ListRemove();
            ListInsert(ResourceList->Head.PrevResource, &ResourceList->Head);
            LastUsedFrame = ResourceList->CurrentFrame;
        }
    }
};
static CLipSyncEntryData g_data;
class CLipSyncDataBank;
class CLipSyncDataBankEntry { public:
    CLipSyncEntryData* Data;
    float Duration;
    void BeginLoading(long index, CLipSyncDataBank* bank) { ++g_loads; Data = &g_data; }
};
template <class T> class CArray { public:
    T* m_pBegin; T* m_pEnd; T* m_pEndOfStorage;
    T* begin() { return m_pBegin; }
    T& operator[](long i) { return *(begin() + i); }
};
class CLipSyncDataBank { public:
    CArray<CLipSyncDataBankEntry> Entries;
    bool EntryExists(long index) { return index == 1; }
    CCountedPointer<const CLipSyncData> GetLipSyncData(long index)
    {
        CLipSyncDataBankEntry* entry = &Entries[index];
        if (EntryExists(index)) {
            CLipSyncDataBankEntry& slot = Entries[index];
            if (slot.Data == 0)
                slot.BeginLoading(index, this);
        }
        if (entry->Data->LipSync.m_pObject == 0)
            entry->Data->BuildLipSync();
        entry->Data->Touch();
        return entry->Data->LipSync;
    }
};
int main()
{
    CResourceList list; list.Head.PrevResource = &list.Head; list.Head.NextResource = &list.Head; list.CurrentFrame = 9;
    g_data.ResourceList = &list; g_data.LastUsedFrame = 0;
    g_data.ListInsert(&list.Head, &list.Head);
    g_block.m_references = 1;
    CLipSyncDataBankEntry entries[2]; entries[0].Data = 0; entries[1].Data = 0;
    CLipSyncDataBank bank; bank.Entries.m_pBegin = entries; bank.Entries.m_pEnd = entries + 2; bank.Entries.m_pEndOfStorage = entries + 2;
    CCountedPointer<const CLipSyncData> a = bank.GetLipSyncData(1);
    int ok = a.m_pObject == &g_lip && entries[1].Data == &g_data && g_loads == 1 && g_builds == 1 && g_block.m_references == 2 && g_data.LastUsedFrame == 9;
    CCountedPointer<const CLipSyncData> b = bank.GetLipSyncData(1);
    ok = ok && g_loads == 1 && g_builds == 1 && g_block.m_references == 3;
    if (!ok) { std::printf("00a42fc0_TEST FAIL\n"); return 1; }
    std::printf("00a42fc0_TEST PASS\n");
    return 0;
}