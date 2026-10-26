// std::basic_string<char>::append<const char*>(const char* first, const char* last, const forward_iterator_tag&) -- STLport range append: grow into a fresh size+max(size, n)+1 block when the range does not fit, else copy it after the old end and move the terminator
// Leaf: ??$append@PBD@?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV01@PBD0ABUforward_iterator_tag@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00567D60, 281 bytes (the int3 VC7.1 emits after the final noreturn call included), `ret 0Ch` (returns *this).
// Identity: the STLport 4.6 member template `basic_string<char>::append(_ForwardIter, _ForwardIter, const
//   forward_iterator_tag&)` for `const char*` (landed before as the descriptive CNetString_Retail::Append; the
//   catalog/manifest "CTCElectrocutedEffect::ReplaceString" was propagated). The byte-identical `<char*>` instance is
//   0x00567400. `const char*` ranges reach here: `append(const char*)` / `operator+=(const char*)` (the HTTP header code
//   0x0083FC4B / 0x0083FC63), operator+(string, const char*)'s literal half 0x00841BF2, and the non-template STLport
//   `assign(const char*, const char*)` 0x005693B0's `append(__f + size(), __l)`. Landed 0x00841B70 binds
//   `??$append@PBD@...` here.
// Callees: malloc 0x00BFEA0E (allocator<char>::allocate, null for 0), memmove 0x00BFEAE6 (__ucopy_trivial),
//   free 0x00BFEA14 (_M_deallocate_block), _String_base<char>::_M_throw_length_error() const 0x00565A30 (the exceptions-off
//   puts("basic_string"); abort(), called through `this`; never returns).
// Layout: STLport _String_base<char> {_M_start, _M_finish, _M_end_of_storage} (12 bytes).
// Shape: the STLport 4.6 source as written: size()/max_size()/capacity() inline, `(max)(old, n)` through references
//   (the `lea ecx,[esp+..]` pick between the spilled size and the reused argument slot), __ucopy_trivial's
//   `last == first ? result : memmove(..) + n` for both copies (the second test folds away against the outer
//   `first != last`), _M_construct_null as a byte store; the length error is `__declspec(noreturn)`.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void  __cdecl free(void* p);                                          // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6

namespace std {

struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };

template <class _Iter> inline int distance(const _Iter& __first, const _Iter& __last) { return __last - __first; }

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

__forceinline void* __ucopy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

template <class _CharT> class char_traits { };

template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _String_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
    void _M_deallocate_block() { if (_M_start) free(_M_start); }
    __declspec(noreturn) void _M_throw_length_error() const;                     // 0x00565A30
};

template <class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
class basic_string : public _String_base<_CharT, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef int difference_type;
    typedef _CharT* pointer;

    size_type size() const { return this->_M_finish - this->_M_start; }
    size_type max_size() const { return size_type(-1) / sizeof(_CharT) - 1; }
    size_type capacity() const { return (this->_M_end_of_storage._M_data - this->_M_start) - 1; }
    void _M_construct_null(_CharT* __p) { *__p = 0; }

    template <class _ForwardIter>
    basic_string& append(_ForwardIter __first, _ForwardIter __last, const forward_iterator_tag&)
    {
        if (__first != __last) {
            const size_type __old_size = size();
            difference_type __n = distance(__first, __last);
            if ((size_type)__n > max_size() || __old_size > max_size() - (size_type)__n)
                this->_M_throw_length_error();
            if (__old_size + __n > capacity()) {
                const size_type __len = __old_size + (max)(__old_size, (size_type)__n) + 1;
                pointer __new_start = this->_M_end_of_storage.allocate(__len);
                pointer __new_finish = __new_start;
                __new_finish = (pointer)__ucopy_trivial(this->_M_start, this->_M_finish, __new_start);
                __new_finish = (pointer)__ucopy_trivial(__first, __last, __new_finish);
                _M_construct_null(__new_finish);
                this->_M_deallocate_block();
                this->_M_start = __new_start;
                this->_M_finish = __new_finish;
                this->_M_end_of_storage._M_data = __new_start + __len;
            }
            else {
                _ForwardIter __f1 = __first;
                ++__f1;
                __ucopy_trivial(__f1, __last, this->_M_finish + 1);
                _M_construct_null(this->_M_finish + __n);
                *this->_M_finish = *__first;
                this->_M_finish += __n;
            }
        }
        return *this;
    }
};

template basic_string<char>& basic_string<char>::append<const char*>(const char*, const char*, const forward_iterator_tag&);

}
