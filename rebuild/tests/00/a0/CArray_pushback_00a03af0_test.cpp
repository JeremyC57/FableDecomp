// Behaviour test for 0x00A03AF0 CArray<CHandleInfo>::push_back(): appends a default (null) slot in place, or through the growing insert when full.
#include <cstdio>
#include <new>

struct SScalarTag { SScalarTag() { } };

class CHandleBasedMemoryPool {
public:
    class CHandleInfo {
    public:
        union {
            void* PArea;
            long  NextUnusedHandle;
        };
        CHandleInfo() : PArea(0) { }
    };
};
typedef CHandleBasedMemoryPool::CHandleInfo CHandleInfo;

static int g_grows = 0;
static CHandleInfo g_storage[8];

template <class T> class CArray {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* pos, const T& x, const SScalarTag& tag, unsigned long fillLen, bool atEnd)
    {
        // test stand-in: "grow" by moving the end of storage within the fixed buffer
        ++g_grows;
        (void)tag; (void)atEnd;
        _M_end_of_storage = _M_end_of_storage + 2;
        for (unsigned long i = 0; i < fillLen; ++i)
            pos[i] = x;
        _M_finish = pos + fillLen;
    }
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, SScalarTag(), 1, true);
    }
    void push_back() { push_back(T()); }
};

int main()
{
    for (int i = 0; i < 8; ++i) g_storage[i].NextUnusedHandle = 77;     // non-null garbage
    CArray<CHandleInfo> a;
    a._M_start = g_storage; a._M_finish = g_storage; a._M_end_of_storage = g_storage + 2;
    a.push_back();
    a.push_back();
    if (a._M_finish - a._M_start != 2 || g_grows != 0 || g_storage[0].PArea != 0 || g_storage[1].PArea != 0) { std::printf("00a03af0_TEST FAIL\n"); return 1; }
    a.push_back();                                                     // full: through the insert
    if (a._M_finish - a._M_start != 3 || g_grows != 1 || g_storage[2].PArea != 0 || a._M_end_of_storage != g_storage + 4) { std::printf("00a03af0_TEST FAIL\n"); return 1; }
    a.push_back();
    if (a._M_finish - a._M_start != 4 || g_grows != 1 || g_storage[3].PArea != 0 || g_storage[4].NextUnusedHandle != 77) { std::printf("00a03af0_TEST FAIL\n"); return 1; }
    std::printf("00a03af0_TEST PASS\n");
    return 0;
}