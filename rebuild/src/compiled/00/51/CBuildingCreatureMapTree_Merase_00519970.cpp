// CBuildingCreatureMapTree::_M_erase(_Rb_tree_node* x) -- STLport _Rb_tree::_M_erase of map<CThingBuilding*, list<CThingAICreature*> >: recursively free a subtree, emptying and freeing each node's creature list on the way
// Leaf: ?_M_erase
//
// Retail 0x00519970, 108 bytes (+ int3 padding to 112), thiscall `ret 4`.  Callees: itself (right subtree), 0x00BFEA14 free (list nodes, the
//   list sentinel, the tree node).
// Callers: the landed tree clear/erase bodies 0x0051AF20 / 0x0051B720 ("CRetailTreeerase00519970").
// Identity: the map of 0x0051AA00 / 0x00517590 (pointer key, list of 4-byte values; see 0x00517590).  Node: color +0,
//   parent +4, left +8, right +0xC, key +0x10, list sentinel pointer +0x14 (0x18 bytes).
// Shape: STLport `while (x != 0) { _M_erase(_S_right(x)); y = _S_left(x); _Destroy(&x->_M_value_field);
//   _M_header.deallocate(x, 1); x = y; }` with the pair's destructor inline: `~_List_base() { clear();
//   _M_node.deallocate(_M_node._M_data, 1); }` where clear() frees every node then re-links the sentinel to itself (the
//   sentinel pointer re-read after each store, which may alias it).  deallocate is `if (p) free(p)`: the tree node's
//   test folds away (x != 0), the sentinel's stays.
// Label: catalog "TreeNode_DestroyRecursive" (propagated).
extern "C" void __cdecl free(void* p);

class CThingBuilding;
class CThingAICreature;

namespace std {
template <class T> class allocator {
public:
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
template <class T> class _List_base {
public:
    _List_node<T>* _M_node;                    // sentinel (alloc proxy data)
    ~_List_base()
    {
        clear();
        allocator<_List_node<T> >().deallocate(_M_node, 1);
    }
    void clear()
    {
        _List_node<T>* cur = (_List_node<T>*)_M_node->_M_next;
        while (cur != _M_node) {
            _List_node<T>* tmp = cur;
            cur = (_List_node<T>*)cur->_M_next;
            allocator<_List_node<T> >().deallocate(tmp, 1);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
};
template <class T> class list : public _List_base<T> {
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T> inline void _Destroy(T* p) { p->~T(); }
}

struct _Rb_tree_node_base {
    char                _M_color;              // +0x0
    _Rb_tree_node_base* _M_parent;             // +0x4
    _Rb_tree_node_base* _M_left;               // +0x8
    _Rb_tree_node_base* _M_right;              // +0xC
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                          // +0x10
};

class CBuildingCreatureMapTree {               // STLport _Rb_tree<CThingBuilding*, pair<CThingBuilding* const, list<CThingAICreature*> >, ...>
public:
    typedef std::pair<CThingBuilding* const, std::list<CThingAICreature*> > value_type;
    typedef _Rb_tree_node<value_type> _Node;
    _Node*        _M_header;                   // +0x0
    unsigned long _M_node_count;               // +0x4
    void _M_erase(_Node* x);
};

void CBuildingCreatureMapTree::_M_erase(_Node* x)
{
    while (x != 0) {
        _M_erase((_Node*)x->_M_right);
        _Node* y = (_Node*)x->_M_left;
        std::_Destroy(&x->_M_value_field);
        std::allocator<_Node>().deallocate(x, 1);
        x = y;
    }
}