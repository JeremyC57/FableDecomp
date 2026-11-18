// Behaviour test for 004babf0: out-of-line __pop_heap for pair<CCharString, CPersistInfo> -- heap-sort seven keyed elements through the row's function (with the
// SGI __adjust_heap/__push_heap written out here) and check ascending keys with each payload still beside its key.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __cdecl
#endif
static const char* const g_names[10] = { "k0", "k1", "k2", "k3", "k4", "k5", "k6", "k7", "k8", "k9" };
#define NUM(s) ((long)((s).c_str()[1] - '0'))
template <class T, class C> void push_impl(T* first, long holeIndex, long topIndex, T value, C comp)
{
    long parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(first[parent], value)) {
        first[holeIndex] = first[parent];
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    first[holeIndex] = value;
}
template <class T, class C> void adjust_impl(T* first, long holeIndex, long len, T value, C comp)
{
    long topIndex = holeIndex;
    long secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(first[secondChild], first[secondChild - 1]))
            secondChild--;
        first[holeIndex] = first[secondChild];
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        first[holeIndex] = first[secondChild - 1];
        holeIndex = secondChild - 1;
    }
    push_impl(first, holeIndex, topIndex, value, comp);
}
static int g_live = 0;
class CCharString {
public:
    void* PStringData;
    CCharString() : PStringData((void*)"") { ++g_live; }
    CCharString(const char* s) : PStringData((void*)s) { ++g_live; }
    CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_live; }
    ~CCharString() { --g_live; }
    const CCharString& operator=(const CCharString& other) { PStringData = other.PStringData; return *this; }
    const char* c_str() const { return (const char*)PStringData; }
};

namespace NQuestManager {
struct CRuntimeQuestPersistence {
    class CPersistInfo { public:                               // Ego_r.pdb, 8 bytes
        CCharString QuestName;                          // +0x00
        CCharString PersistScript;                      // +0x04
    };
};
}

struct PairCharStringPersistInfo {                      // std::pair<CCharString, CPersistInfo>, 0xc bytes
    CCharString                                            first;    // +0x00
    NQuestManager::CRuntimeQuestPersistence::CPersistInfo  second;   // +0x04
    PairCharStringPersistInfo() { }
    PairCharStringPersistInfo(const PairCharStringPersistInfo& other) : first(other.first), second(other.second) { }
};

struct CKeyPairCompareLess {                            // CKeyPairCompareLess<CCharString, CPersistInfo>, empty
    CKeyPairCompareLess() { }
    bool operator()(const PairCharStringPersistInfo& a, const PairCharStringPersistInfo& b) const { return std::strcmp(a.first.c_str(), b.first.c_str()) < 0; }
};

void __adjust_heap(PairCharStringPersistInfo* first, long holeIndex, long len, PairCharStringPersistInfo value,
                              CKeyPairCompareLess comp) { adjust_impl(first, holeIndex, len, value, comp); }   // 0x004BA180

void __pop_heap(PairCharStringPersistInfo* first, PairCharStringPersistInfo* last, PairCharStringPersistInfo* result,
                           PairCharStringPersistInfo value, CKeyPairCompareLess comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}

#define KEY(e) NUM((e).first)
int main()
{
    {
        static const int keys[7] = { 5, 9, 1, 7, 3, 8, 2 };
        static const int sorted[7] = { 1, 2, 3, 5, 7, 8, 9 };
        PairCharStringPersistInfo a[7];
        for (int i = 0; i < 7; ++i) {
            PairCharStringPersistInfo& e = a[i];
            int k = keys[i];
            e.first = CCharString(g_names[k]);
            e.second.PersistScript = CCharString(g_names[k]);
        }
        CKeyPairCompareLess comp;
        for (int i = 1; i < 7; ++i) {
            PairCharStringPersistInfo v(a[i]);
            push_impl(a, (long)i, 0L, v, comp);
        }
        for (int len = 7; len > 1; --len)
            __pop_heap(a, a + len - 1, a + len - 1, PairCharStringPersistInfo(a[len - 1]), comp, (long*)0);
        for (int i = 0; i < 7; ++i) {
            const PairCharStringPersistInfo& e = a[i];
            if (KEY(e) != sorted[i] || !(NUM(e.second.PersistScript) == KEY(e))) { std::printf("004babf0_TEST FAIL\n"); return 1; }
        }
    }
    if (g_live != 0) { std::printf("004babf0_TEST FAIL\n"); return 1; }
    std::printf("004babf0_TEST PASS\n");
    return 0;
}