#include <cstdio>
static int g_loads = 0, g_adds = 0, g_builds = 0;
class CResourceList;
class CResource { public:
    CResourceList* ResourceList;
    CResource* PrevResource;
    CResource* NextResource;
    unsigned long LastUsedFrame;
    void ListRemove() { PrevResource->NextResource = NextResource; NextResource->PrevResource = PrevResource; }
    void ListInsert(CResource* prev, CResource* next) { PrevResource = prev; NextResource = next; prev->NextResource = this; NextResource->PrevResource = this; }
    void AddToResourceList() { ++g_adds; }
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
class C3DAnimationStats { public: int v; };
static C3DAnimationStats g_stats;
static SCountBlock g_block;
struct CAnimationData : CResource {
    CCountedPointer<const C3DAnimationStats> Stats;
    void* LoadTask;
    CAnimationData() : Stats(0, 0), LoadTask(0) {}
    void BuildSharedAnimation() { ++g_builds; Stats.m_pObject = &g_stats; Stats.m_pCount = &g_block; }
    void Touch()
    {
        if (ResourceList && LastUsedFrame != ResourceList->CurrentFrame) {
            ListRemove();
            ListInsert(ResourceList->Head.PrevResource, &ResourceList->Head);
            LastUsedFrame = ResourceList->CurrentFrame;
        }
    }
};
static CAnimationData g_data;
class C3DAnimationInfo { public:
    CAnimationData* Data;
    void BeginLoading(bool wait) { ++g_loads; Data = &g_data; }
    bool HasStats() const { return Data && Data->Stats.m_pObject; }
    bool IsLoading() const { return Data && Data->LoadTask; }
    CCountedPointer<const C3DAnimationStats> GetAnimationStats()
    {
        if (!HasStats()) {
            if (!IsLoading())
                BeginLoading(true);
            Data->AddToResourceList();
            Data->BuildSharedAnimation();
        }
        Data->Touch();
        return Data->Stats;
    }
};
int main()
{
    CResourceList list; list.Head.PrevResource = &list.Head; list.Head.NextResource = &list.Head; list.CurrentFrame = 7;
    g_data.ResourceList = &list; g_data.LastUsedFrame = 3;
    g_data.ListInsert(&list.Head, &list.Head);
    g_block.m_references = 1;
    C3DAnimationInfo info; info.Data = 0;
    CCountedPointer<const C3DAnimationStats> a = info.GetAnimationStats();
    int ok = a.m_pObject == &g_stats && g_block.m_references == 2 && g_loads == 1 && g_adds == 1 && g_builds == 1 && g_data.LastUsedFrame == 7;
    CCountedPointer<const C3DAnimationStats> b = info.GetAnimationStats();   // decoded: no load, no build
    ok = ok && b.m_pObject == &g_stats && g_block.m_references == 3 && g_loads == 1 && g_builds == 1;
    if (!ok) { std::printf("00a269c0_TEST FAIL\n"); return 1; }
    std::printf("00a269c0_TEST PASS\n");
    return 0;
}