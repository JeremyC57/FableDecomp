// Link test for 0x012242A0: links the source object's CAsyncLoadingMemoryPool::LoadingCallbacks (STLport
// vector<CAsyncLoadingProcessCallback*>, 12 bytes); its initializer must leave it empty, and the atexit destructor thunk
// it registers frees the block handed to it here at exit.
#include <cstdio>
#include <cstdlib>
class CAsyncLoadingProcessCallback;
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector();
    ~vector();
};
}
class CAsyncLoadingMemoryPool {
public:
    static std::vector<CAsyncLoadingProcessCallback*> LoadingCallbacks;   // defined by the source object
};
int main()
{
    std::vector<CAsyncLoadingProcessCallback*>& v = CAsyncLoadingMemoryPool::LoadingCallbacks;
    int ok = sizeof(v) == 12 && v._M_start == 0 && v._M_finish == 0 && v._M_end_of_storage == 0;
    v._M_start = (CAsyncLoadingProcessCallback**)std::malloc(4 * sizeof(void*));
    v._M_finish = v._M_start + 1;
    v._M_end_of_storage = v._M_start + 4;
    v._M_start[0] = 0;
    if (!ok || !v._M_start) { std::printf("012242a0_TEST FAIL\n"); return 1; }
    std::printf("012242a0_TEST PASS\n");
    return 0;
}
