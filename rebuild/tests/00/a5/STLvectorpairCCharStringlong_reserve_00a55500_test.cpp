// Behaviour test for 0x00A55500: _STL::vector<pair<CCharString, long> >::reserve (reallocate to n keeping the elements)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

static int g_fail = 0;
static void Check(bool c) { if (!c) g_fail = 1; }
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

static int g_liveStrings = 0;
class CCharString {                                   // test model: a deep-copying string with a live count
public:
    char* m_pData;
    CCharString() : m_pData(0) { ++g_liveStrings; }
    CCharString(const char* s) : m_pData(Dup(s)) { ++g_liveStrings; }
    CCharString(const CCharString& other) : m_pData(Dup(other.m_pData)) { ++g_liveStrings; }
    ~CCharString() { free(m_pData); --g_liveStrings; }
    CCharString& operator=(const CCharString& other)
    {
        if (this != &other) {
            char* p = Dup(other.m_pData);
            free(m_pData);
            m_pData = p;
        }
        return *this;
    }
    const char* c_str() const { return m_pData ? m_pData : ""; }
    bool operator<(const CCharString& o) const { return strcmp(c_str(), o.c_str()) < 0; }
    static char* Dup(const char* s)
    {
        if (s == 0) return 0;
        char* p = (char*)malloc(strlen(s) + 1);
        strcpy(p, s);
        return p;
    }
};
static bool Eq(const CCharString& s, const char* t) { return strcmp(s.c_str(), t) == 0; }

namespace _STL {
struct __false_type { __false_type() { } };
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _T1, class _T2>
class pair {
public:
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) { }
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    for ( ; __first != __last; ++__first, ++__result)
        _Construct(&*__result, *__first);
    return __result;
}
template <class _Tp>
class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    vector() { this->_M_start = this->_M_finish = this->_M_end_of_storage._M_data = 0; }
    ~vector() { if (this->_M_start) _M_clear(); }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    unsigned int capacity() const { return (unsigned int)(this->_M_end_of_storage._M_data - this->_M_start); }
    void _M_clear()                                  // test stand-in for the out-of-line 0x006FBE60
    {
        for (_Tp* p = this->_M_start; p != this->_M_finish; ++p)
            p->~_Tp();
        free(this->_M_start);
    }
    void _M_set(_Tp* __s, _Tp* __f, _Tp* __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    _Tp* _M_allocate_and_copy(unsigned int __n, const _Tp* __first, const _Tp* __last)
    {
        _Tp* __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
    void push_back(const _Tp& x) { _Construct(this->_M_finish, x); ++this->_M_finish; }
    void reserve(unsigned int __n);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::reserve(unsigned int __n)
{
    if (capacity() < __n) {
        const unsigned int __old_size = size();
        _Tp* __tmp;
        if (this->_M_start) {
            __tmp = _M_allocate_and_copy(__n, this->_M_start, this->_M_finish);
            _M_clear();
        }
        else {
            __tmp = this->_M_end_of_storage.allocate(__n);
        }
        _M_set(__tmp, __tmp + __old_size, __tmp + __n);
    }
}
}

typedef _STL::pair<CCharString, long> TEntry;

int main()
{
    {
        _STL::vector<TEntry> v;
        v.reserve(3u);
        Check(v._M_start != 0 && v.size() == 0 && v.capacity() == 3);
        v.push_back(TEntry(CCharString("one"), 1));
        v.push_back(TEntry(CCharString("two"), 2));
        TEntry* old = v._M_start;
        v.reserve(2u);                               // no-op
        Check(v._M_start == old && v.capacity() == 3);
        int live = g_liveStrings;
        v.reserve(8u);
        Check(g_liveStrings == live && v.size() == 2 && v.capacity() == 8);
        Check(Eq(v._M_start[0].first, "one") && v._M_start[0].second == 1);
        Check(Eq(v._M_start[1].first, "two") && v._M_start[1].second == 2);
    }
    Check(g_liveStrings == 0);
    if (g_fail) {
        printf("00a55500_TEST FAIL\n");
        return 1;
    }
    printf("00a55500_TEST PASS\n");
    return 0;
}