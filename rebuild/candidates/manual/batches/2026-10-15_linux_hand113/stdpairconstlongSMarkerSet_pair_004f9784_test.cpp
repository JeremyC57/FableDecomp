#include <cstdio>
// Behaviour test for pair<const long, SMarkerSet>::pair(const pair&) (0x004F9784): key copied, tree copied
// through its own copy constructor, the two trailing words copied.
static int g_treeCopies = 0;
struct SMarkerTree {
    int nodes;
    SMarkerTree(int n) : nodes(n) { }
    SMarkerTree(const SMarkerTree& o) : nodes(o.nodes) { ++g_treeCopies; }
};
struct SMarkerSet {
    SMarkerTree m_tree; long m_first; long m_second;
    SMarkerSet(int n, long a, long b) : m_tree(n), m_first(a), m_second(b) { }
    SMarkerSet(const SMarkerSet& o) : m_tree(o.m_tree), m_first(o.m_first), m_second(o.m_second) { }
};
template <class T1, class T2> struct pair {
    T1 first; T2 second;
    pair(T1 a, const T2& b) : first(a), second(b) { }
    pair(const pair& p) : first(p.first), second(p.second) { }
};
int main()
{
    pair<const long, SMarkerSet> a(17, SMarkerSet(3, 100, -5));
    g_treeCopies = 0;
    pair<const long, SMarkerSet> b(a);
    if (b.first != 17 || b.second.m_tree.nodes != 3 || b.second.m_first != 100 || b.second.m_second != -5 || g_treeCopies != 1) { std::printf("004f9784_TEST FAIL\n"); return 1; }
    std::printf("004f9784_TEST PASS\n");
    return 0;
}
