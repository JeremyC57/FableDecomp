// Behaviour test for 0x00519970 _M_erase of map<CThingBuilding*, list<CThingAICreature*> >: a hand-built 4-node tree whose
// nodes hold 0..3-element lists is torn down completely -- every list node, every list sentinel (after being re-linked to
// itself) and every tree node is freed exactly once; a null subtree frees nothing.
#include <cstdio>
#include <cstdlib>
static void* g_freed[64]; static int g_nf;
static void counted_free(void* p) { g_freed[g_nf++] = p; }
class CThingBuilding;
class CThingAICreature;

template <class T> class allocator {
public:
    void deallocate(T* p, unsigned int) { if (p != 0) counted_free(p); }
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
    typedef pair<CThingBuilding* const, list<CThingAICreature*> > value_type;
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
        _Destroy(&x->_M_value_field);
        allocator<_Node>().deallocate(x, 1);
        x = y;
    }
}

typedef CBuildingCreatureMapTree::_Node Node;
typedef _List_node<CThingAICreature*> LNode;
static void* g_alloc[64]; static int g_na;
static void* track(void* p) { g_alloc[g_na++] = p; return p; }
static Node* make(int listLen)
{
    Node* n = (Node*)track(std::malloc(sizeof(Node)));
    n->_M_left = n->_M_right = 0;
    LNode* h = (LNode*)track(std::malloc(sizeof(LNode)));
    h->_M_next = h; h->_M_prev = h;
    for (int i = 0; i < listLen; ++i) {
        LNode* e = (LNode*)track(std::malloc(sizeof(LNode)));
        e->_M_next = h; e->_M_prev = h->_M_prev; h->_M_prev->_M_next = e; h->_M_prev = e;
    }
    n->_M_value_field.second._M_node = h;
    return n;
}
int main()
{
    CBuildingCreatureMapTree t; t._M_header = 0; t._M_node_count = 0;
    t._M_erase(0);
    int ok = g_nf == 0;
    Node* root = make(2); Node* l = make(0); Node* r = make(3); Node* rl = make(1);
    root->_M_left = l; root->_M_right = r; r->_M_left = rl;
    t._M_erase(root);
    ok &= g_nf == g_na;
    for (int i = 0; i < g_na; ++i) {
        int c = 0;
        for (int j = 0; j < g_nf; ++j) if (g_freed[j] == g_alloc[i]) ++c;
        ok &= c == 1;
    }
    for (int i = 0; i < g_na; ++i) std::free(g_alloc[i]);
    if (!ok) { std::printf("00519970_TEST FAIL\n"); return 1; }
    std::printf("00519970_TEST PASS\n");
    return 0;
}