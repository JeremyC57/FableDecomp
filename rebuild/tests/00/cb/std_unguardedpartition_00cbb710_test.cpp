// Behaviour test for std::__unguarded_partition over (UID, CCountedPointer<CActiveEntityScriptBase>) pairs (0x00CBB710):
// the range is split around the pivot key (everything left of the returned split is <= pivot, everything right >= pivot),
// the pairs keep their counted pointers, and every count block ends with the count it started with (the swap temporary
// and the by-value pivot release what they took; nothing is deleted).
#include <cstdio>
#if defined(_MSC_VER)
#define FASTCALL __fastcall
typedef unsigned __int64 u64;
#else
#define FASTCALL
typedef unsigned long long u64;
#endif
static int g_deletes = 0;
static void FASTCALL DeleteData(void*) { ++g_deletes; }
class CCPPointerInfo { public:
    long  RefCount;
    void  (FASTCALL* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
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
class CActiveEntityScriptBase { public: int id; };
template <class _T1, class _T2> struct pair_t { _T1 first; _T2 second; };
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const pair_t<K, V>& a, const pair_t<K, V>& b) const { return a.first < b.first; }
};
typedef pair_t<u64, CCountedPointer<CActiveEntityScriptBase> > CActiveScriptPair;
typedef CKeyPairCompareLess<u64, CCountedPointer<CActiveEntityScriptBase> > CActiveScriptLess;
namespace stlp {
template <class _Tp> inline void swap(_Tp& __a, _Tp& __b) { _Tp __tmp = __a; __a = __b; __b = __tmp; }
template <class _ForwardIter1, class _ForwardIter2> inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2) { swap(*__i1, *__i2); }
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
}
int main()
{
    static const u64 keys[8] = { 7, 0x100000002ULL, 3, 9, 0x100000001ULL, 1, 5, 8 };
    static CActiveEntityScriptBase objects[8];
    static CCPPointerInfo infos[8];
    static CActiveScriptPair v[8];
    for (int i = 0; i < 8; ++i) {
        objects[i].id = (int)keys[i];
        infos[i].RefCount = 1; infos[i].DeleteFunc = DeleteData; infos[i].Data = &objects[i];
        v[i].first = keys[i]; v[i].second.Data = &objects[i]; v[i].second.Info = &infos[i];
    }
    CActiveScriptPair* cut;
    {
        CActiveScriptPair pivot; pivot.first = 6; pivot.second.Data = 0; pivot.second.Info = 0;   // median-of-three style key, no pointer
        cut = stlp::__unguarded_partition(v, v + 8, pivot, CActiveScriptLess());
    }
    int bad = 0;
    for (CActiveScriptPair* p = v; p < cut; ++p) if (p->first > 6) ++bad;
    for (CActiveScriptPair* p = cut; p < v + 8; ++p) if (p->first < 6) ++bad;
    for (int i = 0; i < 8; ++i) {
        if (infos[i].RefCount != 1) ++bad;                       // every count back where it started
        if ((u64)v[i].second.Data->id != (v[i].first & 0xffffffffULL) && v[i].first < 0x100000000ULL) ++bad;   // pair kept together
        if (v[i].second.Info->Data != v[i].second.Data) ++bad;
    }
    if (cut - v != 3 || g_deletes != 0) ++bad;                    // three keys below the pivot
    // the 64-bit key compares the high word first
    {
        CActiveScriptPair a, b; a.first = 0x100000000ULL; b.first = 0xffffffffULL;
        if (CActiveScriptLess()(a, b) || !CActiveScriptLess()(b, a)) ++bad;
    }
    if (bad) { std::printf("00cbb710_TEST FAIL\n"); return 1; }
    std::printf("00cbb710_TEST PASS\n");
    return 0;
}