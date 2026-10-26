// std::_String_base<char,std::allocator<char> >::_M_throw_out_of_range() const -- STLport 4.6 with exceptions off: `__stl_throw_out_of_range("basic_string")` is puts("basic_string") then abort()
// Leaf: ?_M_throw_out_of_range@?$_String_base@DV?$allocator@D@std@@@std@@QBEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00565360, 18 bytes, thiscall (`this` unused):
//   push offset "basic_string" ; call puts ; add esp,4 ; jmp abort (`abort` is not declared noreturn, so the call is a
//   tail jump).
// Identity: the three byte-identical 18-byte bodies 0x00565360 / 0x00565A30 / 0x0099C550 were landed as a descriptive
//   `BasicStringRaise` (catalogued "OnInitialActivate", a propagated label). Retail has no ICF and identical COMDAT
//   bodies of ONE symbol fold at link, so three copies are three symbols: the STLport `_String_base<_Tp,_Alloc>`
//   members `_M_throw_length_error` / `_M_throw_out_of_range` (both `__stl_throw_*("basic_string")`) for char and the
//   CWideString `unsigned short` instance. This one is reached from the out-of-range check of substr (0x00569340, landed CNetStringRetail::Substr binds `?_M_throw_out_of_range@?$_String_base@DV?$allocator@D@std@@@std@@QBEXXZ` here) and 0x00560B6A.
// Callers: 0x00560B6A, 0x00569340 (2).
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

template <class _Tp, class _Alloc> void _String_base<_Tp, _Alloc>::_M_throw_out_of_range() const
{
    puts("basic_string");
    abort();
}

template void _String_base<char, allocator<char> >::_M_throw_out_of_range() const;

}
