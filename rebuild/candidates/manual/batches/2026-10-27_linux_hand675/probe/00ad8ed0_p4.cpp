// CVertexGroupBlockOptimiser::CountRemainingGroups() const -- number of groups used by the still-remaining input blocks that the current block does not already hold
// Leaf: ?CountRemainingGroups@CVertexGroupBlockOptimiser@@
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace std {
template <class T> class allocator {
public:
    allocator() { }
    template <class U> allocator(const allocator<U>&) { }
    T* allocate(unsigned int n) { return (T*)malloc(n * sizeof(T)); }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};
template <class T> struct less {
    bool operator()(const T& a, const T& b) const { return a < b; }
};
template <class T1, class T2> struct pair {
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
template <class F, class S, class T> inline F uninitialized_fill_n(F first, S n, const T& x)
{
    for (; n > 0; --n, ++first)
        *first = x;
    return first;
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
    explicit vector(size_type n, const T& value, const allocator_type& a = allocator_type())   // 0x00451583 (bool)
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
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
    bool operator==(const _Rb_tree_iterator& y) const { return _M_node == y._M_node; }
    bool operator!=(const _Rb_tree_iterator& y) const { return _M_node != y._M_node; }
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
    typedef _Rb_tree_iterator<V> iterator;
    unsigned int _M_node_count;
    Compare      _M_key_compare;
    static const K& _S_key(_Rb_tree_node_base* x) { return ((_Node*)x)->_M_value_field.first; }
    _Node* _M_root() const { return (_Node*)this->_M_header._M_data->_M_parent; }
    iterator end() { return iterator(this->_M_header._M_data); }
    _Node* _M_find(const K& k) const
    {
        _Rb_tree_node_base* y = this->_M_header._M_data;
        _Rb_tree_node_base* x = _M_root();
        while (x != 0)
            if (!_M_key_compare(_S_key(x), k))
                y = x, x = x->_M_left;
            else
                x = x->_M_right;
        if (y == this->_M_header._M_data || _M_key_compare(k, _S_key(y)))
            y = this->_M_header._M_data;
        return (_Node*)y;
    }
    iterator find(const K& k) { return iterator(_M_find(k)); }
};
template <class K, class T, class C = less<K>, class A = allocator<pair<const K, T> > > class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree<K, value_type, _Select1st<value_type>, C, A> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator end() { return _M_t.end(); }
    iterator find(const K& x) { return _M_t.find(x); }
};
} // namespace std

template <class T> class CArray : public std::vector<T> { };

class CVertexGroupBlock {
public:
    CArray<unsigned long>              Groups;              // +0x00
    std::map<unsigned long, long>      GroupToIndexMap;     // +0x0C
    bool HasGroup(unsigned long group) { return GroupToIndexMap.find(group) != GroupToIndexMap.end(); }
};

class CVertexGroupBlockList {
public:
    CArray<CVertexGroupBlock> Blocks;                       // +0x00
};

class CVertexGroupBlockOptimiser {
public:
    CVertexGroupBlockList      InputList;                   // +0x00
    unsigned char              OptimalBlockList_Blocks[8];  // +0x0C two STLport lists
    CVertexGroupBlock*         CurrentBlock;                // +0x14
    std::vector<bool>          RemainingBlocks;             // +0x18
    unsigned char              Segments[0x18];              // +0x24
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
