// Behaviour test for 0x005289C0 CNPCNameMapTree::_M_create_node: the node is a fresh malloc of sizeof(node), the UID
// is copied and both strings go through their copy constructors (narrow first, then wide).
#include <cstdio>
#include <cstdlib>
#include <new>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#define K64(x) x##ui64
#else
typedef unsigned long long u64;
#define K64(x) x##ULL
#endif
static int g_order[4]; static int g_n;
class CCharString {
public:
    const char* PString;
    CCharString(const char* s) : PString(s) {}
    CCharString(const CCharString& o) : PString(o.PString) { g_order[g_n++] = 1; }
};
class CWideString {
public:
    const unsigned short* PString;
    CWideString(const unsigned short* s) : PString(s) {}
    CWideString(const CWideString& o) : PString(o.PString) { g_order[g_n++] = 2; }
};
template <class T1, class T2> struct pair {
    T1 first; T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
};
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
struct _Rb_tree_node_base {
    char _M_color; _Rb_tree_node_base* _M_parent; _Rb_tree_node_base* _M_left; _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base { V _M_value_field; };
class CNPCNameMapTree {
public:
    typedef pair<const u64, pair<CCharString, CWideString> > value_type;
    typedef _Rb_tree_node<value_type> _Node;
    _Node* _M_header;
    unsigned long _M_node_count;
    static _Node* allocate(unsigned int n) { return (_Node*)std::malloc(n * sizeof(_Node)); }
    _Node* _M_create_node(const value_type& x);
};
CNPCNameMapTree::_Node* CNPCNameMapTree::_M_create_node(const value_type& x)
{
    _Node* tmp = allocate(1);
    _Construct(&tmp->_M_value_field, x);
    return tmp;
}

int main()
{
    static const char name[] = "Guard";
    static const unsigned short wide[] = { 'G', 'u', 'a', 'r', 'd', 0 };
    CNPCNameMapTree t; t._M_header = 0; t._M_node_count = 0;
    g_n = 0;   // the value_type below copies the strings once while it is built
    CNPCNameMapTree::value_type v(K64(0x123456789ABCDEF0), pair<CCharString, CWideString>(CCharString(name), CWideString(wide)));
    g_n = 0;
    CNPCNameMapTree::_Node* p = t._M_create_node(v);
    int ok = p != 0 && p->_M_value_field.first == K64(0x123456789ABCDEF0) && p->_M_value_field.second.first.PString == name
          && p->_M_value_field.second.second.PString == wide && g_n == 2 && g_order[0] == 1 && g_order[1] == 2;
    std::free(p);
    if (!ok) { std::printf("005289c0_TEST FAIL\n"); return 1; }
    std::printf("005289c0_TEST PASS\n");
    return 0;
}