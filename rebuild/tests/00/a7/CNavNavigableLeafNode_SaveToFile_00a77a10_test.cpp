#include <stdio.h>
// Behaviour test for CNavNavigableLeafNode::SaveToFile (0x00A77A10): base record, region id (low 16 bits or 0),
// preferability byte, neighbour count, then each neighbour's index from the table.
class CAFile { public:
    virtual void Slot0() {}
    virtual void Slot1() {}
    virtual void Slot2() {}
    virtual void Slot3() {}
    virtual void Write(const void* data, long size, long flags) { for (long i = 0; i < size; ++i) buf[n++] = ((const unsigned char*)data)[i]; if (flags) bad = 1; }
    unsigned char buf[256]; long n; int bad;
};
class C2DVector { public: float X, Y; };
class CANavQuadTreeNode;
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
template <class T, int N> class CSmallVector { public:
    T*          m_data;                            // +0
    signed char m_size;                            // +4
    signed char m_capacity;                        // +5
    T* begin() const { return m_data; }
    T* end() const { return m_data + m_size; }
    long size() const { return m_size; }
};
class CNavRegion { public:
    long RegionID;                                 // +0x00
};
class CANavQuadTreeNode {
public:
    C2DVector     Position;                        // +0x04
    unsigned char QuadTreeLevel;                   // +0x0c
    unsigned char NavLayer;                        // +0x0d
    bool          LeafNode;                        // +0x0e
    bool          Blocked;                         // +0x0f
public:
    virtual void SaveToFile(CAFile& file, const CNodeIndexTable&, bool, const C2DVector&) const { unsigned char m = 0xAB; file.Write(&m, 1, 0); }
};
class CNavNavigableLeafNode : public CANavQuadTreeNode {
public:
    CNavRegion*                               PRegion;            // +0x10
    CSmallVector<const CANavQuadTreeNode*, 8> Neighbours;         // +0x14
    unsigned char                             PreferabilityCost;  // +0x1c
public:
    virtual void SaveToFile(CAFile& file, const CNodeIndexTable& table, bool saveNeighbours, const C2DVector& origin) const;
};

static inline void WriteLong(CAFile& file, long v) { file.Write(&v, sizeof(v), 0); }
void CNavNavigableLeafNode::SaveToFile(CAFile& file, const CNodeIndexTable& table, bool saveNeighbours, const C2DVector& origin) const
{
    CANavQuadTreeNode::SaveToFile(file, table, saveNeighbours, origin);
    long regionID;
    if (PRegion)
        regionID = PRegion->RegionID & 0xffff;
    else
        regionID = 0;
    file.Write(&regionID, sizeof(regionID), 0);
    file.Write(&PreferabilityCost, sizeof(PreferabilityCost), 0);
    WriteLong(file, Neighbours.size());
    for (const CANavQuadTreeNode* const* it = Neighbours.begin(); it != Neighbours.end(); ++it) {
        const CANavQuadTreeNode* neighbour = *it;
        WriteLong(file, table.find(neighbour)->second);
    }
}

struct RawNode : std::_Rb_tree_node_base { const CANavQuadTreeNode* key; long val; };
static long rd(const unsigned char* p) { return p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24); }
int main()
{
    int ok = 1;
    CANavQuadTreeNode a, b;
    RawNode head, na, nb;
    CNodeIndexTable table; table._M_header = &head; table._M_node_count = 2;
    const CANavQuadTreeNode* lo = (&a < &b) ? &a : &b; const CANavQuadTreeNode* hi = (&a < &b) ? &b : &a;
    na.key = lo; na.val = 11;
    nb.key = hi; nb.val = 22;
    head._M_parent = &na; na._M_parent = &head; na._M_left = 0; na._M_right = &nb; nb._M_parent = &na; nb._M_left = nb._M_right = 0;
    const CANavQuadTreeNode* list[2] = { &b, &a };
    CNavRegion region; region.RegionID = 0x12345;
    CNavNavigableLeafNode leaf; leaf.PRegion = &region; leaf.Neighbours.m_data = list; leaf.Neighbours.m_size = 2; leaf.PreferabilityCost = 9;
    CAFile f; f.n = 0; f.bad = 0; C2DVector o = { 0, 0 };
    leaf.SaveToFile(f, table, true, o);
    long ib = (&b == lo) ? 11 : 22, ia = (&a == lo) ? 11 : 22;
    if (f.n != 1 + 4 + 1 + 4 + 8 || f.buf[0] != 0xAB || rd(f.buf + 1) != 0x2345 || f.buf[5] != 9 || rd(f.buf + 6) != 2) ok = 0;
    if (rd(f.buf + 10) != ib || rd(f.buf + 14) != ia || f.bad) ok = 0;
    leaf.PRegion = 0; leaf.Neighbours.m_size = 0; f.n = 0;
    leaf.SaveToFile(f, table, false, o);
    if (f.n != 10 || rd(f.buf + 1) != 0 || rd(f.buf + 6) != 0) ok = 0;
    if (ok) { printf("00a77a10_TEST PASS\n"); return 0; }
    printf("00a77a10_TEST FAIL\n"); return 1;
}