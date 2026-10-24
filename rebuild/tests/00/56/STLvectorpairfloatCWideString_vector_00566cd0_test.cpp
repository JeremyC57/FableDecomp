// Behaviour test for 00566cd0: a copy of the source's model and logic with its out-of-line callees stubbed.
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
static int g_copies = 0;
class CWideString {
public:
    int PData;
    CWideString(int v) : PData(v) { }
    CWideString(const CWideString& other) : PData(other.PData) { ++g_copies; }
    ~CWideString() { }
};
struct __false_type { __false_type() { } };
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& p) : first(p.first), second(p.second) { }
};
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)std::malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
typedef pair<float, CWideString> SScoreRow;
static int g_ucopyCalls = 0;
static SScoreRow* __uninitialized_copy(const SScoreRow* first, const SScoreRow* last, SScoreRow* result,
                                       const __false_type&)            // stub for 0x005654B0
{
    ++g_ucopyCalls;
    for (; first != last; ++first, ++result)
        new (result) SScoreRow(*first);
    return result;
}
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    vector(_Tp* first, _Tp* last) : _Vector_base<_Tp, _Alloc>((unsigned int)(last - first), allocator_type())
    {
        this->_M_finish = __uninitialized_copy(first, last, this->_M_start, __false_type());
    }
    vector(const vector& __x) : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const _Tp*)__x._M_start, (const _Tp*)__x._M_finish, this->_M_start,
                                               __false_type());
    }
};
static void RunTest()
{
    SScoreRow rows[3] = { SScoreRow(1.5f, CWideString(10)), SScoreRow(2.5f, CWideString(20)), SScoreRow(-3.0f, CWideString(30)) };
    vector<SScoreRow> src(rows, rows + 3);
    g_copies = 0;
    g_ucopyCalls = 0;
    vector<SScoreRow> copy(src);
    CHECK(copy.size() == 3 && copy._M_start != src._M_start && copy._M_end_of_storage._M_data == copy._M_start + 3);
    CHECK(copy._M_start[0].first == 1.5f && copy._M_start[2].first == -3.0f && copy._M_start[1].second.PData == 20);
    CHECK(g_copies == 3 && g_ucopyCalls == 1);
    vector<SScoreRow> none(rows, rows);              // an empty source allocates nothing
    vector<SScoreRow> copy2(none);
    CHECK(copy2._M_start == 0 && copy2._M_finish == 0 && copy2._M_end_of_storage._M_data == 0);
    std::free(src._M_start);
    std::free(copy._M_start);
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("00566cd0_TEST FAIL\n");
        return 1;
    }
    std::printf("00566cd0_TEST PASS\n");
    return 0;
}