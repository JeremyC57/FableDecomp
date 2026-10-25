// Behaviour test for 00cbd0f0: __partial_sort of (name, entity script binding) pairs: the three smallest names, sorted, at the front
#include <cstdio>
#include <cstring>
#include <new>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_stringCopies = 0;
static int g_deletes = 0;
// test model of CCharString: a shared text pointer; operator< and FastCompare order by strcmp
class CCharString {
public:
    const char* m_pText;
    CCharString() : m_pText("") { }
    CCharString(const char* text) : m_pText(text) { }
    CCharString(const CCharString& other) : m_pText(other.m_pText) { ++g_stringCopies; }
    ~CCharString() { }
    CCharString& operator=(const CCharString& other) { m_pText = other.m_pText; return *this; }
    bool operator<(const CCharString& other) const { return std::strcmp(m_pText, other.m_pText) < 0; }
    bool FastCompare(const CCharString& other) const { return std::strcmp(m_pText, other.m_pText) < 0; }
};
// CCountedPointer count block + the D-form release used by the sources
struct CCPPointerInfo {
    long  RefCount;
    void  (*DeleteFunc)(void* data);
    void* Data;
};
static void TestDeleteFunc(void*) { ++g_deletes; }
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(T* data, CCPPointerInfo* info) : Data(data), Info(info) { if (Info) ++Info->RefCount; }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    operator delete(Info);
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
class CActiveEntityScriptBase { public: int Tag; };
class CEntityScriptBindingBase { public: int Tag; };
struct CScriptInfo {                       // reduced: the name and the ID
    CCharString Name;
    long        ID;
};
struct CPersistInfo {                      // NScript::CRuntimeScriptedEntityPersist::CPersistInfo
    u64         UID;
    CCharString PersistScript;
};
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
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
typedef std::pair<CCharString, CScriptInfo> CScriptInfoPair;
typedef CKeyPairCompareLess<CCharString, CScriptInfo> CScriptInfoLess;
typedef std::pair<u64, CPersistInfo> CPersistPair;
typedef CKeyPairCompareLess<u64, CPersistInfo> CPersistLess;
typedef std::pair<u64, CCountedPointer<CActiveEntityScriptBase> > CActiveScriptPair;
typedef CKeyPairCompareLess<u64, CCountedPointer<CActiveEntityScriptBase> > CActiveScriptLess;
typedef std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> > CBindingPair;
typedef CCompareString<CCountedPointer<CEntityScriptBindingBase> > CBindingLess;

static CPersistPair MakePersist(u64 uid, const char* script)
{
    CPersistPair p;
    p.first = uid;
    p.second.UID = uid;
    p.second.PersistScript = CCharString(script);
    return p;
}
static CScriptInfoPair MakeScriptInfo(const char* name, long id)
{
    CScriptInfoPair p;
    p.first = CCharString(name);
    p.second.Name = CCharString(name);
    p.second.ID = id;
    return p;
}
// counted-pointer fixtures: every block is also held once by g_holders, so after an algorithm each block must be back
// at 2 references (array + holder) and nothing may have been deleted
static CActiveEntityScriptBase g_scripts[8];
static CEntityScriptBindingBase g_bindings[8];
static CCPPointerInfo* g_infos[8];
static void MakeInfos(int n, void* const* datas)
{
    int i;
    for (i = 0; i < n; ++i) {
        g_infos[i] = new CCPPointerInfo;
        g_infos[i]->RefCount = 1;          // the holder's reference
        g_infos[i]->DeleteFunc = TestDeleteFunc;
        g_infos[i]->Data = datas[i];
    }
}
static CActiveScriptPair MakeActive(u64 uid, int slot)
{
    CActiveScriptPair p;
    p.first = uid;
    p.second = CCountedPointer<CActiveEntityScriptBase>(&g_scripts[slot], g_infos[slot]);
    return p;
}
static CBindingPair MakeBinding(const char* name, int slot)
{
    CBindingPair p;
    p.first = CCharString(name);
    p.second = CCountedPointer<CEntityScriptBindingBase>(&g_bindings[slot], g_infos[slot]);
    return p;
}
static bool CountsAre(int n, long count)
{
    int i;
    for (i = 0; i < n; ++i)
        if (g_infos[i]->RefCount != count)
            return false;
    return g_deletes == 0;
}
namespace std {
template <class _Tp> void swap(_Tp& __a, _Tp& __b);
template <class _ForwardIter1, class _ForwardIter2> void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2);
template <class _Tp, class _Compare> const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp);
template <class _RandomAccessIter, class _Tp, class _Compare> void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance> _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*);
template <class _BidirectionalIter1, class _BidirectionalIter2> _BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result);
template <class _RandomAccessIter, class _Tp, class _Compare> void __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp);
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare> void __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp);
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare> void __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance> void __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*);
template <class _RandomAccessIterator, class _Compare> void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance> void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result, _Tp __val, _Compare __comp, _Distance*);
template <class _RandomAccessIterator, class _Compare> void sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);
template <class _Tp> void swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}
template <class _ForwardIter1, class _ForwardIter2> void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    swap(*__i1, *__i2);
}
template <class _Tp, class _Compare> const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c)) return __b;
        else if (__comp(__a, __c)) return __c;
        else return __a;
    else if (__comp(__a, __c)) return __a;
    else if (__comp(__b, __c)) return __c;
    else return __b;
}
template <class _RandomAccessIter, class _Tp, class _Compare> void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
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
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance> _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = (_Distance)(__last - __first); __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template <class _BidirectionalIter1, class _BidirectionalIter2> _BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _RandomAccessIter, class _Tp, class _Compare> void __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare> void __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare> void __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
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
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance> void __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2) return;
    _Distance __len = (_Distance)(__last - __first);
    _Distance __parent = (__len - 2) / 2;
    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0) return;
        __parent--;
    }
}
template <class _Tp> _Tp* __value_type(_Tp*) { return 0; }
template <class _RandomAccessIterator, class _Compare> void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type(__first), (int*)0);
}
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance> void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result, _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, (_Distance)0, (_Distance)(__last - __first), __val, __comp);
}
template <class _RandomAccessIterator, class _Compare> void sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1) {
        --__last;
        __pop_heap(__first, __last, __last, *__last, __comp, (int*)0);
    }
}
}
// ---- the function under test, copied from src/00cbd0f0.cpp (calling convention dropped) ----
namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, (int*)0);
    sort_heap(__first, __middle, __comp);
}
}
static bool SortedB(const CPersistPair* a, int n)
{
    int i;
    for (i = 1; i < n; ++i)
        if (a[i].first < a[i - 1].first) return false;
    for (i = 0; i < n; ++i)
        if (a[i].second.UID != a[i].first) return false;   // payload travels with its key
    return true;
}
static bool SortedC(const CActiveScriptPair* a, int n)
{
    int i;
    for (i = 1; i < n; ++i)
        if (a[i].first < a[i - 1].first) return false;
    for (i = 0; i < n; ++i)
        if (a[i].second.Data->Tag != (int)a[i].first) return false;
    return true;
}
static void InitC(int n)
{
    void* d[8];
    int i;
    for (i = 0; i < n; ++i) d[i] = &g_scripts[i];
    MakeInfos(n, d);
}
static bool SortedD(const CBindingPair* a, int n)
{
    int i;
    for (i = 1; i < n; ++i)
        if (std::strcmp(a[i].first.m_pText, a[i - 1].first.m_pText) < 0) return false;
    for (i = 0; i < n; ++i)
        if (a[i].second.Data->Tag != (int)a[i].first.m_pText[0]) return false;
    return true;
}
static void InitD(int n)
{
    void* d[8];
    int i;
    for (i = 0; i < n; ++i) d[i] = &g_bindings[i];
    MakeInfos(n, d);
}
static int Fail() { std::printf("00cbd0f0_TEST FAIL\n"); return 1; }
int main()
{
    static const char* names[7] = { "g", "c", "e", "a", "f", "b", "d" };
    InitD(7);
    int i;
    for (i = 0; i < 7; ++i) g_bindings[i].Tag = names[i][0];
    CBindingPair a[7];
    for (i = 0; i < 7; ++i) a[i] = MakeBinding(names[i], i);
    std::__partial_sort(a + 0, a + 3, a + 7, (CBindingPair*)0, CBindingLess());
    if (a[0].first.m_pText[0] != 'a' || a[1].first.m_pText[0] != 'b' || a[2].first.m_pText[0] != 'c') return Fail();
    for (i = 0; i < 7; ++i) if (a[i].second.Data->Tag != a[i].first.m_pText[0]) return Fail();
    if (!CountsAre(7, 2)) return Fail();
    std::printf("00cbd0f0_TEST PASS\n");
    return 0;
}