// behaviour test for 0x006B9ED0 std::__unguarded_insertion_sort_aux over pair<unsigned long, CCountedPointer<CFaction> > (model copy of the
// source with a stand-in __unguarded_linear_insert that records each call and checks the counted copy it receives)
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
class CCPPointerInfo {                                       // Ego_r.pdb, 12 bytes
public:
    unsigned long RefCount;                                  // +0
    void (__fastcall* DeleteFunc)(void* data);               // +4, Data in ecx
    void* Data;                                              // +8
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                    // +0
    CCPPointerInfo* Info;                                    // +4
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
class CFaction;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}   // namespace std
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;
};

template <class K, class V> bool CKeyPairCompareLess<K, V>::operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
typedef std::pair<unsigned long, CCountedPointer<CFaction> > _Entry;
static int g_calls = 0; static _Entry* g_slot[8]; static long g_key[8]; static unsigned long g_refs[8];
namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    g_slot[g_calls] = __last; g_key[g_calls] = (long)__val.first;
    g_refs[g_calls] = __val.second.Info ? __val.second.Info->RefCount : 0;   // the copy holds one extra reference
    ++g_calls;
    if (__val.second.Info) --__val.second.Info->RefCount;                   // stand-in for the copy's destructor
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
}
int main()
{
    CCPPointerInfo info; info.RefCount = 1; info.DeleteFunc = 0; info.Data = 0;
    _Entry e[3];
    for (int i = 0; i < 3; ++i) { e[i].first = 10 - i; e[i].second.Data = 0; e[i].second.Info = (i == 1) ? &info : 0; }
    std::__unguarded_insertion_sort_aux(e, e + 3, (_Entry*)0, CKeyPairCompareLess<unsigned long, CCountedPointer<CFaction> >());
    int ok = g_calls == 3 && g_slot[0] == e && g_slot[1] == e + 1 && g_slot[2] == e + 2
          && g_key[0] == 10 && g_key[1] == 9 && g_key[2] == 8 && g_refs[1] == 2 && g_refs[0] == 0 && info.RefCount == 1;
    g_calls = 0;
    std::__unguarded_insertion_sort_aux(e, e, (_Entry*)0, CKeyPairCompareLess<unsigned long, CCountedPointer<CFaction> >());
    if (g_calls != 0) ok = 0;
    if (!ok) { std::printf("006b9ed0_TEST FAIL\n"); return 1; }
    std::printf("006b9ed0_TEST PASS\n");
    return 0;
}