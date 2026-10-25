// QuadricOptimiserInternals::COptimisedVertex::Remove() -- retire a vertex that a pair contraction merged away: clear Exists, empty its face list and its pair-contraction map
// Leaf: ?Remove@COptimisedVertex@QuadricOptimiserInternals
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD71B0, 0x6A bytes, thiscall, plain `ret`.
// Identity: `this` is a COptimisedVertex (retail 0xB8 layout of the landed ~COptimisedVertex 0x00AD7400): FaceList's
// set<unsigned long> at +0x84 (its node erase 0x004CF810), Exists byte at +0x90, PairContractions
// map<unsigned long, CContractionReference> at +0xA8 (its node erase 0x00AD5240). The only caller is the end of the
// pair contraction 0x00ACCBF0 (after the merged cost is copied to the surviving vertex, the other vertex is retired).
// No Ego_r.pdb method matches (inlined there), so the name is descriptive; the catalog's
// "COptimisedPrimitive_InitializeTrees" is descriptive too.
// Callees: _Rb_tree<unsigned long, ...>::_M_erase(node) @ 0x004CF810 (manifest "LTextTreeWalkThrough::BuildTreeArray"
// is a propagation), _Rb_tree<unsigned long, pair<const unsigned long, CContractionReference>>::_M_erase(node)
// @ 0x00AD5240 (thiscall on the tree, root pushed).
// Shape: `Exists = false;` then two inline STLport clear() calls (`if (_M_node_count != 0) { _M_erase(_M_root());
// _M_leftmost() = header; _M_root() = 0; _M_rightmost() = header; _M_node_count = 0; }`) sharing one zero register
// (ebx) for the flag, the count tests and the stores; the tree's address is formed once (`lea esi`).
namespace std {
template <class T> class less { };
template <class T> class allocator { };
template <class T1, class T2> class pair {
public:
    T1 first;
    T2 second;
};
template <class T> struct _Identity { };
template <class P> struct _Select1st { };
struct _Rb_tree_node_base {
    bool                _M_color;                                // +0x00
    _Rb_tree_node_base* _M_parent;                               // +0x04
    _Rb_tree_node_base* _M_left;                                 // +0x08
    _Rb_tree_node_base* _M_right;                                // +0x0C
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;                                            // +0x10
};
template <class K, class V, class KeyOfValue, class Compare, class A> class _Rb_tree {
public:
    typedef _Rb_tree_node<V> _Node;
    _Node*       _M_header;                                      // +0x00 (alloc proxy)
    unsigned int _M_node_count;                                  // +0x04
    Compare      _M_key_compare;                                 // +0x08

    _Rb_tree_node_base*& _M_root() const { return _M_header->_M_parent; }
    _Rb_tree_node_base*& _M_leftmost() const { return _M_header->_M_left; }
    _Rb_tree_node_base*& _M_rightmost() const { return _M_header->_M_right; }
    void _M_erase(_Node* __x);                                   // per instantiation (0x004CF810 / 0x00AD5240)
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Node*)_M_root());
            _M_leftmost() = _M_header;
            _M_root() = 0;
            _M_rightmost() = _M_header;
            _M_node_count = 0;
        }
    }
};
template <class K, class C = less<K>, class A = allocator<K> > class set {
public:
    _Rb_tree<K, K, _Identity<K>, C, A> _M_t;
    void clear() { _M_t.clear(); }
};
template <class K, class T, class C = less<K>, class A = allocator<pair<const K, T> > > class map {
public:
    _Rb_tree<K, pair<const K, T>, _Select1st<pair<const K, T> >, C, A> _M_t;
    void clear() { _M_t.clear(); }
};
} // namespace std

namespace QuadricOptimiserInternals {

class CContractionReference {                                    // 12 bytes: CCountedPointer + SubIndex
public:
    void*         ContractionData;
    void*         ContractionInfo;
    unsigned long SubIndex;
};

class CIndexList {
public:
    std::set<unsigned long> Indices;                            // +0x00 (12 bytes in retail)
};

class COptimisedVertex {
public:
    unsigned char Position_to_Cost[0x84];                        // +0x00 Position, Normal, TextureCoord, PositionError, Cost
    CIndexList    FaceList;                                      // +0x84
    bool          Exists;                                        // +0x90
    unsigned long UniqueIndex;                                   // +0x94
    unsigned long OutputIndex;                                   // +0x98
    std::map<unsigned long, float>                 Groups;            // +0x9C
    std::map<unsigned long, CContractionReference> PairContractions;  // +0xA8
    void Remove();
};

void COptimisedVertex::Remove()
{
    Exists = false;
    FaceList.Indices.clear();
    PairContractions.clear();
}

} // namespace QuadricOptimiserInternals