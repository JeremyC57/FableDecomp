// std::_Destroy(CEngineLocalDetailObjectDef* first, CEngineLocalDetailObjectDef* last) -- STLport range destroy: run the virtual destructor of every 0x88-byte detail-object def in [first, last)
// Leaf: ??$_Destroy@PAVCEngineLocalDetailObjectDef@@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004774B9, 29 bytes, `__fastcall` (first ecx, last edx), plain `ret`;
//   size-optimised def TU (unaligned entry) -- `#pragma optimize("s", on)`.
// Identity: the landed vector<CEngineLocalDetailObjectDef>::erase 0x00477E36 binds this address as
//   ??$_Destroy@PAVCEngineLocalDetailObjectDef@@@std@@YIXPAVCEngineLocalDetailObjectDef@@0@Z; the other callers are the
//   vector's operator= 0x00477AB7, _M_clear 0x00477BFC and the owner's destructor 0x0047747C.  Stride 0x88 = the retail
//   CEngineLocalDetailObjectDef (vtable 0x0123634C, class in RTTI; Ego_r 0x8C with a 16-byte vector).  The landed name
//   "DestroyRange_Elem0x88" / module "vector<CIntelligentPointer<CThing const>>::_Destroy" was propagated.
// Shape: STLport 4.6 `_Destroy(first, last)` -> `__destroy(first, last, value_type)` -> `__destroy_aux(.., __false_type)`
//   (no __type_traits specialisation for the class) -> `_Destroy(&*first)` = `p->~T()`: the virtual destructor through
//   vtable slot 0 with the 0 (no delete) flag; the loop tests first != last before the first call. The size-optimised TU
//   keeps the inline helpers out of line unless they are __forceinline.
#pragma optimize("s", on)

class CEngineLocalDetailObjectDef {                         // retail 0x88 bytes, polymorphic
public:
    virtual ~CEngineLocalDetailObjectDef();
    unsigned char _body[0x84];
};

namespace std {

struct __false_type { __false_type() { } };
template <class _Tp> struct __type_traits {
    typedef __false_type has_trivial_destructor;
};
template <class _Tp> __forceinline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

template <class _Tp>
__forceinline void _Destroy(_Tp* __pointer)
{
    __pointer->~_Tp();
}

template <class _ForwardIterator>
__forceinline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIterator, class _Tp>
__forceinline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}

template <class _ForwardIterator>
void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, __value_type(__first));
}

template void __fastcall _Destroy<CEngineLocalDetailObjectDef*>(CEngineLocalDetailObjectDef*, CEngineLocalDetailObjectDef*);

} // namespace std
