// std::fill(bool* first, bool* last, const bool& val) -- STLport generic fill over a byte-wise vector<bool> range, the value re-read every pass
// Leaf: ??$fill@PA_N_N@std@@
//
// Retail 0x00685430, 29 bytes (+ int3 padding to 32), `ret 4` (/Gr: first in ecx, last in edx, &val on the stack).
// Callers: 0x00681F04 (CTCCreatureOpinionOfHero's personality-override table build 0x00681E60: fill of AttitudeMask, retail
//   +0x24..+0x28) and 0x0068225C in CTCCreatureOpinionOfHero::SetOpinionDeedMask(long) 0x006821E0 (fill of DeedMask,
//   retail +0x3C) -- Ego_r.pdb types both members vector<bool>, and the retail vector stores one byte per flag.
// Shape: STLport _algobase.h `for (; first != last; ++first) *first = val;` -- STLport only routes the char types to
//   memset, so a bool range keeps the byte loop (`mov al,[esi] ; mov [ecx],al` with the value re-read because the
//   destination may alias it).  /Gr TU written as explicit __fastcall.
// Label: catalog "fill<char*,char>" (the char overloads would be memset in STLport); the element is bool.
namespace std {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}
}

template void __fastcall std::fill<bool*, bool>(bool*, bool*, const bool&);