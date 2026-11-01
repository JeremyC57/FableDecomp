// CVertexGroupBlockOptimiser::CountRemainingGroups() const -- number of groups used by the still-remaining input blocks that the current block does not already hold
// Leaf: ?CountRemainingGroups@CVertexGroupBlockOptimiser@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD8ED0, 269 bytes, thiscall, plain `ret`, returns unsigned long.
// Identity: Ego_r.pdb ?CountRemainingGroups@CVertexGroupBlockOptimiser@@ABEKXZ (private const, no argument, unsigned long).
//   Callers: CalcCurrentBestTheoreticalResultNewBlock 0x00AD8FE0 (landed, "remaining = CountRemainingGroups()") and Process
//   0x00ADA240 (landed, `OptimalSize = (CountRemainingGroups() + maxN - 3) / (maxN - 1)`); both declare it, bound QBEKXZ
//   (public) -- the PDB says private (ABEKXZ): NAME_FIXES. Catalog "_global::VertexListMarkConnected" is descriptive.
// Layout (retail, STLport sizes; Ego_r is Dinkumware): InputList.Blocks CArray<CVertexGroupBlock> +0x00 (24-byte
//   CVertexGroupBlock: Groups vector +0, GroupToIndexMap map +0xC -- the `imul 2AAAAAABh ; sar 2` divide by 24),
//   OptimalBlockList / Blocks lists +0x0C / +0x10 (4 bytes each), CurrentBlock +0x14, RemainingBlocks vector<bool> +0x18
//   (one byte per element, as retail STLport's generic vector), CurrentBlockSegmentGroups +0x24, CurrentBlockSegmentIndices
//   +0x30, CurrentBlockSegmentCount +0x3C, MaxGroupIndex +0x40, MaxN +0x44 (the landed siblings' offsets).
// Callees: std::vector<bool>::vector(unsigned int, const bool&, const allocator<bool>&) 0x00451583, free 0x00BFEA14
//   (the inline ~vector).
// Shape: a local vector<bool>(MaxGroupIndex + 1, false); the default allocator temporary and the `false` temporary share
//   one frame byte, and used._M_start stays in ebp from the ctor to the free at the end. Both need the STLport ctor body
//   compiled in the TU BEFORE this function (explicit instantiation) and not expanded: retail calls 0x00451583 from all 12
//   call sites and never inlines it, while VC7.1 here expands the in-class body unless it is __declspec(noinline) (p4-p6).
//   With a declared-only ctor VC7.1 must assume `this` escapes and reloads _M_start after every byte store (p1-p3).
//   The inner loop takes the block by reference and hoists the group count (`n`); the stores go through used[...].
//   CVertexGroupBlock::HasGroup (Ego_r ?HasGroup@CVertexGroupBlock@@QBE_NK@Z, a non-inline member of the same TU; no
//   retail out-of-line copy) is defined before and auto-inlined; its find result is a NAMED const_iterator, which makes
//   VC7.1 re-read end() (`cmp edx,[ebx+0Ch]`) instead of reusing the header register (p13 v p14).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* p) { return p; }

namespace std {
template <class T> class allocator {
public:
    allocator() { }
    template <class U> allocator(const allocator<U>&) { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};
template <class T> struct less {
    bool operator()(const T& a, const T& b) const { return a < b; }
};
template <class T1, class T2> struct pair {
    typedef T1 first_type;
    T1 first;
    T2 second;
    pair() { }
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
template <class V, class Tp, class A> class _STLP_alloc_proxy : public A {
public:
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) { }
};
struct __true_type { };
struct __false_type { };
template <class T> struct __type_traits { typedef __false_type is_POD_type; };
template <> struct __type_traits<bool> { typedef __true_type is_POD_type; };
template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
{
    for ( ; __n > 0; --__n, ++__first)
        *__first = __val;
    return __first;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __true_type&)
{
    return fill_n(__first, __n, __x);
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        new (&*__cur) _Tp(__x);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    typedef typename __type_traits<_Tp>::is_POD_type _Is_POD;
    return __uninitialized_fill_n(__first, __n, __x, _Is_POD());
}
template <class T, class A> class _Vector_base {
public:
    _Vector_base(const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
    _Vector_base(unsigned int n, const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, T, A> _M_end_of_storage;
};
template <class T, class A = allocator<T> > class vector : public _Vector_base<T, A> {
public:
    typedef A allocator_type;
    typedef unsigned int size_type;
    __declspec(noinline) explicit vector(size_type n, const T& value, const allocator_type& a = allocator_type())   // 0x00451583 (bool)
        : _Vector_base<T, A>(n, a)
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, n, value);
    }
    ~vector() { }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    T& operator[](size_type n) { return *(this->_M_start + n); }
    const T& operator[](size_type n) const { return *(this->_M_start + n); }
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;
};
struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
};
inline bool operator==(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node == __y._M_node; }
inline bool operator!=(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node != __y._M_node; }
template <class V> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_node<V>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) { _M_node = __it._M_node; }
};
template <class V, class A> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<V> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class P> struct _Select1st {
    const typename P::first_type& operator()(const P& x) const { return x.first; }
};
template <class K, class V, class KeyOfValue, class Compare, class A> class _Rb_tree : public _Rb_tree_base<V, A> {
public:
    typedef _Rb_tree_node<V> _Node;
    typedef _Rb_tree_node<V>* _Link_type;
    typedef K key_type;
    typedef _Rb_tree_iterator<V> iterator;
    typedef _Rb_tree_iterator<V> const_iterator;
protected:
    unsigned int _M_node_count;
    Compare      _M_key_compare;
    _Link_type& _M_root() const { return (_Link_type&) this->_M_header._M_data->_M_parent; }
    static _Link_type& _S_left(_Link_type __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& _S_right(_Link_type __x) { return (_Link_type&)(__x->_M_right); }
    static V& _S_value(_Link_type __x) { return __x->_M_value_field; }
    static const K& _S_key(_Link_type __x) { return KeyOfValue()(_S_value(__x)); }
public:
    iterator end() { return iterator(this->_M_header._M_data); }
    const_iterator end() const { return const_iterator(this->_M_header._M_data); }
    iterator find(const key_type& __x) { return iterator(_M_find(__x)); }
    const_iterator find(const key_type& __x) const { return const_iterator(_M_find(__x)); }
private:
    _Rb_tree_node<V>* _M_find(const key_type& __k) const
    {
        _Rb_tree_node<V>* __y = (_Rb_tree_node<V>*)this->_M_header._M_data;
        _Rb_tree_node<V>* __x = _M_root();
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = _S_left(__x);
            else
                __x = _S_right(__x);
        if (__y == this->_M_header._M_data || _M_key_compare(__k, _S_key(__y)))
            __y = this->_M_header._M_data;
        return __y;
    }
};
template <class K, class T, class C = less<K>, class A = allocator<pair<const K, T> > > class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree<K, value_type, _Select1st<value_type>, C, A> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    typedef typename _Rep_type::const_iterator const_iterator;
    iterator end() { return _M_t.end(); }
    const_iterator end() const { return _M_t.end(); }
    iterator find(const K& x) { return _M_t.find(x); }
    const_iterator find(const K& x) const { return _M_t.find(x); }
};
} // namespace std


namespace std {
template <class T> struct _List_node_base_stub { _List_node_base_stub* _M_next; _List_node_base_stub* _M_prev; };
template <class T, class A = allocator<T> > class list {   // STLport 4.6 list: one node pointer (alloc proxy), 4 bytes
public:
    _List_node_base_stub<T>* _M_node;
};
} // namespace std

template <class T> class CArray : public std::vector<T> { };

class CVertexGroupBlock {
public:
    CArray<unsigned long>              Groups;              // +0x00
    std::map<unsigned long, long>      GroupToIndexMap;     // +0x0C
    bool HasGroup(unsigned long group) const;
};

bool CVertexGroupBlock::HasGroup(unsigned long group) const
{
    std::map<unsigned long, long>::const_iterator it = GroupToIndexMap.find(group);
    return it != GroupToIndexMap.end();
}

class CVertexGroupBlockList {
public:
    CArray<CVertexGroupBlock> Blocks;                       // +0x00
};

class CVertexGroupBlockOptimiser {
public:
    CVertexGroupBlockList      InputList;                   // +0x00
    std::list<CVertexGroupBlock> OptimalBlockList;          // +0x0C
    std::list<CVertexGroupBlock> Blocks;                    // +0x10
    CVertexGroupBlock*         CurrentBlock;                // +0x14
    std::vector<bool>          RemainingBlocks;             // +0x18
    std::vector<CVertexGroupBlock> CurrentBlockSegmentGroups; // +0x24
    std::vector<long>          CurrentBlockSegmentIndices;  // +0x30
    unsigned long              CurrentBlockSegmentCount;    // +0x3C
    unsigned long              MaxGroupIndex;               // +0x40
private:
    unsigned long CountRemainingGroups() const;
};

unsigned long CVertexGroupBlockOptimiser::CountRemainingGroups() const
{
    std::vector<bool> used(MaxGroupIndex + 1, false);
    unsigned long i;
    for (i = 0; i < InputList.Blocks.size(); ++i) {
        if (RemainingBlocks[i]) {
            const CVertexGroupBlock& block = InputList.Blocks[i];
            unsigned long n = block.Groups.size();
            for (unsigned long j = 0; j < n; ++j)
                used[block.Groups[j]] = true;
        }
    }
    unsigned long count = 0;
    for (i = 0; i <= MaxGroupIndex; ++i) {
        if (used[i] && !CurrentBlock->HasGroup(i))
            ++count;
    }
    return count;
}
