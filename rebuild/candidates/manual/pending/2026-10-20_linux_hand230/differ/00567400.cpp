// std::basic_string<char>::append(const char* first, const char* last, const forward_iterator_tag&) -- STLport range append: grow into a fresh max(size, n)+size+1 block when the range does not fit, else copy it after the old end and move the terminator
// Grade: DIFFER(281v280) -- byte-identical except the int3 VC7.1 emits after the final noreturn _M_throw_length_error call (q.py trims retail padding, not the build; same grader gap as 00a5b260: land by hand)
// Leaf: append
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00567400, 280 bytes (+ int3 padding), `ret 0Ch`. Label correction: the
//   catalog row "CTCElectrocutedEffect::InsertStringRange" is propagated; the body is the STLport
//   `basic_string<char>::append(_ForwardIter, _ForwardIter, const forward_iterator_tag&)` instantiation for
//   `const char*` (16 callers in two string-building TUs). A second copy of the same body sits at 0x00567D60.
// Callees: malloc 0x00BFEA0E (allocator<char>::allocate, null for 0), memmove 0x00BFEAE6 (__ucopy_trivial),
//   free 0x00BFEA14 (_M_deallocate_block), basic_string::_M_throw_length_error 0x00565A30 (the exceptions-off
//   puts("basic_string"); abort() trampoline, called through `this`; never returns).
// Layout: STLport _String_base<char> {_M_start, _M_finish, _M_end_of_storage} (12 bytes).
// Shape: the STLport 4.6 source as written: size()/max_size()/capacity() inline, `(max)(old, n)` through references
//   (the `lea ecx,[esp+..]` pick between the spilled size and the reused argument slot), __ucopy_trivial's
//   `last == first ? result : memmove(..) + n` for both copies (the second test folds away against the outer
//   `first != last`), _M_construct_null as a byte store; the length error is `__declspec(noreturn)`.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void  __cdecl free(void* p);                                          // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6

namespace std {

struct forward_iterator_tag { };

template <class _Iter> inline int distance(const _Iter& __first, const _Iter& __last) { return __last - __first; }

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

__forceinline void* __ucopy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

class basic_string {
public:
    typedef unsigned int size_type;
    typedef int difference_type;
    char* _M_start;
    char* _M_finish;
    char* _M_end_of_storage;

    size_type size() const { return _M_finish - _M_start; }
    size_type max_size() const { return size_type(-1) - 1; }
    size_type capacity() const { return (_M_end_of_storage - _M_start) - 1; }
    char* allocate(size_type __n) { return __n != 0 ? (char*)malloc(__n) : 0; }
    void _M_deallocate_block() { if (_M_start) free(_M_start); }
    void _M_construct_null(char* __p) { *__p = 0; }
    __declspec(noreturn) void _M_throw_length_error() const;                     // 0x00565A30

    template <class _ForwardIter>
    basic_string& append(_ForwardIter __first, _ForwardIter __last, const forward_iterator_tag&)
    {
        if (__first != __last) {
            const size_type __old_size = size();
            difference_type __n = distance(__first, __last);
            if ((size_type)__n > max_size() || __old_size > max_size() - (size_type)__n)
                _M_throw_length_error();
            if (__old_size + __n > capacity()) {
                const size_type __len = __old_size + (max)(__old_size, (size_type)__n) + 1;
                char* __new_start = allocate(__len);
                char* __new_finish = __new_start;
                __new_finish = (char*)__ucopy_trivial(_M_start, _M_finish, __new_start);
                __new_finish = (char*)__ucopy_trivial(__first, __last, __new_finish);
                _M_construct_null(__new_finish);
                _M_deallocate_block();
                _M_start = __new_start;
                _M_finish = __new_finish;
                _M_end_of_storage = __new_start + __len;
            }
            else {
                _ForwardIter __f1 = __first;
                ++__f1;
                __ucopy_trivial(__f1, __last, _M_finish + 1);
                _M_construct_null(_M_finish + __n);
                *_M_finish = *__first;
                _M_finish += __n;
            }
        }
        return *this;
    }
};

template basic_string& basic_string::append<const char*>(const char*, const char*, const forward_iterator_tag&);

}
