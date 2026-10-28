// Behaviour test for 0x00A6F610: __unguarded_partition over helper-point entries
#include <cstdio>
#include <cstdlib>
#include <new>

template <class T> class CBasicString {
public:
    const T* m_pData;
    long Compare(const CBasicString& other) const
    {
        const T* b = other.m_pData;
        const T* a = m_pData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return 0;
            if (*a < *b)
                return -1;
            if (*a > *b)
                return 1;
            ++a;
            ++b;
        }
    }
    bool operator<(const CBasicString& other) const { return Compare(other) < 0; }
};

static CBasicString<char> gStrData[6] = { { "alpha" }, { "bravo" }, { "charlie" }, { "delta" }, { "echo" }, { "foxtrot" } };
static int gLiveStrings = 0;

class CCharString {
public:
    CBasicString<char>* PData;
    CCharString() : PData(0) { ++gLiveStrings; }
    explicit CCharString(int k) : PData(&gStrData[k]) { ++gLiveStrings; }
    CCharString(const CCharString& other) : PData(other.PData) { ++gLiveStrings; }
    ~CCharString() { --gLiveStrings; }
    const CCharString& operator=(const CCharString& other) { PData = other.PData; return *this; }
    bool operator<(const CCharString& other) const
    {
        if (other.PData == PData)
            return false;
        if (PData == 0)
            return true;
        if (other.PData == 0)
            return false;
        return *PData < *other.PData;
    }
    bool FastCompare(const CCharString& other) const { return *this < other; }
    int Key() const { return PData ? (int)(PData - gStrData) : -1; }
};

class CCPPointerInfo {
public:
    long  RefCount;
    void (*DeleteFunc)(void* data);
    void* Data;
};

static int gDeleted = 0;
static void DeleteData(void*) { ++gDeleted; }

template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info && --Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
        Data = 0;
        Info = 0;
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (Info)
                ++Info->RefCount;
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other) { ShareData(other.Data, other.Info); return *this; }
};

class CContainmentVolume {
public:
    int Tag;
};

class C3DMeshVolume {
public:
    CCharString                         Name;
    CCountedPointer<CContainmentVolume> ConvexVolume;
    C3DMeshVolume& operator=(const C3DMeshVolume& other)
    {
        Name = other.Name;
        ConvexVolume = other.ConvexVolume;
        return *this;
    }
};

static CContainmentVolume gVol[8];
static CCPPointerInfo gInfoStore[8];
static CCPPointerInfo* gInfo[8];
static void InitVolumes()
{
    for (int i = 0; i < 8; ++i) {
        gVol[i].Tag = i;
        gInfoStore[i].RefCount = 1;      // the outside owner: never reaches zero unless a count is lost
        gInfoStore[i].DeleteFunc = DeleteData;
        gInfoStore[i].Data = &gVol[i];
        gInfo[i] = &gInfoStore[i];
    }
}

class C3DVector {
public:
    float X, Y, Z;
};

class C3DHelperPoint {
public:
    C3DVector Position;
    long      BoneIndex;
};

class CMatrix3x4 {
public:
    float M[12];
};

class C3DDummyObject {
public:
    CMatrix3x4 Transform;
    long       BoneIndex;
};

namespace std {
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair() : first(), second() { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

template <class V> class CCompareString {
public:
    bool operator()(const std::pair<CCharString, V>& a, const std::pair<CCharString, V>& b) const { return a.first.FastCompare(b.first); }
};

typedef std::pair<CCharString, C3DHelperPoint> THelperEntry;
typedef std::pair<CCharString, C3DDummyObject> TDummyEntry;
typedef std::pair<long, C3DMeshVolume>         TNavEntry;
typedef std::pair<CCharString, C3DMeshVolume>  TVolumeEntry;
typedef CCompareString<C3DHelperPoint>               THelperLess;
typedef CCompareString<C3DDummyObject>               TDummyLess;
typedef CKeyPairCompareLess<long, C3DMeshVolume>        TNavLess;
typedef CKeyPairCompareLess<CCharString, C3DMeshVolume> TVolumeLess;

static void Set(THelperEntry& e, int key, long id)
{
    e.first = CCharString(key);
    e.second.Position.X = (float)id;
    e.second.Position.Y = 2.0f * id;
    e.second.Position.Z = -1.0f;
    e.second.BoneIndex = id;
}
static void Set(TDummyEntry& e, int key, long id)
{
    e.first = CCharString(key);
    for (int i = 0; i < 12; ++i)
        e.second.Transform.M[i] = (float)(id * 12 + i);
    e.second.BoneIndex = id;
}
static void Set(TNavEntry& e, int key, long id)
{
    e.first = key;
    e.second.Name = CCharString((int)(id % 6));
    e.second.ConvexVolume.ShareData(&gVol[id], gInfo[id]);
}
static void Set(TVolumeEntry& e, int key, long id)
{
    e.first = CCharString(key);
    e.second.Name = CCharString((int)(id % 6));
    e.second.ConvexVolume.ShareData(&gVol[id], gInfo[id]);
}

static long IdOf(const THelperEntry& e) { return e.second.BoneIndex; }
static long IdOf(const TDummyEntry& e) { return e.second.BoneIndex; }
static long IdOf(const TNavEntry& e) { return (long)(e.second.ConvexVolume.Data - gVol); }
static long IdOf(const TVolumeEntry& e) { return (long)(e.second.ConvexVolume.Data - gVol); }
static long KeyOf(const THelperEntry& e) { return e.first.Key(); }
static long KeyOf(const TDummyEntry& e) { return e.first.Key(); }
static long KeyOf(const TNavEntry& e) { return e.first; }
static long KeyOf(const TVolumeEntry& e) { return e.first.Key(); }

// payload intact (the per-id fields travelled with the id)
static bool PayloadOk(const THelperEntry& e)
{
    long id = e.second.BoneIndex;
    return e.second.Position.X == (float)id && e.second.Position.Y == 2.0f * id && e.second.Position.Z == -1.0f;
}
static bool PayloadOk(const TDummyEntry& e)
{
    for (int i = 0; i < 12; ++i)
        if (e.second.Transform.M[i] != (float)(e.second.BoneIndex * 12 + i))
            return false;
    return true;
}
static bool PayloadOk(const TNavEntry& e)
{
    long id = IdOf(e);
    return e.second.ConvexVolume.Info == gInfo[id] && e.second.Name.Key() == (int)(id % 6);
}
static bool PayloadOk(const TVolumeEntry& e)
{
    long id = IdOf(e);
    return e.second.ConvexVolume.Info == gInfo[id] && e.second.Name.Key() == (int)(id % 6);
}

// keys and ids of a[0..n) are exactly the expected ones
template <class E> bool Expect(const E* a, int n, const long* keys, const long* ids)
{
    for (int i = 0; i < n; ++i) {
        if (keys && KeyOf(a[i]) != keys[i])
            return false;
        if (ids && IdOf(a[i]) != ids[i])
            return false;
        if (!PayloadOk(a[i]))
            return false;
    }
    return true;
}

// a[0..n) holds each id of [0, n) exactly once
template <class E> bool IsPermutation(const E* a, int n)
{
    unsigned int mask = 0;
    for (int i = 0; i < n; ++i) {
        long id = IdOf(a[i]);
        if (id < 0 || id >= n || (mask & (1u << id)) || !PayloadOk(a[i]))
            return false;
        mask |= 1u << id;
    }
    return true;
}

template <class E, class C> bool IsSorted(const E* a, int n, C comp)
{
    for (int i = 1; i < n; ++i)
        if (comp(a[i], a[i - 1]))
            return false;
    return true;
}

template <class E, class C> bool IsHeap(const E* a, int n, C comp)
{
    for (int i = 1; i < n; ++i)
        if (comp(a[(i - 1) / 2], a[i]))
            return false;
    return true;
}

// every volume's count is the outside owner plus the entries of a[0..n) that hold it
template <class E> bool RefsOk(const E* a, int n)
{
    for (int v = 0; v < 8; ++v) {
        long c = 0;
        for (int i = 0; i < n; ++i)
            if (a[i].second.ConvexVolume.Data == &gVol[v])
                ++c;
        if (gInfo[v]->RefCount != 1 + c)
            return false;
    }
    return gDeleted == 0;
}
// plain-value entries hold no counted pointer
static bool RefsOk(const THelperEntry*, int) { return gDeleted == 0; }
static bool RefsOk(const TDummyEntry*, int) { return gDeleted == 0; }

namespace std {

class __false_type { public: __false_type() { } };
class random_access_iterator_tag { public: random_access_iterator_tag() { } };

template <class _Tp>
inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _Tp>
inline int* __distance_type(const _Tp*) { return (int*)0; }

template <class _Tp>
void swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    swap(*__i1, *__i2);
}

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _BI1, class _BI2>
inline _BI2 copy_backward(_BI1 __first, _BI1 __last, _BI2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
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

template <class _RandomAccessIter, class _Tp, class _Compare>
void __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}

template <class _RandomAccessIter, class _Compare>
void __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last)
        return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, *__i, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}

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

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2)
        return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;
    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0)
            return;
        __parent--;
    }
}

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type(__first), __distance_type(__first));
}

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result, _Tp __val,
                _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIterator, class _Compare>
void sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1) {
        --__last;
        __pop_heap(__first, __last, __last, *__last, __comp, (int*)0);
    }
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type(__first));
    sort_heap(__first, __middle, __comp);
}

}

int main()
{
    int ok = 1;
    InitVolumes();
    int live0 = gLiveStrings;
    {
        THelperEntry a[7];
        static const long k[7] = { 3, 0, 5, 2, 4, 1, 2 };
        for (int i = 0; i < 7; ++i)
            Set(a[i], (int)k[i], i);
        long pivotKey = KeyOf(a[3]);
        int live1 = gLiveStrings;
        THelperEntry* cut = std::__unguarded_partition(a, a + 7, a[3], THelperLess());
        if (cut <= a || cut >= a + 7)
            ok = 0;
        else {
            for (THelperEntry* p = a; p != cut; ++p)
                if (KeyOf(*p) > pivotKey)
                    ok = 0;
            for (THelperEntry* q = cut; q != a + 7; ++q)
                if (KeyOf(*q) < pivotKey)
                    ok = 0;
        }
        if (!IsPermutation(a, 7) || gLiveStrings != live1 || !RefsOk(a, 7))
            ok = 0;
    }
    if (gLiveStrings != live0)
        ok = 0;

    if (!ok) {
        std::printf("00a6f610_TEST FAIL\n");
        return 1;
    }
    std::printf("00a6f610_TEST PASS\n");
    return 0;
}