// Behaviour test for 0x006BEB80 _STL::__unguarded_insertion_sort_aux<pair<long,CMusicManager::CRegisteredMusicEntry>*>.
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("006beb80_TEST FAIL\n"); return 1; } } while (0)

static int g_copies = 0, g_assigns = 0, g_dtors = 0;
class CWideString {                                     // test model: a short owned text
public:
    char Text[16];
    CWideString() { Text[0] = 0; }
    CWideString(const char* s) { std::strncpy(Text, s, 15); Text[15] = 0; }
    CWideString(const CWideString& o) { std::memcpy(Text, o.Text, 16); ++g_copies; }
    ~CWideString() { ++g_dtors; }
    CWideString& operator=(const CWideString& o) { std::memcpy(Text, o.Text, 16); ++g_assigns; return *this; }
    bool Is(const char* s) const { return std::strcmp(Text, s) == 0; }
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template <class _RandomAccessIter, class _Tp, class _Compare>
void unguarded_linear_insert_model(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}
// the function under test: STLport __unguarded_insertion_sort_aux
template <class _RandomAccessIter, class _Tp, class _Compare>
void unguarded_insertion_sort_aux_under_test(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    _RandomAccessIter __i;
    for (__i = __first; __i != __last; ++__i)
        unguarded_linear_insert_model(__i, _Tp(*__i), __comp);
}
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};

class CMusicManager {
public:
    class CRegisteredMusicEntry {
    public:
        CWideString SrcFileName;
        CWideString DstFileName;
        float       Gain;
        float       Offset;
        bool        CacheToHD;
        CRegisteredMusicEntry(const char* s, const char* d, float g, float o, bool c) : SrcFileName(s), DstFileName(d), Gain(g), Offset(o), CacheToHD(c) { }
    };
};
typedef pair<long, CMusicManager::CRegisteredMusicEntry> TMusicEntryPair;

int main()
{
    int i;
    TMusicEntryPair v[5] = { TMusicEntryPair(0, CMusicManager::CRegisteredMusicEntry("s0", "d0", 0.0f, 0.0f, false)),
        TMusicEntryPair(4, CMusicManager::CRegisteredMusicEntry("s4", "d4", 0.4f, 4.0f, true)),
        TMusicEntryPair(2, CMusicManager::CRegisteredMusicEntry("s2", "d2", 0.2f, 2.0f, false)),
        TMusicEntryPair(3, CMusicManager::CRegisteredMusicEntry("s3", "d3", 0.3f, 3.0f, true)),
        TMusicEntryPair(1, CMusicManager::CRegisteredMusicEntry("s1", "d1", 0.1f, 1.0f, true)) };
    unguarded_insertion_sort_aux_under_test(v + 1, v + 5, (TMusicEntryPair*)0, CKeyPairCompareLess<long, CMusicManager::CRegisteredMusicEntry>());
    const char* src[5] = { "s0", "s1", "s2", "s3", "s4" };
    for (i = 0; i < 5; ++i) {
        CHECK(v[i].first == i && v[i].second.SrcFileName.Is(src[i]));
        float g = v[i].second.Gain - 0.1f * i;
        CHECK(g < 0.001f && g > -0.001f);
    }
    CHECK(v[1].second.CacheToHD && !v[2].second.CacheToHD && v[4].second.CacheToHD);
    std::printf("006beb80_TEST PASS\n");
    return 0;
}