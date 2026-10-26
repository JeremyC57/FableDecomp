// std::_String_base<unsigned short,std::allocator<unsigned short> >::_M_throw_length_error() const -- STLport 4.6 with exceptions off: `__stl_throw_length_error("basic_string")` is puts("basic_string") then abort()
// Leaf: ?_M_throw_length_error@?$_String_base@GV?$allocator@G@std@@@std@@QBEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099C550, 18 bytes, thiscall (`this` unused):
//   push offset "basic_string" ; call puts ; add esp,4 ; jmp abort (`abort` is not declared noreturn, so the call is a
//   tail jump).
// Identity: the three byte-identical 18-byte bodies 0x00565360 / 0x00565A30 / 0x0099C550 were landed as a descriptive
//   `BasicStringRaise` (catalogued "OnInitialActivate", a propagated label). Retail has no ICF and identical COMDAT
//   bodies of ONE symbol fold at link, so three copies are three symbols: the STLport `_String_base<_Tp,_Alloc>`
//   members `_M_throw_length_error` / `_M_throw_out_of_range` (both `__stl_throw_*("basic_string")`) for char and the
//   CWideString `unsigned short` instance. This one is reached from the wide (`basic_string<unsigned short>`, CWideString) `_M_allocate_block` sites; the landed wide rows bind `?_M_throw_length_error@?$_String_base@GV?$allocator@G@std@@@std@@QBEXXZ` here.
// Callers: 0x0099BA36, 0x0099C650, 0x0099C6CC, 0x0099C77C, 0x0099C90C, 0x0099C9A2, 0x0099CAE0, 0x0099CB8B, 0x0099CD07 (9).
extern "C" int  __cdecl puts(const char* text);     // 0x00BFEAF2
extern "C" void __cdecl abort();                     // 0x00BFEAEC

namespace std {

template <class _Tp> class allocator { };

template <class _Tp, class _Alloc> class _String_base {
public:
    _Tp* _M_start;              // +0x00
    _Tp* _M_finish;             // +0x04
    _Tp* _M_end_of_storage;     // +0x08 (the allocator proxy's pointer)
    void _M_throw_length_error() const;
    void _M_throw_out_of_range() const;
};

template <class _Tp, class _Alloc> void _String_base<_Tp, _Alloc>::_M_throw_length_error() const
{
    puts("basic_string");
    abort();
}

template void _String_base<unsigned short, allocator<unsigned short> >::_M_throw_length_error() const;

}
