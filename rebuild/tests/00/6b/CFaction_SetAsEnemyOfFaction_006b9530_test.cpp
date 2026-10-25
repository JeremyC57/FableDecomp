// Test for 0x006B9530 CFaction::SetAsEnemyOfFaction(const CFaction&): after the call the other faction's name is
// enemy only; a second call changes nothing.
#include <stdio.h>
#include <string.h>
class CCharString {
public:
    void* PStringData;                            // +0x0 CCharStringData*
};

namespace std {
template <class T> struct less {};
template <class T> struct _Identity {};
template <class T> class allocator {};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
struct _Rb_tree_node_base;
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
};
template <class K, class V, class KoV, class Cmp, class A>
class _Rb_tree {
public:
    typedef _Rb_tree_iterator<V> iterator;
    typedef unsigned int size_type;
    _Rb_tree_node_base* _M_header;                // +0x0
    size_type           _M_node_count;            // +0x4
    Cmp                 _M_key_compare;           // +0x8
    pair<iterator, bool> insert_unique(const V& v);   // 0x004AC380
    size_type erase(const K& k);                       // 0x004BB270
};
template <class K, class Cmp = less<K>, class A = allocator<K> >
class set {
public:
    typedef _Rb_tree<K, K, _Identity<K>, Cmp, A> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    typedef typename _Rep_type::size_type size_type;
    _Rep_type _M_t;
    pair<iterator, bool> insert(const K& x) { return _M_t.insert_unique(x); }
    size_type erase(const K& x) { return _M_t.erase(x); }
};
}

class CFaction {
public:
    CCharString                Name;              // +0x00
    std::set<CCharString>      AlliedFactions;    // +0x04
    std::set<CCharString>      EnemyFactions;     // +0x10
    void SetAsEnemyOfFaction(const CFaction& faction);
};


// Test stand-in for the two out-of-line tree members: a set of up to 8 name pointers kept in a static side table keyed by
// the tree's address (the tree's own words are left alone).
struct SFake { const void* tree; const void* names[8]; int n; };
static SFake g_sets[4]; static int g_nsets;
static SFake* Find(const void* tree)
{
    for (int i = 0; i < g_nsets; ++i) if (g_sets[i].tree == tree) return &g_sets[i];
    g_sets[g_nsets].tree = tree; g_sets[g_nsets].n = 0; return &g_sets[g_nsets++];
}
static int IndexOf(SFake* s, const void* k) { for (int i = 0; i < s->n; ++i) if (s->names[i] == k) return i; return -1; }
typedef std::_Rb_tree<CCharString, CCharString, std::_Identity<CCharString>, std::less<CCharString>, std::allocator<CCharString> > Tree;
Tree::size_type Tree::erase(const CCharString& k)
{
    SFake* s = Find(this); int i = IndexOf(s, k.PStringData);
    if (i < 0) return 0;
    s->names[i] = s->names[--s->n]; return 1;
}
std::pair<Tree::iterator, bool> Tree::insert_unique(const CCharString& v)
{
    SFake* s = Find(this); bool added = IndexOf(s, v.PStringData) < 0;
    if (added) s->names[s->n++] = v.PStringData;
    return std::pair<iterator, bool>(iterator(0), added);
}
static bool In(std::set<CCharString>& set, const CFaction& f) { return IndexOf(Find(&set._M_t), f.Name.PStringData) >= 0; }
int main()
{
    static CFaction a, b;
    memset(&a, 0, sizeof a); memset(&b, 0, sizeof b);
    a.Name.PStringData = (void*)"GUARDS"; b.Name.PStringData = (void*)"BANDITS";
    a.AlliedFactions.insert(b.Name); a.EnemyFactions.insert(b.Name);       // start in both sets
    a.SetAsEnemyOfFaction(b);
    bool inA = In(a.AlliedFactions, b), inE = In(a.EnemyFactions, b);
    if (!(!inA && inE)) return 1;
    a.SetAsEnemyOfFaction(b);
    inA = In(a.AlliedFactions, b); inE = In(a.EnemyFactions, b);
    if (!(!inA && inE)) return 2;
    if (Find(&a.AlliedFactions._M_t)->n > 1 || Find(&a.EnemyFactions._M_t)->n > 1) return 3;
    puts("006b9530_TEST PASS");
    return 0;
}
