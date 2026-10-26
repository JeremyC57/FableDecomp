#include <cstdio>

// Behaviour model of CCountedPointer<CMusicManager>::DeleteData (0x004AE510): null-tested delete; the implicit
// destructor frees the registered-music vector, then releases the sound def (Destroy on the last reference) and
// clears the pointer.
static int g_vectorsFreed = 0, g_defsDestroyed = 0, g_deleted = 0;
struct CDefBase {
    long m_count;
    virtual void Destroy() { ++g_defsDestroyed; }
    void Release() const { if (--const_cast<CDefBase*>(this)->m_count == 0) const_cast<CDefBase*>(this)->Destroy(); }
};
struct CSoundDefPointer { const CDefBase* m_p; ~CSoundDefPointer() { if (m_p) { m_p->Release(); m_p = 0; } } };
struct CEntryVector { int n; ~CEntryVector() { ++g_vectorsFreed; } };
class CMusicManager { public:
    long CurrentState[6];
    CSoundDefPointer PSoundDef;
    CEntryVector RegisteredMusic;
    long CurrentMusicID;
    static void operator delete(void* p) { ++g_deleted; ::operator delete(p); }
};
static void DeleteData(void* data) { delete (CMusicManager*)data; }

int main()
{
    DeleteData(0);
    if (g_deleted != 0) { std::printf("004ae510_TEST FAIL\n"); return 1; }
    CDefBase shared; shared.m_count = 2;
    CMusicManager* a = new CMusicManager; a->PSoundDef.m_p = &shared;
    DeleteData(a);                                                     // 2 -> 1: no Destroy
    if (g_vectorsFreed != 1 || g_defsDestroyed != 0 || shared.m_count != 1 || g_deleted != 1) { std::printf("004ae510_TEST FAIL\n"); return 1; }
    CMusicManager* b = new CMusicManager; b->PSoundDef.m_p = &shared;
    DeleteData(b);                                                     // last reference: Destroy
    if (g_vectorsFreed != 2 || g_defsDestroyed != 1 || shared.m_count != 0 || g_deleted != 2) { std::printf("004ae510_TEST FAIL\n"); return 1; }
    CMusicManager* c = new CMusicManager; c->PSoundDef.m_p = 0;
    DeleteData(c);                                                     // no def held
    if (g_vectorsFreed != 3 || g_defsDestroyed != 1 || g_deleted != 3) { std::printf("004ae510_TEST FAIL\n"); return 1; }
    std::printf("004ae510_TEST PASS\n");
    return 0;
}