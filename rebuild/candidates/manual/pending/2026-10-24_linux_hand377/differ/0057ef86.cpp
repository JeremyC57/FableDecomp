// GFLimitReturn<float>(const float& value, const float& lo, const float& hi) -- clamp by reference: lo when value < lo, hi when value > hi, else value
// Leaf: ??$GFLimitReturn@M@@YIABMABM00@Z
// Grade: DIFFER(37v37) -- two spots: retail loads `hi` (`mov edx,[esp+4]`) AFTER the second `fld [ecx]`, ours before it; and retail's `> hi` arm is `jne L ; mov ecx,edx ; L: mov eax,ecx` (the value pointer kept in ecx, then copied) where ours is `mov eax,edx ; je exit ; mov eax,ecx`. Tried: speed TU (38, separate `ret 4`), size TU if/if/return (37), `return value > hi ? hi : value` (37, same diff), a `const T* result` local (35).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057EF86, 37 bytes, __fastcall (value ecx, lo edx, hi on the stack) `ret 4`,
//   returns the winning reference in eax. FableWin.pdb / Ego_r.pdb `??$GFLimitReturn@M@@YIABMABM00@Z`; called by the
//   landed CTCHeroStats::SetFatness 0x0057B169 and its SetAge/SetSunTan twins, 0x006D25FB, 0x006E3A6F, 0x00700A80,
//   0x0071BB9F, 0x0071BBDE. Label correction: the catalog "GFLimitReturn<double>" has the wrong type (`fld dword`).
// Shape: `if (value < lo) return lo; if (value > hi) return hi; return value;` -- `<` as `test ah,5 ; jp`, `>` as
//   `test ah,41h ; jne`, value re-loaded for the second compare; the single shared `ret 4` exit is the size TU.
#pragma optimize("s", on)

template <class T> const T& __fastcall GFLimitReturn(const T& value, const T& lo, const T& hi)
{
    if (value < lo)
        return lo;
    if (value > hi)
        return hi;
    return value;
}

template const float& __fastcall GFLimitReturn<float>(const float&, const float&, const float&);
