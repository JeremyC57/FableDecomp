#include <cstdio>
struct CCPPointerInfo {
    long RefCount;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer();
};
class CRegionConnectionNode;
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
}
template <class K, class V> struct CKeyPairCompareLess {
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
namespace std {
template <class RandomIter, class T, class Compare>
void __fastcall __unguarded_linear_insert(RandomIter last, T val, Compare comp);   // 0x00448520 for this instance
template <class RandomIter, class T, class Compare>
void __fastcall __unguarded_insertion_sort_aux(RandomIter first, RandomIter last, T*, Compare comp)
{
    for (RandomIter i = first; i != last; ++i)
        __unguarded_linear_insert(i, T(*i), comp);
}
typedef pair<long, CCountedPointer<CRegionConnectionNode> > _NodeEntry;
template void __fastcall __unguarded_insertion_sort_aux<_NodeEntry*, _NodeEntry, CKeyPairCompareLess<long, CCountedPointer<CRegionConnectionNode> > >(_NodeEntry*, _NodeEntry*, _NodeEntry*, CKeyPairCompareLess<long, CCountedPointer<CRegionConnectionNode> >);
}

static long g_keys[8]; static long g_refs[8]; static long* g_pos[8]; static int g_n = 0;
template <class T> CCountedPointer<T>::~CCountedPointer() { if (Info) --Info->RefCount; }
namespace std {
template <class RandomIter, class T, class Compare>
void __fastcall __unguarded_linear_insert(RandomIter last, T val, Compare comp)
{ g_keys[g_n] = val.first; g_refs[g_n] = val.second.Info ? val.second.Info->RefCount : -1; g_pos[g_n] = &last->first; ++g_n; }
}
int main()
{
    static CCPPointerInfo info = { 1 };
    static unsigned long raw[9] = { 5, 0, 0, 3, 0, 0, 9, 0, 0 };
    raw[2] = (unsigned long)&info;
    std::_NodeEntry* e = (std::_NodeEntry*)raw;
    std::__unguarded_insertion_sort_aux(e, e + 3, (std::_NodeEntry*)0, CKeyPairCompareLess<long, CCountedPointer<CRegionConnectionNode> >());
    bool ok = g_n == 3 && g_keys[0] == 5 && g_keys[1] == 3 && g_keys[2] == 9 && g_refs[0] == 2 && g_refs[1] == -1
        && info.RefCount == 1 && g_pos[1] == &e[1].first;
    if (ok) std::printf("00448910_TEST PASS\n");
    return 0;
}