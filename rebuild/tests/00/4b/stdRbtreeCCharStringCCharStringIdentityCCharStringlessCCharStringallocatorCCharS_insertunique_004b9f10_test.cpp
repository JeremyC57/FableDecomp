// Behaviour test for 004b9f10 (_Rb_tree<CCharString, ...>::insert_unique(first, last)): every string of the range is offered to insert_unique once, in order; duplicates are rejected; an empty range inserts nothing.
#include <cstdio>
#include <cstring>
class CCharString {
public:
    void* PStringData;
    const char* c() const { return (const char*)PStringData; }
};
template <class T> struct less {};
template <class T> struct _Identity {};
template <class T> class allocator {};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
struct _Rb_tree_node_base { int index; };
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
};
static int g_offers = 0;
template <class K, class V, class KoV, class Cmp, class A>
class _Rb_tree {
public:
    typedef _Rb_tree_iterator<V> iterator;
    V                  m_values[8];               // the stand-in keeps the unique keys in insertion order
    _Rb_tree_node_base m_nodes[8];
    unsigned int       _M_node_count;
    _Rb_tree() : _M_node_count(0) { }
    pair<iterator, bool> insert_unique(const V& v)    // stands in for 0x004AC380
    {
        ++g_offers;
        for (unsigned int i = 0; i < _M_node_count; ++i)
            if (std::strcmp(m_values[i].c(), v.c()) == 0)
                return pair<iterator, bool>(iterator(&m_nodes[i]), false);
        m_values[_M_node_count] = v;
        m_nodes[_M_node_count].index = (int)_M_node_count;
        ++_M_node_count;
        return pair<iterator, bool>(iterator(&m_nodes[_M_node_count - 1]), true);
    }
    template <class II> void insert_unique(II first, II last)
    {
        for (; first != last; ++first)
            insert_unique(*first);
    }
};
typedef _Rb_tree<CCharString, CCharString, _Identity<CCharString>, less<CCharString>, allocator<CCharString> > CStringSetTree;
int main()
{
    static const char* const names[5] = { "mesh", "skin", "mesh", "bone", "skin" };
    CCharString strings[5];
    for (int i = 0; i < 5; ++i)
        strings[i].PStringData = (void*)names[i];
    CStringSetTree tree;
    const CCharString* first = strings;
    tree.insert_unique(first, first);
    if (g_offers != 0 || tree._M_node_count != 0) { std::printf("004b9f10_TEST FAIL\n"); return 1; }
    tree.insert_unique(first, first + 5);
    if (g_offers != 5 || tree._M_node_count != 3) { std::printf("004b9f10_TEST FAIL\n"); return 1; }
    if (std::strcmp(tree.m_values[0].c(), "mesh") != 0 || std::strcmp(tree.m_values[1].c(), "skin") != 0 || std::strcmp(tree.m_values[2].c(), "bone") != 0) { std::printf("004b9f10_TEST FAIL\n"); return 1; }
    std::printf("004b9f10_TEST PASS\n");
    return 0;
}