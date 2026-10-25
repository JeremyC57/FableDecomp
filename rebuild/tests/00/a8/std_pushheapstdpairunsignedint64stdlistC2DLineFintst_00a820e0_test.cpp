// Link test for 0x00A820E0: the source object's std::__push_heap over pair<unsigned __int64, list<C2DLineF> > keeps a
// max-heap on the unsigned 64-bit key and destroys its by-value element; list<C2DLineF>'s operator= (0x00517720) and
// destructor (0x005127A0) are supplied here as counters.
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
    ~list();                                       // 0x005127A0
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp);   // from the source object (/Gr)
static int g_assigns = 0, g_dtors = 0;
template <> list<C2DLineF>& list<C2DLineF>::operator=(const list<C2DLineF>& __x) { ++g_assigns; _M_node = __x._M_node; return *this; }
template <> list<C2DLineF>::~list() { ++g_dtors; }
}
typedef std::pair<unsigned __int64, std::list<C2DLineF> > Elem;
typedef CKeyPairCompareLess<unsigned __int64, std::list<C2DLineF> > Less;
int main()
{
    static int tags[8];
    Elem h[10];
    int n = 0;
    unsigned __int64 keys[8] = { 9, ((unsigned __int64)3 << 32), 0xFFFFFFFFu, ((unsigned __int64)3 << 32) + 7, 1, 0, ((unsigned __int64)1 << 33), 5 };
    for (int i = 0; i < 8; ++i) {
        Elem v;
        v.first = keys[i];
        v.second._M_node = &tags[i];
        int d0 = std::g_dtors;
        std::__push_heap(h, n, 0, v, Less());
        ++n;
        if (std::g_dtors != d0 + 1) { std::printf("00a820e0_TEST FAIL dtor\n"); return 1; }
        for (int c = 1; c < n; ++c)
            if (h[(c - 1) / 2].first < h[c].first) { std::printf("00a820e0_TEST FAIL heap\n"); return 1; }
    }
    if (h[0].first != ((unsigned __int64)3 << 32) + 7 || h[0].second._M_node != &tags[3] || std::g_assigns < 8) { std::printf("00a820e0_TEST FAIL root\n"); return 1; }
    std::printf("00a820e0_TEST PASS\n");
    return 0;
}
