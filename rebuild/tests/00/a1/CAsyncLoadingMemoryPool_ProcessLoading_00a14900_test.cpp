// Behaviour test for CAsyncLoadingMemoryPool::ProcessLoading (0x00A14900): every registered callback runs once,
// in registration order, and an empty list runs nothing.
#include <cstdio>

namespace std {
template <class _Tp> class allocator { public: allocator() {} };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy { public: _Tp _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
    typedef _Tp* iterator;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
};
}

class CAsyncLoadingProcessCallback {
public:
    virtual void ProcessLoading() = 0;
};

class CAsyncLoadingMemoryPool {
public:
    static void ProcessLoading();
    static std::vector<CAsyncLoadingProcessCallback*> LoadingCallbacks;
};

std::vector<CAsyncLoadingProcessCallback*> CAsyncLoadingMemoryPool::LoadingCallbacks;

void CAsyncLoadingMemoryPool::ProcessLoading()
{
    for (std::vector<CAsyncLoadingProcessCallback*>::iterator it = LoadingCallbacks.begin(); it != LoadingCallbacks.end(); ++it)
        (*it)->ProcessLoading();
}

static int g_order[8];
static int g_count = 0;

class CCountingCallback : public CAsyncLoadingProcessCallback {
public:
    int Id;
    int Calls;
    CCountingCallback(int id) : Id(id), Calls(0) {}
    virtual void ProcessLoading() { ++Calls; if (g_count < 8) g_order[g_count++] = Id; }
};

int main()
{
    CCountingCallback a(1), b(2), c(3);
    CAsyncLoadingProcessCallback* list[3] = { &a, &b, &c };
    int ok = 1;

    CAsyncLoadingMemoryPool::LoadingCallbacks._M_start = list;
    CAsyncLoadingMemoryPool::LoadingCallbacks._M_finish = list;          // empty
    CAsyncLoadingMemoryPool::ProcessLoading();
    if (g_count != 0) ok = 0;

    CAsyncLoadingMemoryPool::LoadingCallbacks._M_finish = list + 3;
    CAsyncLoadingMemoryPool::ProcessLoading();
    if (g_count != 3 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 3) ok = 0;
    if (a.Calls != 1 || b.Calls != 1 || c.Calls != 1) ok = 0;

    if (!ok) { std::printf("00a14900_TEST FAIL\n"); return 1; }
    std::printf("00a14900_TEST PASS\n");
    return 0;
}