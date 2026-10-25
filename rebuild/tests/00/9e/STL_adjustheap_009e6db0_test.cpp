// 009e6db0 behaviour test: __adjust_heap over (name, symbol info) pairs ordered by CCharString::FastCompare (length first, then bytes; null first): make_heap + sort_heap give the length-major order
#include <cstdio>
#include <string.h>
#define ADDR "009e6db0"

// CCharString model: a nullable payload pointer; operator< is the retail inline (same payload -> false,
// null this -> true, null other -> false, else the payload compare); FastCompare = length first, then bytes.
struct CCharString {
    const char* p;
    CCharString() : p(0) {}
    CCharString(const char* s) : p(s) {}
    bool operator<(const CCharString& other) const
    {
        if (other.p == p) return false;
        if (!p) return true;
        if (!other.p) return false;
        return strcmp(p, other.p) < 0;
    }
    bool FastCompare(const CCharString& other) const
    {
        if (p == other.p) return false;
        if (!p) return true;
        if (!other.p) return false;
        unsigned int la = (unsigned int)strlen(p), lb = (unsigned int)strlen(other.p);
        if (la < lb) return true;
        if (la > lb) return false;
        return memcmp(p, other.p, la) < 0;
    }
};
static bool SameString(const CCharString& a, const CCharString& b)
{
    if (!a.p || !b.p) return a.p == b.p;
    return strcmp(a.p, b.p) == 0;
}

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() : first(), second() {}
    pair(const T1& a, const T2& b) : first(a), second(b) {}
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
};

// STLport 4.6 heap primitives, as in the source
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
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
    __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result,
                _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}
static const char* kKeys[] = { "mesh", 0, "alpha", "zeta", "beta", "alpha", 0, "omega", "b", "aa", "z" };
static const char* kSorted[] = { 0, 0, "b", "z", "aa", "beta", "mesh", "zeta", "alpha", "alpha", "omega" };
struct Payload { CCharString GroupName; CCharString EntryName; long Val; };

typedef pair<CCharString, Payload> Elem;
struct Comp { bool operator()(const Elem& a, const Elem& b) const { return a.first.FastCompare(b.first); } };
static Comp MakeComp() { return Comp(); }
static int KeyIndexOf(const CCharString& s) { for (int i = 0; i < 11; ++i) if (s.p == kKeys[i]) return i; return -1; }
static Elem MakeElem(int i) { Payload v; v.GroupName = CCharString(kKeys[i]); v.EntryName = CCharString("entry"); v.Val = i; return Elem(CCharString(kKeys[i]), v); }
static bool PayloadOk(const Elem& e) { int i = KeyIndexOf(e.first); if (i < 0) return false; const Payload& v = e.second; return SameString(v.GroupName, e.first) && SameString(v.EntryName, CCharString("entry")) && SameString(CCharString(kKeys[v.Val]), e.first); }

static bool OrderOk(Elem* a, int n) { for (int i = 0; i < n; ++i) if (!SameString(a[i].first, CCharString(kSorted[i]))) return false; return true; }
static bool AfterOk() { return true; }

static int Fail(const char* why) { std::printf("%s_TEST FAIL (%s)\n", ADDR, why); return 1; }

static bool IsHeap(Elem* a, int n, Comp comp)
{
    for (int i = 1; i < n; ++i)
        if (comp(a[(i - 1) / 2], a[i])) return false;
    return true;
}

int main()
{
    const int N = 11;
    Comp comp = MakeComp();
    {
        Elem a[11];
        for (int i = 0; i < N; ++i) a[i] = MakeElem(i);
        for (int parent = (N - 2) / 2; ; --parent) { __adjust_heap(a, parent, N, Elem(a[parent]), comp); if (parent == 0) break; }
        if (!IsHeap(a, N, comp)) return Fail("heap property");
        for (int n = N; n > 1; --n) {
            Elem v(a[n - 1]); a[n - 1] = a[0]; __adjust_heap(a, 0, n - 1, v, comp);
        }
        for (int i = 1; i < N; ++i)
            if (comp(a[i], a[i - 1])) return Fail("sort order");
        for (int i = 0; i < N; ++i)
            if (!PayloadOk(a[i])) return Fail("payload travelled with its key");
        if (!OrderOk(a, N)) return Fail("expected order");
    }
    if (!AfterOk()) return Fail("ownership");
    std::printf("%s_TEST PASS\n", ADDR);
    return 0;
}