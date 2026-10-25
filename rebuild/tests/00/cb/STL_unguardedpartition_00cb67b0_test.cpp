// Behaviour test for 0x00CB67B0 _STL::__unguarded_partition over pair<CCharString,CScriptInfo>: everything before the split sorts no later than the pivot, everything after no earlier.
#include <cstdio>
#include <cstring>

class CCharString {                                     // a name; NULL data sorts first
public:
    const char* PData;
    CCharString() : PData(0) {}
    CCharString(const char* s) : PData(s) {}
    bool operator<(const CCharString& other) const
    {
        if (other.PData == PData)
            return false;
        if (PData == 0)
            return true;
        if (other.PData == 0)
            return false;
        return std::strcmp(PData, other.PData) < 0;
    }
};

class CScriptInfo {
public:
    void*       AllocFunc;
    void*       MasterScriptDataAlloc;
    CCharString Name;
    int         ID;
    bool        MasterScript;
};

template <class _T1, class _T2>
struct pair_ {
    _T1 first;
    _T2 second;
    pair_() {}
    pair_(const _T1& a, const _T2& b) : first(a), second(b) {}
    pair_(const pair_& o) : first(o.first), second(o.second) {}
};
typedef pair_<CCharString, CScriptInfo> SScriptInfoPair;

struct SScriptInfoLess {
    bool operator()(const SScriptInfoPair& a, const SScriptInfoPair& b) const { return a.first < b.first; }
};

static void iter_swap_(SScriptInfoPair* a, SScriptInfoPair* b) { SScriptInfoPair t = *a; *a = *b; *b = t; }

static void __unguarded_linear_insert(SScriptInfoPair* __last, SScriptInfoPair __val, SScriptInfoLess __comp)
{
    SScriptInfoPair* __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

static SScriptInfoPair* __unguarded_partition(SScriptInfoPair* __first, SScriptInfoPair* __last,
                                              SScriptInfoPair __pivot, SScriptInfoLess __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap_(__first, __last);
        ++__first;
    }
}

static void __adjust_heap(SScriptInfoPair* __first, int __holeIndex, int __len, SScriptInfoPair __val, SScriptInfoLess __comp)
{
    int __topIndex = __holeIndex;
    int __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    int __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}

static void __make_heap(SScriptInfoPair* __first, SScriptInfoPair* __last, SScriptInfoLess __comp, SScriptInfoPair*, int*)
{
    if (__last - __first < 2) return;
    int __len = (int)(__last - __first);
    int __parent = (__len - 2) / 2;
    for (;;) {
        __adjust_heap(__first, __parent, __len, SScriptInfoPair(*(__first + __parent)), __comp);
        if (__parent == 0) return;
        __parent--;
    }
}

static SScriptInfoPair* copy_backward_(SScriptInfoPair* first, SScriptInfoPair* last, SScriptInfoPair* result)
{
    for (int n = (int)(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}

static void __linear_insert(SScriptInfoPair* __first, SScriptInfoPair* __last, SScriptInfoPair __val, SScriptInfoLess __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward_(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}

static const char* g_names[] = { "alpha", "bravo", "charlie", "delta", "echo", "foxtrot", "golf", "hotel", "india" };

static SScriptInfoPair Make(int nameIndex, int id)
{
    CScriptInfo info;
    info.AllocFunc = 0;
    info.MasterScriptDataAlloc = 0;
    info.Name = CCharString(nameIndex < 0 ? 0 : g_names[nameIndex]);
    info.ID = id;
    info.MasterScript = (id & 1) != 0;
    return SScriptInfoPair(info.Name, info);
}

static bool Sorted(const SScriptInfoPair* a, int n)
{
    for (int i = 1; i < n; ++i)
        if (a[i].first < a[i - 1].first) return false;
    return true;
}

int main()
{
    bool ok = true;
    SScriptInfoLess less;

    SScriptInfoPair a[8] = { Make(7, 7), Make(1, 1), Make(5, 5), Make(3, 3), Make(8, 8), Make(0, 0), Make(4, 4), Make(2, 2) };
    SScriptInfoPair pivot = Make(4, 4);                                   // the median name, "echo"
    SScriptInfoPair* cut = __unguarded_partition(&a[0], &a[8], pivot, less);
    int k = (int)(cut - a);
    ok = ok && k > 0 && k < 8;
    for (int i = 0; i < k; ++i) ok = ok && !(pivot.first < a[i].first);
    for (int j = k; j < 8; ++j) ok = ok && !(a[j].first < pivot.first);
    int idSum = 0;
    for (int m = 0; m < 8; ++m) { idSum += a[m].second.ID; ok = ok && a[m].first.PData == a[m].second.Name.PData; }
    ok = ok && idSum == 0 + 1 + 2 + 3 + 4 + 5 + 7 + 8;                    // a permutation, pairs intact
    if (!ok) { std::printf("00cb67b0_TEST FAIL\n"); return 1; }
    std::printf("00cb67b0_TEST PASS\n");
    return 0;
}