// Behaviour test for 0x004BB6B0: CVectorMap<CCharString, CPersistInfo, CKeyPairCompareLess>::DeallocateMemory() --
// destroy every entry, give the block back by swapping with an empty vector, clear the dirty flag.
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
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _Tp> inline void swap(_Tp& __a, _Tp& __b) { _Tp __tmp = __a; __a = __b; __b = __tmp; }
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                   const random_access_iterator_tag&, _Distance*)                  // test copy of 0x004B84F0
{
    for (_Distance __n = (_Distance)(__last - __first); __n > 0; --__n, ++__result, ++__first)
        *__result = *__first;
    return __result;
}
template <class _Tp> inline void _Destroy1(_Tp* __p) { __p->~_Tp(); }
template <class _ForwardIterator>
void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)   // test copy of 0x004B93E0
{
    for ( ; __first != __last; ++__first)
        _Destroy1(&*__first);
}
template <class _Tp> inline _Tp* copy(_Tp* __first, _Tp* __last, _Tp* __result)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p) { ++gFrees; std::free(__p); } }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    _Tp* begin() { return this->_M_start; }
    _Tp* end() { return this->_M_finish; }
    _Tp* erase(_Tp* __first, _Tp* __last)
    {
        _Tp* __i = copy(__last, this->_M_finish, __first);
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
    void swap(vector& __x)
    {
        std::swap(this->_M_start, __x._M_start);
        std::swap(this->_M_finish, __x._M_finish);
        std::swap(this->_M_end_of_storage, __x._M_end_of_storage);
    }
};
}
template <class K, class V> class CKeyPairCompareLess {
};
template <class K, class V, class C> class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    C    Compare;
    bool Dirty;
    void DeallocateMemory()
    {
        this->clear();
        std::vector<std::pair<K, V> > empty;
        this->swap(empty);
        Dirty = false;
    }
};
typedef std::pair<CCharString, TPersistInfo> TElem;
typedef CVectorMap<CCharString, TPersistInfo, CKeyPairCompareLess<CCharString, TPersistInfo> > TMap;

int main()
{
    int live = LiveObjects();
    bool ok = true;
    {
        TMap m;
        TElem* p = (TElem*)std::malloc(8 * sizeof(TElem));
        ++gMallocs;
        for (int i = 0; i < 5; ++i)
            new (p + i) TElem(MkString(0, i), TPersistInfo(MkString('q', i), MkString('s', i)));
        m._M_start = p;
        m._M_finish = p + 5;
        m._M_end_of_storage._M_data = p + 8;
        m.Dirty = true;
        ok = LiveObjects() == live + 15;
        m.DeallocateMemory();
        ok = ok && m._M_start == 0 && m._M_finish == 0 && m._M_end_of_storage._M_data == 0 && !m.Dirty &&
             LiveObjects() == live && gFrees == 1;
        m.DeallocateMemory();                           // already empty: nothing more to free
        ok = ok && m._M_start == 0 && gFrees == 1;
    }
    if (!ok || LiveObjects() != live || gFrees != gMallocs) {
        std::printf("004bb6b0_TEST FAIL\n");
        return 1;
    }
    std::printf("004bb6b0_TEST PASS\n");
    return 0;
}