// Behaviour test for 0x00756420 _STL::_Rb_tree<CCharString,pair<long,long>>::erase(iterator first, iterator last)
#include <cstdio>

static int g_dtorCalls = 0;
static void* g_freed[16];
static int g_freeCount = 0;
static void* g_victims[16];
static int g_victimCount = 0;
static void* g_erasedRoot = 0;

static void FreeNode(void* p) { g_freed[g_freeCount++] = p; }

class CCharString {
public:
    void* PStringData;
    ~CCharString() { ++g_dtorCalls; }
};

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
struct SCrimeValue { CCharString first; long Duration; long Fine; };
struct _Rb_tree_node_CrimeMap : _Rb_tree_node_base { SCrimeValue _M_value_field; };

// stand-in: records the victim and leaves the links intact (the successor walk ran before it)
_Rb_tree_node_base* _Rebalance_for_erase(_Rb_tree_node_base* z, _Rb_tree_node_base*&, _Rb_tree_node_base*&, _Rb_tree_node_base*&)
{
    g_victims[g_victimCount++] = z;
    return z;
}

struct _Rb_tree_iterator_CrimeMap {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator_CrimeMap(_Rb_tree_node_base* node) : _M_node(node) {}
    void _M_increment()
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* y = _M_node->_M_parent;
            while (_M_node == y->_M_right) {
                _M_node = y;
                y = y->_M_parent;
            }
            if (_M_node->_M_right != y)
                _M_node = y;
        }
    }
    _Rb_tree_iterator_CrimeMap operator++(int) { _Rb_tree_iterator_CrimeMap tmp = *this; _M_increment(); return tmp; }
    bool operator==(const _Rb_tree_iterator_CrimeMap& other) const { return _M_node == other._M_node; }
    bool operator!=(const _Rb_tree_iterator_CrimeMap& other) const { return _M_node != other._M_node; }
};

struct _Rb_tree_CrimeMap {
    typedef _Rb_tree_iterator_CrimeMap iterator;
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    iterator begin() { return iterator(_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
    void _M_erase(_Rb_tree_node_base* x) { g_erasedRoot = x; }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
    }
    void erase(iterator position)
    {
        _Rb_tree_node_CrimeMap* y = (_Rb_tree_node_CrimeMap*)_Rebalance_for_erase(position._M_node,
            _M_header->_M_parent, _M_header->_M_left, _M_header->_M_right);
        y->_M_value_field.first.~CCharString();
        if (y)
            FreeNode(y);
        --_M_node_count;
    }
    void erase(iterator first, iterator last);
};

void _Rb_tree_CrimeMap::erase(iterator first, iterator last)
{
    if (first == begin() && last == end())
        clear();
    else
        while (first != last)
            erase(first++);
}

// 5-node tree: n3 root, n1 (n0, n2), n4; header parent = n3, left = n0, right = n4
static void Build(_Rb_tree_node_CrimeMap* h, _Rb_tree_node_CrimeMap* n, _Rb_tree_CrimeMap& t)
{
    for (int i = 0; i < 5; ++i) { n[i]._M_left = n[i]._M_right = 0; n[i]._M_color = false; }
    n[3]._M_parent = h; n[3]._M_left = &n[1]; n[3]._M_right = &n[4];
    n[1]._M_parent = &n[3]; n[1]._M_left = &n[0]; n[1]._M_right = &n[2];
    n[0]._M_parent = &n[1]; n[2]._M_parent = &n[1]; n[4]._M_parent = &n[3];
    h->_M_parent = &n[3]; h->_M_left = &n[0]; h->_M_right = &n[4];
    t._M_header = h; t._M_node_count = 5;
}

int main()
{
    int ok = 1;
    _Rb_tree_node_CrimeMap h, n[5];
    _Rb_tree_CrimeMap t;
    // a middle range: n1 .. n4 (exclusive) erases n1, n2, n3 in order
    Build(&h, n, t);
    t.erase(_Rb_tree_iterator_CrimeMap(&n[1]), _Rb_tree_iterator_CrimeMap(&n[4]));
    if (g_victimCount != 3 || g_victims[0] != &n[1] || g_victims[1] != &n[2] || g_victims[2] != &n[3]) ok = 0;
    if (g_freeCount != 3 || g_freed[0] != &n[1] || g_freed[2] != &n[3] || g_dtorCalls != 3) ok = 0;
    if (t._M_node_count != 2 || g_erasedRoot != 0) ok = 0;
    // the tail range [n2, end) is not the full range: node by node
    Build(&h, n, t); g_victimCount = 0;
    t.erase(_Rb_tree_iterator_CrimeMap(&n[2]), t.end());
    if (g_victimCount != 3 || g_victims[0] != &n[2] || g_victims[1] != &n[3] || g_victims[2] != &n[4] || t._M_node_count != 2) ok = 0;
    // [begin, end): clear() through _M_erase(root)
    Build(&h, n, t); g_victimCount = 0;
    t.erase(t.begin(), t.end());
    if (g_victimCount != 0 || g_erasedRoot != &n[3] || t._M_node_count != 0) ok = 0;
    if (h._M_left != &h || h._M_right != &h || h._M_parent != 0) ok = 0;
    // empty range: nothing
    Build(&h, n, t); g_victimCount = 0;
    t.erase(_Rb_tree_iterator_CrimeMap(&n[2]), _Rb_tree_iterator_CrimeMap(&n[2]));
    if (g_victimCount != 0 || t._M_node_count != 5) ok = 0;
    if (!ok) { std::printf("00756420_TEST FAIL\n"); return 1; }
    std::printf("00756420_TEST PASS\n");
    return 0;
}