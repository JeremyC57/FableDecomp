// CVertexGroupBlockOptimiser::CountRemainingCurrentBlockSegments() const -- how many distinct block segments the still-unused blocks of the current block belong to (1 when there is only one segment)
// Leaf: ?CountRemainingCurrentBlockSegments@CVertexGroupBlockOptimiser@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD9120, 0xCF bytes (+ int3 padding), thiscall, plain `ret`, unsigned long.
// Identity: Ego_r.pdb ?CountRemainingCurrentBlockSegments@CVertexGroupBlockOptimiser@@ABEKXZ (const, no argument);
// the members match the retail layout of the landed CalcCurrentBestTheoreticalResult 0x00AD9D70: RemainingBlocks
// (one byte per input block) +0x18, CurrentBlockSegmentIndices vector<long> +0x30, CurrentBlockSegmentCount +0x3C.
// No direct caller (reached through an inlined/indirect path); catalog "QuadricOptimiser_BuildGroupTree" is descriptive.
// Callees: malloc @ 0x00BFEA0E (the local set's header), set<long>::insert -> _Rb_tree::insert_unique(const long&)
// @ 0x00512DA0 (signed `setl` compare; pair<iterator,bool> through the hidden pointer), the set's node erase
// @ 0x004A9CB0 (manifest "LTextTreeWalkThrough::BuildTreeArray" is a propagation), free @ 0x00BFEA14.
// Shape: `if (CurrentBlockSegmentCount == 1) return 1;` (the 1 is loaded into eax for the compare and reused as the
// result); a local STLport set<long> (default constructor inline), a loop over the segment indices (size re-read
// every pass, the RemainingBlocks byte tested against the shared zero register) inserting each live block's
// segment; the result is the set's size, captured before the inline ~set (clear() + free(header)). STLport pair has user
// constructors, so insert_unique returns pair<iterator,bool> through a hidden pointer (a POD pair comes back in edx:eax).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace _STL {
template <class T> class allocator {
public:
    allocator() { }
    template <class U> allocator(const allocator<U>&) { }
    T* allocate(unsigned int n) { return (T*)malloc(n * sizeof(T)); }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};
template <class T> class less {
public:
    bool operator()(const T& a, const T& b) const { return a < b; }
};
template <class T1, class T2> class pair {
public:
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
struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0C
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;                                       // +0x10
};
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(const _Rb_tree_iterator& x) : _M_node(x._M_node) { }
};
template <class V, class A> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<V> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
    _Rb_tree_base(const A& a) : _M_header(allocator<_Node>(a), (_Node*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
};
template <class T> struct _Identity { };
template <class K, class V, class KeyOfValue, class Compare, class A> class _Rb_tree : public _Rb_tree_base<V, A> {
public:
    typedef _Rb_tree_node<V> _Node;
    typedef _Rb_tree_iterator<V> iterator;
    unsigned int _M_node_count;
    Compare      _M_key_compare;
    _Rb_tree(const Compare& comp, const A& a) : _Rb_tree_base<V, A>(a), _M_node_count(0), _M_key_compare(comp)
    {
        _M_empty_initialize();
    }
    ~_Rb_tree() { clear(); }
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;          // red
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
    void _M_erase(_Node* x);                                // 0x004A9CB0
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Node*)this->_M_header._M_data->_M_parent);
            this->_M_header._M_data->_M_left = this->_M_header._M_data;
            this->_M_header._M_data->_M_parent = 0;
            this->_M_header._M_data->_M_right = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
    unsigned int size() const { return _M_node_count; }
    pair<iterator, bool> insert_unique(const V& x);         // 0x00512DA0
};
template <class K, class C = less<K>, class A = allocator<K> > class set {
public:
    typedef _Rb_tree<K, K, _Identity<K>, C, A> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    set() : _M_t(C(), A()) { }
    unsigned int size() const { return _M_t.size(); }
    pair<iterator, bool> insert(const K& x) { return _M_t.insert_unique(x); }
};
} // namespace _STL

struct CLongArray {                                         // CArray<long> (retail STLport vector)
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    const long& operator[](unsigned long n) const { return *(_M_start + n); }
};

struct CByteArray {                                         // the retail RemainingBlocks (one byte per block)
    bool* _M_start;
    bool* _M_finish;
    bool* _M_end_of_storage;
    bool operator[](unsigned long n) const { return *(_M_start + n); }
};

class CVertexGroupBlockOptimiser {
public:
    unsigned char InputList_to_CurrentBlock[0x18];           // +0x00 InputList, OptimalBlockList, Blocks, CurrentBlock
    CByteArray    RemainingBlocks;                           // +0x18
    unsigned char CurrentBlockSegmentGroups[0x0C];           // +0x24
    CLongArray    CurrentBlockSegmentIndices;                // +0x30
    unsigned long CurrentBlockSegmentCount;                  // +0x3C
    unsigned long CountRemainingCurrentBlockSegments() const;
};

unsigned long CVertexGroupBlockOptimiser::CountRemainingCurrentBlockSegments() const
{
    if (CurrentBlockSegmentCount == 1)
        return 1;
    _STL::set<long> segments;
    for (unsigned long i = 0; i < CurrentBlockSegmentIndices.size(); ++i)
        if (RemainingBlocks[i])
            segments.insert(CurrentBlockSegmentIndices[i]);
    return segments.size();
}