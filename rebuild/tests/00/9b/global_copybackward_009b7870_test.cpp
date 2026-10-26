// Behaviour test for 009b7870: __copy_backward over pair<unsigned long, CDefinitionManager::CDefClassInfo> -- an
// overlapping shift right by two (the vector-insert use) assigns every element from the back and returns the new start.
#include <cstdio>
#include <cstring>

static int g_stringAssigns = 0;
static int g_arrayAssigns = 0;
class CCharString {
public:
    const char* PStringData;
    CCharString& operator=(const CCharString& other) { PStringData = other.PStringData; ++g_stringAssigns; return *this; }
};
template <class T> class CArray {
public:
    T* Start;
    T* Finish;
    T* EndOfStorage;
    CArray& operator=(const CArray& other) { Start = other.Start; Finish = other.Finish; EndOfStorage = other.EndOfStorage; ++g_arrayAssigns; return *this; }
};
typedef void* (*TDefAllocFunc)();
namespace CDefinitionManager {
class CDefClassInfo { public:
    CCharString           Name;
    TDefAllocFunc         AllocFunc;
    CArray<unsigned long> GlobalIndices;
};
}
struct PairULongDefClassInfo {
    unsigned long                     first;
    CDefinitionManager::CDefClassInfo second;
};
struct random_access_iterator_tag { };

PairULongDefClassInfo* __copy_backward(PairULongDefClassInfo* first, PairULongDefClassInfo* last,
                                        PairULongDefClassInfo* result, const random_access_iterator_tag&, long*)
{
    for (long n = long(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}

static const char* const g_names[6] = { "A", "B", "C", "D", "E", "F" };
static unsigned long g_slots[6];
int main()
{
    PairULongDefClassInfo v[6];
    for (int i = 0; i < 6; ++i) {
        v[i].first = 100 + i;
        v[i].second.Name.PStringData = g_names[i];
        v[i].second.AllocFunc = 0;
        v[i].second.GlobalIndices.Start = &g_slots[i];
        v[i].second.GlobalIndices.Finish = &g_slots[i];
        v[i].second.GlobalIndices.EndOfStorage = &g_slots[i];
    }
    random_access_iterator_tag tag;
    PairULongDefClassInfo* r = __copy_backward(v, v + 4, v + 6, tag, (long*)0);   // [0,4) -> [2,6)
    if (r != v + 2 || g_stringAssigns != 4 || g_arrayAssigns != 4) { std::printf("009b7870_TEST FAIL\n"); return 1; }
    static const unsigned long want[6] = { 100, 101, 100, 101, 102, 103 };
    for (int i = 0; i < 6; ++i) {
        int src = (i < 2) ? i : i - 2;
        if (v[i].first != want[i] || std::strcmp(v[i].second.Name.PStringData, g_names[src]) != 0 ||
            v[i].second.GlobalIndices.Start != &g_slots[src]) { std::printf("009b7870_TEST FAIL\n"); return 1; }
    }
    PairULongDefClassInfo* e = __copy_backward(v, v, v + 3, tag, (long*)0);           // empty range: result unchanged
    if (e != v + 3 || g_stringAssigns != 4) { std::printf("009b7870_TEST FAIL\n"); return 1; }
    std::printf("009b7870_TEST PASS\n");
    return 0;
}