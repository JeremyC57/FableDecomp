#include <cstdio>
static int g_destroyed = 0;
struct CBankFileAsyncData { bool done; bool IsFinished() const { return done; } };
struct CResourceObject { virtual void v00() { } virtual void Destroy() { ++g_destroyed; } long m_refCount; void Release() { if (--m_refCount == 0) Destroy(); } };
struct CIVCountedPointer_Resource { CResourceObject* m_p; CIVCountedPointer_Resource() : m_p(0) { } ~CIVCountedPointer_Resource() { Release(); }
    void Release() { if (m_p) { m_p->Release(); m_p = 0; } } CIVCountedPointer_Resource& operator=(const CIVCountedPointer_Resource& o) { Release(); m_p = o.m_p; if (m_p) ++m_p->m_refCount; return *this; } };
struct CEmitterData { unsigned char _pad[0x1c]; CBankFileAsyncData* m_pAsyncData; unsigned char _pad20[0x0c]; CIVCountedPointer_Resource* m_pResourceHolder;
    bool UnloadResource() { if (m_pAsyncData != 0) { if (!m_pAsyncData->IsFinished()) return false; } *m_pResourceHolder = CIVCountedPointer_Resource(); return true; } };
int main() { CResourceObject obj; obj.m_refCount = 1; CIVCountedPointer_Resource holder; holder.m_p = &obj; CBankFileAsyncData async = { false };
    CEmitterData e; e.m_pAsyncData = &async; e.m_pResourceHolder = &holder; if (e.UnloadResource() || holder.m_p != &obj) { std::printf("00af1440_TEST FAIL\n"); return 1; }
    async.done = true; if (!e.UnloadResource() || holder.m_p != 0 || g_destroyed != 1) { std::printf("00af1440_TEST FAIL\n"); return 1; } e.m_pAsyncData = 0; if (!e.UnloadResource() || g_destroyed != 1) { std::printf("00af1440_TEST FAIL\n"); return 1; } std::printf("00af1440_TEST PASS\n"); return 0; }