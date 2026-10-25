// Behaviour test for 0x00CB6420 _STL::__unguarded_linear_insert over pair<CCharString,CScriptInfo>: the value moves down past every larger name, whole pairs travel.
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

    SScriptInfoPair a[5] = { Make(0, 10), Make(2, 12), Make(4, 14), Make(6, 16), Make(0, 0) };
    __unguarded_linear_insert(&a[4], Make(3, 13), less);
    ok = ok && Sorted(a, 5) && a[2].second.ID == 13 && a[3].second.ID == 14 && a[4].second.ID == 16;
    ok = ok && a[2].first.PData == g_names[3] && a[2].second.Name.PData == g_names[3];
    SScriptInfoPair b[3] = { Make(-1, 1), Make(5, 2), Make(0, 0) };
    __unguarded_linear_insert(&b[2], Make(1, 3), less);                  // stops at the NULL-name sentinel
    ok = ok && b[0].second.ID == 1 && b[1].second.ID == 3 && b[2].second.ID == 2;
    SScriptInfoPair c[3] = { Make(0, 1), Make(1, 2), Make(0, 0) };
    __unguarded_linear_insert(&c[2], Make(8, 9), less);                  // already in place
    ok = ok && c[2].second.ID == 9 && c[1].second.ID == 2;
    if (!ok) { std::printf("00cb6420_TEST FAIL\n"); return 1; }
    std::printf("00cb6420_TEST PASS\n");
    return 0;
}