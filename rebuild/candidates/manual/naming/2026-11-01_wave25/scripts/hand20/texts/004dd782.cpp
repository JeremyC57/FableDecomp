#pragma optimize("s", on)
// std::__destroy_aux<std::pair<long, CCharString>*>(first, last, const std::__false_type&) -- STLport 4.6 _construct.h: destroy every element of
//   [first, last) through its (non-inlined) implicit destructor, the pair's scalar deleting destructor 0x004DA68A called with flag 0
// Leaf: ??$__destroy_aux@PAU?$pair@JVCCharString@@@std@@@std@@YIXPAU?$pair@JVCCharString@@@0@0ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DD782, 29 bytes, __fastcall (first in ecx, last in edx, the tag pushed), `ret 4`.
// Identity: called by ~vector<pair<long, CCharString> > 0x004E34B2 (_Destroy(start, finish) -> __destroy_aux(..., __false_type())); the
//   8-byte stride is pair<long, CCharString>; the callee 0x004DA68A is that pair's ??_G (catalog, landed). The TU is an optimize("s") TU: the
//   implicit ~pair is not inlined, VC7.1 then calls the scalar deleting destructor with flag 0.
// Naming wave 19 (wave 18 recheck-fail re-run): landed as the free __fastcall Destroy_range_CFlash(CFlash*, CFlash*, allocator_flash*) stand-in (NCreatureHitNotification::CFlash
//   label); re-authored on the STLport text (name-only: the bytes are unchanged).
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                             // 0x0099EAE0
};
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
}
template void __fastcall std::__destroy_aux<std::pair<long, CCharString>*>(std::pair<long, CCharString>*, std::pair<long, CCharString>*, const std::__false_type&);
