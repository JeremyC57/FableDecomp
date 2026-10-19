#include <cstdio>
// Behaviour test for 00a77ca0 CNavQuadTreeNode::ConvertIndexesToPointers: the four child indexes become the table's node
// pointers (in TL, TR, BL, BR order) and each child is then asked to convert its own indexes with the same table.
static int g_finds = 0;
struct C2DVector { float X, Y; };
struct CANavQuadTreeNode;
namespace std {
template <class T> struct less { };
template <class T> struct allocator { };
template <class T1, class T2> struct pair { T1 first; T2 second; };
struct _Rb_tree_node_base {
    unsigned char       _M_color;                  // +0x00
    _Rb_tree_node_base* _M_parent;                 // +0x04
    _Rb_tree_node_base* _M_left;                   // +0x08
    _Rb_tree_node_base* _M_right;                  // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                              // +0x10
};
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* n) : _M_node(n) {}
    _Rb_tree_iterator(const _Rb_tree_iterator& it) : _M_node(it._M_node) {}
    V* operator->() const { return &((_Rb_tree_node<V>*)_M_node)->_M_value_field; }
};
template <class K, class V, class KoV, class C, class A> struct _Rb_tree {
    typedef _Rb_tree_iterator<V> iterator;
    _Rb_tree_node_base* _M_header;
    unsigned long       _M_node_count;
    iterator find(const K& key)                    // test stand-in for 0x00A7F880: lower bound then exact test
    {
        _Rb_tree_node_base* y = _M_header;
        _Rb_tree_node_base* x = _M_header->_M_parent;
        while (x) {
            if (!(((_Rb_tree_node<V>*)x)->_M_value_field.first < key)) { y = x; x = x->_M_left; }
            else x = x->_M_right;
        }
        if (y == _M_header || key < ((_Rb_tree_node<V>*)y)->_M_value_field.first) y = _M_header;
        ++g_finds;
        return iterator(y);
    }
};
template <class P> struct _Select1st { };
template <class K, class V, class C = less<K>, class A = allocator<pair<const K, V> > > class map {
public:
    typedef pair<const K, V> value_type;
    typedef _Rb_tree<K, value_type, _Select1st<value_type>, C, A> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator find(const K& key) { return _M_t.find(key); }
};
}
typedef std::map<long, CANavQuadTreeNode*> CNodeIndexMap;
class CANavQuadTreeNode {
public:
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual void v07() {}
    virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {} virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual void v15() {}
    virtual void v16() {} virtual void v17() {} virtual void v18() {} virtual void v19() {} virtual void v20() {} virtual void v21() {}
    virtual void ConvertIndexesToPointers(CNodeIndexMap& table) {}   // slot 22 (+0x58)
    C2DVector     Position;                        // +0x04
    unsigned char QuadTreeLevel;                   // +0x0c
    unsigned char NavLayer;                        // +0x0d
    bool          LeafNode;                        // +0x0e
    bool          Blocked;                         // +0x0f
};
class CNavQuadTreeNode : public CANavQuadTreeNode {
public:
    CANavQuadTreeNode* TLNode;                     // +0x10
    CANavQuadTreeNode* TRNode;                     // +0x14
    CANavQuadTreeNode* BLNode;                     // +0x18
    CANavQuadTreeNode* BRNode;                     // +0x1c
    virtual void ConvertIndexesToPointers(CNodeIndexMap& table);
};

void CNavQuadTreeNode::ConvertIndexesToPointers(CNodeIndexMap& table)
{
    TLNode = table.find((long)TLNode)->second;
    TRNode = table.find((long)TRNode)->second;
    BLNode = table.find((long)BLNode)->second;
    BRNode = table.find((long)BRNode)->second;
    TLNode->ConvertIndexesToPointers(table);
    TRNode->ConvertIndexesToPointers(table);
    BLNode->ConvertIndexesToPointers(table);
    BRNode->ConvertIndexesToPointers(table);
}

struct CLeafProbe : CANavQuadTreeNode {
    int calls; CNodeIndexMap* seen;
    virtual void ConvertIndexesToPointers(CNodeIndexMap& table) { ++calls; seen = &table; }
};
typedef std::_Rb_tree_node<std::pair<const long, CANavQuadTreeNode*> > Node;
static Node* MakeNode(long key, CANavQuadTreeNode* value)
{
    static char pool[4][sizeof(Node)]; static int used = 0; Node* n = (Node*)pool[used++]; *const_cast<long*>(&n->_M_value_field.first) = key; n->_M_value_field.second = value;
    n->_M_left = n->_M_right = n->_M_parent = 0; n->_M_color = 0; return n;
}
int main()
{
    CLeafProbe leaf[4];
    for (int i = 0; i < 4; ++i) { leaf[i].calls = 0; leaf[i].seen = 0; }
    // keys 10, 20, 30, 40 -> leaf[0..3]; root 20, left 10, right 30, 30's right 40
    std::_Rb_tree_node_base header;
    Node* n10 = MakeNode(10, &leaf[0]); Node* n20 = MakeNode(20, &leaf[1]); Node* n30 = MakeNode(30, &leaf[2]); Node* n40 = MakeNode(40, &leaf[3]);
    n20->_M_left = n10; n20->_M_right = n30; n10->_M_parent = n20; n30->_M_parent = n20; n30->_M_right = n40; n40->_M_parent = n30;
    header._M_parent = n20; n20->_M_parent = &header; header._M_left = n10; header._M_right = n40;
    CNodeIndexMap table; table._M_t._M_header = &header; table._M_t._M_node_count = 4;
    CNavQuadTreeNode node;
    node.TLNode = (CANavQuadTreeNode*)(long)30; node.TRNode = (CANavQuadTreeNode*)(long)10;
    node.BLNode = (CANavQuadTreeNode*)(long)40; node.BRNode = (CANavQuadTreeNode*)(long)20;
    node.ConvertIndexesToPointers(table);
    bool ok = node.TLNode == &leaf[2] && node.TRNode == &leaf[0] && node.BLNode == &leaf[3] && node.BRNode == &leaf[1];
    ok = ok && g_finds == 4;
    for (int i = 0; i < 4; ++i) ok = ok && leaf[i].calls == 1 && leaf[i].seen == &table;
    if (!ok) { std::printf("00a77ca0_TEST FAIL\n"); return 1; }
    std::printf("00a77ca0_TEST PASS\n");
    return 0;
}
