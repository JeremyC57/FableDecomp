// Link test for 0x00A81E80: the source object's std::sort_heap over pair<unsigned __int64,CANavQuadTreeNode*> sorts a max-heap
// ascending by the unsigned 64-bit key, with __adjust_heap (out of line in retail) supplied here as STLport's body.
#include <cstdio>
class CANavQuadTreeNode { public: int id; };
static CANavQuadTreeNode g_nodes[8];
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                     // +0x00
    _T2 second;                                    // +0x08
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);

template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // from the source object
typedef pair<unsigned __int64, CANavQuadTreeNode*> _CEntry;
typedef CKeyPairCompareLess<unsigned __int64, CANavQuadTreeNode*> _CLess;
static int g_adjusts = 0;
template <> void __fastcall __adjust_heap<_CEntry*, int, _CEntry, _CLess>(_CEntry* __first, int __holeIndex, int __len, _CEntry __val, _CLess __comp)
{
    ++g_adjusts;
    int __topIndex = __holeIndex;
    int __secondChild = 2 * __holeIndex + 2;
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
    int __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}
}
typedef std::_CEntry Entry;
int main()
{
    unsigned __int64 keys[8] = { 9, ((unsigned __int64)3 << 32), 0xFFFFFFFFu, ((unsigned __int64)3 << 32) + 7, 1, 0, ((unsigned __int64)1 << 33), 5 };
    unsigned char raw[8 * sizeof(Entry)];
    Entry* h = (Entry*)raw;
    int n = 0;
    for (int i = 0; i < 8; ++i) {                // push_heap by hand: a max-heap on the key
        int hole = n++;
        while (hole > 0 && h[(hole - 1) / 2].first < keys[i]) { h[hole].first = h[(hole - 1) / 2].first; h[hole].second = h[(hole - 1) / 2].second; hole = (hole - 1) / 2; }
        h[hole].first = keys[i];
        h[hole].second = (CANavQuadTreeNode*)&g_nodes[i];
    }
    std::sort_heap(h, h + 8, std::_CLess());
    int ok = sizeof(Entry) == 16 && std::g_adjusts == 7;
    for (int c = 1; c < 8; ++c) ok = ok && !(h[c].first < h[c - 1].first);
    for (int k = 0; k < 8; ++k) {
        int src = -1;
        for (int j = 0; j < 8; ++j) if (keys[j] == h[k].first) src = j;
        ok = ok && src >= 0;
        if (src >= 0) { int i = src; ok = ok && h[k].second == ((CANavQuadTreeNode*)&g_nodes[i]); }
    }
    std::sort_heap(h, h + 1, std::_CLess());
    ok = ok && std::g_adjusts == 7;
    if (!ok) { std::printf("00a81e80_TEST FAIL\n"); return 1; }
    std::printf("00a81e80_TEST PASS\n");
    return 0;
}
