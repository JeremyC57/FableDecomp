// QuadricOptimiserInternals::CVertexPairContraction::IsIllegal() -- clean the contraction up, then true when a vertex index appears twice among the sub-contractions (checked with a local CIndexList)
// Grade: DIFFER(481v471) -- VC7.1 inlines the second HasIndex's set find where retail calls the out-of-line find 0x00AD2790 (per-function inline budget); everything else lines up (esi/ebp swap for `this` follows from it)
// Leaf: ?IsIllegal@CVertexPairContraction@QuadricOptimiserInternals@@QAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC8360, 471 bytes, thiscall, returns bool (aligned ebp frame).
// Identity: Ego_r.pdb / FableWin.pdb ?IsIllegal@CVertexPairContraction@QuadricOptimiserInternals@@QAE_NXZ -- the class's only
//   no-argument non-const bool member; the body starts with CleanUp() (0x00AC7B00, catalog CVertexPairContraction::CleanUp)
//   and walks SubContractions +0x04 (CArray<CSubContraction>, 8-byte {Index1, Index2}) as Ego_r lays the class out; sole
//   caller 0x00ACDA3C in COptimisedPrimitive::SimplifyByVertexPairs. The local set is a CIndexList (PDB HasIndex(unsigned long)
//   const / AddIndex(unsigned long)): the by-value key is copied to the frame before the insert / out-of-line find, and
//   CIndexList::HasIndex's own out-of-line copy 0x00AC7F90 (same TU) is `call find 0x00AD2790 ; cmp ; setne`. The catalog
//   label "QuadricOptimiser_VertexPairContraction_Initialize" is wrong.
// Callees: CleanUp 0x00AC7B00, malloc 0x00BFEA0E (set header), _Rb_tree<unsigned long,...>::insert_unique 0x00514C10,
//   find 0x00AD2790 (second check only), _M_erase 0x004CF810, free 0x00BFEA14.
// Next: the inline budget: retail spent more of it before the second HasIndex; try the real STLport 4.6 _tree.h text for
//   every layer the set ctor / ~set / insert expand through (e.g. _Rb_tree_base ctor `allocate` wrapper, _STLP_alloc_proxy
//   members), or a heavier inline CArray (std::vector base) for SubContractions.
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
    typedef _Key key_type;
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
    iterator find(const key_type& __x) { return iterator(_M_find(__x)); }
    const_iterator find(const key_type& __x) const { return const_iterator(_M_find(__x)); }
private:
    static _Value& _S_value(_Link_type __x) { return __x->_M_value_field; }
    static const _Key& _S_key(_Link_type __x) { return _KeyOfValue()(_S_value(__x)); }
    static _Value& _S_value(_Base_ptr __x) { return ((_Link_type)__x)->_M_value_field; }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(_S_value(_Link_type(__x))); }
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    _Rb_tree_node<_Value>* _M_find(const key_type& __k) const
    {
        _Link_type __y = (_Link_type)this->_M_header._M_data;
        _Link_type __x = _M_root();
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = _S_left(__x);
            else
                __x = _S_right(__x);
        if (__y == this->_M_header._M_data || _M_key_compare(__k, _S_key(__y)))
            __y = (_Link_type)this->_M_header._M_data;
        return __y;
    }
public:
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
    iterator find(const key_type& __x) const { return _M_t.find(__x); }
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

template <class T> class CArray {                      // Lionhead CArray<T> (retail STLport vector, 12 bytes)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const T& operator[](unsigned long n) const { return *(_M_start + n); }
};

namespace QuadricOptimiserInternals {
class CIndexList {
public:
    std::set<unsigned long, std::less<unsigned long>, std::allocator<unsigned long> > Indices;   // +0x00
    CIndexList() { }
    ~CIndexList() { }
    std::set<unsigned long, std::less<unsigned long>, std::allocator<unsigned long> >::const_iterator End() const { return Indices.end(); }
    bool HasIndex(unsigned long index) const { return Indices.find(index) != End(); }
    void AddIndex(unsigned long index) { Indices.insert(index); }
};
class CSubContraction {
public:
    unsigned long Index1;                              // +0x00
    unsigned long Index2;                              // +0x04
};
class CVertexPairContraction {
public:
    bool                    Exists;                    // +0x00
    bool                    Unsplit;                   // +0x01
    CArray<CSubContraction> SubContractions;           // +0x04
    float                   Weight2;                   // +0x10
    float                   Cost;                      // +0x14
    C3DVector               ResultingPosition;         // +0x18
    void*                   ListIterator;              // +0x24
    void CleanUp();                                    // 0x00AC7B00
    unsigned long GetSubContractionCount() const { return SubContractions.size(); }
    const CSubContraction& GetSubContraction(unsigned long index) const { return SubContractions[index]; }
    bool IsIllegal();
};

bool CVertexPairContraction::IsIllegal()
{
    CleanUp();
    CIndexList indices;
    for (unsigned long i = 0; i < GetSubContractionCount(); ++i) {
        const CSubContraction& sub = GetSubContraction(i);
        if (indices.HasIndex(sub.Index1))
            return true;
        indices.AddIndex(sub.Index1);
        if (indices.HasIndex(sub.Index2))
            return true;
        indices.AddIndex(sub.Index2);
    }
    return false;
}
}
