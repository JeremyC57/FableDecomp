#include <cstdio>
// Behaviour test for 0x00a7d6f0: _Push_heap over a 16-byte pair<unsigned __int64, CANavQuadTreeNode*> keyed max-heap.
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
class CANavQuadTreeNode { public: int id; };
static CANavQuadTreeNode g_nodes[9];
typedef CANavQuadTreeNode* Payload;
#define SETPAY(p,i) ((p) = &g_nodes[i])
#define CHKPAY(p,i) ((p) == &g_nodes[i])
template<class K, class V> struct pair { K first; V second; };
template<class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const pair<K, V>& a, const pair<K, V>& b) const { return a.first < b.first; }
};
template<class _RanIt, class _Diff, class _Ty, class _Pr>
inline void _Push_heap(_RanIt _First, _Diff _Hole, _Diff _Top, _Ty _Val, _Pr _Pred)
{
    for (_Diff _Idx = (_Hole - 1) / 2; _Hole > _Top && _Pred(*(_First + _Idx), _Val); _Idx = (_Hole - 1) / 2) {
        *(_First + _Hole) = *(_First + _Idx);
        _Hole = _Idx;
    }
    *(_First + _Hole) = _Val;
}
typedef pair<u64, Payload> Elem;
int main()
{
    Elem h[12]; int n = 0;
    u64 keys[9] = { 5, ((u64)1 << 32), 3, 0xFFFFFFFFu, ((u64)1 << 32) + 1, 0, 7, ((u64)2 << 32), 1 };
    for (int i = 0; i < 9; ++i) {
        Elem v; v.first = keys[i]; SETPAY(v.second, i);
        _Push_heap(h, n, 0, v, CKeyPairCompareLess<u64, Payload>()); ++n;
        for (int c = 1; c < n; ++c)
            if (h[(c - 1) / 2].first < h[c].first) { std::printf("00a7d6f0_TEST FAIL\n"); return 1; }
    }
    if (h[0].first != keys[7] || !CHKPAY(h[0].second, 7)) { std::printf("00a7d6f0_TEST FAIL\n"); return 1; }
    std::printf("00a7d6f0_TEST PASS\n");
    return 0;
}