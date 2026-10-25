#include <cstdio>
static int g_created = 0, g_setBlocks = 0, g_refreshes = 0, g_hwSets = 0, g_swSets = 0, g_commits = 0;
static unsigned int g_lastDirty = 0; static void* g_lastVoice = (void*)1; static unsigned int g_lastId = 0; static const void* g_lastSet = 0;
struct SListenerBlock { int x; };
struct CListenerState {
    const SListenerBlock* block; bool refreshed;
    void SetBlock(const SListenerBlock* b) { ++g_setBlocks; block = b; }
    void Refresh() { ++g_refreshes; refreshed = true; }
};
static CListenerState g_state;
struct CListenerStateFactory { CListenerState* Create() { ++g_created; return &g_state; } };
static CListenerStateFactory g_listenerStateFactory;
struct CHardwareEAX {
    int SetProperty(const void* set, unsigned int id, void* data, unsigned int size)
    { (void)data; (void)size; ++g_hwSets; g_lastSet = set; g_lastId = id; return 33; }
};
static CHardwareEAX g_hw; static CHardwareEAX* g_pHardwareEAX = &g_hw;
struct CEAXListenerEffect {
    int status;
    int SetProperty(void* voice, unsigned int id, void* data, unsigned int size, unsigned int* dirty)
    { (void)data; (void)size; ++g_swSets; g_lastVoice = voice; g_lastId = id; if (status == 0) *dirty = 0x11; return status; }
};
struct CEAXVoice;
struct CEAXManager {
    CListenerState* m_pListenerState; SListenerBlock m_listenerBlock; CEAXListenerEffect* m_pListenerEffect; int m_mode;
    void QueueCommit(unsigned int dirty, CEAXVoice* voice) { ++g_commits; g_lastDirty = dirty; g_lastVoice = voice; }
    int SetListenerProperty(const void* set, unsigned int id, void* data, unsigned int size)
    {
        int result = 0;
        switch (m_mode) {
        case 0: {
            if (!m_pListenerState) {
                m_pListenerState = g_listenerStateFactory.Create();
                m_pListenerState->SetBlock(&m_listenerBlock);
                m_pListenerState->Refresh();
            }
            unsigned int dirty = 0;
            result = m_pListenerEffect->SetProperty(0, id, data, size, &dirty);
            QueueCommit(dirty, 0);
            break;
        }
        case 1:
            result = g_pHardwareEAX->SetProperty(set, id, data, size);
            break;
        }
        return result;
    }
};
#define CHECK(x) do { if (!(x)) { std::printf("00c52980_TEST FAIL line %d\n", __LINE__); return 1; } } while (0)
int main()
{
    int guid = 0, data = 0; CEAXListenerEffect fx; fx.status = 0;
    CEAXManager m; m.m_pListenerState = 0; m.m_pListenerEffect = &fx; m.m_mode = 0;
    CHECK(m.SetListenerProperty(&guid, 4, &data, 4) == 0);
    CHECK(g_created == 1 && g_setBlocks == 1 && g_refreshes == 1 && g_state.block == &m.m_listenerBlock && m.m_pListenerState == &g_state);
    CHECK(g_swSets == 1 && g_lastId == 4 && g_commits == 1 && g_lastDirty == 0x11 && g_lastVoice == 0);
    fx.status = -9;
    CHECK(m.SetListenerProperty(&guid, 5, &data, 4) == -9 && g_created == 1 && g_commits == 2 && g_lastDirty == 0);
    m.m_mode = 1;
    CHECK(m.SetListenerProperty(&guid, 6, &data, 4) == 33 && g_hwSets == 1 && g_lastSet == &guid && g_lastId == 6 && g_commits == 2);
    m.m_mode = 2;
    CHECK(m.SetListenerProperty(&guid, 7, &data, 4) == 0 && g_hwSets == 1 && g_swSets == 2 && g_commits == 2);
    std::printf("00c52980_TEST PASS\n");
    return 0;
}