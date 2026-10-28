// Behaviour test for 00cbc7b0: vector<(UID, persist info)>::push_back: in-place copy while there is room, else the overflow path
#include <cstdio>
#include <cstring>
#include <new>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
class CCharString {
public:
    const char* m_pText;
    CCharString(const char* text) : m_pText(text) { }
    CCharString(const CCharString& other) : m_pText(other.m_pText) { }
    const CCharString& operator=(const CCharString& other) { m_pText = other.m_pText; return *this; }
};
namespace NScript {
class CRuntimeScriptedEntityPersist {
public:
    class CPersistInfo {
    public:
        u64         UID;
        CCharString PersistScript;
        CPersistInfo(u64 uid, const CCharString& script) : UID(uid), PersistScript(script) { }
    };
};
}
typedef NScript::CRuntimeScriptedEntityPersist::CPersistInfo CPersistInfo;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) { }
};
struct __false_type { __false_type() { } };
template <class _Tp> class vector {                      // STLport layout: start, finish, end of storage
public:
    typedef _Tp* iterator;
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector()
    {
        for (_Tp* p = _M_start; p != _M_finish; ++p) p->~_Tp();
        operator delete(_M_start);
    }
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    size_type size() const { return (size_type)(_M_finish - _M_start); }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend)
    {
        size_type __old = size();
        size_type __len = __old + (__old > __fill_len ? __old : __fill_len);
        _Tp* __new = (_Tp*)operator new(__len * sizeof(_Tp));
        _Tp* __cur = __new;
        _Tp* __p;
        for (__p = _M_start; __p != __position; ++__p, ++__cur) new (__cur) _Tp(*__p);
        for (size_type __n = 0; __n < __fill_len; ++__n, ++__cur) new (__cur) _Tp(__x);
        if (!__atend)
            for (__p = __position; __p != _M_finish; ++__p, ++__cur) new (__cur) _Tp(*__p);
        for (__p = _M_start; __p != _M_finish; ++__p) __p->~_Tp();
        operator delete(_M_start);
        _M_start = __new;
        _M_finish = __cur;
        _M_end_of_storage = __new + __len;
    }
    void push_back(const _Tp& __x);
};
}
typedef std::pair<u64, CPersistInfo> CPersistPair;
// ---- the function under test, copied from src/00cbc7b0.cpp (calling convention dropped) ----
template <class _Tp>
void std::vector<_Tp>::push_back(const _Tp& __x)
{
    if (this->_M_finish != this->_M_end_of_storage) {
        new (this->_M_finish) _Tp(__x);
        ++this->_M_finish;
    }
    else
        _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
}
static int Fail() { std::printf("00cbc7b0_TEST FAIL\n"); return 1; }
int main()
{
    std::vector<CPersistPair> v;
    static const char* names[5] = { "a", "b", "c", "d", "e" };
    unsigned int i;
    for (i = 0; i < 5; ++i)
        v.push_back(CPersistPair((u64)(i + 1), CPersistInfo((u64)(i + 1), CCharString(names[i]))));
    if (v.size() != 5) return Fail();
    for (i = 0; i < 5; ++i)
        if (v._M_start[i].first != i + 1 || v._M_start[i].second.UID != i + 1 || v._M_start[i].second.PersistScript.m_pText != names[i]) return Fail();
    CPersistPair* oldStart = v._M_start;
    unsigned int room = (unsigned int)(v._M_end_of_storage - v._M_finish);
    if (room > 0) {                                      // the in-place branch must not reallocate
        v.push_back(CPersistPair(6, CPersistInfo(6, CCharString("f"))));
        if (v._M_start != oldStart || v.size() != 6 || v._M_start[5].first != 6) return Fail();
    }
    std::printf("00cbc7b0_TEST PASS\n");
    return 0;
}