// CNavNavigableLeafNode::SaveToFile(CAFile&, const map<const CANavQuadTreeNode*, long>&, bool, const C2DVector&) const -- save the base node, then the region ID (low 16 bits, 0 without a region), the preferability cost byte, the neighbour count and each neighbour's index from the node table
// Leaf: ?SaveToFile@CNavNavigableLeafNode
// Grade: DIFFER(226v226) -- frame-slot homing only: retail homes regionID in the dead origin-argument slot (+0x10) and the shared count/index long in the bool slot (+0xc); ours swaps the two slots (tried separate count/index locals, a cached key local, one shared value local, declaration order)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A77A10, 226 bytes (+ int3 padding; `ret 10h`). Protected virtual (MBE) slot 21.
// Callees: CANavQuadTreeNode::SaveToFile @ 0x00A778D0 (qualified base call), CAFile::Write(const void*, long, long) (virtual slot 4,
//   +0x10).
// Layout = Ego_r.pdb: CNavNavigableLeafNode : CANavQuadTreeNode (0x10 bytes with vptr) {PRegion +0x10, Neighbours +0x14
//   (CSmallVector: data +0, signed char size +4), PreferabilityCost +0x1c}; CNavRegion::RegionID +0. The table is an STLport
//   map: header pointer, node count; node {colour, parent +4, left +8, right +0xc, key +0x10, value +0x14}.
// Shape: the region ID and one long `value` (the neighbour count, then each index) are locals VC homes in dead argument slots; the table lookup is the inline
//   STLport map::find (lower bound walk, then the `k < key(y)` reject) and its ->second is written without an end() test; the
//   loop re-reads the neighbour vector's data and size for its end test.
struct CAFile {
    virtual void Slot0();
    virtual void Slot1();
    virtual void Slot2();
    virtual void Slot3();
    virtual void Write(const void* data, long size, long flags);   // slot 4 (+0x10)
};
struct C2DVector { float X, Y; };
struct CANavQuadTreeNode;
namespace std {
template <class T> struct allocator { };
template <class T> struct less { bool operator()(const T& a, const T& b) const { return a < b; } };
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
template <class K, class V, class C = less<K>, class A = allocator<pair<const K, V> > > struct map {
    typedef pair<const K, V> value_type;
    typedef _Rb_tree_node<value_type> _Node;
    _Rb_tree_node_base* _M_header;                 // +0x00
    unsigned long       _M_node_count;             // +0x04
    C                   _M_key_compare;            // +0x08
    const value_type* find(const K& k) const
    {
        _Rb_tree_node_base* y = _M_header;
        _Rb_tree_node_base* x = _M_header->_M_parent;
        while (x != 0) {
            if (!_M_key_compare(((_Node*)x)->_M_value_field.first, k)) {
                y = x;
                x = x->_M_left;
            } else {
                x = x->_M_right;
            }
        }
        if (y == _M_header || _M_key_compare(k, ((_Node*)y)->_M_value_field.first))
            y = _M_header;
        return &((_Node*)y)->_M_value_field;
    }
};
}
typedef std::map<const CANavQuadTreeNode*, long> CNodeIndexTable;
template <class T, int N> struct CSmallVector {
    T*          m_data;                            // +0
    signed char m_size;                            // +4
    signed char m_capacity;                        // +5
    T* begin() const { return m_data; }
    T* end() const { return m_data + m_size; }
    long size() const { return m_size; }
};
struct CNavRegion {
    long RegionID;                                 // +0x00
};
class CANavQuadTreeNode {
public:
    C2DVector     Position;                        // +0x04
    unsigned char QuadTreeLevel;                   // +0x0c
    unsigned char NavLayer;                        // +0x0d
    bool          LeafNode;                        // +0x0e
    bool          Blocked;                         // +0x0f
protected:
    virtual void SaveToFile(CAFile& file, const CNodeIndexTable& table, bool saveNeighbours, const C2DVector& origin) const;   // 0x00A778D0
};
class CNavNavigableLeafNode : public CANavQuadTreeNode {
public:
    CNavRegion*                               PRegion;            // +0x10
    CSmallVector<const CANavQuadTreeNode*, 8> Neighbours;         // +0x14
    unsigned char                             PreferabilityCost;  // +0x1c
protected:
    virtual void SaveToFile(CAFile& file, const CNodeIndexTable& table, bool saveNeighbours, const C2DVector& origin) const;
};

void CNavNavigableLeafNode::SaveToFile(CAFile& file, const CNodeIndexTable& table, bool saveNeighbours, const C2DVector& origin) const
{
    CANavQuadTreeNode::SaveToFile(file, table, saveNeighbours, origin);
    long regionID;
    long value;
    if (PRegion)
        regionID = PRegion->RegionID & 0xffff;
    else
        regionID = 0;
    file.Write(&regionID, sizeof(regionID), 0);
    file.Write(&PreferabilityCost, sizeof(PreferabilityCost), 0);
    value = Neighbours.size();
    file.Write(&value, sizeof(value), 0);
    for (const CANavQuadTreeNode* const* it = Neighbours.begin(); it != Neighbours.end(); ++it) {
        const CANavQuadTreeNode* neighbour = *it;
        value = table.find(neighbour)->second;
        file.Write(&value, sizeof(value), 0);
    }
}
