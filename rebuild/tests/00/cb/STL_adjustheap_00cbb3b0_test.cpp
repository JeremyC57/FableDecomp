// 00cbb3b0 behaviour test: __adjust_heap over (64-bit id, counted script pointer) pairs: the high dword decides first, then the low dword; counted pointers stay on their ids and the counts balance
#include <cstdio>
#include <string.h>
#define ADDR "00cbb3b0"

// CCountedPointer model (retail semantics: copy bumps the shared count; assignment releases the old block first
// unless both share it; the last release calls the block's destroy function on the object and frees the block).
static int g_destroyed = 0;
static void DestroyObject(void* data) { (void)data; ++g_destroyed; }
struct CCPPointerInfo {
    unsigned int RefCount;
    void (*DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    CCountedPointer() : Data(0), Info(0) {}
    explicit CCountedPointer(T* object) : Data(object), Info(0)
    {
        if (object) {
            Info = new CCPPointerInfo;
            Info->RefCount = 1; Info->DeleteFunc = DestroyObject; Info->Data = object;
        }
    }
    CCountedPointer(const CCountedPointer<T>& other) : Data(other.Data), Info(other.Info)
    {
        if (other.Info) ++other.Info->RefCount;
    }
    ~CCountedPointer() { Release(); Data = 0; Info = 0; }
    CCountedPointer<T>& operator=(const CCountedPointer<T>& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info) ++info->RefCount;
        }
        return *this;
    }
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    T* Data;
    CCPPointerInfo* Info;
};
struct CVolumeObject { int id; };
static CVolumeObject g_objects[16];

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
#ifdef _MSC_VER
typedef unsigned __int64 U64;
#else
typedef unsigned long long U64;
#endif
#define MK(hi, lo) ((((U64)(hi)) << 32) | (U64)(lo))
static const U64 kKeys[] = { MK(1, 0), MK(0, 5), MK(0, 0xFFFFFFFFu), MK(1, 1), MK(0, 7), MK(0x80000000u, 0), MK(0, 0), MK(2, 0), MK(0, 6), MK(1, 0xFFFFFFFFu), MK(0, 1) };
static const U64 kSorted[] = { MK(0, 0), MK(0, 1), MK(0, 5), MK(0, 6), MK(0, 7), MK(0, 0xFFFFFFFFu), MK(1, 0), MK(1, 1), MK(1, 0xFFFFFFFFu), MK(2, 0), MK(0x80000000u, 0) };

static CCountedPointer<CVolumeObject>* g_ptrs[11];
typedef pair<U64, CCountedPointer<CVolumeObject> > Elem;
struct Comp { bool operator()(const Elem& a, const Elem& b) const { return a.first < b.first; } };
static Comp MakeComp()
{
    for (int i = 0; i < 11; ++i) { g_objects[i].id = i; g_ptrs[i] = new CCountedPointer<CVolumeObject>(&g_objects[i]); }
    return Comp();
}
static Elem MakeElem(int i) { return Elem(kKeys[i], *g_ptrs[i]); }
static bool PayloadOk(const Elem& e)
{
    const CCountedPointer<CVolumeObject>& p = e.second;
    if (!p.Data || !p.Info) return false;
    return kKeys[p.Data->id] == e.first && p.Info->RefCount == 2;
}

static bool OrderOk(Elem* a, int n) { for (int i = 0; i < n; ++i) if (!(a[i].first == kSorted[i])) return false; return true; }

static bool AfterOk()
{
    for (int i = 0; i < 11; ++i) { if (g_ptrs[i]->Info->RefCount != 1) return false; }
    for (int i = 0; i < 11; ++i) delete g_ptrs[i];
    return g_destroyed == 11;
}

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