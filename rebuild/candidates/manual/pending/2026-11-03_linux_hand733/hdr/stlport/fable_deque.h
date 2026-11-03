#ifndef FABLE_STLPORT_DEQUE_H
#define FABLE_STLPORT_DEQUE_H
// STLport 4.6 excerpts: stl/_deque.h / _deque.c / _queue.h (chunk 733, first shared model; the landed rows 0x00408990
// ~CUserProfileManager, 0x0040BB30 ProcessPendingTutorials and the CTCGuard deque<CCrimeWaitingForBribe> rows 0x007B4xxx model
// the same layout locally). Retail layout (0x28): _M_start {_M_cur, _M_first, _M_last, _M_node} +0x00, _M_finish +0x10,
// _M_map +0x20, _M_map_size +0x24 (CUserProfileManager::PendingTutorialCategories at +0x98: +0xA8 is _M_finish._M_cur).
// Node buffers are 128 bytes (__deque_buf_size: 128 / sizeof(T) elements; `add eax,80h` for 4-byte T in _M_set_node), freed
// through the malloc allocator (`test ; je ; push ; call free` = allocator<T>::deallocate).
// Members retail keeps out of line elsewhere (~deque 0x00412210 for ETutorialCategory, the out-of-line pop_front copy
// 0x00410ED0) are declared here with their STLport bodies where a retail caller expands them (pop_front / _M_pop_front_aux in
// CUserProfileManager::SetDefaultValues 0x0040C790); the rest are declarations only.
#include "stlport/fable_containers.h"
namespace std {
inline size_t __deque_buf_size(size_t __size) { return __size < 128 ? (size_t)(128 / __size) : (size_t)1; }
template <class _Tp> struct _Deque_iterator_base {
    typedef _Tp value_type;
    typedef ptrdiff_t difference_type;
    typedef _Tp** _Map_pointer;
    static size_t _S_buffer_size() { return __deque_buf_size(sizeof(_Tp)); }
    _Tp* _M_cur;
    _Tp* _M_first;
    _Tp* _M_last;
    _Map_pointer _M_node;
    bool _M_equal(const _Deque_iterator_base& __y) const { return _M_cur == __y._M_cur; }
    void _M_set_node(_Map_pointer __new_node) {
        _M_last = (_M_first = *(_M_node = __new_node)) + difference_type(_S_buffer_size());
    }
};
template <class _Tp, class _Traits> struct _Deque_iterator : public _Deque_iterator_base<_Tp> {
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _Deque_iterator() { }
    reference operator*() const { return *this->_M_cur; }
    bool operator==(const _Deque_iterator& __y) const { return this->_M_equal(__y); }
    bool operator!=(const _Deque_iterator& __y) const { return !this->_M_equal(__y); }
};
template <class _Tp, class _Alloc> class _Deque_base {
public:
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _Deque_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    typedef allocator<_Tp*> _Map_alloc_type;
    iterator _M_start;                                                // +0x00
    iterator _M_finish;                                               // +0x10
    _STLP_alloc_proxy<_Tp**, _Tp*, _Map_alloc_type> _M_map;           // +0x20
    _STLP_alloc_proxy<size_t, _Tp, _Alloc> _M_map_size;               // +0x24
};
template <class _Tp, class _Alloc = allocator<_Tp> > class deque : protected _Deque_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    typedef _Tp& reference;
    typedef const _Tp& const_reference;
    typedef size_t size_type;
    typedef typename _Deque_base<_Tp, _Alloc>::iterator iterator;
    static size_t buffer_size() { return _Deque_iterator_base<_Tp>::_S_buffer_size(); }
    deque();
    ~deque();                                                         // out of line (ETutorialCategory: 0x00412210)
    bool empty() const { return this->_M_finish == this->_M_start; }
    reference front() { return *this->_M_start; }
    void pop_front() {
        if (this->_M_start._M_cur != this->_M_start._M_last - 1) {
            _Destroy(this->_M_start._M_cur);
            ++this->_M_start._M_cur;
        }
        else
            _M_pop_front_aux();
    }
protected:
    void _M_pop_front_aux();
};
// _deque.c
template <class _Tp, class _Alloc> void deque<_Tp, _Alloc>::_M_pop_front_aux() {
    _Destroy(this->_M_start._M_cur);
    this->_M_map_size.deallocate(this->_M_start._M_first, this->buffer_size());
    this->_M_start._M_set_node(this->_M_start._M_node + 1);
    this->_M_start._M_cur = this->_M_start._M_first;
}
// _queue.h
template <class _Tp, class _Sequence = deque<_Tp> > class queue {
public:
    typedef typename _Sequence::value_type value_type;
    typedef typename _Sequence::size_type size_type;
    typedef typename _Sequence::reference reference;
    bool empty() const { return c.empty(); }
    reference front() { return c.front(); }
    void pop() { c.pop_front(); }
protected:
    _Sequence c;
};
}
#endif
