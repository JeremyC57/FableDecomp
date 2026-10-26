#include <cstdio>
#include <cstdlib>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static u64 U64(unsigned int hi, unsigned int lo) { return ((u64)hi << 32) | lo; }

// Model: CTCBoastingCrowdControl::OnKillCrowd -- find the hero's UID in the map and erase the node (rebalance,
// destroy the intelligent pointer, free, --count).
static int g_ptrDtors = 0;

class CThing {
public:
    unsigned char    _pad00[0x18];
    u64 UniqueID;
    u64 GetUID() const { return UniqueID; }
};

struct CBaseIntelligentPointer {
    void* m_pItem;
    ~CBaseIntelligentPointer() { ++g_ptrDtors; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: };

struct _Rb_tree_node_base {
    long                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

static _Rb_tree_node_base* g_lastRoot;
static _Rb_tree_node_base* g_lastLeft;
static _Rb_tree_node_base* g_lastRight;

struct _Rb_global {
    // stand-in: unlinks a leaf node (the test only erases leaves) and reports the header words it was handed
    static _Rb_tree_node_base* _Rebalance_for_erase(_Rb_tree_node_base* z, _Rb_tree_node_base*& root,
                                                    _Rb_tree_node_base*& leftmost, _Rb_tree_node_base*& rightmost)
    {
        g_lastRoot = root;
        g_lastLeft = leftmost;
        g_lastRight = rightmost;
        _Rb_tree_node_base* p = z->_M_parent;
        if (z == root)
            root = 0;
        else if (p->_M_left == z)
            p->_M_left = 0;
        else
            p->_M_right = 0;
        if (leftmost == z)
            leftmost = (z == g_lastRoot) ? p : p;
        if (rightmost == z)
            rightmost = p;
        return z;
    }
};

template <class K, class V> struct pair {
    K first;
    V second;
};

template <class T> inline void _Destroy(T* p) { p->~T(); }

struct CHeroCrowdMap {
    typedef pair<const u64, CIntelligentPointer<CThing> > value_type;
    struct _Node : _Rb_tree_node_base {
        value_type _M_value_field;
    };
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;

    _Rb_tree_node_base* find(const u64& key)
    {
        _Rb_tree_node_base* n = _M_header->_M_parent;
        while (n) {
            u64 k = static_cast<_Node*>(n)->_M_value_field.first;
            if (k == key)
                return n;
            n = key < k ? n->_M_left : n->_M_right;
        }
        return _M_header;
    }
    _Rb_tree_node_base* end() { return _M_header; }
    void erase(_Rb_tree_node_base* pos)
    {
        _Node* y = static_cast<_Node*>(_Rb_global::_Rebalance_for_erase(pos, _M_header->_M_parent,
                                                                         _M_header->_M_left, _M_header->_M_right));
        _Destroy(&y->_M_value_field);
        if (y)
            std::free(y);
        --_M_node_count;
    }
};

class CTCBoastingCrowdControl {
public:
    CHeroCrowdMap HeroCrowdThings;
    void OnKillCrowd(CThing& hero)
    {
        _Rb_tree_node_base* it = HeroCrowdThings.find(hero.GetUID());
        if (it != HeroCrowdThings.end())
            HeroCrowdThings.erase(it);
    }
};

static CHeroCrowdMap::_Node* NewNode(u64 key, _Rb_tree_node_base* parent)
{
    CHeroCrowdMap::_Node* n = (CHeroCrowdMap::_Node*)std::malloc(sizeof(CHeroCrowdMap::_Node));
    *(u64*)&n->_M_value_field.first = key;
    n->_M_value_field.second.m_pItem = 0;
    n->_M_parent = parent;
    n->_M_left = 0;
    n->_M_right = 0;
    return n;
}

int main()
{
    _Rb_tree_node_base header;
    CHeroCrowdMap::_Node* root = NewNode(U64(5, 1), &header);
    CHeroCrowdMap::_Node* left = NewNode(U64(1, 2), root);
    CHeroCrowdMap::_Node* right = NewNode(U64(9, 3), root);
    root->_M_left = left;
    root->_M_right = right;
    header._M_parent = root;
    header._M_left = left;
    header._M_right = right;
    CTCBoastingCrowdControl control;
    control.HeroCrowdThings._M_header = &header;
    control.HeroCrowdThings._M_node_count = 3;

    CThing hero;
    hero.UniqueID = U64(7, 0);                      // not present: nothing happens
    control.OnKillCrowd(hero);
    if (control.HeroCrowdThings._M_node_count != 3 || g_ptrDtors != 0)
        goto fail;
    hero.UniqueID = U64(1, 2);                      // the leftmost leaf goes
    control.OnKillCrowd(hero);
    if (control.HeroCrowdThings._M_node_count != 2 || g_ptrDtors != 1 || root->_M_left != 0
        || g_lastRoot != root || g_lastLeft != left || g_lastRight != right || header._M_left != root)
        goto fail;
    hero.UniqueID = U64(9, 3);                      // then the rightmost leaf
    control.OnKillCrowd(hero);
    if (control.HeroCrowdThings._M_node_count != 1 || g_ptrDtors != 2 || root->_M_right != 0 || header._M_right != root)
        goto fail;
    std::free(root);
    std::printf("00783240_TEST PASS\n");
    return 0;
fail:
    std::printf("00783240_TEST FAIL\n");
    return 1;
}