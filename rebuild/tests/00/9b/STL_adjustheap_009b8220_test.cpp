// 009b8220 behaviour test: __adjust_heap over (unsigned CRC key, def-class info) pairs: make_heap + sort_heap with the row's sift-down give an UNSIGNED ascending order with every payload kept on its key
#include <cstdio>
#include <string.h>
#define ADDR "009b8220"

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
static const unsigned int kKeys[] = { 7u, 0x80000000u, 3u, 0xFFFFFFF0u, 3u, 42u, 0u, 0x7FFFFFFFu, 19u, 0x80000001u, 5u };
static const unsigned int kSorted[] = { 0u, 3u, 3u, 5u, 7u, 19u, 42u, 0x7FFFFFFFu, 0x80000000u, 0x80000001u, 0xFFFFFFF0u };

struct CArrayUL { unsigned int tag; };
struct CDefClassInfo { CCharString Name; unsigned int AllocFunc; CArrayUL GlobalIndices; };
typedef pair<unsigned int, CDefClassInfo> Elem;
struct Comp { bool operator()(const Elem& a, const Elem& b) const { return a.first < b.first; } };
static Comp MakeComp() { return Comp(); }
static Elem MakeElem(int i)
{
    CDefClassInfo info; info.Name = CCharString("def"); info.AllocFunc = kKeys[i] ^ 0x5A5A5A5Au; info.GlobalIndices.tag = kKeys[i] + 1u;
    return Elem(kKeys[i], info);
}
static bool PayloadOk(const Elem& e) { return e.second.AllocFunc == (e.first ^ 0x5A5A5A5Au) && e.second.GlobalIndices.tag == e.first + 1u && SameString(e.second.Name, CCharString("def")); }

static bool OrderOk(Elem* a, int n) { for (int i = 0; i < n; ++i) if (!(a[i].first == kSorted[i])) return false; return true; }
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