// _STL::__copy(const unsigned char* first, const unsigned char* last, back_insert_iterator<vector<unsigned char>> result, const random_access_iterator_tag&, int*) -- STLport random-access copy of a byte range through a back inserter; returns the advanced inserter
// Leaf: ??$__copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E9900, length 93 bytes (`ret 10h`: last, the inserter, the tag, the Distance*).
// Label correction: the catalog's "Vector_AppendRange" is STLport `__copy<const unsigned char*, back_insert_iterator<vector<
//   unsigned char> >, int>` (callers: the CMemoryFile buffer copies at 0x009E97A2 / 0x009E9976 / 0x009E99A6 / 0x009E9A22 and
//   0x00A5B6FE).
// Callees: vector<unsigned char>::_M_insert_overflow @ 0x00485B6C (landed).
// Convention: /Gr algorithm TU -- `__fastcall` returning the iterator class through the hidden pointer in ecx, `first` in edx.
// Shape: STLport 4.x verbatim: `for (_Distance n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }
//   return result;` -- the inserter's assignment is the inline push_back (null-tested byte store + `inc [finish]`, else the
//   overflow with the __true_type tag homed in the inserter's dead stack slot, the container held in edi); the counted loop
//   runs `dec ebx ; jne` and the result word is stored once at each exit.
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

template back_insert_iterator<vector<unsigned char> > __fastcall
__copy(const unsigned char*, const unsigned char*, back_insert_iterator<vector<unsigned char> >, const random_access_iterator_tag&, int*);
}