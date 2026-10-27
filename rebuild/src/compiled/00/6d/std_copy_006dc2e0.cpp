// std::__copy(const long* first, const long* last, back_insert_iterator<vector<long> > result, const random_access_iterator_tag&, int*) -- push_back every long of the range onto the iterator's vector
// Leaf: ??$__copy@PBJV?$back_insert_iterator@V?$vector@JV?$allocator@J@std@@@std@@@std@@H@std@@YI?AV?$back_insert_iterator@V?$vector@JV?$allocator@J@std@@@std@@@0@PBJ0V10@ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe @ 0x006DC2E0, 104 bytes, __fastcall (ecx = the hidden back_insert_iterator result, edx = first),
//   `ret 10h` (last, the iterator's vector pointer, the tag reference, the null distance pointer).
// Identity (probable): the STLport 4.6 random-access `__copy` loop (`for (n = last - first; n > 0; --n) *result =
//   *first, ++first, ++result`) with the inline back_insert_iterator `operator=` -> vector::push_back (placement copy
//   when finish != end_of_storage, else `_M_insert_overflow(finish, x, __true_type(), 1, true)`); the overflow callee
//   0x00440990 is the landed std::vector<long>::_M_insert_overflow, so the element is long. FableWin.pdb has the matching
//   `copy<vector<long>::const_iterator, back_insert_iterator<vector<long> > >`. Callers: four copy() forwarders
//   (0x006D8E01 / 0x006D8E19 in the CTCHeroExperience method at 0x006D8D90, 0x006DC8B6, 0x006DC906).
inline void* operator new(unsigned int, void* where) { return where; }

namespace std {
struct __true_type { __true_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };

template <class _Tp> class allocator { };

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp value_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len,
                            bool __atend);                                  // 0x00440990
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            new (this->_M_finish) _Tp(__x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
};

template <class _Container> class back_insert_iterator {
protected:
    _Container* container;
public:
    typedef _Container container_type;
    explicit back_insert_iterator(_Container& __x) : container(&__x) {}
    back_insert_iterator<_Container>& operator=(const typename _Container::value_type& __value);
    back_insert_iterator<_Container>& operator*() { return *this; }
    back_insert_iterator<_Container>& operator++() { return *this; }
    back_insert_iterator<_Container>& operator++(int) { return *this; }
};
template <> inline back_insert_iterator<vector<long> >&
back_insert_iterator<vector<long> >::operator=(const long& __value)
{
    container->push_back(__value);
    return *this;
}

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template back_insert_iterator<vector<long> > __fastcall __copy(const long*, const long*, back_insert_iterator<vector<long> >,
                                                               const random_access_iterator_tag&, int*);
}