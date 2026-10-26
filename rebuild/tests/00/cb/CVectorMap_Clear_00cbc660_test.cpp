#include <cstdio>
#include <cstdlib>
#include <new>
// Behaviour of CVectorMap::Clear (0x00CBC660): all entries destroyed, storage freed, the vector emptied, Dirty cleared.
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static long g_strings = 0;
static long g_frees = 0;
class CCharString { public: long m_id; CCharString(long id) : m_id(id) { ++g_strings; } ~CCharString() { --g_strings; } };
struct CPersistInfo { u64 UID; CCharString PersistScript; CPersistInfo(u64 uid, long id) : UID(uid), PersistScript(id) {} };
template <class K, class V> struct SPair { K first; V second; SPair(K k, const V& v) : first(k), second(v.UID, v.PersistScript.m_id) {} };
template <class T> inline void Swap(T& left, T& right) { T tmp = left; left = right; right = tmp; }
template <class T> struct CRetailVector {
    T* _First; T* _Last; T* _End;
    CRetailVector() : _First(0), _Last(0), _End(0) {}
    ~CRetailVector() { _Tidy(); }
    T* begin() { return _First; }
    T* end() { return _Last; }
    T* erase(T* first, T* last)
    {
        T* p;
        for (p = first; p != last; ++p) p->~T();
        T* d = first;
        for (p = last; p != _Last; ++p, ++d) { new (d) T(*p); p->~T(); }
        _Last = d;
        return first;
    }
    void clear() { erase(begin(), end()); }
    void swap(CRetailVector& x) { Swap(_First, x._First); Swap(_Last, x._Last); Swap(_End, x._End); }
    void _Tidy()
    {
        for (T* p = _First; p != _Last; ++p) p->~T();
        if (_First != 0) { std::free(_First); ++g_frees; }
        _First = 0; _Last = 0; _End = 0;
    }
};
struct CKeyPairCompareLess {};
struct CVectorMap {
    CRetailVector< SPair<u64, CPersistInfo> > Data;
    CKeyPairCompareLess Compare;
    bool Dirty;
    void Clear()
    {
        Data.clear();
        {
            CRetailVector< SPair<u64, CPersistInfo> > empty;
            Data.swap(empty);
            Dirty = false;
        }
    }
};
int main()
{
    typedef SPair<u64, CPersistInfo> E;
    CVectorMap m; m.Dirty = true;
    m.Data._First = (E*)std::malloc(sizeof(E) * 4);
    m.Data._End = m.Data._First + 4;
    m.Data._Last = m.Data._First;
    for (long i = 0; i < 3; ++i) { CPersistInfo info(100 + i, i); new (m.Data._Last) E(5 + i, info); ++m.Data._Last; }
    if (g_strings != 3) { std::printf("00cbc660_TEST FAIL\n"); return 1; }
    m.Clear();
    if (g_strings != 0 || g_frees != 1 || m.Data._First != 0 || m.Data._Last != 0 || m.Data._End != 0 || m.Dirty) { std::printf("00cbc660_TEST FAIL\n"); return 1; }
    m.Dirty = true; m.Clear();                       // empty map: nothing freed, flag still cleared
    if (g_frees != 1 || m.Dirty) { std::printf("00cbc660_TEST FAIL\n"); return 1; }
    std::printf("00cbc660_TEST PASS\n");
    return 0;
}