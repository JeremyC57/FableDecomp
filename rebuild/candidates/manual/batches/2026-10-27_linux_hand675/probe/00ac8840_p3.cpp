// QuadricOptimiserInternals::COptimisedPrimitive::GetRemainingVertices(const CIndexList& indices) const -- the indices whose vertex still exists
// Leaf: ?GetRemainingVertices@COptimisedPrimitive@QuadricOptimiserInternals@@
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Arg1, class _Arg2, class _Result> struct binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};
template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> inline less<_Tp> __less(_Tp*) { return less<_Tp>(); }
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};

struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                               // +0x00
    _Base_ptr   _M_parent;                              // +0x04
    _Base_ptr   _M_left;                                // +0x08
    _Base_ptr   _M_right;                               // +0x0C
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};

template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* __x);
};
typedef _Rb_global<bool> _Rb_global_inst;
template <class _Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_right != 0) {
        _M_node = _M_node->_M_right;
        while (_M_node->_M_left != 0)
            _M_node = _M_node->_M_left;
    }
    else {
        _Base_ptr __y = _M_node->_M_parent;
        while (_M_node == __y->_M_right) {
            _M_node = __y;
            __y = __y->_M_parent;
        }
        if (_M_node->_M_right != __y)
            _M_node = __y;
    }
    return _M_node;
}

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
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> >& __it) { _M_node = __it._M_node; }
    typename _Traits::reference operator*() const { return _Link_type(_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
};

template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    _Rb_tree_base(const _Alloc&) : _M_header(allocator<_Node>(), (_Rb_tree_node_base*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
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
    public:
    protected:
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    public:
    protected:
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
    public:

    _Rb_tree() : _Rb_tree_base<_Value, _Alloc>(_Alloc()), _M_node_count(0) { _M_empty_initialize(); }
    _Rb_tree(const _Rb_tree& __x);                      // 0x00AD3A10 for set<unsigned long>
    ~_Rb_tree() { clear(); }
    private:
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;      // red: tells the header from the root in operator++
        _M_root() = 0;
        _M_leftmost() = (_Link_type)this->_M_header._M_data;
        _M_rightmost() = (_Link_type)this->_M_header._M_data;
    }
    public:
private:
    void _M_erase(_Link_type __x);                      // 0x004CF810 for set<unsigned long>
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
    pair<iterator, bool> insert_unique(const value_type& __x);   // 0x00514C10 for set<unsigned long>
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
    iterator begin() const { return _M_t.begin(); }
    iterator end() const { return _M_t.end(); }
    pair<iterator, bool> insert(const value_type& __x)
    {
        typedef typename _Rep_type::iterator _Rep_iterator;
        pair<_Rep_iterator, bool> __p = _M_t.insert_unique(__x);
        return pair<iterator, bool>(__p.first, __p.second);
    }
};

}   // namespace std

class C3DVector {
public:
    float X, Y, Z;
};

namespace QuadricOptimiserInternals {
class CIndexList {
public:
    std::set<unsigned long, std::less<unsigned long>, std::allocator<unsigned long> > Indices;   // +0x00
};
class CQuadric {
public:
    double AA, AB, AC, AD, BB, BC, BD, CC, CD, DD, Weight;
};
class COptimisedVertex {                               // retail 0xB8 (Ego_r order, STLport containers, 8-aligned)
public:
    C3DVector     Position;                            // +0x00
    C3DVector     Normal;                              // +0x0C
    C3DVector     TextureCoord;                        // +0x18
    CQuadric      PositionError;                       // +0x28
    float         Cost;                                // +0x80
    CIndexList    FaceList;                            // +0x84
    bool          Exists;                              // +0x90
    unsigned long UniqueIndex;                         // +0x94
    unsigned long OutputIndex;                         // +0x98
    unsigned char Groups[0x0C];                        // +0x9C map<unsigned long, float>
    unsigned char PairContractions[0x0C];              // +0xA8 map<unsigned long, CContractionReference>
};
template <class T> class CArray {                      // retail STLport vector, 12 bytes
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T& operator[](unsigned long n) const { return *(_M_start + n); }
};
class COptimisedPrimitive {
public:
    long                     MaterialIndex;            // +0x00
    unsigned char            UniqueVertices[0x0C];     // +0x04 CArray<CUniqueVertex>
    CArray<COptimisedVertex> Vertices;                 // +0x10
private:
    CIndexList GetRemainingVertices(const CIndexList& indices) const;
};

CIndexList COptimisedPrimitive::GetRemainingVertices(const CIndexList& indices) const
{
    CIndexList remaining;
    for (std::set<unsigned long, std::less<unsigned long>, std::allocator<unsigned long> >::const_iterator it =
             indices.Indices.begin(); it != indices.Indices.end(); ++it) {
        if (Vertices[*it].Exists) {
            unsigned long index = *it;
            remaining.Indices.insert(index);
        }
    }
    return remaining;
}
}
