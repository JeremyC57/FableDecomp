// Behaviour test for the FRONTEND_SCREEN-keyed map tree's clear() (0x0059AF2A): a non-empty tree erases from the root and is reset
// to the empty header state; an empty tree is left untouched.
#include <cstdio>

static int g_erase = 0;
static const void* g_root = 0;

enum FRONTEND_SCREEN { FRONTEND_SCREEN_NONE };
namespace NUISystem { class CComponent; }
namespace std {
template <class _Tp> class allocator;
template <class _T1, class _T2> struct pair;
template <class _Pair> struct _Select1st;
template <class _Tp> struct less;
struct _Rb_tree_node_base {
    bool _M_color; _Rb_tree_node_base* _M_parent; _Rb_tree_node_base* _M_left; _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node;
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_node_base*    _M_header;
    unsigned int           _M_node_count;
private:
    void _M_erase(_Link_type __x);
public:
    void clear();
};
typedef pair<const FRONTEND_SCREEN, NUISystem::CComponent*> TScreenValue;
typedef _Rb_tree<FRONTEND_SCREEN, TScreenValue, _Select1st<TScreenValue>, less<FRONTEND_SCREEN>,
                 allocator<TScreenValue> > TTree;
template <> void TTree::_M_erase(_Link_type __x) { ++g_erase; g_root = __x; }
}

int main()
{
    static unsigned char headerRaw[0x20], rootRaw[0x20];
    std::_Rb_tree_node_base* header = (std::_Rb_tree_node_base*)headerRaw;
    std::_Rb_tree_node_base* root = (std::_Rb_tree_node_base*)rootRaw;
    header->_M_parent = root; header->_M_left = root; header->_M_right = root;
    std::TTree t;
    t._M_header = header;
    t._M_node_count = 3;
    t.clear();
    if (g_erase != 1 || g_root != root || header->_M_parent != 0 || header->_M_left != header ||
        header->_M_right != header || t._M_node_count != 0) { std::printf("0059af2a_TEST FAIL\n"); return 1; }
    header->_M_parent = root; header->_M_left = root;
    t.clear();                                           // count 0: untouched
    if (g_erase != 1 || header->_M_parent != root || header->_M_left != root) { std::printf("0059af2a_TEST FAIL empty\n"); return 1; }
    std::printf("0059af2a_TEST PASS\n");
    return 0;
}
