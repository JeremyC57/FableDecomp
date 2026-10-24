// Behaviour test for 0052b8b0: a copy of the source's model and logic with its out-of-line callees stubbed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_fail = 0;
static void Check(bool ok, int line)
{
    if (!ok) {
        g_fail = 1;
        std::printf("check failed at line %d\n", line);
    }
}
#define CHECK(c) Check((c) ? true : false, __LINE__)

static int g_strings = 0;                     // live CCharString + CWideString objects
class CCharString {
public:
    const char* PData;
    CCharString() : PData("") { ++g_strings; }
    CCharString(const CCharString& o) : PData(o.PData) { ++g_strings; }
    ~CCharString() { --g_strings; }
};
class CWideString {
public:
    int PData;
    CWideString() : PData(0) { ++g_strings; }
    CWideString(const CWideString& o) : PData(o.PData) { ++g_strings; }
    ~CWideString() { --g_strings; }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair() : first(T1()), second(T2()) { }
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& p) : first(p.first), second(p.second) { }
};
typedef pair<CCharString, CWideString> SNPCName;
typedef pair<const u64, SNPCName> SNPCNamePair;
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
struct SNPCNameNode : _Rb_tree_node_base {
    SNPCNamePair _M_value_field;
    SNPCNameNode(const SNPCNamePair& v) : _M_value_field(v) { }
};
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) : _M_node(it._M_node) { }
    SNPCNamePair& operator*() const { return ((SNPCNameNode*)_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& y) const { return _M_node == y._M_node; }
};
static int g_inserts = 0;
static _Rb_tree_node_base* g_lastHint = 0;
class CNPCNameTree {
public:
    typedef _Rb_tree_iterator iterator;
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    less<u64>           _M_key_compare;
    static const u64& _S_key(_Rb_tree_node_base* x) { return ((SNPCNameNode*)x)->_M_value_field.first; }
    _Rb_tree_node_base* _M_lower_bound(const u64& k) const      // stub for 0x00528610 (the real walk)
    {
        _Rb_tree_node_base* y = _M_header;
        _Rb_tree_node_base* x = _M_header->_M_parent;
        while (x != 0)
            if (!_M_key_compare(_S_key(x), k))
                y = x, x = x->_M_left;
            else
                x = x->_M_right;
        return y;
    }
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
    iterator lower_bound(const u64& k) { return iterator(_M_lower_bound(k)); }
    iterator insert_unique(iterator position, const SNPCNamePair& v)   // stub for 0x0052A2D0: plain BST insert
    {
        ++g_inserts;
        g_lastHint = position._M_node;
        SNPCNameNode* z = new SNPCNameNode(v);
        z->_M_left = z->_M_right = 0;
        _Rb_tree_node_base* y = _M_header;
        _Rb_tree_node_base* x = _M_header->_M_parent;
        bool left = true;
        while (x != 0) {
            y = x;
            left = v.first < _S_key(x);
            x = left ? x->_M_left : x->_M_right;
        }
        z->_M_parent = y;
        if (y == _M_header) _M_header->_M_parent = z;
        else if (left) y->_M_left = z;
        else y->_M_right = z;
        ++_M_node_count;
        return iterator(z);
    }
    less<u64> key_comp() const { return _M_key_compare; }
};
class CNPCNameMap {
public:
    typedef CNPCNameTree::iterator iterator;
    CNPCNameTree _M_t;
    iterator end() { return _M_t.end(); }
    iterator lower_bound(const u64& k) { return _M_t.lower_bound(k); }
    iterator insert(iterator position, const SNPCNamePair& x) { return _M_t.insert_unique(position, x); }
    less<u64> key_comp() const { return _M_t.key_comp(); }
    SNPCName& operator[](const u64& k)
    {
        iterator i = lower_bound(k);
        if (i == end() || key_comp()(k, (*i).first))
            i = insert(i, SNPCNamePair(k, SNPCName()));
        return (*i).second;
    }
};
static void FreeTree(_Rb_tree_node_base* n)
{
    if (n == 0) return;
    FreeTree(n->_M_left);
    FreeTree(n->_M_right);
    delete (SNPCNameNode*)n;
}
static void RunTest()
{
    _Rb_tree_node_base header;
    header._M_parent = 0;
    header._M_left = header._M_right = &header;
    CNPCNameMap map;
    map._M_t._M_header = &header;
    map._M_t._M_node_count = 0;

    u64 k1 = ((u64)5 << 32) | 9;
    SNPCName& a = map[k1];                         // absent: inserted with an empty name pair, hint = end()
    CHECK(g_inserts == 1 && g_lastHint == &header && map._M_t._M_node_count == 1);
    CHECK(a.second.PData == 0);
    a.second.PData = 77;
    a.first.PData = "Bob";
    SNPCName& b = map[k1];                         // present: no insert, the same mapped pair
    CHECK(&b == &a && g_inserts == 1 && b.second.PData == 77);
    u64 k0 = 3;                                    // smaller: lower_bound is k1's node, k < key -> insert there
    SNPCName& c = map[k0];
    CHECK(g_inserts == 2 && &c != &a && c.second.PData == 0 && g_lastHint == header._M_parent);
    c.second.PData = 11;
    CHECK(map[k0].second.PData == 11 && map[k1].second.PData == 77 && g_inserts == 2);
    // only the node values remain alive: 2 nodes x 2 strings, every temporary destroyed
    CHECK(g_strings == 4);
    FreeTree(header._M_parent);
    CHECK(g_strings == 0);
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("0052b8b0_TEST FAIL\n");
        return 1;
    }
    std::printf("0052b8b0_TEST PASS\n");
    return 0;
}