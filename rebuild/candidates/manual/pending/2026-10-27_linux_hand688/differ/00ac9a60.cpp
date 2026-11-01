// QuadricOptimiserInternals::COptimisedPrimitive::FindAdjacentFaces() -- rebuild the face adjacency: clear the edge list, then for every face edge that has no edge record yet intersect the face lists of the edge's two unique vertices and join the face with every other face of that intersection whose normal is within the fold limit (dot > -0.99)
// Grade: DIFFER(743v745) -- two residues: VC7.1 swaps the roles of ebx/edi (retail keeps the zero constant and the `other` face index in ebx and the UniqueVertexIndices pointer / normal pointer / erase cursor in edi; ours the reverse, so the three inline set destructors use `cmp edi,ebx` + the loop-exit zero instead of retail's `test edi,edi` + `xor ebx,ebx`), and the X term of the normal dot product loads the operands in the opposite order; everything else (the clear, the size loop, the two copies, the intersection, the walk, the normals, the join) matches
// Leaf: ?FindAdjacentFaces@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC9A60, 745 bytes, thiscall, no arguments. Ego_r.pdb names the private member; its
//   only caller is COptimisedPrimitive::Initialise 0x00ACA830. The catalog's "QuadricOptimiser_PreprocessAdjacencyAndNormals"
//   is descriptive.
// Callees: vector<COptimisedEdge>::erase(iterator, iterator) 0x00AD3CC0 (the inline clear()), the set<unsigned long> tree copy
//   constructor 0x00AD3A10 (CIndexList's implicit copy, x2), CIndexList::GetIntersection 0x00AC8960 (hidden result),
//   COptimisedPrimitive::GetFaceNormal(const COptimisedFace&) const 0x00AC7CB0 (C3DVector by hidden pointer, x2),
//   COptimisedPrimitive::JoinAdjacentFaces(unsigned long, unsigned long) 0x00AC8AE0 (bool, ignored), the set's _M_erase
//   0x004CF810 (the recursion of the one-level-inlined body in the three destructors), free 0x00BFEA14. Constant -0.99f
//   0x0129F054.
// Layout: Ego_r.pdb COptimisedFace (0x48: VertexIndices, UniqueVertexIndices +0xC, Edges +0x18 ...), CUniqueVertex (retail
//   0x1C: Exists, VertexIndices +4, FaceList +0x10), CIndexList {set<unsigned long>} (the landed GetIntersection model);
//   COptimisedPrimitive UniqueVertices +4, Faces +0x1C, Edges +0x28 as the landed UpdateContractions row (the shared
//   engine/COptimisedPrimitive.h keeps the arrays opaque -- header debt).
// Shape: `Faces.size()` is re-evaluated per iteration (the signed /0x48 division); the two unique-vertex indices are read
//   before the first copy (`vertexB` named first); `_M_erase` is defined in-class so VC7.1 inlines one level of the recursion
//   into each destructor and calls the COMDAT for the right subtrees; `other = *it` is a named local (retail loads the
//   node value once). Probes: 16 (probe/00ac9a60.log).
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
class C3DVector { public:
    float X, Y, Z;
    float Dot(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
template <class _Tp> struct _Identity { const _Tp& operator()(const _Tp& __x) const { return __x; } };
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                 // +0x00
    _Base_ptr _M_parent;                                // +0x04
    _Base_ptr _M_left;                                  // +0x08
    _Base_ptr _M_right;                                 // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Rb_tree_node_base_ptr;
    static _Rb_tree_node_base_ptr _M_increment(_Rb_tree_node_base_ptr __x)
    {
        if (__x->_M_right != 0) {
            __x = __x->_M_right;
            while (__x->_M_left != 0)
                __x = __x->_M_left;
        } else {
            _Rb_tree_node_base_ptr __y = __x->_M_parent;
            while (__x == __y->_M_right) {
                __x = __y;
                __y = __y->_M_parent;
            }
            if (__x->_M_right != __y)
                __x = __y;
        }
        return __x;
    }
};
typedef _Rb_global<bool> _Rb_global_inst;
struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
    bool operator==(const _Rb_tree_base_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_base_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; typedef const _Tp* pointer; };
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Value value_type;
    typedef typename _Traits::reference reference;
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> >& __it) { _M_node = __it._M_node; }
    reference operator*() const { return ((_Link_type)_M_node)->_M_value_field; }
    _Self& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    _Rb_tree_base(const _Alloc&) : _M_header(allocator<_Node>(), (_Rb_tree_node_base*)0) { _M_header._M_data = _M_header.allocate(1); }
    ~_Rb_tree_base() { _M_header.deallocate((_Node*)_M_header._M_data, 1); }
    _STLP_alloc_proxy<_Rb_tree_node_base*, _Node, allocator<_Node> > _M_header;   // +0x00
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
    typedef _Rb_tree_iterator<value_type, _Const_traits<value_type> > const_iterator;
    unsigned int _M_node_count;                         // +0x04
    _Compare     _M_key_compare;                        // +0x08
protected:
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
    static _Link_type _S_left(_Link_type __x) { return (_Link_type)__x->_M_left; }
    static _Link_type _S_right(_Link_type __x) { return (_Link_type)__x->_M_right; }
public:
    _Rb_tree() : _Rb_tree_base<_Value, _Alloc>(_Alloc()), _M_node_count(0) { _M_empty_initialize(); }
    _Rb_tree(const _Rb_tree& __x);                      // 0x00AD3A10 for set<unsigned long>
    ~_Rb_tree() { clear(); }
private:
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;
        _M_root() = 0;
        _M_leftmost() = (_Link_type)this->_M_header._M_data;
        _M_rightmost() = (_Link_type)this->_M_header._M_data;
    }
    void _M_put_node(_Link_type __p) { this->_M_header.deallocate(__p, 1); }
    void _M_erase(_Link_type __x)                       // 0x004CF810 for set<unsigned long> (the out-of-line recursion)
    {
        while (__x != 0) {
            _M_erase(_S_right(__x));
            _Link_type __y = _S_left(__x);
            _M_put_node(__x);
            __x = __y;
        }
    }
public:
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = (_Link_type)this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = (_Link_type)this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
    iterator begin() { return iterator(_M_leftmost()); }
    const_iterator begin() const { return const_iterator(_M_leftmost()); }
    iterator end() { return iterator((_Link_type)this->_M_header._M_data); }
    const_iterator end() const { return const_iterator((_Link_type)this->_M_header._M_data); }
};
template <class _Key, class _Compare, class _Alloc> class set {
public:
    typedef _Key key_type;
    typedef _Key value_type;
    typedef _Compare key_compare;
    typedef _Rb_tree<key_type, value_type, _Identity<value_type>, key_compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::const_iterator iterator;
    typedef typename _Rep_type::const_iterator const_iterator;
    _Rep_type _M_t;                                     // +0x00
    set() : _M_t() { }
    set(const set& __x) : _M_t(__x._M_t) { }
    iterator begin() const { return _M_t.begin(); }
    iterator end() const { return _M_t.end(); }
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    iterator erase(iterator __first, iterator __last);  // 0x00AD3CC0 for vector<COptimisedEdge>
    void clear() { erase(begin(), end()); }
};
}   // namespace std

namespace QuadricOptimiserInternals {
class CIndexList {
public:
    std::set<unsigned long, std::less<unsigned long>, std::allocator<unsigned long> > Indices;   // +0x00
    CIndexList GetIntersection(const CIndexList& other) const;     // 0x00AC8960
};
class CUniqueVertex {                                   // retail 0x1C
public:
    bool       Exists;                                  // +0x00
    CIndexList VertexIndices;                           // +0x04
    CIndexList FaceList;                                // +0x10
};
class COptimisedFace {                                  // 0x48
public:
    unsigned long VertexIndices[3];                     // +0x00
    unsigned long UniqueVertexIndices[3];               // +0x0C
    long          Edges[3];                             // +0x18
    C3DVector     UTangent;                             // +0x24
    C3DVector     VTangent;                             // +0x30
    bool          Exists;                               // +0x3C
    unsigned long OutputIndex;                          // +0x40
    long          Segment;                              // +0x44
};
class COptimisedEdge {                                  // 0x10
public:
    unsigned long FaceA, FaceB, UniqueVertex1, UniqueVertex2;
};
class COptimisedVertex;
class COptimisedPrimitive {
public:
    long                         MaterialIndex;         // +0x00
    std::vector<CUniqueVertex>   UniqueVertices;        // +0x04
    std::vector<COptimisedVertex*> Vertices;            // +0x10 (0xB8-byte COptimisedVertex, unused here)
    std::vector<COptimisedFace>  Faces;                 // +0x1C
    std::vector<COptimisedEdge>  Edges;                 // +0x28
private:
    C3DVector GetFaceNormal(const COptimisedFace& face) const;          // 0x00AC7CB0
    bool JoinAdjacentFaces(unsigned long faceA, unsigned long faceB);   // 0x00AC8AE0
    void FindAdjacentFaces();
};

void COptimisedPrimitive::FindAdjacentFaces()
{
    Edges.clear();
    for (unsigned long f = 0; f < Faces.size(); ++f) {
        COptimisedFace& face = Faces[f];
        for (unsigned long i = 0; i < 3; ++i) {
            if (face.Edges[i] == -1) {
                unsigned long vertexB = face.UniqueVertexIndices[(i + 1) % 3];
                CIndexList first(UniqueVertices[face.UniqueVertexIndices[i]].FaceList);
                CIndexList second(UniqueVertices[vertexB].FaceList);
                CIndexList shared = first.GetIntersection(second);
                for (std::set<unsigned long, std::less<unsigned long>, std::allocator<unsigned long> >::const_iterator it = shared.Indices.begin();
                     it != shared.Indices.end(); ++it) {
                    unsigned long other = *it;
                    if (other != f) {
                        if (GetFaceNormal(face).Dot(GetFaceNormal(Faces[other])) > -0.99f)
                            JoinAdjacentFaces(f, other);
                    }
                }
            }
        }
    }
}
}
