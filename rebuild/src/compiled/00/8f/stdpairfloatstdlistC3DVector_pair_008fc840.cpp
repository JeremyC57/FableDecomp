// std::pair<float, std::list<C3DVector> >::pair(const pair&) -- the implicit copy constructor: copy the key and deep-copy the point list
// Leaf: ??0?$pair@MV?$list@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@@std@@QAE@ABU01@@Z
// Retail: Fable.exe @ 0x008FC840, 73 bytes, thiscall `ret 4`, this in eax.
// Identity (proven): the body copies a 4-byte first member and then runs an STLport list copy constructor whose sentinel is a
// 0x14-byte node (8 + a 12-byte element) and whose range insert is 0x008FBF10 (12-byte element copied as three dwords into
// 0x14-byte nodes). Ego_r.pdb has exactly one pair<4-byte, list<12-byte> >: std::pair<float, std::list<C3DVector> > (with
// list<pair<float, list<C3DVector> > > around it); the 0x10-byte outer nodes built by its callers 0x008FC980 / 0x008FCA00
// (malloc 0x10 = 8 + sizeof(pair)) through the /Gr _Construct 0x008FC950 agree. 0x008FC900 is the sibling
// pair(const float&, const list&) (`ret 8`). Catalog "_gapscan::sub_008fc840".
// Callees: malloc @ 0x00BFEA0E (sentinel), std::list<C3DVector>::_M_insert_dispatch<const_iterator>(iterator,
// const_iterator, const_iterator, const __false_type&) @ 0x008FBF10 (catalog "LinkedList_CloneNodes"; declared only).
// Layout: STLport 4.6 list = one sentinel pointer; pair = { float first +0; list second +4 }.
// Shape: list(const list& x) : _List_base(x.get_allocator()) { insert(begin(), x.begin(), x.end()); } with the template
// insert forwarding to _M_insert_dispatch(..., _Integral()) out of line; the position iterator is copy-constructed in its
// argument slot (`push ecx ; mov ecx,esp ; mov [ecx],eax`): STLport's _List_iterator(const iterator&) is the user copy
// constructor of iterator only, so the two const_iterators are pushed as plain words. Emitted out of line by a placement-new
// helper, as retail's own
// _Construct does.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

class C3DVector {
public:
    float X, Y, Z;
};

namespace std {

struct __false_type { __false_type() { } };
template <class _Tp> struct _Is_integer { typedef __false_type _Integral; };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Tp> class _STLP_alloc_proxy : public allocator<_Tp> {
public:
    _Tp* _M_data;
    _STLP_alloc_proxy(const allocator<_Tp>& __a, _Tp* __p) : allocator<_Tp>(__a), _M_data(__p) { }
};

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x0
    _List_node_base* _M_prev;                               // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x8
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp& reference; };

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }   // copy ctor for iterator only
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _List_base(const allocator<_Tp>&) : _M_node(allocator<_Node>(), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    _STLP_alloc_proxy<_Node> _M_node;                       // sentinel
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;

    allocator<_Tp> get_allocator() const { return allocator<_Tp>(); }
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }

    list(const list<_Tp, _Alloc>& __x) : _List_base<_Tp, _Alloc>(__x.get_allocator())
    {
        insert(begin(), __x.begin(), __x.end());
    }
    template <class _InputIterator>
    void insert(iterator __pos, _InputIterator __first, _InputIterator __last)
    {
        typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
        _M_insert_dispatch(__pos, __first, __last, _Integral());
    }
    template <class _InputIter>
    void _M_insert_dispatch(iterator __pos, _InputIter __first, _InputIter __last, const __false_type&);   // 0x008FBF10
};

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }   // 0x008FC900
};

} // namespace std

std::pair<float, std::list<C3DVector> >* CopyCurvePair_Emit(void* p, const std::pair<float, std::list<C3DVector> >& x)
{
    return new (p) std::pair<float, std::list<C3DVector> >(x);
}