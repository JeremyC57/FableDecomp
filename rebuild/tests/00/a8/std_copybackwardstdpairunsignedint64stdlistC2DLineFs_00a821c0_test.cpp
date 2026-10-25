// Link test for 0x00A821C0: the source object's std::__copy_backward over pair<unsigned __int64, list<C2DLineF> >
// entries: an overlapping backward move of the 64-bit keys, with the list assignment (0x00517720) supplied here.
#include <cstdio>
class C2DVector {
public:
    float X, Y;
};
class C2DLineF {                                   // 16 bytes (Ego_r.pdb)
public:
    C2DVector Point1;                              // +0x00
    C2DVector Point2;                              // +0x08
};

namespace std {
template <class _Tp> class allocator { };

template <class _Tp, class _Alloc = allocator<_Tp> >
class list {                                       // STLport 4.6: one word, the sentinel node pointer
public:
    void* _M_node;
    list<_Tp, _Alloc>& operator=(const list<_Tp, _Alloc>& __x);   // 0x00517720
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

struct random_access_iterator_tag { };

template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
_BidirectionalIter2 __fastcall __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result,
                                               const random_access_iterator_tag&, _Distance*);   // from the source object
static int g_assigns = 0;
template <> list<C2DLineF>& list<C2DLineF>::operator=(const list<C2DLineF>& __x) { ++g_assigns; _M_node = __x._M_node; return *this; }
}
typedef std::pair<unsigned __int64, std::list<C2DLineF> > Entry;
int main()
{
    static int nodes[5];
    Entry e[5];
    for (int i = 0; i < 5; ++i) { e[i].first = ((unsigned __int64)(i + 1) << 32) | (unsigned)(0xA0 + i); e[i].second._M_node = &nodes[i]; }
    std::random_access_iterator_tag tag;
    Entry* r = std::__copy_backward(e + 0, e + 3, e + 5, tag, (int*)0);
    int ok = sizeof(Entry) == 16 && r == e + 2 && std::g_assigns == 3;
    ok = ok && e[4].first == (((unsigned __int64)3 << 32) | 0xA2) && e[3].first == (((unsigned __int64)2 << 32) | 0xA1)
            && e[2].first == (((unsigned __int64)1 << 32) | 0xA0) && e[1].first == (((unsigned __int64)2 << 32) | 0xA1);
    ok = ok && e[4].second._M_node == &nodes[2] && e[3].second._M_node == &nodes[1] && e[2].second._M_node == &nodes[0];
    ok = ok && std::__copy_backward(e + 1, e + 1, e + 4, tag, (int*)0) == e + 4 && std::g_assigns == 3;
    if (!ok) { std::printf("00a821c0_TEST FAIL\n"); return 1; }
    std::printf("00a821c0_TEST PASS\n");
    return 0;
}
