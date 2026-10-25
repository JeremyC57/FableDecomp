// QuadricOptimiserInternals::COptimisedPrimitive::UpdateContractions(const CSubContraction& sub) -- after a sub-pair contraction, recompute every pair contraction that references the sub-pair's first vertex and re-key its cost
// Leaf: ?UpdateContractions@COptimisedPrimitive@QuadricOptimiserInternals@@AAE
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC96E0, 236 bytes (+4 int3), thiscall `ret 4`.
// Identity: Ego_r.pdb `?UpdateContractions@COptimisedPrimitive@QuadricOptimiserInternals@@AAEXABVCSubContraction@2@@Z`
//   (private); its only caller is the landed COptimisedPrimitive::PerformContraction 0x00ACD840 (third loop, declared
//   there as UpdateSubPair_00ac96e0). Callees by PDB signature: ComputeContraction(CVertexPairContraction&) 0x00AC92C0
//   (one pointer argument; tests Unsplit +1 and defers to ComputeSelfContraction 0x00AC7EB0) and
//   UpdateCost(const CCountedPointer<CVertexPairContraction>&) 0x00AC8A60; the count block's DeleteFunc, operator
//   delete 0x00BFE9BC. Catalog label "QuadricOptimiser_ProcessVertexContraction" is descriptive.
// Layout (retail, STLport 12-byte containers): COptimisedPrimitive Vertices +0x10 (0xB8-byte COptimisedVertex);
//   COptimisedVertex::PairContractions (map<unsigned long, CContractionReference>) +0xA8; node value +0x10 = key,
//   Contraction {Data +0x14, Info +0x18}, SubIndex +0x1C.
// Shape: the map walk re-reads end() (the header) every pass and inlines `_Rb_global<bool>::_M_increment`; per entry
//   two add-refs: the CContractionReference copy (kept in registers, released last with its dead zeroing removed) and
//   the counted-pointer local handed to UpdateCost (in the frame, released first, Data/Info zeroed). The two callees
//   carry same-TU `__declspec(noinline)` bodies (UpdateCost following its retail code, a ComputeContraction stand-in): with them
//   declaration-only, VC7.1 treats the frame local as escaped -- it re-reads Data for ComputeContraction and Info
//   after UpdateCost and in the release (252 bytes); with the bodies visible it keeps both in eax/esi as retail.
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
inline void* operator new(unsigned int, void* p) { return p; }

struct CCPPointerInfo {
    long  RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4, Data in ecx
    void* Data;                                         // +8
};

template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0
    CCPPointerInfo* Info;                               // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    T& operator*() const { return *Data; }
    T* operator->() const { return Data; }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

namespace QuadricOptimiserInternals {
class CVertexPairContraction;
class CContractionReference {
public:
    CCountedPointer<CVertexPairContraction> Contraction;   // +0
    unsigned long                           SubIndex;      // +8
};
}

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2>
inline bool __fastcall operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
}

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
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
    static void __cdecl _Rebalance(_Base_ptr __x, _Base_ptr& __root);                         // 0x0042971D
    static _Base_ptr __cdecl _Rebalance_for_erase(_Base_ptr __z, _Base_ptr& __root,
                                                  _Base_ptr& __leftmost, _Base_ptr& __rightmost);   // 0x0042A34E
    static _Base_ptr __cdecl _M_increment(_Base_ptr _M_node)
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
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> >& __it) { _M_node = __it._M_node; }
    typename _Traits::reference operator*() const { return _Link_type(_M_node)->_M_value_field; }
    typename _Traits::pointer operator->() const { return &(operator*()); }
    _Self& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Node* _Link_type;
    typedef _Key key_type;
    typedef _Value value_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
    typedef _Rb_tree_iterator<value_type, _Const_traits<value_type> > const_iterator;

    _STLP_alloc_proxy<_Link_type, allocator<_Node> > _M_header;   // +0x00
    unsigned int _M_node_count;                                    // +0x04
    _Compare     _M_key_compare;                                   // +0x08

    _Link_type _M_allocate_node() { return (_Link_type)malloc(sizeof(_Node)); }
    void _M_deallocate_node(_Link_type __p) { if (__p != 0) free(__p); }
    _Link_type _M_create_node(const value_type& __x)
    {
        _Link_type __tmp = _M_allocate_node();
        new (&__tmp->_M_value_field) value_type(__x);
        return __tmp;
    }
    _Link_type _M_clone_node(_Link_type __x)
    {
        _Link_type __tmp = _M_create_node(__x->_M_value_field);
        __tmp->_M_color = __x->_M_color;
        __tmp->_M_left = 0;
        __tmp->_M_right = 0;
        return __tmp;
    }
    _Link_type& _M_root() const { return (_Link_type&)_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)_M_header._M_data->_M_right; }
    static _Link_type& _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }

    iterator begin() { return iterator(_M_leftmost()); }
    iterator end() { return iterator(_M_header._M_data); }

    iterator insert_unique(iterator __position, const value_type& __x);
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);
    _Link_type _M_copy(_Link_type __x, _Link_type __p);
    pair<iterator, bool> insert_unique(const value_type& __x);
    iterator insert_equal(const value_type& __x);       // 0x00AD54B0 for the cost multimap
    _Base_ptr _M_lower_bound(const key_type& __k) const;
    void erase(iterator __position)
    {
        _Link_type __y = (_Link_type)_Rb_global_inst::_Rebalance_for_erase(__position._M_node,
            _M_header._M_data->_M_parent, _M_header._M_data->_M_left, _M_header._M_data->_M_right);
        __y->_M_value_field.~value_type();
        _M_deallocate_node(__y);
        --_M_node_count;
    }
};


template <class _Key, class _Tp, class _Compare, class _Alloc> class multimap {
public:
    typedef _Key key_type;
    typedef _Tp mapped_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;                                     // +0x00
    iterator insert(const value_type& __x) { return _M_t.insert_equal(__x); }
    void erase(iterator __position) { _M_t.erase(__position); }
};

template <class _Key, class _Tp, class _Compare, class _Alloc> class map {
public:
    typedef _Key key_type;
    typedef _Tp mapped_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    typedef typename _Rep_type::const_iterator const_iterator;
    _Rep_type _M_t;                                     // +0x00
    iterator begin() { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
};

}   // namespace std

struct C3DVector { float X, Y, Z; };

namespace QuadricOptimiserInternals {

class CSubContraction {
public:
    unsigned long Vertex1;                              // +0x00
    unsigned long Vertex2;                              // +0x04
};

template <class T> class CArray {                       // retail STLport vector, 12 bytes
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned long n) { return _M_start[n]; }
};

typedef std::multimap<float, CCountedPointer<CVertexPairContraction>, std::less<float>,
                       std::allocator<std::pair<const float, CCountedPointer<CVertexPairContraction> > > > TContractionQueue;

class CVertexPairContraction {                          // retail layout (landed PerformContraction 0x00ACD840)
public:
    bool                      Exists;                   // +0x00
    bool                      Unsplit;                  // +0x01
    CArray<CSubContraction>   SubContractions;          // +0x04
    float                     Weight2;                  // +0x10
    float                     Cost;                     // +0x14
    C3DVector                 ResultingPosition;        // +0x18
    TContractionQueue::iterator ListIterator;           // +0x24
};

class COptimisedVertex {                                // retail 0xB8 bytes
public:
    C3DVector     Position;                             // +0x00
    C3DVector     Normal;                               // +0x0C
    C3DVector     TextureCoord;                         // +0x18
    unsigned char _pad24[0xA8 - 0x24];                  // PositionError +0x28, Cost +0x80, FaceList +0x84, Exists +0x90,
                                                        // UniqueIndex +0x94, OutputIndex +0x98, Groups +0x9C
    std::map<unsigned long, CContractionReference, std::less<unsigned long>,
              std::allocator<std::pair<const unsigned long, CContractionReference> > > PairContractions;   // +0xA8
    unsigned char _padB4[4];
};

class COptimisedPrimitive {
public:
    long                       MaterialIndex;           // +0x00
    unsigned char              UniqueVertices[0xC];     // +0x04
    CArray<COptimisedVertex>   Vertices;                // +0x10
    unsigned char              Faces[0xC];              // +0x1C
    unsigned char              Edges[0xC];              // +0x28
    TContractionQueue          VertexPairContractions;  // +0x34
private:
    void ComputeSelfContraction(CVertexPairContraction& contraction);                          // 0x00AC7EB0
    void ComputeContraction(CVertexPairContraction& contraction);                              // 0x00AC92C0
    void UpdateCost(const CCountedPointer<CVertexPairContraction>& contraction);               // 0x00AC8A60
    void UpdateContractions(const CSubContraction& sub);
};

// Same-TU callees (retail holds both bodies in this TU). UpdateCost follows its retail code (0x00AC8A60: erase the
// contraction's queue entry, re-insert it under its new cost); ComputeContraction is a stand-in that keeps only the
// opening Unsplit dispatch of the 0xD4-frame retail body 0x00AC92C0. Both stay out of line as in retail.
__declspec(noinline) void COptimisedPrimitive::ComputeContraction(CVertexPairContraction& contraction)
{
    if (contraction.Unsplit)
        ComputeSelfContraction(contraction);
}

__declspec(noinline) void COptimisedPrimitive::UpdateCost(const CCountedPointer<CVertexPairContraction>& contraction)
{
    if (contraction->Exists) {
        VertexPairContractions.erase(contraction->ListIterator);
        contraction->ListIterator = VertexPairContractions.insert(TContractionQueue::value_type(contraction->Cost, contraction));
    }
}

void COptimisedPrimitive::UpdateContractions(const CSubContraction& sub)
{
    COptimisedVertex& vertex = Vertices[sub.Vertex1];
    for (std::map<unsigned long, CContractionReference, std::less<unsigned long>,
                   std::allocator<std::pair<const unsigned long, CContractionReference> > >::iterator i =
             vertex.PairContractions.begin();
         i != vertex.PairContractions.end(); ++i) {
        CContractionReference reference = i->second;
        CCountedPointer<CVertexPairContraction> contraction = reference.Contraction;
        ComputeContraction(*contraction);
        UpdateCost(contraction);
    }
}

}