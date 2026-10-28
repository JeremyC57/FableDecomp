// CNavNavigableLeafNode::ConvertIndexesToPointers(const map<long, CANavQuadTreeNode*>& nodes) -- after loading, each neighbour slot holds a node index: replace it with the node the index map gives
// Leaf: ?ConvertIndexesToPointers@CNavNavigableLeafNode@@MAEXABV?$map@JPAVCANavQuadTreeNode@@U?$less@J@std@@V?$allocator@U?$pair@$$CBJPAVCANavQuadTreeNode@@@std@@@3@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A779B0, 91 bytes, `ret 4`. Name: Ego_r.pdb
//   ?ConvertIndexesToPointers@CNavNavigableLeafNode@@MAEXABV?$map@JPAVCANavQuadTreeNode@@...; vtable 0x0129DDA4 slot 22; the
//   catalog label agrees.
// Layout: Ego_r.pdb CNavNavigableLeafNode Neighbours CSmallVector {Start +0x14, schar Size +0x18}; STLport map node
//   {color, parent +4, left +8, right +0xC, key +0x10, value +0x14}, header pointer at +0 of the map.
// Shape: STLport 4.6 `map::find` inline (lower-bound walk, then the end / key test) on the neighbour's index, `->second`
//   stored back over the slot; the size re-read (movsx) each pass.
class CANavQuadTreeNode;
class CNavRegion;
namespace std {
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
};
struct _Rb_tree_node_base {
    char                _M_color;                       // +0x00
    _Rb_tree_node_base* _M_parent;                      // +0x04
    _Rb_tree_node_base* _M_left;                        // +0x08
    _Rb_tree_node_base* _M_right;                       // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};
template <class _Value> struct _Rb_tree_const_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_const_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    const _Value* operator->() const { return &((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_const_iterator<value_type> const_iterator;
    _Rb_tree_node_base* _M_header;                      // +0x00
    unsigned int        _M_node_count;                  // +0x04
    _Compare            _M_key_compare;                 // +0x08
    static const _Key& _S_key(_Rb_tree_node_base* __x) { return ((_Rb_tree_node<value_type>*)__x)->_M_value_field.first; }
    const_iterator find(const _Key& __k) const
    {
        _Rb_tree_node_base* __y = _M_header;
        _Rb_tree_node_base* __x = _M_header->_M_parent;
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = __x->_M_left;
            else
                __x = __x->_M_right;
        if (__y == _M_header || _M_key_compare(__k, _S_key(__y)))
            __y = _M_header;
        return const_iterator(__y);
    }
};
}
template <class T, int N> class CSmallVector {
public:
    T*          Start;                                  // +0x00
    signed char Size;                                   // +0x04
    long size() const { return Size; }
    T& operator[](unsigned long i) { return Start[i]; }
};
class C2DVector { public: float X, Y; };
class CANavQuadTreeNode {
public:
    virtual ~CANavQuadTreeNode();
    C2DVector     Position;                             // +0x04
    unsigned char QuadTreeLevel;                        // +0x0C
    unsigned char NavLayer;                             // +0x0D
    bool          LeafNode;                             // +0x0E
    bool          Blocked;                              // +0x0F
};
class CNavNavigableLeafNode : public CANavQuadTreeNode {
public:
    CNavRegion*                               PRegion;              // +0x10
    CSmallVector<const CANavQuadTreeNode*, 8> Neighbours;           // +0x14
    unsigned char                             PreferabilityCost;    // +0x1C
protected:
    virtual void ConvertIndexesToPointers(const std::map<long, CANavQuadTreeNode*>& nodes);
};

void CNavNavigableLeafNode::ConvertIndexesToPointers(const std::map<long, CANavQuadTreeNode*>& nodes)
{
    for (long i = 0; i < Neighbours.size(); ++i)
        Neighbours[i] = nodes.find((long)Neighbours[i])->second;
}