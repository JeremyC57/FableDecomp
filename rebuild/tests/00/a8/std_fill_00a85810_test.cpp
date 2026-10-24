// behaviour test for 0x00a85810 (std::fill over 40-byte {vector, vector, vector map} records)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_stringCopies = 0, g_stringAssigns = 0;
static int g_vectorAssigns = 0;
namespace std {
template <class T>
struct vector {                                 // STLport vector: 12 bytes
    T* _M_start;                                // +0x00
    T* _M_finish;                               // +0x04
    T* _M_end_of_storage;                       // +0x08
    vector<T>& operator=(const vector<T>& x) { _M_start = x._M_start; _M_finish = x._M_finish; _M_end_of_storage = x._M_end_of_storage; ++g_vectorAssigns; return *this; }
};
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
};
}
template <class K, class V>
struct CKeyPairCompareLess {
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
template <class K, class V, class Compare>
struct CVectorMap {                             // retail 16 bytes
    std::vector<std::pair<K, V> > Data;         // +0x00
    Compare                       Comparator;   // +0x0C (empty)
    bool                          Dirty;        // +0x0D
};

struct SMapValue12 { long m_0, m_4, m_8; };    // the map's 12-byte mapped value (pair stride 16, `sar 4` in 0x00A84940)

struct SNavContainerRecord {                    // 40 bytes
    std::vector<long>          m_first;         // +0x00 (operator= 0x00A80EE0)
    std::vector<unsigned long> m_second;        // +0x0C (operator= 0x00A83D70)
    CVectorMap<long, SMapValue12, CKeyPairCompareLess<long, SMapValue12> > m_map;   // +0x18 (vector operator= 0x00A84940)
};

namespace std {
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
template void __fastcall fill(SNavContainerRecord*, SNavContainerRecord*, const SNavContainerRecord&);
}

int main()
{
    static long la[2]; static unsigned long ua[2]; static std::pair<long, SMapValue12> pa[2];
    SNavContainerRecord v[3];
    for (int i = 0; i < 3; ++i) { v[i].m_first._M_start = 0; v[i].m_second._M_start = 0; v[i].m_map.Data._M_start = 0; v[i].m_map.Dirty = false; }
    SNavContainerRecord val;
    val.m_first._M_start = la; val.m_first._M_finish = la + 2; val.m_first._M_end_of_storage = la + 2;
    val.m_second._M_start = ua; val.m_second._M_finish = ua + 1; val.m_second._M_end_of_storage = ua + 2;
    val.m_map.Data._M_start = pa; val.m_map.Data._M_finish = pa + 2; val.m_map.Data._M_end_of_storage = pa + 2; val.m_map.Dirty = true;
    std::fill(v, v + 2, val);
    int ok = g_vectorAssigns == 6 && v[0].m_first._M_finish == la + 2 && v[1].m_second._M_finish == ua + 1 && v[1].m_map.Data._M_start == pa
          && v[0].m_map.Dirty && v[1].m_map.Dirty && v[2].m_map.Data._M_start == 0 && !v[2].m_map.Dirty;
    if (!ok) { printf("00a85810_TEST FAIL\n"); return 1; }
    printf("00a85810_TEST PASS\n");
    return 0;
}