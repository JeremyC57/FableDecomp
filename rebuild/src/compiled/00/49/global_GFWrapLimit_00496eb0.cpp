// GFWrapLimit<long, int, int>(long& value, const int& minimum, const int& maximum) -- wrap value to maximum below minimum and to
// minimum above maximum
// Leaf: ??$GFWrapLimit@JHH@@YIXAAJABH1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00496EB0 (__fastcall: value in ecx, minimum in edx, maximum on the stack).
// Name (NF197, 2026-11-20): Ego_r.pdb 0x0049FD80 + FableWin.pdb 0x01A46C40 ??$GFWrapLimit@JHH@@YIXAAJABH1@Z (the header template
//   instance; references, not the old pointer stand-in GFWrapLimit_00496eb0); CFrontendGameComponent::Input 0x004963A0 calls it with
//   ecx = &CursorHighlight, edx = &1, push &7 and binds this spelling. The template text is 0x004963A0's (no shared header carries
//   GFWrapLimit): its noinline pick-any copy there is byte-identical to this explicit instantiation.
template <class T, class TMin, class TMax> __declspec(noinline) void __fastcall GFWrapLimit(T& value, const TMin& minimum, const TMax& maximum)
{
    if (value < minimum)
        value = maximum;
    else if (value > maximum)
        value = minimum;
}

template void __fastcall GFWrapLimit<long, int, int>(long& value, const int& minimum, const int& maximum);
