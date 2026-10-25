// Behaviour test for 0x0052BB40: STLport __uninitialized_copy<const CEntityScriptInfo*, CEntityScriptInfo*> -- copy-construct
// [first, last) into raw storage (deep copies of the script name and the SaveData bytes); returns the end.
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

static int gBlocks = 0;                                 // live byte blocks of the SaveData vectors
namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)std::memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { if (__n != 0) ++gBlocks; return __n != 0 ? (_Tp*)std::malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) --gBlocks; std::free(__p); }
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
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp> inline _Tp* __uninitialized_copy(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    vector(const vector& __x) : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const _Tp*)__x._M_start, (const _Tp*)__x._M_finish, this->_M_start, __true_type());
    }
    vector(const _Tp* __p, unsigned int __n) : _Vector_base<_Tp, _Alloc>(__n, allocator_type())   // test constructor
    {
        this->_M_finish = __uninitialized_copy(__p, __p + __n, this->_M_start, __true_type());
    }
    ~vector() { }
};
}

template <class T> class CArray : public std::vector<T> {
public:
    CArray(const T* p, unsigned int n) : std::vector<T>(p, n) { }   // test constructor
};

namespace NThingManager {
class CEntityRuntimePersistence {
public:
    class CEntityScriptInfo {
    public:
        CCharString           Script;
        CArray<unsigned char> SaveData;
        long                  UncompressedSize;
        bool                  Valid;
        CEntityScriptInfo(const char* s, const unsigned char* d, unsigned int n, long size, bool valid)   // test constructor
            : Script(s), SaveData(d, n), UncompressedSize(size), Valid(valid) { }
    };
};
}
typedef NThingManager::CEntityRuntimePersistence::CEntityScriptInfo CEntityScriptInfo;

namespace std {
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                         const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
}

static bool SameRecord(const CEntityScriptInfo& c, const CEntityScriptInfo& x)
{
    if (c.Script.PStringData == x.Script.PStringData || std::strcmp(c.Script.PStringData, x.Script.PStringData) != 0)
        return false;
    if (c.SaveData.size() != x.SaveData.size() || c.UncompressedSize != x.UncompressedSize || c.Valid != x.Valid)
        return false;
    if (x.SaveData.size() != 0 && (c.SaveData._M_start == x.SaveData._M_start ||
                                   std::memcmp(c.SaveData._M_start, x.SaveData._M_start, x.SaveData.size()) != 0))
        return false;
    return true;
}

int main()
{
    int live = LiveObjects();
    bool ok = true;
    {
        static const unsigned char b0[3] = { 1, 2, 3 };
        static const unsigned char b2[6] = { 4, 5, 6, 7, 8, 9 };
        CEntityScriptInfo* src = (CEntityScriptInfo*)std::malloc(3 * sizeof(CEntityScriptInfo));
        new (src + 0) CEntityScriptInfo("Alpha", b0, 3, 30, true);
        new (src + 1) CEntityScriptInfo("Beta", b0, 0, 0, false);
        new (src + 2) CEntityScriptInfo("Gamma", b2, 6, 60, true);
        CEntityScriptInfo* dst = (CEntityScriptInfo*)std::malloc(3 * sizeof(CEntityScriptInfo));
        CEntityScriptInfo* end = std::__uninitialized_copy((const CEntityScriptInfo*)src, (const CEntityScriptInfo*)src + 3, dst,
                                                            std::__false_type());
        ok = end == dst + 3 && LiveObjects() == live + 6 && gBlocks == 4;
        for (int i = 0; ok && i < 3; ++i)
            if (!SameRecord(dst[i], src[i]))
                ok = false;
        CEntityScriptInfo* none = std::__uninitialized_copy((const CEntityScriptInfo*)src, (const CEntityScriptInfo*)src, dst,
                                                             std::__false_type());
        ok = ok && none == dst;
        for (int i = 0; i < 3; ++i) {
            dst[i].~CEntityScriptInfo();
            src[i].~CEntityScriptInfo();
        }
        std::free(dst);
        std::free(src);
    }
    if (!ok || LiveObjects() != live || gBlocks != 0) {
        std::printf("0052bb40_TEST FAIL\n");
        return 1;
    }
    std::printf("0052bb40_TEST PASS\n");
    return 0;
}