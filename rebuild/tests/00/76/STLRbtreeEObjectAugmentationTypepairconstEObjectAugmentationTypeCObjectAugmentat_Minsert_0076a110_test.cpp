// Behaviour test for 0x0076A110 _Rb_tree::_M_insert: new nodes link left of the header (first node: root,
// leftmost, rightmost), left of y when the key is smaller (leftmost maintained), else right (rightmost
// maintained); children cleared, parent set, count bumped, rebalance called once per insert.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

static int gStrCopies = 0;

class CCharString {
public:
    char* PStringData;
    CCharString(const char* s, long) { PStringData = new char[std::strlen(s) + 1]; std::strcpy(PStringData, s); }
    CCharString(const CCharString& o) { ++gStrCopies; PStringData = new char[std::strlen(o.PStringData) + 1]; std::strcpy(PStringData, o.PStringData); }
    ~CCharString() { delete[] PStringData; }
};

enum EObjectAugmentationType { AUG_NONE, AUG_FIRE, AUG_LIGHTNING, AUG_SILVER };

class CObjectAugmentationSoundSet { public:
    bool          Played;
    CCharString   Sound;
    unsigned int  Handle;
    CObjectAugmentationSoundSet() : Played(false), Sound("", -1), Handle(0xFFFFFFFFu) { }
};

template <class _T1, class _T2>
struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};

struct _Rb_tree_node_base {
    long                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

typedef pair<const EObjectAugmentationType, CObjectAugmentationSoundSet> SSoundSetValue;
struct SSoundSetNode : _Rb_tree_node_base { SSoundSetValue _M_value_field; };

static int gRebalances = 0;
static void Rebalance(_Rb_tree_node_base*, _Rb_tree_node_base*&) { ++gRebalances; }   // colour fix-up stand-in

struct CSoundSetTree {
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    SSoundSetNode* _M_create_node(const SSoundSetValue& __x)
    {
        SSoundSetNode* __tmp = (SSoundSetNode*)std::malloc(sizeof(SSoundSetNode));
        new (&__tmp->_M_value_field) SSoundSetValue(__x);
        return __tmp;
    }
    _Rb_tree_node_base* _M_insert(_Rb_tree_node_base* __x_, _Rb_tree_node_base* __y_, const SSoundSetValue& __v, _Rb_tree_node_base* __w_)
    {
        SSoundSetNode* __w = (SSoundSetNode*)__w_;
        SSoundSetNode* __y = (SSoundSetNode*)__y_;
        SSoundSetNode* __x = (SSoundSetNode*)__x_;
        SSoundSetNode* __z;
        if (__y == _M_header || (__w == 0 && (__x != 0 || __v.first < __y->_M_value_field.first))) {
            __z = _M_create_node(__v);
            __y->_M_left = __z;
            if (__y == _M_header) {
                _M_header->_M_parent = __z;
                _M_header->_M_right = __z;
            }
            else if (__y == _M_header->_M_left)
                _M_header->_M_left = __z;
        }
        else {
            __z = _M_create_node(__v);
            __y->_M_right = __z;
            if (__y == _M_header->_M_right)
                _M_header->_M_right = __z;
        }
        __z->_M_parent = __y;
        __z->_M_left = 0;
        __z->_M_right = 0;
        Rebalance(__z, _M_header->_M_parent);
        ++_M_node_count;
        return __z;
    }
};

int main()
{
    _Rb_tree_node_base header = { 0, 0, 0, 0 };
    header._M_left = &header; header._M_right = &header;
    CSoundSetTree tree = { &header, 0 };
    CObjectAugmentationSoundSet set;
    _Rb_tree_node_base* a = tree._M_insert(0, &header, SSoundSetValue(AUG_LIGHTNING, set), 0);
    int ok = header._M_parent == a && header._M_left == a && header._M_right == a && a->_M_parent == &header;
    _Rb_tree_node_base* b = tree._M_insert(0, a, SSoundSetValue(AUG_FIRE, set), 0);          // smaller: left of a
    if (a->_M_left != b || header._M_left != b || header._M_right != a || b->_M_parent != a) ok = 0;
    _Rb_tree_node_base* c = tree._M_insert(0, a, SSoundSetValue(AUG_SILVER, set), 0);        // larger: right of a
    if (a->_M_right != c || header._M_right != c || header._M_left != b) ok = 0;
    if (c->_M_left || c->_M_right || tree._M_node_count != 3 || gRebalances != 3) ok = 0;
    if (((SSoundSetNode*)c)->_M_value_field.first != AUG_SILVER) ok = 0;
    _Rb_tree_node_base* nodes[3] = { a, b, c };
    for (int i = 0; i < 3; ++i) { ((SSoundSetNode*)nodes[i])->_M_value_field.~SSoundSetValue(); std::free(nodes[i]); }
    if (!ok) { std::printf("0076a110_TEST FAIL\n"); return 1; }
    std::printf("0076a110_TEST PASS\n");
    return 0;
}