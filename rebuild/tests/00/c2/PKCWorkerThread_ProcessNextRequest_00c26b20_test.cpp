// behaviour test for 0x00C26B20 PKCWorkerThread::ProcessNextRequest: wait, pop the front request under the lock, run it; 1 for the null stop request
#include <cstdio>
#include <string.h>

static int g_waits = 0, g_locks = 0, g_unlocks = 0, g_processed = 0, g_lockedDuringProcess = 0;
static int g_depth = 0;

struct CScopedLock {
    void* m_pSection;
    unsigned long m_tick;
    CScopedLock(void* section) : m_pSection(section), m_tick(0) { ++g_locks; ++g_depth; }
    ~CScopedLock() { ++g_unlocks; --g_depth; }
};

struct PKCSemaphore {
    void* m_hSemaphore;
    void Wait() { ++g_waits; }
};

struct PKCRequest { int m_id; };

struct PKCRequestHandler {
    int m_lastId;
    virtual void Process(PKCRequest* request) { ++g_processed; m_lastId = request->m_id; if (g_depth) ++g_lockedDuringProcess; }
};

namespace pkstd {
template<class _Ty> class _Ptrit {
public:
    _Ptrit() { }
    explicit _Ptrit(_Ty* _Ptr) : current(_Ptr) { }
    _Ty& operator*() const { return (*current); }
    _Ty* current;
};

template<class _Ty> class vector {
public:
    typedef _Ptrit<_Ty> iterator;
    unsigned int size() const { return (_Myfirst == 0 ? 0 : _Mylast - _Myfirst); }
    bool empty() const { return (size() == 0); }
    iterator begin() { return (iterator(_Myfirst)); }
    iterator erase(iterator _Where)
    {
        memmove(_Where.current, _Where.current + 1, (_Mylast - (_Where.current + 1)) * sizeof(_Ty));
        --_Mylast;
        return (_Where);
    }
    int  _Alval;
    _Ty* _Myfirst;
    _Ty* _Mylast;
    _Ty* _Myend;
};
}

struct PKCWorkerThread {
    virtual ~PKCWorkerThread() { }
    virtual long ProcessNextRequest();
    PKCRequestHandler*         m_pHandler;
    PKCSemaphore*             m_pSemaphore;
    void*                     m_pSection;
    pkstd::vector<PKCRequest*>* m_pRequests;
};

long PKCWorkerThread::ProcessNextRequest()
{
    m_pSemaphore->Wait();
    long stop = 0;
    PKCRequest* request = 0;
    {
        CScopedLock lock(m_pSection);
        if (!m_pRequests->empty()) {
            pkstd::vector<PKCRequest*>::iterator first = m_pRequests->begin();
            request = *first;
            m_pRequests->erase(first);
            if (!request)
                stop = 1;
        }
    }
    if (request)
        m_pHandler->Process(request);
    return stop;
}

int main()
{
    PKCRequest r1 = { 11 }, r2 = { 22 };
    PKCRequest* slots[4] = { &r1, &r2, 0, 0 };
    pkstd::vector<PKCRequest*> requests; requests._Myfirst = slots; requests._Mylast = slots + 3; requests._Myend = slots + 4;
    PKCRequestHandler handler; handler.m_lastId = 0;
    PKCSemaphore sem; int section = 0;
    PKCWorkerThread t; t.m_pHandler = &handler; t.m_pSemaphore = &sem; t.m_pSection = &section; t.m_pRequests = &requests;
    long a = t.ProcessNextRequest();             // r1
    bool ok = a == 0 && handler.m_lastId == 11 && requests.size() == 2 && slots[0] == &r2;
    long b = t.ProcessNextRequest();             // r2
    ok = ok && b == 0 && handler.m_lastId == 22 && requests.size() == 1;
    long c = t.ProcessNextRequest();             // the null stop request
    ok = ok && c == 1 && g_processed == 2 && requests.size() == 0;
    long d = t.ProcessNextRequest();             // empty queue
    ok = ok && d == 0 && g_processed == 2 && g_waits == 4 && g_locks == 4 && g_unlocks == 4 && g_lockedDuringProcess == 0;
    pkstd::vector<PKCRequest*> none; none._Myfirst = 0; none._Mylast = 0; none._Myend = 0;   // never allocated
    t.m_pRequests = &none;
    ok = ok && t.ProcessNextRequest() == 0 && g_processed == 2;
    if (!ok) { std::printf("00c26b20_TEST FAIL\n"); return 1; }
    std::printf("00c26b20_TEST PASS\n");
    return 0;
}