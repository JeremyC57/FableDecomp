#include <cstdio>
#include <cstring>
struct CCharStringData { const char* m_pData; unsigned long m_length; };
static int g_compares = 0;
static long Compare(const char* a, const char* b) { ++g_compares; return std::strcmp(a, b); }
struct CCharString {
    CCharStringData* PStringData;
    bool operator==(const CCharString& other) const
    {
        CCharStringData* a = PStringData; CCharStringData* b = other.PStringData;
        if (a == b) return true;
        if (a && b && a->m_length == b->m_length) return Compare(a->m_pData, b->m_pData) == 0;
        return false;
    }
};
static CCharStringData Block(const char* s) { CCharStringData d; d.m_pData = s; d.m_length = (unsigned long)std::strlen(s); return d; }
// Behaviour model of the CScriptBase spawned-thread walkers (0x00CB7AA0 / 0x00CB7B00 / 0x00CB7B60).
struct CSpawnedFuncBase {
    bool m_terminate; unsigned long ID; CCharString Name; CCharString AssociatedRegionName; bool FuncPaused;
    void SetFuncAsPaused(bool paused) { FuncPaused = paused; }
};
struct SNode { SNode* next; SNode* prev; CSpawnedFuncBase* m_pData; void* m_pInfo; };
struct CScriptBase {
    SNode* Threads;
    void KillSpawnedFunction(const CCharString& name, unsigned long id)
    {
        for (SNode* n = Threads->next; n != Threads; n = n->next) { CSpawnedFuncBase* f = n->m_pData; if (f->Name == name && f->ID == id) f->m_terminate = true; }
    }
    void OnRegionLoaded(const CCharString& region)
    {
        for (SNode* n = Threads->next; n != Threads; n = n->next) { CSpawnedFuncBase* f = n->m_pData; if (f->AssociatedRegionName == region) f->SetFuncAsPaused(false); }
    }
    void OnRegionUnloaded(const CCharString& region)
    {
        for (SNode* n = Threads->next; n != Threads; n = n->next) { CSpawnedFuncBase* f = n->m_pData; if (f->AssociatedRegionName == region) f->SetFuncAsPaused(true); }
    }
};
int main()
{
    CCharStringData fa = Block("Wander"), fb = Block("Wander"), fc = Block("Guard"), ra = Block("Oakvale"), rb = Block("Bowerstone"), key = Block("Oakvale"), kname = Block("Wander");
    CSpawnedFuncBase t1, t2, t3;
    t1.m_terminate = false; t1.ID = 1; t1.Name.PStringData = &fa; t1.AssociatedRegionName.PStringData = &ra; t1.FuncPaused = true;
    t2.m_terminate = false; t2.ID = 2; t2.Name.PStringData = &fb; t2.AssociatedRegionName.PStringData = &rb; t2.FuncPaused = true;
    t3.m_terminate = false; t3.ID = 1; t3.Name.PStringData = &fc; t3.AssociatedRegionName.PStringData = &ra; t3.FuncPaused = false;
    SNode head, n1, n2, n3; head.next = &n1; n1.next = &n2; n2.next = &n3; n3.next = &head; n1.m_pData = &t1; n2.m_pData = &t2; n3.m_pData = &t3;
    CScriptBase s; s.Threads = &head;
    int ok = 1;
    CCharString region; region.PStringData = &key;
    s.OnRegionLoaded(region);
    ok = !t1.FuncPaused && t2.FuncPaused && !t3.FuncPaused;
    if (!ok) { std::printf("00cb7b00_TEST FAIL\n"); return 1; }
    std::printf("00cb7b00_TEST PASS\n");
    return 0;
}