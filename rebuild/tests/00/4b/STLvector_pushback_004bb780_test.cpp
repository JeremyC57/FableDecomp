// Behaviour test for 0x004BB780: STLport vector<pair<CCharString, CPersistInfo> >::push_back(x) -- copy-construct at the
// end while there is room, else grow through _M_insert_overflow (test copy of 0x004BB400).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

// Test stand-in for CCharString: an owned C string (retail shares a CBasicString<char> block). The compare keeps
// retail's inline CCharString::operator< null rules and the CBasicString<char>::operator< 0x00429950 loop.
static int gLiveStrings = 0;
class CCharString {
public:
    char* PStringData;
    CCharString(const char* s) : PStringData(Dup(s)) { ++gLiveStrings; }
    CCharString(const CCharString& other) : PStringData(Dup(other.PStringData)) { ++gLiveStrings; }
    ~CCharString() { std::free(PStringData); --gLiveStrings; }
    CCharString& operator=(const CCharString& other)
    {
        if (this != &other) {
            char* p = Dup(other.PStringData);
            std::free(PStringData);
            PStringData = p;
        }
        return *this;
    }
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (PStringData == 0)
            return true;
        if (other.PStringData == 0)
            return false;
        const char* b = other.PStringData;
        const char* a = PStringData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return false;
            if (*a < *b)
                return true;
            if (*a > *b)
                return false;
            ++a;
            ++b;
        }
    }
    static char* Dup(const char* s)
    {
        if (s == 0)
            return 0;
        unsigned int n = (unsigned int)std::strlen(s) + 1;
        char* p = (char*)std::malloc(n);
        std::memcpy(p, s, n);
        return p;
    }
};
static CCharString MkString(char prefix, int v)        // v < 0 -> the null string
{
    if (v < 0)
        return CCharString(0);
    char buf[16];
    if (prefix)
        std::sprintf(buf, "%c%d", prefix, v);
    else
        std::sprintf(buf, "%04d", v);
    return CCharString(buf);
}
static int StringValue(const CCharString& s, char prefix)
{
    if (s.PStringData == 0)
        return -1;
    if (prefix)
        return s.PStringData[0] == prefix ? std::atoi(s.PStringData + 1) : -1000;
    return std::atoi(s.PStringData);
}
static int LiveObjects() { return gLiveStrings; }

namespace NQuestManager {
class CRuntimeQuestPersistence {
public:
    class CPersistInfo {
    public:
        CCharString QuestName;
        CCharString PersistScript;
        CPersistInfo(const CCharString& q, const CCharString& s) : QuestName(q), PersistScript(s) { }
    };
};
}
typedef NQuestManager::CRuntimeQuestPersistence::CPersistInfo TPersistInfo;
static int gMallocs = 0;
static int gFrees = 0;

namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend)
    {                                                   // test copy of 0x004BB400 (STLport 4.x growth: size + max(size, n))
        unsigned int __old_size = size();
        unsigned int __len = __old_size + (__old_size > __fill_len ? __old_size : __fill_len);
        _Tp* __new_start = (_Tp*)std::malloc(__len * sizeof(_Tp));
        ++gMallocs;
        _Tp* __new_finish = __new_start;
        for (_Tp* __p = _M_start; __p != __position; ++__p, ++__new_finish)
            _Construct(__new_finish, *__p);
        for (unsigned int __i = 0; __i < __fill_len; ++__i, ++__new_finish)
            _Construct(__new_finish, __x);
        if (!__atend)
            for (_Tp* __p = __position; __p != _M_finish; ++__p, ++__new_finish)
                _Construct(__new_finish, *__p);
        for (_Tp* __p = _M_start; __p != _M_finish; ++__p)
            __p->~_Tp();
        if (_M_start) {
            ++gFrees;
            std::free(_M_start);
        }
        _M_start = __new_start;
        _M_finish = __new_finish;
        _M_end_of_storage._M_data = __new_start + __len;
    }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};
}
typedef std::pair<CCharString, TPersistInfo> TElem;
typedef std::vector<TElem> TVec;

int main()
{
    int live = LiveObjects();
    bool ok = true;
    TVec v;
    v._M_start = 0;
    v._M_finish = 0;
    v._M_end_of_storage._M_data = 0;
    static const int caps[7] = { 1, 2, 4, 4, 8, 8, 8 };
    for (int i = 0; i < 7; ++i) {
        TElem e(MkString(0, 100 - i), TPersistInfo(MkString('q', i), MkString('s', i)));
        v.push_back(e);
        if (v.size() != (unsigned int)(i + 1) || v._M_end_of_storage._M_data - v._M_start != caps[i])
            ok = false;
    }
    ok = ok && gMallocs == 4 && gFrees == 3 && LiveObjects() == live + 21;
    for (int i = 0; ok && i < 7; ++i)
        if (StringValue(v._M_start[i].first, 0) != 100 - i || StringValue(v._M_start[i].second.QuestName, 'q') != i ||
            StringValue(v._M_start[i].second.PersistScript, 's') != i)
            ok = false;
    for (TElem* p = v._M_start; p != v._M_finish; ++p)
        p->~TElem();
    std::free(v._M_start);
    if (!ok || LiveObjects() != live) {
        std::printf("004bb780_TEST FAIL\n");
        return 1;
    }
    std::printf("004bb780_TEST PASS\n");
    return 0;
}