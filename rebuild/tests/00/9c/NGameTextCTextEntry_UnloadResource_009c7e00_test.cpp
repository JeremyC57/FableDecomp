#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x009C7E00: exercises the same logic through a copy of the model (compiled separately from the source)
static int g_destroyed = 0;
struct CBankFileAsyncData { bool m_finished; bool IsFinished() const { return m_finished; } };
struct CResourceObject_Retail {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    long m_refCount;
    void Release() { if (--m_refCount == 0) Destroy(); }
};
struct CIVCountedPointer_Resource {
    CResourceObject_Retail* m_p;
    CIVCountedPointer_Resource() : m_p(0) { }
    ~CIVCountedPointer_Resource() { Release(); }
    void Release() { if (m_p) { m_p->Release(); m_p = 0; } }
    CIVCountedPointer_Resource& operator=(const CIVCountedPointer_Resource& other) { Release(); m_p = other.m_p; if (m_p) ++m_p->m_refCount; return *this; }
};
struct CDataBankEntryOwner_Retail { CIVCountedPointer_Resource m_resource; };
struct CEntry {
    CBankFileAsyncData* PFileData; CDataBankEntryOwner_Retail* Owner;
    bool UnloadResource() { if (PFileData == 0 || PFileData->IsFinished()) Owner->m_resource = CIVCountedPointer_Resource(); return true; }
};
static bool Run()
{
    CResourceObject_Retail resource; resource.m_refCount = 1;
    CDataBankEntryOwner_Retail owner; owner.m_resource.m_p = &resource;
    CBankFileAsyncData async; async.m_finished = false;
    CEntry e; e.PFileData = &async; e.Owner = &owner;
    if (!e.UnloadResource()) return false;          // pending: true, nothing released
    if (owner.m_resource.m_p != &resource || g_destroyed != 0) return false;
    async.m_finished = true;
    if (!e.UnloadResource()) return false;          // finished: released and destroyed at zero
    if (owner.m_resource.m_p != 0 || g_destroyed != 1) return false;
    resource.m_refCount = 2; owner.m_resource.m_p = &resource; e.PFileData = 0;
    if (!e.UnloadResource()) return false;          // no async record: released (count 2 -> 1, not destroyed)
    bool ok = owner.m_resource.m_p == 0 && resource.m_refCount == 1 && g_destroyed == 1;
    owner.m_resource.m_p = 0;
    return ok;
}
int main()
{
    if (!Run()) { std::printf("009c7e00_TEST FAIL\n"); return 1; }
    std::printf("009c7e00_TEST PASS\n");
    return 0;
}