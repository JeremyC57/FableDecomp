// std::deque<ETutorialCategory>::~deque() -- STLport 4.6 deque destructor: _Destroy over [start, finish) through the out-of-line __destroy_aux (__false_type), then ~_Deque_base frees the node buffers and the map
// Leaf: ??1?$deque@W4ETutorialCategory@@
// Grade: RELOCATION_MATCH (150v150). Lever (sec1b): STLport 4.6 verbatim -- _Deque_iterator derives from _Deque_iterator_base and
//   its copy ctor is `: _Deque_iterator_base<_Tp>(__x)` (the base's implicit POD copy), and the three inline by-value layers
//   _Destroy -> __destroy -> __destroy_aux(first, last, _Trivial_destructor()) give retail's two 16-byte argument copies.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00412210, 150 bytes, __fastcall. Callers: CUserProfileManager's dtor (0x00408A04, `lea ecx,[esi+98h]`:
//   the queue<ETutorialCategory> PendingTutorialCategories, Ego_r CUserProfileManager +0xA8) and the thunk 0x00412870.
//   Neighbour 0x00412860 jumps to the landed vector<ETutorialCategory>::_M_fill_insert 0x00411F10 (same TU).
// Callees: std::__destroy_aux<_Deque_iterator<ETutorialCategory,...> > 0x00410D80 (catalog "std::__destroy_aux"; __fastcall, the
//   const __false_type& tag in ecx, both iterators by value, ret 20h); _Deque_base::_M_destroy_nodes 0x0040E130; free 0x00BFEA14.
#include <stdlib.h>
namespace std {
struct __false_type { __false_type() {} };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp* pointer; typedef _Tp& reference; };
template <class _Tp> struct _Deque_iterator_base {
    typedef _Tp value_type;
    _Tp* _M_cur; _Tp* _M_first; _Tp* _M_last; _Tp** _M_node;
};
template <class _Tp, class _Traits> struct _Deque_iterator : public _Deque_iterator_base<_Tp> {
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _Deque_iterator(const iterator& __x) : _Deque_iterator_base<_Tp>(__x) {}
};
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x00410D80
template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, (typename _ForwardIterator::value_type*)0);
}
template <class _Tp> class allocator {};
template <class _Tp, class _Alloc>
class _Deque_base {
public:
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator _M_start;
    iterator _M_finish;
    _Tp** _M_map;
    unsigned int _M_map_size;
protected:
    void _M_destroy_nodes(_Tp** __nstart, _Tp** __nfinish);   // 0x0040E130
    void _M_deallocate_map(_Tp** __p, unsigned int) { if (__p) free(__p); }   // STLport __node_alloc / malloc_alloc for 4-byte maps
public:
    ~_Deque_base()
    {
        if (_M_map) {
            _M_destroy_nodes(_M_start._M_node, _M_finish._M_node + 1);
            _M_deallocate_map(_M_map, _M_map_size);
        }
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class deque : protected _Deque_base<_Tp, _Alloc> {
public:
    ~deque() { _Destroy(this->_M_start, this->_M_finish); }
};
}
enum ETutorialCategory { ETUTORIAL_CATEGORY_NONE };
template class std::deque<ETutorialCategory>;