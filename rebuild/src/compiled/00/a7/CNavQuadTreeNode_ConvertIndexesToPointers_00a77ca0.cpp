// CNavQuadTreeNode::ConvertIndexesToPointers(map<long, CANavQuadTreeNode*>&) -- after loading, the four child slots hold node indexes: look each up in the index table, store the node pointer, then recurse into the four children
// Leaf: ConvertIndexesToPointers
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A77CA0, 187 bytes (+ int3 padding; `ret 4`). Virtual slot 22 (+0x58).
// Callees: the map's _Rb_tree::find(const long&) @ 0x00A7F880 (out of line under the inline map::find wrapper, iterator returned through the hidden
//   pointer; landed as the placeholder CRetailTree_longkey::find), each child's ConvertIndexesToPointers (slot 22, +0x58).
// Layout = engine/CNavQuadTreeNode.h: TLNode +0x10, TRNode +0x14, BLNode +0x18, BRNode +0x1c (the CANavQuadTreeNode base is
//   0x10 bytes with its vptr); a map node keeps its key at +0x10 and the mapped pointer at +0x14.
// Shape: each index is copied into a long local (the key reference) and the iterator lands in a frame slot VC alternates with
//   the dead table-argument slot; `->second` is read without an end() test.
class C2DVector { public: float X, Y; };
class CANavQuadTreeNode;
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
    iterator find(const K& key);                   // 0x00A7F880 for map<long, CANavQuadTreeNode*>
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
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void ConvertIndexesToPointers(CNodeIndexMap& table);   // slot 22 (+0x58)
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