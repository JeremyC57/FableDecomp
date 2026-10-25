// Link test for 0x00A6F290: the source object's CVectorMap<CCharString,C3DHelperPoint,...>::LowerBound over a sorted
// HelperPoints array, with CCharString::FastCompare supplied here (length first, then the bytes, as a stand-in order).
#include <cstdio>
#include <cstring>
template <class C> class CBasicString {
public:
    const char* Text;
};
class CCharString {
public:
    CBasicString<char>* PData;
    bool FastCompare(const CCharString& other) const;
};
static int g_compares = 0;
bool CCharString::FastCompare(const CCharString& other) const
{
    ++g_compares;
    unsigned int n = (unsigned int)std::strlen(PData->Text), m = (unsigned int)std::strlen(other.PData->Text);
    if (n != m) return n < m;
    return std::strcmp(PData->Text, other.PData->Text) < 0;
}
class C3DVector {
public:
    float X, Y, Z;
};
class C3DHelperPoint {                                   // 16 bytes (Ego_r.pdb)
public:
    C3DVector Position;                                  // +0x00
    long      BoneIndex;                                 // +0x0C
};
namespace std {
template <class A, class B> struct pair {
    A first;
    B second;
};
template <class T> class vector {
public:
    typedef const T* const_iterator;
    T* _M_start;                                         // +0x00
    T* _M_finish;                                        // +0x04
    T* _M_end_of_storage;                                // +0x08
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }
};
}
template <class V> class CCompareString {
public:
    bool operator()(const std::pair<CCharString, V>& entry, const CCharString& key) const { return entry.first.FastCompare(key); }
};
template <class K, class V, class C> class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    typedef std::pair<K, V> value_type;
    typedef const value_type* const_iterator;
    C    Compare;                                        // +0x0C
    bool Dirty;                                          // +0x0D
    const_iterator LowerBound(const K& key) const;   // from the source object
};
typedef CVectorMap<CCharString, C3DHelperPoint, CCompareString<C3DHelperPoint> > CHelperPointMap;
static CBasicString<char> g_text[8];
static CCharString Name(int i, const char* t) { g_text[i].Text = t; CCharString c; c.PData = &g_text[i]; return c; }
int main()
{
    int ok = sizeof(CHelperPointMap::value_type) == 20 && sizeof(CHelperPointMap) == 16;
    CHelperPointMap::value_type e[4];
    e[0].first = Name(0, "ab");  e[0].second.BoneIndex = 0;
    e[1].first = Name(1, "cd");  e[1].second.BoneIndex = 1;
    e[2].first = Name(2, "abc"); e[2].second.BoneIndex = 2;
    e[3].first = Name(3, "abd"); e[3].second.BoneIndex = 3;
    CHelperPointMap m;
    m._M_start = e; m._M_finish = e + 4; m._M_end_of_storage = e + 4;
    CCharString k;
    k = Name(4, "cd");   ok = ok && m.LowerBound(k) == e + 1;
    k = Name(5, "abc");  ok = ok && m.LowerBound(k) == e + 2;
    k = Name(6, "a");    ok = ok && m.LowerBound(k) == e;
    k = Name(7, "zzzz"); ok = ok && m.LowerBound(k) == e + 4;
    ok = ok && g_compares > 0;
    m._M_finish = e;     ok = ok && m.LowerBound(k) == e;
    if (!ok) { std::printf("00a6f290_TEST FAIL\n"); return 1; }
    std::printf("00a6f290_TEST PASS\n");
    return 0;
}
