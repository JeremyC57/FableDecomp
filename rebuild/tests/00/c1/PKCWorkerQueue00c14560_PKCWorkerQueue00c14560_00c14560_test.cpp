// Behaviour test for 00c14560: PKCWorkerQueue destructor -- under the queue lock, appends the null "stop" request and signals the semaphore; then joins and deletes the worker thread; the members (request vector, semaphore, critical section) are destroyed afterwards
#include <cstdio>
#include <cstdlib>

static char g_log[64];
static int g_pos = 0;
static void Log(char c) { g_log[g_pos++] = c; g_log[g_pos] = 0; }
static int g_lockDepth = 0, g_sawStop = 0, g_pendingKept = 0;

struct PKCCriticalSection { ~PKCCriticalSection() { Log('C'); } };
struct CScopedLock {
    CScopedLock(PKCCriticalSection*) { ++g_lockDepth; Log('L'); }
    ~CScopedLock() { --g_lockDepth; Log('U'); }
};
struct PKCSemaphore {
    int m_count;
    void Release() { if (g_lockDepth == 1) Log('R'); ++m_count; }
    ~PKCSemaphore() { Log('S'); }
};
struct PKCRequest { int m_id; };

struct PKCThread {
    struct RequestVector* m_pQueue;
    int m_sawStop;
    virtual ~PKCThread() { Log('D'); }
    void Join();
};

struct RequestVector {                     // the Dinkumware vector<PKCRequest*> shape: first / last / end
    PKCRequest** m_first; PKCRequest** m_last; PKCRequest** m_end;
    RequestVector() : m_first(0), m_last(0), m_end(0) { }
    ~RequestVector() { if (m_first) { std::free(m_first); Log('V'); } }
    unsigned int size() const { return m_first == 0 ? 0 : (unsigned int)(m_last - m_first); }
    PKCRequest** end() { return m_last; }
    void insert(PKCRequest** where, PKCRequest* value)
    {
        unsigned int off = (unsigned int)(where - m_first), n = size();
        PKCRequest** p = (PKCRequest**)std::malloc((n + 1) * sizeof(PKCRequest*));
        for (unsigned int i = 0; i < off; ++i) p[i] = m_first[i];
        p[off] = value;
        for (unsigned int i = off; i < n; ++i) p[i + 1] = m_first[i];
        std::free(m_first);
        m_first = p; m_last = p + n + 1; m_end = m_last;
    }
};

void PKCThread::Join()
{
    Log('J');
    g_sawStop = m_pQueue->size() == 2 && m_pQueue->m_last[-1] == 0;   // the stop request is queued last
    g_pendingKept = m_pQueue->size() == 2 && m_pQueue->m_first[0] != 0 && m_pQueue->m_first[0]->m_id == 7;
}

class PKCWorkerQueue {
public:
    ~PKCWorkerQueue()
    {
        {
            CScopedLock lock(&m_section);
            m_requests.insert(m_requests.end(), 0);
            m_semaphore.Release();
        }
        m_pThread->Join();
        delete m_pThread;
    }
    PKCCriticalSection m_section;
    RequestVector      m_requests;
    PKCSemaphore       m_semaphore;
    PKCThread*         m_pThread;
};

int main()
{
    PKCRequest pending = { 7 };
    {
        PKCWorkerQueue* q = new PKCWorkerQueue;
        q->m_semaphore.m_count = 0;
        q->m_requests.insert(q->m_requests.end(), &pending);
        q->m_pThread = new PKCThread;
        q->m_pThread->m_pQueue = &q->m_requests;
        q->m_pThread->m_sawStop = 0;
        delete q;
    }
    // L insert R U, J, D (thread), then members in reverse: S (semaphore), V (vector storage), C (section)
    const char* expect = "LRUJDSVC";
    for (int i = 0; expect[i] || g_log[i]; ++i)
        if (expect[i] != g_log[i]) { std::printf("00c14560_TEST FAIL\n"); return 1; }
    if (!g_sawStop || !g_pendingKept) { std::printf("00c14560_TEST FAIL\n"); return 1; }
    std::printf("00c14560_TEST PASS\n");
    return 0;
}