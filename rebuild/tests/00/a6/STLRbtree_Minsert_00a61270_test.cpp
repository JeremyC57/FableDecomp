// Behaviour test for 0x00A61270 _STL::_Rb_tree<CWideString, pair<const CWideString, CEntry>, ...>::_M_insert(x, y, v, w)
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_copies = 0;

class CWideString {
public:
    const wchar_t* PStringData;
    CWideString(const wchar_t* text) : PStringData(text) { }
    CWideString(const CWideString& other) : PStringData(other.PStringData) { ++g_copies; }   // 0x0099B720 stand-in
    ~CWideString() { }
    bool operator<(const CWideString& other) const          // 0x0099AF70 stand-in: ordinal wide compare
    {
        const wchar_t* a = PStringData;
        const wchar_t* b = other.PStringData;
        while (*a != 0 && *a == *b) { ++a; ++b; }
        return *a < *b;
    }
};
class CChecksumCacheFile {
public:
    class CEntry {
    public:
        unsigned long Data[8];
    };
};

namespace _STL {
template <class T1, class T2> class pair {
public:
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
template <class T> struct less {
    bool operator()(const T& a, const T& b) const { return a < b; }
};
template <class Pair> struct _Select1st {
    const typename Pair::first_type& operator()(const Pair& x) const { return x.first; }
};
struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class Value> struct _Rb_tree_node : _Rb_tree_node_base {
    Value _M_value_field;
};
struct _Rb_global {
    static void _Rebalance(_Rb_tree_node_base*, _Rb_tree_node_base*&) { }   // 0x0042971D stand-in: no recolouring
};

template <class Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
};

template <class Key, class Value, class KeyOfValue, class Compare> class _Rb_tree {
public:
    typedef _Rb_tree_node<Value> _Node;
    typedef _Rb_tree_iterator<Value> iterator;
    _Rb_tree_node_base* _M_header;
    unsigned int _M_node_count;
    Compare _M_key_compare;
    _Rb_tree() : _M_node_count(0)
    {
        _M_header = (_Rb_tree_node_base*)std::malloc(sizeof(_Node));
        _M_header->_M_color = false;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header->_M_right = _M_header;
    }
    static const Key& _S_key(_Rb_tree_node_base* x) { return KeyOfValue()(((_Node*)x)->_M_value_field); }

    _Node* _M_create_node(const Value& x)
    {
        _Node* tmp = (_Node*)std::malloc(sizeof(_Node));
        new (&tmp->_M_value_field) Value(x);
        return tmp;
    }
    iterator _M_insert(_Rb_tree_node_base* x_, _Rb_tree_node_base* y_, const Value& v, _Rb_tree_node_base* w_ = 0)
    {
        _Node* w = (_Node*)w_;
        _Node* x = (_Node*)x_;
        _Node* y = (_Node*)y_;
        _Node* z;
        if (y == _M_header || (w == 0 && (x != 0 || _M_key_compare(KeyOfValue()(v), _S_key(y))))) {
            z = _M_create_node(v);
            y->_M_left = z;
            if (y == _M_header) {
                _M_header->_M_parent = z;
                _M_header->_M_right = z;
            }
            else if (y == _M_header->_M_left)
                _M_header->_M_left = z;
        }
        else {
            z = _M_create_node(v);
            y->_M_right = z;
            if (y == _M_header->_M_right)
                _M_header->_M_right = z;
        }
        z->_M_parent = y;
        z->_M_left = 0;
        z->_M_right = 0;
        _Rb_global::_Rebalance(z, _M_header->_M_parent);
        ++_M_node_count;
        return iterator(z);
    }
};
}   // namespace _STL

typedef _STL::pair<const CWideString, CChecksumCacheFile::CEntry> CValue;
typedef _STL::_Rb_tree<CWideString, CValue, _STL::_Select1st<CValue>, _STL::less<CWideString> > CTree;
typedef _STL::_Rb_tree_node_base CBase;

static const wchar_t* KeyOf(CBase* n) { return ((CTree::_Node*)n)->_M_value_field.first.PStringData; }
static bool Same(const wchar_t* a, const wchar_t* b) { while (*a != 0 && *a == *b) { ++a; ++b; } return *a == *b; }
static CBase* Next(CBase* n)                                // in-order successor (test helper)
{
    if (n->_M_right) { n = n->_M_right; while (n->_M_left) n = n->_M_left; return n; }
    CBase* p = n->_M_parent;
    while (n == p->_M_right) { n = p; p = p->_M_parent; }
    return n->_M_right != p ? p : n;
}
static bool InOrder(CTree& t, const wchar_t* const* keys, int n)
{
    CBase* it = t._M_header->_M_left;
    for (int i = 0; i < n; ++i, it = Next(it))
        if (it == t._M_header || !Same(KeyOf(it), keys[i])) return false;
    return it == t._M_header && (int)t._M_node_count == n;
}
static CValue Val(const wchar_t* key, unsigned long checksum)
{
    CChecksumCacheFile::CEntry e;
    for (int i = 0; i < 8; ++i) e.Data[i] = checksum + i;
    return CValue(CWideString(key), e);
}

int main()
{
    CTree t;
    CValue m = Val(L"m", 10), c = Val(L"c", 20), x = Val(L"x", 30), a = Val(L"a", 40);
    CTree::iterator i1 = t._M_insert(0, t._M_header, m);            // empty tree: y == header
    bool ok = t._M_header->_M_parent == i1._M_node && t._M_header->_M_left == i1._M_node &&
              t._M_header->_M_right == i1._M_node && i1._M_node->_M_parent == t._M_header;
    CBase* root = i1._M_node;
    CTree::iterator i2 = t._M_insert(0, root, x);                   // "x" > "m": right child, new rightmost
    CTree::iterator i3 = t._M_insert(0, root, c);                   // "c" < "m": left child, new leftmost
    CTree::iterator i4 = t._M_insert(i3._M_node, i3._M_node, a);    // x != 0 forces the left link
    ok = ok && root->_M_right == i2._M_node && t._M_header->_M_right == i2._M_node && root->_M_left == i3._M_node &&
         i3._M_node->_M_left == i4._M_node && t._M_header->_M_left == i4._M_node && i4._M_node->_M_parent == i3._M_node;
    const wchar_t* keys[4] = { L"a", L"c", L"m", L"x" };
    ok = ok && InOrder(t, keys, 4) && g_copies >= 4 &&
         ((CTree::_Node*)i2._M_node)->_M_value_field.second.Data[7] == 37;   // the CEntry copied whole
    if (!ok) { std::printf("00a61270_TEST FAIL\n"); return 1; }
    std::printf("00a61270_TEST PASS\n");
    return 0;
}