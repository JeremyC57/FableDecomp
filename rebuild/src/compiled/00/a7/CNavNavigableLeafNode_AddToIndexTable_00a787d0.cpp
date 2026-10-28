// CNavNavigableLeafNode::AddToIndexTable(long& index, map<const CANavQuadTreeNode*, long>& table) const -- number a navigable leaf for saving: record this node under the next index
// Leaf: ?AddToIndexTable@CNavNavigableLeafNode@@MBEXAAJAAV?$map@PBVCANavQuadTreeNode@@JU?$less@PBVCANavQuadTreeNode@@@std@@V?$allocator@U?$pair@QBVCANavQuadTreeNode@@J@std@@@3@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A787D0, 44 bytes, thiscall, `ret 8`.
// Identity (proven): Ego_r.pdb ?AddToIndexTable@CNavNavigableLeafNode@@MBEXAAJAAV?$map@...@@Z (protected virtual const);
//   the leaf override of CANavQuadTreeNode slot 20 whose quad-tree version is the landed 0x00A789F0 (same tail).
// Callees: STLport _Rb_tree<...>::insert_unique(const value_type&) @ 0x00A819B0 under the bound spelling of the
//   landed 0x00A789F0 (pair<iterator, bool> returned through the hidden pointer).
// Shape: `table.insert(std::make_pair(this, index)); ++index;` -- the make_pair temporary converted to value_type by the
//   pair template ctor (the landed 0x00A789F0 shape).
class CNavRegion;
class CAFile;
class CANavQuadTreeNode;
namespace std {
template <class T> class allocator { };
template <class T> struct less { };
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    template <class U1, class U2> pair(const pair<U1, U2>& p) : first(p.first), second(p.second) { }
};
template <class T1, class T2> inline pair<T1, T2> make_pair(const T1& a, const T2& b) { return pair<T1, T2>(a, b); }
struct _Rb_tree_node_base;
template <class K, class V, class C = less<K>, class A = allocator<pair<const K, V> > > class map { public:
    typedef pair<const K, V> value_type;
    struct iterator { _Rb_tree_node_base* _M_node; };
    struct _Rep_type {                                       // _Rb_tree: header, node count, comparator
        _Rb_tree_node_base* _M_header;
        unsigned int        _M_node_count;
        pair<iterator, bool> insert_unique(const value_type& v);   // 0x00A819B0 for <const CANavQuadTreeNode*, long>
    };
    _Rep_type _M_t;
    pair<iterator, bool> insert(const value_type& v) { return _M_t.insert_unique(v); }
};
}
template <class T, int N> class CSmallVector { public:
    T*          m_data;                                    // +0
    signed char m_size;                                    // +4
    signed char m_capacity;                                // +5
};
class C2DVector { public: float X, Y; };
class CANavQuadTreeNode { public:
    virtual ~CANavQuadTreeNode();
protected:
    virtual void AddToIndexTable(long& index, std::map<const CANavQuadTreeNode*, long>& table) const;   // slot 20 (+0x50)
public:
    C2DVector     Position;                                // +0x04
    unsigned char QuadTreeLevel;                           // +0x0c
    unsigned char NavLayer;                                // +0x0d
    bool          LeafNode;                                // +0x0e
    bool          Blocked;                                 // +0x0f
};
class CNavNavigableLeafNode : public CANavQuadTreeNode { public:
    CNavRegion*                               PRegion;            // +0x10
    CSmallVector<const CANavQuadTreeNode*, 8> Neighbours;         // +0x14
    unsigned char                             PreferabilityCost;  // +0x1c
protected:
    void AddToIndexTable(long& index, std::map<const CANavQuadTreeNode*, long>& table) const;
};
void CNavNavigableLeafNode::AddToIndexTable(long& index, std::map<const CANavQuadTreeNode*, long>& table) const
{
    table.insert(std::make_pair(this, index));
    ++index;
}