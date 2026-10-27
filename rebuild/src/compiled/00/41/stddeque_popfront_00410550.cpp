// std::deque::pop_front() -- (deque<ETutorialCategory>) remove the front element: step the start cursor, or (at the end of its node) free the node and move to the next
// Leaf: ?pop_front@?$deque@W4ETutorialCategory@@V?$allocator@W4ETutorialCategory@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00410550, 67 bytes, thiscall, plain `ret`.
// Identity (probable): 4-byte element (`sub 4`/`add 4`, 0x80-byte node), in the CUserProfileManager TU beside deque<ETutorialCategory>::_M_initialize_map 0x0040F4A0 and the out-of-line _M_pop_front_aux 0x0040F560; it has no caller or reference. The byte-identical 0x00410ED0 is the copy ProcessPendingTutorials calls; one of the two is deque::pop_front and the other the queue<ETutorialCategory>::pop() that wraps it (Ego_r.pdb CUserProfileManager::PendingTutorialCategories is a queue). This row takes deque::pop_front, which is probable only.
// Callees: free 0x00BFEA14 (the node allocator's null-tested deallocate).
// Shape (family M014, representative 0x00410550): the landed deque<C2DVector> TU model (0x00A0EE50: STLport 4.6 _deque.h with
//   _STLP_alloc_proxy map members, inline iterator steps, the trivial `__copy_trivial` node moves) instantiated for
//   ETutorialCategory (4-byte element, 128-byte nodes); `if (cur != last - 1) ++cur; else _M_pop_front_aux();` with the aux inlined (deallocate the front node, _M_set_node(node + 1), cur = first).
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6
inline void* operator new(unsigned int, void* p) { return p; }

enum ETutorialCategory { TUTORIAL_CATEGORY_NONE = 0 };
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

inline void* __copy_trivial_backward(const void* __first, const void* __last, void* __result)
{
    const int _Num = (const char*)__last - (const char*)__first;
    return (_Num > 0) ? memmove((char*)__result - _Num, __first, _Num) : __result;
}

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp*) { }
template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; typedef const _Tp* pointer; };

template <class _Tp> struct _Deque_iterator_base {          // 16 bytes
    enum _Constants { _blocksize = 128, __buffer_size = (sizeof(_Tp) < (unsigned int)_blocksize ? ((unsigned int)_blocksize / sizeof(_Tp)) : 1) };
    typedef int difference_type;
    typedef _Tp** _Map_pointer;
    typedef _Deque_iterator_base<_Tp> _Self;

    _Tp*        _M_cur;                                     // +0x00
    _Tp*        _M_first;                                   // +0x04
    _Tp*        _M_last;                                    // +0x08
    _Map_pointer _M_node;                                   // +0x0c

    _Deque_iterator_base() : _M_cur(0), _M_first(0), _M_last(0), _M_node(0) { }
    difference_type _M_subtract(const _Self& __x) const
    {
        return difference_type(__buffer_size) * (_M_node - __x._M_node - 1) + (_M_cur - _M_first) + (__x._M_last - __x._M_cur);
    }
    void _M_increment()
    {
        if (++_M_cur == _M_last) {
            _M_set_node(_M_node + 1);
            _M_cur = _M_first;
        }
    }
    void _M_decrement()
    {
        if (_M_cur == _M_first) {
            _M_set_node(_M_node - 1);
            _M_cur = _M_last;
        }
        --_M_cur;
    }
    void _M_advance(difference_type __n);                   // 0x00A0E2A0
    void _M_set_node(_Map_pointer __new_node)
    {
        _M_last = (_M_first = *(_M_node = __new_node)) + difference_type(__buffer_size);
    }
};

template <class _Tp, class _Traits> struct _Deque_iterator : public _Deque_iterator_base<_Tp> {
    typedef typename _Traits::reference reference;
    typedef int difference_type;
    typedef _Deque_iterator<_Tp, _Traits> _Self;
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > _Nonconst_self;

    _Deque_iterator() { }
    _Deque_iterator(const _Nonconst_self& __x) : _Deque_iterator_base<_Tp>(__x) { }

    reference operator*() const { return *this->_M_cur; }
    difference_type operator-(const _Self& __x) const { return this->_M_subtract(__x); }
    _Self& operator++() { this->_M_increment(); return *this; }
    _Self& operator--() { this->_M_decrement(); return *this; }
    _Self& operator+=(difference_type __n) { this->_M_advance(__n); return *this; }
    _Self operator+(difference_type __n) const
    {
        _Self __tmp = *this;
        return __tmp += __n;
    }
    _Self& operator-=(difference_type __n) { return *this += -__n; }
    _Self operator-(difference_type __n) const
    {
        _Self __tmp = *this;
        return __tmp -= __n;
    }
    reference operator[](difference_type __n) const { return *(*this + __n); }
};

template <class _Tp, class _Traits1, class _Traits2>
inline bool __fastcall operator==(const _Deque_iterator<_Tp, _Traits1>& __x, const _Deque_iterator<_Tp, _Traits2>& __y) { return __x._M_cur == __y._M_cur; }
template <class _Tp, class _Traits1, class _Traits2>
inline bool __fastcall operator!=(const _Deque_iterator<_Tp, _Traits1>& __x, const _Deque_iterator<_Tp, _Traits2>& __y) { return __x._M_cur != __y._M_cur; }

template <class _Tp, class _Alloc = allocator<_Tp> > class deque {
public:
    typedef _Tp value_type;
    typedef unsigned int size_type;
    typedef int difference_type;
    typedef _Tp** _Map_pointer;
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _Deque_iterator<_Tp, _Const_traits<_Tp> > const_iterator;

    iterator     _M_start;                                  // +0x00
    iterator     _M_finish;                                 // +0x10
    _STLP_alloc_proxy<_Map_pointer, _Tp*, allocator<_Tp*> > _M_map;   // +0x20
    _STLP_alloc_proxy<size_type, _Tp, _Alloc> _M_map_size;            // +0x24

    static size_type buffer_size() { return (size_type)_Deque_iterator_base<_Tp>::__buffer_size; }
    size_type size() const { return this->_M_finish - this->_M_start; }

    void push_front(const value_type& __t)
    {
        if (this->_M_start._M_cur != this->_M_start._M_first) {
            _Construct(this->_M_start._M_cur - 1, __t);
            --this->_M_start._M_cur;
        }
        else
            _M_push_front_aux_v(__t);
    }
    void pop_back()
    {
        if (this->_M_finish._M_cur != this->_M_finish._M_first) {
            --this->_M_finish._M_cur;
            _Destroy(this->_M_finish._M_cur);
        }
        else
            _M_pop_back_aux();
    }
    iterator erase(iterator __first, iterator __last);                                   // 0x00A0F040
    void insert(iterator __pos, size_type __n, const value_type& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const value_type& __x);
    void clear();

    void _M_fill_insert(iterator __pos, size_type __n, const value_type& __x);          // 0x00A0FAA0
    void _M_push_front_aux_v(const value_type& __t);                                     // 0x00A0F3B0
    void _M_pop_back_aux();
    void pop_front();
    void _M_pop_front_aux();
    void _M_reallocate_map(size_type __nodes_to_add, bool __add_at_front);
    void _M_reserve_map_at_back(size_type __nodes_to_add = 1)
    {
        if (__nodes_to_add + 1 > this->_M_map_size._M_data - (this->_M_finish._M_node - this->_M_map._M_data))
            _M_reallocate_map(__nodes_to_add, false);
    }
    void _M_reserve_map_at_front(size_type __nodes_to_add = 1)
    {
        if (__nodes_to_add > size_type(this->_M_start._M_node - this->_M_map._M_data))
            _M_reallocate_map(__nodes_to_add, true);
    }
    iterator _M_reserve_elements_at_front(size_type __n);
    void _M_new_elements_at_front(size_type __new_elements);                            // 0x00A0F300
    void _M_new_elements_at_back(size_type __new_elements);                             // 0x00A0F350
};

template <class _Tp, class _Alloc>
void deque<_Tp, _Alloc>::_M_reallocate_map(size_type __nodes_to_add, bool __add_at_front)
{
    size_type __old_num_nodes = this->_M_finish._M_node - this->_M_start._M_node + 1;
    size_type __new_num_nodes = __old_num_nodes + __nodes_to_add;

    _Map_pointer __new_nstart;
    if (this->_M_map_size._M_data > 2 * __new_num_nodes) {
        __new_nstart = this->_M_map._M_data + (this->_M_map_size._M_data - __new_num_nodes) / 2
                       + (__add_at_front ? __nodes_to_add : 0);
        if (__new_nstart < this->_M_start._M_node)
            __copy_trivial(this->_M_start._M_node, this->_M_finish._M_node + 1, __new_nstart);
        else
            __copy_trivial_backward(this->_M_start._M_node, this->_M_finish._M_node + 1,
                                    __new_nstart + __old_num_nodes);
    } else {
        size_type __new_map_size =
            this->_M_map_size._M_data + (max)(this->_M_map_size._M_data, __nodes_to_add) + 2;

        _Map_pointer __new_map = this->_M_map.allocate(__new_map_size);
        __new_nstart = __new_map + (__new_map_size - __new_num_nodes) / 2
                       + (__add_at_front ? __nodes_to_add : 0);
        __copy_trivial(this->_M_start._M_node, this->_M_finish._M_node + 1, __new_nstart);
        this->_M_map.deallocate(this->_M_map._M_data, this->_M_map_size._M_data);

        this->_M_map._M_data = __new_map;
        this->_M_map_size._M_data = __new_map_size;
    }

    this->_M_start._M_set_node(__new_nstart);
    this->_M_finish._M_set_node(__new_nstart + __old_num_nodes - 1);
}


}namespace std {
template <class _Tp, class _Alloc>
void deque<_Tp, _Alloc>::pop_front()
{
    if (this->_M_start._M_cur != this->_M_start._M_last - 1) {
        _Destroy(this->_M_start._M_cur);
        ++this->_M_start._M_cur;
    }
    else
        _M_pop_front_aux();
}
template <class _Tp, class _Alloc>
inline void deque<_Tp, _Alloc>::_M_pop_front_aux()
{
    _Destroy(this->_M_start._M_cur);
    this->_M_map_size.deallocate(this->_M_start._M_first, buffer_size());
    this->_M_start._M_set_node(this->_M_start._M_node + 1);
    this->_M_start._M_cur = this->_M_start._M_first;
}
template void deque<ETutorialCategory, allocator<ETutorialCategory > >::pop_front();
}