// Behaviour test for 0053cfc2: a copy of the source's model and logic with its out-of-line callees stubbed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_fail = 0;
static void Check(bool ok, int line)
{
    if (!ok) {
        g_fail = 1;
        std::printf("check failed at line %d\n", line);
    }
}
#define CHECK(c) Check((c) ? true : false, __LINE__)

#include <new>
static int g_mapCopies = 0, g_overflows = 0;
struct __false_type { __false_type() {} };
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
class CUIStateMap {                               // map<unsigned long, NUISystem::CUIState>
public:
    void*         _M_header;
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    CUIStateMap(unsigned long n) : _M_header(0), _M_node_count(n), _M_key_compare(0) { }
    CUIStateMap(const CUIStateMap& x) : _M_header(0), _M_node_count(x._M_node_count), _M_key_compare(0) { ++g_mapCopies; }   // stub for 0x0053CB9B
};
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)   // stub for 0x0053CEF6
    {
        ++g_overflows;
        CHECK(position == _M_finish && fill_len == 1 && atend);
        unsigned int n = (unsigned int)(_M_finish - _M_start);
        T* p = (T*)std::malloc((n * 2 + 1) * sizeof(T));
        if (n) std::memcpy((void*)p, (void*)_M_start, n * sizeof(T));
        _Construct(p + n, x);
        std::free(_M_start);
        _M_start = p;
        _M_finish = p + n + 1;
        _M_end_of_storage = p + n * 2 + 1;
    }
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);
    }
};
static void RunTest()
{
    vector<CUIStateMap> v;
    v._M_start = v._M_finish = v._M_end_of_storage = 0;
    for (unsigned long i = 0; i < 5; ++i)
        v.push_back(CUIStateMap(i + 100));
    CHECK(v._M_finish - v._M_start == 5 && g_mapCopies == 5);
    CHECK(v._M_start[0]._M_node_count == 100 && v._M_start[4]._M_node_count == 104);
    CHECK(g_overflows == 3);                    // capacity 0 -> 1 -> 3 -> 7: grows at elements 0, 1 and 3
    std::free(v._M_start);
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("0053cfc2_TEST FAIL\n");
        return 1;
    }
    std::printf("0053cfc2_TEST PASS\n");
    return 0;
}