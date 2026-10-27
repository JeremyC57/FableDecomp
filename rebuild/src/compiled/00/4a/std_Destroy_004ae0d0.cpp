// std::_Destroy(pair<unsigned long, CCountedPointer<CFaction> >* first, pair<unsigned long, CCountedPointer<CFaction> >* last) -- STLport: destroy the range through the out-of-line __destroy_aux with the __false_type tag
// Leaf: ??$_Destroy@PAU?$pair@KV?$CCountedPointer@VCFaction@@@@@std@@@std@@YIXPAU?$pair@KV?$CCountedPointer@VCFaction@@@@@0@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AE0D0, 13 bytes, __fastcall (first ecx, last edx passed straight on), plain
//   `ret`: push ecx ; lea eax,[esp+3] ; push eax ; call 0x004AD9E0 ; pop ecx ; ret.
// Identity: the callee 0x004AD9E0 is bound as std::__destroy_aux<pair<unsigned long, CCountedPointer<CFaction> >*>
//   (reloc_bindings, PDB-form name), and STLport 4.6 `_Destroy(first, last)` -> `__destroy(first, last, value_type)` ->
//   `__destroy_aux(first, last, _Trivial_destructor())` is the only two-argument wrapper that forwards the range with a
//   tag. Probable: the row has no direct caller (gap-scan COMDAT; the xref.py hit 0x004F0A0C decodes to no instruction).
// Shape: the empty tag temporary sits in the `push ecx` frame dword (it needs a user constructor, else VC7.1 zero-stores
//   its byte); ecx/edx are not touched before the call.
namespace std {
struct __true_type { };
struct __false_type { __false_type() { } };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x004AD9E0
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }

template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}

template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }

template <class _ForwardIterator>
void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, __value_type(__first));
}
} // namespace std

class CCPPointerInfo;
template <class T> class CCountedPointer {      // 8 bytes: Data, Info
public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer();
};

class CFaction;

template void __fastcall std::_Destroy<std::pair<unsigned long, CCountedPointer<CFaction> >*>(
    std::pair<unsigned long, CCountedPointer<CFaction> >*, std::pair<unsigned long, CCountedPointer<CFaction> >*);