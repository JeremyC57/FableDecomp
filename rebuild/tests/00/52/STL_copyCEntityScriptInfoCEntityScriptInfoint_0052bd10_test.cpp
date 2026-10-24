// Behaviour test for 0052bd10: a copy of the source's model and logic with its out-of-line callees stubbed.
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
static int g_strCopies = 0, g_strAssigns = 0, g_strDtors = 0, g_vecAssigns = 0, g_vecFrees = 0;
class CCharString {
public:
    int PData;
    CCharString(int v) : PData(v) { }
    CCharString(const CCharString& other) : PData(other.PData) { ++g_strCopies; }
    ~CCharString() { ++g_strDtors; }
    CCharString& operator=(const CCharString& other) { PData = other.PData; ++g_strAssigns; return *this; }   // stub for 0x0099EFB0
};
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class T> class CArray {                // retail CArray<T>: an STLport vector (start, finish, end of storage)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CArray(const T* data, unsigned int n) : _M_start(0), _M_finish(0), _M_end_of_storage(0) { Assign(data, n); }
    CArray(const CArray& x) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        Assign(x._M_start, (unsigned int)(x._M_finish - x._M_start));
    }
    ~CArray() { if (_M_start != 0) { std::free(_M_start); ++g_vecFrees; } }
    CArray& operator=(const CArray& x)           // stub for 0x005295B0
    {
        ++g_vecAssigns;
        if (this != &x) {
            if (_M_start != 0) std::free(_M_start);
            _M_start = _M_finish = _M_end_of_storage = 0;
            Assign(x._M_start, (unsigned int)(x._M_finish - x._M_start));
        }
        return *this;
    }
    void Assign(const T* data, unsigned int n)
    {
        if (n != 0) {
            _M_start = (T*)std::malloc(n * sizeof(T));
            std::memcpy(_M_start, data, n * sizeof(T));
        }
        _M_finish = _M_end_of_storage = _M_start + n;
    }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
};
struct CEntityScriptInfo {                       // NThingManager::CEntityRuntimePersistence::CEntityScriptInfo
    CCharString           Script;
    CArray<unsigned char> SaveData;
    long                  UncompressedSize;
    bool                  Valid;
    CEntityScriptInfo(int id, const unsigned char* data, unsigned int n)
        : Script(id), SaveData(data, n), UncompressedSize(id * 100), Valid((id & 1) != 0) { }
};
static const unsigned char g_bytes[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
static CEntityScriptInfo* MakeRange(int n, int base)   // n records in raw storage, ids base..base+n-1
{
    CEntityScriptInfo* p = (CEntityScriptInfo*)std::malloc(n * sizeof(CEntityScriptInfo));
    for (int i = 0; i < n; ++i)
        new (p + i) CEntityScriptInfo(base + i, g_bytes, (unsigned int)(i + 1));
    return p;
}
static bool SameRecord(const CEntityScriptInfo& a, const CEntityScriptInfo& b)
{
    return a.Script.PData == b.Script.PData && a.SaveData.size() == b.SaveData.size() &&
           (a.SaveData.size() == 0 || std::memcmp(a.SaveData._M_start, b.SaveData._M_start, a.SaveData.size()) == 0) &&
           a.SaveData._M_start != b.SaveData._M_start && a.UncompressedSize == b.UncompressedSize && a.Valid == b.Valid;
}
static void DestroyRange(CEntityScriptInfo* p, int n)
{
    for (int i = 0; i < n; ++i)
        p[i].~CEntityScriptInfo();
    std::free(p);
}
static void ResetCounters() { g_strCopies = g_strAssigns = g_strDtors = g_vecAssigns = g_vecFrees = 0; }

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                   _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
static void RunTest()
{
    CEntityScriptInfo* src = MakeRange(3, 10);
    CEntityScriptInfo* dst = MakeRange(4, 50);
    ResetCounters();
    CEntityScriptInfo* end = __copy<CEntityScriptInfo*, CEntityScriptInfo*, int>(src, src + 3, dst, random_access_iterator_tag(), (int*)0);
    CHECK(end == dst + 3);
    CHECK(SameRecord(src[0], dst[0]) && SameRecord(src[1], dst[1]) && SameRecord(src[2], dst[2]));
    CHECK(dst[3].Script.PData == 53);                 // untouched past the copied count
    CHECK(g_strAssigns == 3 && g_vecAssigns == 3 && g_strCopies == 0);
    // overlapping forward copy (the erase use): shift records 1..3 down to 0..2
    ResetCounters();
    end = __copy<CEntityScriptInfo*, CEntityScriptInfo*, int>(dst + 1, dst + 4, dst, random_access_iterator_tag(), (int*)0);
    CHECK(end == dst + 3 && dst[0].Script.PData == 11 && dst[1].Script.PData == 12 && dst[2].Script.PData == 53);
    // empty range
    ResetCounters();
    end = __copy<CEntityScriptInfo*, CEntityScriptInfo*, int>(src, src, dst, random_access_iterator_tag(), (int*)0);
    CHECK(end == dst && g_strAssigns == 0);
    DestroyRange(src, 3);
    DestroyRange(dst, 4);
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("0052bd10_TEST FAIL\n");
        return 1;
    }
    std::printf("0052bd10_TEST PASS\n");
    return 0;
}