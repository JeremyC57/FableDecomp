// std::basic_string<unsigned short>::resize(size_type n, unsigned short c) -- STLport wide-string resize: truncate through erase (moving the terminator down) or append n - size() copies of c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099CD60, length 95 bytes (96 with int3 padding).
// Leaf: resize
// Callees: basic_string<unsigned short>::append(size_type n, unsigned short c) @ 0x0099CB00 (thiscall, `ret 8`),
// memmove @ 0x00BFEAE6 (cdecl, `add esp,0xc`).
// Label correction: the manifest's "CHistogram<long>::SetSize" is propagated. The element is 2 bytes (`sar 1`,
// `add eax,eax`), the erase moves (finish - last) + 1 elements -- the STLport string erase whose move "includes
// the terminating _CharT()" -- and the grow path 0x0099CB00 checks max_size 0x7FFFFFFE, reserves through
// 0x0099C990 and stores the 16-bit terminator (`mov word ptr [eax+ecx],0`): this is the STLport 4.x
// basic_string<unsigned short> (Fable's wide string) with _String_base { _M_start, _M_finish,
// _M_end_of_storage }; the caller 0x0099BCAF resizes a string to a count with c = 0.
// Shape: STLport `resize` (`n <= size()` -> erase(begin() + n, end()), else append(n - size(), c)) with the
// inline erase (`first != last` guard; char_traits move = `n == 0 ? s1 : memmove(s1, s2, n * 2)`, giving the
// `inc ; je` count test; new finish = finish - (last - first); the destroy of trivial elements vanishes).
namespace std {

typedef unsigned int size_t;

extern "C" void* __cdecl memmove(void* dst, const void* src, size_t n);   // 0x00BFEAE6

template<class _CharT> struct char_traits {
    typedef _CharT char_type;
    static char_type* move(char_type* s1, const char_type* s2, size_t n)
    {
        return n == 0 ? s1 : (char_type*)memmove(s1, s2, n * sizeof(char_type));
    }
};

template<class _Tp> struct allocator { };

template<class _Tp> inline void _Destroy(_Tp*, _Tp*) { }

template<class _CharT, class _Traits = char_traits<_CharT>, class _Alloc = allocator<_CharT> >
class basic_string {
public:
    typedef _CharT        value_type;
    typedef _Traits       traits_type;
    typedef _CharT*       pointer;
    typedef _CharT*       iterator;
    typedef size_t        size_type;
    typedef int           difference_type;

    pointer _M_start;            // +0x00
    pointer _M_finish;           // +0x04
    pointer _M_end_of_storage;   // +0x08

    iterator  begin() { return _M_start; }
    iterator  end()   { return _M_finish; }
    size_type size() const { return _M_finish - _M_start; }

    basic_string& append(size_type n, _CharT c);   // 0x0099CB00

    iterator erase(iterator first, iterator last)
    {
        if (first != last) {
            // The move includes the terminating _CharT().
            traits_type::move(first, last, (_M_finish - last) + 1);
            pointer new_finish = _M_finish - (last - first);
            _Destroy(new_finish + 1, _M_finish + 1);
            _M_finish = new_finish;
        }
        return first;
    }

    void resize(size_type n, _CharT c)
    {
        if (n <= size())
            erase(begin() + n, end());
        else
            append(n - size(), c);
    }
};

template void basic_string<unsigned short>::resize(size_type, unsigned short);

}