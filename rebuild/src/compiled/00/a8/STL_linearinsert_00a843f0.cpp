// std::__linear_insert(SKeyedList* first, SKeyedList* last, SKeyedList value, CKeyPairCompareLess comp) -- SGI insertion-sort step over (unsigned __int64 key, list) pairs: a value below the front shifts the whole run up one slot and lands at the front, otherwise it is inserted by the unguarded backward scan
// Leaf: __linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A843F0, 128 bytes (with int3 padding), `ret 14h`: /Gr `__fastcall` (first
//   in ecx -> esi, last in edx -> edi; the 16-byte value and the empty comparator on the stack).
// Identity: the only caller 0x00A84924 is __insertion_sort 0x00A848C0 (`__linear_insert(first, i, value_type(*i), comp)`),
//   in the navigation quad-tree TU's CVectorMap<unsigned __int64, list<C2DLineF> > sort family (0x00A83570 __pop_heap,
//   0x00A84470 sort_heap). The catalog's "InsertionSort_Callback_TreeNode" is descriptive.
// Callees: __copy_backward(first, last, result, random_access_iterator_tag, int*) 0x00A821C0, list::operator= 0x00517720,
//   list::~list 0x005127A0 (the by-value `value` destroyed by the callee), the pair copy constructor 0x00A825D0 and
//   __unguarded_linear_insert(last, value, comp) 0x00A81E20 (`__fastcall`).
// Shape: SGI `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else
//   __unguarded_linear_insert(last, val, comp);` -- the inline key compare (high dword first), copy_backward's empty tag
//   homed in the dead comparator slot, the pair assignment's key dwords kept in ebx/ebp across the call, the by-value
//   copy for the unguarded scan built in its argument slot by the OUT-OF-LINE pair copy constructor, and `value`'s list
//   destroyed on both exits. The same-TU ~list stand-in keeps `value` from being address-exposed (see 0x00A83570).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);             // 0x00BFEA14 (stand-in body only)
inline void* operator new(unsigned int, void* where) { return where; }

struct __false_type { __false_type() { } };

struct SListPayload {                                   // 16-byte list element (C2DLineF: four dwords)
    unsigned int m_words[4];
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
struct _List_node : _List_node_base {                   // 0x18 bytes
    SListPayload _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) { }
};
struct _List_iterator : _List_iterator_base {
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator(const _List_iterator& x) : _List_iterator_base(x._M_node) { }   // copy constructor
};
struct _List_const_iterator : _List_iterator_base {
    _List_const_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_const_iterator(const _List_iterator& x) : _List_iterator_base(x._M_node) { }   // converting, not a copy
};
template <class T> struct allocator {
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) { }
};
struct _List_base {
    _STLP_alloc_proxy<_List_node> _M_node;
    _List_base(const allocator<_List_node>& a) : _M_node(a, (_List_node*)0)
    {
        _List_node* n = _M_node.allocate(1);
        n->_M_next = n;
        n->_M_prev = n;
        _M_node._M_data = n;
    }
};
struct list : _List_base {
    allocator<_List_node> get_allocator() const { return allocator<_List_node>(); }
    _List_iterator begin() { return _List_iterator(_M_node._M_data->_M_next); }
    _List_const_iterator begin() const { return _List_const_iterator(_M_node._M_data->_M_next); }
    _List_const_iterator end() const { return _List_const_iterator(_M_node._M_data); }
    __declspec(noinline) void _M_insert_dispatch(_List_iterator pos, _List_const_iterator first, _List_const_iterator last,
                                                 const __false_type&)   // 0x005152C0 (same-TU body)
    {
        for (; first._M_node != last._M_node; first._M_node = first._M_node->_M_next) {
            _List_node* n = _M_node.allocate(1);
            new (&n->_M_data) SListPayload(((_List_node*)first._M_node)->_M_data);
            n->_M_next = pos._M_node;
            n->_M_prev = pos._M_node->_M_prev;
            pos._M_node->_M_prev->_M_next = n;
            pos._M_node->_M_prev = n;
        }
    }
    void insert(_List_iterator pos, _List_const_iterator first, _List_const_iterator last)
    {
        _M_insert_dispatch(pos, first, last, __false_type());
    }
    list(const list& x) : _List_base(x.get_allocator()) { insert(begin(), x.begin(), x.end()); }
    list& operator=(const list& x);                     // 0x00517720 (same-TU body below)
    ~list();                                            // 0x005127A0
};

// Same-TU stand-in for list::~list 0x005127A0 (the STLport clear + sentinel free): with only a declaration VC7.1 treats
// the by-value `value` as address-exposed for the whole function and keeps its key loads behind the esp adjustments;
// with a body it can see that `this` does not escape and schedules the loads early as retail.
#pragma auto_inline(off)
__declspec(noinline) list::~list()
{
    _List_node_base* cur = _M_node._M_data->_M_next;
    while (cur != _M_node._M_data) {
        _List_node_base* tmp = cur;
        cur = cur->_M_next;
        free(tmp);
    }
    _M_node._M_data->_M_next = _M_node._M_data;
    _M_node._M_data->_M_prev = _M_node._M_data;
    if (_M_node._M_data)
        free(_M_node._M_data);
}
#pragma auto_inline(on)

struct SKeyedList {                                     // std::pair<unsigned __int64, list<C2DLineF> >, 0x10 bytes
    unsigned __int64 first;                             // +0x00
    list             second;                            // +0x08 (+0x0C padding)
    SKeyedList(const SKeyedList& other);                // 0x00A825D0 (out of line in this caller; same-TU body below)
};

// Same-TU stand-ins for the pair copy constructor 0x00A825D0 and list::operator= 0x00517720: with declarations only,
// VC7.1 treats `value` as address-exposed (its address goes to both) and re-reads its key after the copy_backward call;
// with bodies that do not let the addresses escape it keeps the key in ebx/ebp as retail.
#pragma auto_inline(off)
__declspec(noinline) SKeyedList::SKeyedList(const SKeyedList& other) : first(other.first), second(other.second) { }
__declspec(noinline) list& list::operator=(const list& x)
{
    if (this != &x) {
        _List_node_base* cur = _M_node._M_data->_M_next;
        while (cur != _M_node._M_data) {
            _List_node_base* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
        insert(begin(), x.begin(), x.end());
    }
    return *this;
}
#pragma auto_inline(on)
struct CKeyPairCompareLess {                            // empty, passed by value
    bool operator()(const SKeyedList& a, const SKeyedList& b) const { return a.first < b.first; }
};
struct random_access_iterator_tag { random_access_iterator_tag() { } };


SKeyedList* __fastcall __copy_backward(SKeyedList* first, SKeyedList* last, SKeyedList* result,
                                       const random_access_iterator_tag&, int*);                   // 0x00A821C0
void __fastcall __unguarded_linear_insert(SKeyedList* last, SKeyedList value, CKeyPairCompareLess comp);   // 0x00A81E20

inline SKeyedList* copy_backward(SKeyedList* first, SKeyedList* last, SKeyedList* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

void __fastcall __linear_insert(SKeyedList* first, SKeyedList* last, SKeyedList value, CKeyPairCompareLess comp)
{
    if (comp(value, *first)) {
        copy_backward(first, last, last + 1);
        *first = value;
    } else
        __unguarded_linear_insert(last, value, comp);
}