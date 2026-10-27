// N00f27bc0::vector<CHeroSoulsWaveDef>::_M_allocate_and_copy(unsigned int n, CHeroSoulsWaveDef* first, CHeroSoulsWaveDef* last) -- allocate n elements and copy-construct [first, last) into them
// Leaf: ?_M_allocate_and_copy@?$vector@UCHeroSoulsWaveDef@N00f27bc0@@@N00f27bc0@@QAEPAUCHeroSoulsWaveDef@2@IPAU32@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00f27d50, 74 bytes, thiscall (`this` unused), `ret 0Ch`.
// Identity (proven): bound ?_M_allocate_and_copy@?$vector@UCHeroSoulsWaveDef@N00f27bc0@@@N00f27bc0@@... by its caller, the landed N00f27bc0::vector<CHeroSoulsWaveDef>::operator= 0x00F27BC0 (whose model this follows, namespace and all); _Construct 0x00F26F00 is bound ?_Construct@N00f27bc0@@YIXPAUCHeroSoulsWaveDef@1@PBU21@@Z.
// Callees: malloc 0x00BFEA0E (n * 0x38; 0 when n is 0: the malloc_alloc allocator), _Construct 0x00F26F00 (/Gr __fastcall:
//   destination ecx, source edx; out of line).
// Shape (family L142 = the landed N00f27bc0::vector<CHeroSoulsWaveDef> model of 0x00F27BC0): the copy loop is __uninitialized_copy
//   with the out-of-line _Construct (a same-TU noinline body); the output pointer is strength-reduced to source + delta.
#include <new>
extern "C" void* __cdecl malloc(unsigned int);
namespace N00f27bc0 {
struct CHeroSoulsWaveDef {                         // retail 0x38 bytes (def vtable 0x012FB498 over CDefClassBase)
    unsigned char _body[0x38];
    CHeroSoulsWaveDef(const CHeroSoulsWaveDef&);
};
#pragma auto_inline(off)
__declspec(noinline) void __fastcall _Construct(CHeroSoulsWaveDef* out, const CHeroSoulsWaveDef* input) { if (out) new (out) CHeroSoulsWaveDef(*input); }   // 0x00F26F00
#pragma auto_inline(on)
struct __false_type { };
template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(__cur, __first);
    return __cur;
}
template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* _M_allocate_and_copy(unsigned int, T*, T*);
};
template <class T> T* vector<T>::_M_allocate_and_copy(unsigned int n, T* first, T* last)
{
    T* allocation = n ? (T*)malloc(n * sizeof(T)) : 0;
    __uninitialized_copy(first, last, allocation, __false_type());
    return allocation;
}
template CHeroSoulsWaveDef* vector<CHeroSoulsWaveDef>::_M_allocate_and_copy(unsigned int, CHeroSoulsWaveDef*, CHeroSoulsWaveDef*);
}