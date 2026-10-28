// std::copy(deque<CCrimeWaitingForBribe>::iterator first, deque<CCrimeWaitingForBribe>::iterator last, back_insert_iterator<vector<CCrimeWaitingForBribe> > result) -- STLport copy of the guard's pending-crime deque into a vector through a back inserter
// Leaf: ??$copy@U?$_Deque_iterator@VCCrimeWaitingForBribe@@
// Retail: Fable.exe @ 0x007B52B0, 89 bytes, /Gr __fastcall `ret 24h` (the 4-byte back_insert_iterator returned through the
//   hidden pointer in ecx; the two 16-byte deque iterators and the inserter by value on the stack).
// Identity (proven): the only caller is the landed CTCGuard::OnSerialise 0x007B3760 (call 0x007B37CC), whose source
//   binds this address as `std::copy(PendingCrimes.begin(), PendingCrimes.end(), back_inserter(crimes))` (its model's
//   comment: "the deque-iterator instance stays out of line"). The catalog label "CEditControlCentre::GenerateNewFractal"
//   is propagated; the old park's CFractalParams class was invented.
// Callees: std::__copy_aux<deque iterator, back_insert_iterator<vector> >(first, last, result, const __false_type&)
//   0x007B51B0 (__fastcall, out of line; it calls __copy 0x007B5030 with a random_access_iterator_tag and (int*)0).
// Shape: STLport `return __copy_aux(first, last, result, __false_type());` -- the tag reference goes in edx and its empty
//   temporary is homed in the dead inserter slot (same-TU noinline __copy_aux body); the iterators are rebuilt in
//   their argument slots word by word (the user copy constructor). Model: the landed 0x007B3760 STLport TU.
class CCrimeWaitingForBribe {                               // 8 bytes
public:
    long DeedHandle;                                        // +0x00
    long Type;                                              // +0x04 EOpinionDeedType
};

enum ECrimeSeverity { CRIME_SEVERITY_NONE };

namespace std {

struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct output_iterator_tag { output_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };

template <class _Tp> struct _Deque_iterator_base {
    _Tp*  _M_cur;                                           // +0x00
    _Tp*  _M_first;                                         // +0x04
    _Tp*  _M_last;                                          // +0x08
    _Tp** _M_node;                                          // +0x0C
};

template <class _Tp, class _Traits> struct _Deque_iterator : public _Deque_iterator_base<_Tp> {
    typedef _Deque_iterator<_Tp, _Traits> _Self;
    _Deque_iterator(const _Self& __x) : _Deque_iterator_base<_Tp>(__x) { }
    static int _S_buffer_size() { return sizeof(_Tp) < 128 ? int(128 / sizeof(_Tp)) : 1; }   // STLport _MAX_BYTES = 128 (0x80-byte nodes)
    typename _Traits::reference operator*() const { return *this->_M_cur; }
    int operator-(const _Self& __x) const
    {
        return _S_buffer_size() * (this->_M_node - __x._M_node - 1) + (this->_M_cur - this->_M_first) + (__x._M_last - __x._M_cur);
    }
    _Self& operator++()
    {
        ++this->_M_cur;
        if (this->_M_cur == this->_M_last) {
            ++this->_M_node;
            this->_M_first = *this->_M_node;
            this->_M_last = this->_M_first + _S_buffer_size();
            this->_M_cur = this->_M_first;
        }
        return *this;
    }
};

template <class _Tp, class _Traits>
inline random_access_iterator_tag __ITERATOR_CATEGORY(const _Deque_iterator<_Tp, _Traits>&)
{
    return random_access_iterator_tag();
}
template <class _Tp, class _Traits> inline int* __DISTANCE_TYPE(const _Deque_iterator<_Tp, _Traits>&) { return (int*)0; }
template <class _Tp> inline random_access_iterator_tag __ITERATOR_CATEGORY(const _Tp*) { return random_access_iterator_tag(); }
template <class _Tp> inline int* __DISTANCE_TYPE(const _Tp*) { return (int*)0; }

template <class _Tp, class _Alloc> class _Deque_base {
public:
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator _M_start;                                      // +0x00
    iterator _M_finish;                                     // +0x10
    _STLP_alloc_proxy<_Tp**, _Tp*, allocator<_Tp*> > _M_map;                  // +0x20
    _STLP_alloc_proxy<unsigned int, _Tp, allocator<_Tp> > _M_map_size;        // +0x24
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class deque : protected _Deque_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    typedef typename _Deque_base<_Tp, _Alloc>::iterator iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    void push_back(const value_type& __t);
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    typedef const _Tp* const_iterator;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    const_iterator begin() const { return this->_M_start; }
    const_iterator end() const { return this->_M_finish; }
    void push_back(const _Tp& __x);
};

template <class _Container> class back_insert_iterator {
protected:
    _Container* container;
public:
    explicit back_insert_iterator(_Container& __x) : container(&__x) { }
    back_insert_iterator<_Container>& operator=(const typename _Container::value_type& __value)
    {
        container->push_back(__value);
        return *this;
    }
    back_insert_iterator<_Container>& operator*() { return *this; }
    back_insert_iterator<_Container>& operator++() { return *this; }
};

template <class _Container> inline back_insert_iterator<_Container> back_inserter(_Container& __x)
{
    return back_insert_iterator<_Container>(__x);
}

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                              _OutputIter __result, const random_access_iterator_tag&, _Distance*);   // 0x007B5030

// Same-TU body of __copy_aux (0x007B51B0, out of line in retail): it never reads its tag, so VC7.1 homes this
// function's __false_type temporary in the dead inserter slot.
#pragma auto_inline(off)
template <class _InputIter, class _OutputIter>
__declspec(noinline) _OutputIter __fastcall __copy_aux(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, __ITERATOR_CATEGORY(__first), __DISTANCE_TYPE(__first));
}
#pragma auto_inline(on)

template <class _InputIter, class _OutputIter>
_OutputIter __fastcall copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    return __copy_aux(__first, __last, __result, __false_type());
}

}

template std::back_insert_iterator<std::vector<CCrimeWaitingForBribe> > __fastcall
std::copy(std::deque<CCrimeWaitingForBribe>::iterator, std::deque<CCrimeWaitingForBribe>::iterator,
          std::back_insert_iterator<std::vector<CCrimeWaitingForBribe> >);