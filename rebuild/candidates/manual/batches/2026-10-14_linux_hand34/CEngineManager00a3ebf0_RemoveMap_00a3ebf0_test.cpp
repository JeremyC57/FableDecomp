#include <cstdio>
static int g_n = 0;
struct CMapListener_00a3ebf0 { void* m_removed; CMapListener_00a3ebf0() : m_removed(0) { } virtual void RemoveMap(void* m) { m_removed = m; ++g_n; } };
struct SListenerNode { SListenerNode* m_pNext; SListenerNode* m_pPrev; CMapListener_00a3ebf0* m_pListener; };
struct CEngineManager_00a3ebf0 { unsigned char _pad0[0x44]; CMapListener_00a3ebf0* m_pOwner; unsigned char _pad1[4]; SListenerNode* m_pListeners;
    void RemoveMap(void* map) { for (SListenerNode* n = m_pListeners->m_pNext; n != m_pListeners; n = n->m_pNext) n->m_pListener->RemoveMap(map); m_pOwner->RemoveMap(map); } };
int main() {
    SListenerNode head, n1; CMapListener_00a3ebf0 l1, owner; head.m_pNext = &n1; n1.m_pNext = &head; n1.m_pListener = &l1;
    CEngineManager_00a3ebf0* m = (CEngineManager_00a3ebf0*)new char[sizeof(CEngineManager_00a3ebf0)]; m->m_pOwner = &owner; m->m_pListeners = &head; int map = 0;
    m->RemoveMap(&map); if (l1.m_removed != &map || owner.m_removed != &map || g_n != 2) { std::printf("00a3ebf0_TEST FAIL\n"); return 1; }
    delete[] (char*)m;
    std::printf("00a3ebf0_TEST PASS\n"); return 0;
}
