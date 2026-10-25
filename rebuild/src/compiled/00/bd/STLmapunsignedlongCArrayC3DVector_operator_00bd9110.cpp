// std::map<unsigned long, CArray<C3DVector> >::operator[](const unsigned long& k) -- find a mesh's cached bone-scaling array by key, inserting an empty one when absent
// Leaf: ??A?$map@KV?$CArray@VC3DVector@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BD9110, 161 bytes, thiscall, `ret 4`.
// Identity: called by CEngineInternalPrimitiveAnimatedMesh::InitialiseBoneScalings 0x00BD2A90 (call 0x00BD2AE2): Ego_r.pdb
//   CEngineInternalPrimitiveAnimatedMesh::CCache::BoneScalings is map<unsigned long, CArray<C3DVector> >; the tree's
//   _M_copy is 0x00BD8B30. Catalog "std_map_find_or_insert_uint" is descriptive.
// Callees: CArray<C3DVector> copy constructor 0x006F0810 (the pair's second, from the default temporary),
//   _Rb_tree::insert_unique(iterator, const value_type&) 0x00BD8DC0 (result homed in the dead key slot), free @
//   0x00BFEA14 (the pair's array storage; the default temporary is known empty, so its release folds away).
// Shape: the landed STLport operator[] (0x009F1470 and siblings) -- inline lower_bound with an unsigned compare
//   (`jb` / `jae`), `__default_constructed((_Tp*)0)` and map::insert as an inline wrapper over insert_unique; the
//   array copy constructor is a same-TU noinline stand-in (below) so the default temporary's release folds away.
extern "C" void* __cdecl malloc(unsigned int size);          // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

#pragma pack(push, 1)
class C3DVector {                                       // 1-aligned in retail
public:
    float X, Y, Z;
};
#pragma pack(pop)

template <class T> class CArray {                       // retail CArray: an STLport vector (first, last, end)
public:
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    CArray() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    CArray(const CArray& other);                        // 0x006F0810 (the vector<C3DVector> copy constructor)
    ~CArray()
    {
        if (m_pFirst != 0)
            free(m_pFirst);
    }
};

// Same-TU stand-in for the out-of-line copy constructor 0x006F0810 (STLport vector copy: allocate size() elements, copy
// them), compiled under auto_inline(off) + noinline: retail's whole-program build knew it only reads `other`, so the
// default temporary handed to it stays known-empty and its release folds away. Only operator[] is graded.
#pragma auto_inline(off)
template <class T> __declspec(noinline) CArray<T>::CArray(const CArray& other)
{
    unsigned int n = other.m_pLast - other.m_pFirst;
    m_pFirst = n ? (T*)malloc(n * sizeof(T)) : 0;
    m_pLast = m_pFirst;
    m_pEnd = m_pFirst + n;
    for (const T* p = other.m_pFirst; p != other.m_pLast; ++p, ++m_pLast)
        *m_pLast = *p;
}
#pragma auto_inline(on)

namespace std {

template <class _T1, class _T2> struct pair
{
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
};

template <class _Tp> inline _Tp __default_constructed(_Tp*) { return _Tp(); }   // _STLP_DEFAULT_CONSTRUCTED

template <class _Tp> struct less
{
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

struct _Rb_tree_node_base
{
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base
{
    _Value _M_value_field;                          // +0x10
};

template <class _Value> struct _Rb_tree_iterator
{
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() {}
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) {}
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) {}
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
};

template <class _Key, class _Tp, class _Compare = less<_Key> > class map
{
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    typedef _Rb_tree_node<value_type> _Node;

    _Rb_tree_node_base* _M_header;                  // +0x00 (alloc proxy)
    unsigned int        _M_node_count;              // +0x04
    _Compare            _M_key_compare;             // +0x08

    static const _Key& _S_key(_Rb_tree_node_base* __x) { return ((_Node*)__x)->_M_value_field.first; }
    iterator end() { return iterator(_M_header); }
    _Compare key_comp() const { return _M_key_compare; }
    iterator lower_bound(const _Key& __k)
    {
        _Rb_tree_node_base* __y = _M_header;        // last node which is not less than __k
        _Rb_tree_node_base* __x = _M_header->_M_parent;
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = __x->_M_left;
            else
                __x = __x->_M_right;
        return iterator(__y);
    }
    iterator insert_unique(iterator __position, const value_type& __x);    // 0x00BD8DC0 (_Rb_tree::insert_unique)
    iterator insert(iterator __position, const value_type& __x) { return insert_unique(__position, __x); }
    _Tp& operator[](const _Key& __k)
    {
        iterator __i = lower_bound(__k);
        if (__i == end() || key_comp()(__k, (*__i).first))
            __i = insert(__i, value_type(__k, __default_constructed((_Tp*)0)));
        return (*__i).second;
    }
};

template class map<unsigned long, CArray<C3DVector> >;

} // namespace std