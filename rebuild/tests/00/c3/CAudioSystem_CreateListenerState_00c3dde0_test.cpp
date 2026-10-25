// Behaviour test for 00c3dde0: CAudioSystem::CreateListenerState -- pooled, allocated, budget error and Init failure paths
#include <cstdio>
#include <cstdlib>
static long g_lastError = 0; static int g_reports = 0; static long g_reported = 0;
static int g_getFree = 0, g_popFree = 0, g_push = 0, g_discard = 0, g_ctor = 0; static long g_initResult = 0;
typedef void (*TAudioErrorCallback)(long);
static void OnError(long code) { ++g_reports; g_reported = code; }
class CPooledNode { public: CPooledNode* m_pPrev; CPooledNode* m_pNext; CPooledNode() : m_pPrev(0), m_pNext(0) { ++g_ctor; } virtual ~CPooledNode() { } };
class CListenerState : public CPooledNode {
public:
    long m_7c; CListenerState() : m_7c(0) { }
    long Init() { return g_initResult; }
};
static CListenerState g_pooled;
class CNodePool {
public:
    CPooledNode* GetFree() { ++g_getFree; return g_getFree == 2 ? 0 : &g_pooled; }
    void PushActive(CPooledNode*) { ++g_push; }
    void PopFree() { ++g_popFree; }
};
class CAudioSystem;
extern CAudioSystem g_audioSystem;
class CAudioSystem {
public:
    CNodePool m_listenerPool; long m_listenerBudget; TAudioErrorCallback m_pfnErrorCallback;
    void SetError(long code) { if (g_lastError == 0) { g_lastError = code; if (m_pfnErrorCallback) m_pfnErrorCallback(code); } }
    void DiscardListenerState(CListenerState*) { ++g_discard; }
    CListenerState* CreateListenerState()
    {
        CListenerState* state = 0;
        if (m_listenerBudget > 0) state = (CListenerState*)m_listenerPool.GetFree();
        else if (m_listenerBudget == -1) state = new CListenerState;
        else g_audioSystem.SetError(-9);
        if (!state) return 0;
        long error = state->Init();
        if (error) { g_audioSystem.SetError(error); DiscardListenerState(state); state = 0; }
        if (m_listenerBudget > 0) m_listenerPool.PopFree();
        m_listenerPool.PushActive(state);
        return state;
    }
};
CAudioSystem g_audioSystem;
int main()
{
    CAudioSystem& s = g_audioSystem; s.m_pfnErrorCallback = OnError; s.m_listenerBudget = 2;
    if (s.CreateListenerState() != &g_pooled || g_popFree != 1 || g_push != 1) { std::printf("00c3dde0_TEST FAIL\n"); return 1; }
    if (s.CreateListenerState() != 0 || g_popFree != 1 || g_push != 1) { std::printf("00c3dde0_TEST FAIL\n"); return 1; }
    g_initResult = -14;
    if (s.CreateListenerState() != 0 || g_discard != 1 || g_push != 2 || g_lastError != -14 || g_reported != -14) { std::printf("00c3dde0_TEST FAIL\n"); return 1; }
    g_initResult = 0; s.m_listenerBudget = -1; int ctors = g_ctor;
    CListenerState* fresh = s.CreateListenerState();
    if (!fresh || fresh == &g_pooled || g_ctor != ctors + 1 || fresh->m_7c != 0 || g_push != 3) { std::printf("00c3dde0_TEST FAIL\n"); return 1; }
    delete fresh;
    s.m_listenerBudget = 0; g_lastError = 0;
    if (s.CreateListenerState() != 0 || g_lastError != -9 || g_reports != 2) { std::printf("00c3dde0_TEST FAIL\n"); return 1; }
    std::printf("00c3dde0_TEST PASS\n");
    return 0;
}
