// _STL::back_insert_iterator<vector<unsigned char>>::operator=(const unsigned char& value) -- STLport back inserter assignment: push the byte onto the container and return the iterator
// Leaf: ??4?$back_insert_iterator@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E98C0, length 59 bytes (`ret 4`, returns `this`).
// Label correction: the catalog's "Vector_PushBackByte" is STLport `back_insert_iterator<vector<unsigned char> >::operator=`
//   (`this` holds the container pointer at +0; the byte vector is the 12-byte STLport vector) -- the same push_back the
//   STLport `__copy` 0x009E9900 inlines for the CMemoryFile byte buffers. No direct callers (an out-of-line COMDAT copy).
// Callees: vector<unsigned char>::_M_insert_overflow @ 0x00485B6C (landed; POD grow with the __true_type tag).
// Shape: STLport 4.x `container->push_back(value); return *this;` with push_back inline: the null-tested placement byte
//   store and `inc dword ptr [ecx+4]`, else `_M_insert_overflow(finish, x, __true_type(), 1, true)` with the empty tag
//   homed in the `value` reference's own slot (re-read for the push right after).
inline void* operator new(unsigned int, void* p) { return p; }

namespace _STL {
struct __true_type { __true_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp>
class vector {
public:
    typedef _Tp value_type;
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;

    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len, bool __atend);   // 0x00485B6C
    void push_back(const _Tp& __x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, __x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, __x, __true_type(), 1UL, true);
    }
};

template <class _Container>
class back_insert_iterator {
public:
    _Container* container;
    explicit back_insert_iterator(_Container& __x) : container(&__x) {}
    back_insert_iterator<_Container>& operator=(const typename _Container::value_type& __value)
    {
        container->push_back(__value);
        return *this;
    }
    back_insert_iterator<_Container>& operator*() { return *this; }
    back_insert_iterator<_Container>& operator++() { return *this; }
    back_insert_iterator<_Container>& operator++(int) { return *this; }
};

template class back_insert_iterator<vector<unsigned char> >;
}