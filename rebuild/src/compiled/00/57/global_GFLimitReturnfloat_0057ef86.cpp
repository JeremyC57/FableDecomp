// GFLimitReturn<float>(const float& value, const float& lo, const float& hi) -- clamp by reference: lo when value < lo, hi when value > hi, else value
// Leaf: ??$GFLimitReturn@M@@YIABMABM00@Z
// Grade: MATCH (near-miss retry agent901) -- the one-expression nested ternary (the landed <double> twin 0x0041919C form)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0057EF86, 37 bytes, __fastcall (value ecx, lo edx, hi on the stack) `ret 4`,
//   returns the winning reference in eax. FableWin.pdb / Ego_r.pdb `??$GFLimitReturn@M@@YIABMABM00@Z`; called by the
//   landed CTCHeroStats::SetFatness 0x0057B169 and its SetAge/SetSunTan twins, 0x006D25FB, 0x006E3A6F, 0x00700A80,
//   0x0071BB9F, 0x0071BBDE. Label correction: the catalog "GFLimitReturn<double>" has the wrong type (`fld dword`).
// Shape: `if (value < lo) return lo; if (value > hi) return hi; return value;` -- `<` as `test ah,5 ; jp`, `>` as
//   `test ah,41h ; jne`, value re-loaded for the second compare; the single shared `ret 4` exit is the size TU.
// Fix (agent901): the one-expression nested ternary of the landed <double> twin 0x0041919C (same bytes but for the
//   operand width) -- the if/return form schedules the `hi` load early and keeps a different > arm.
#pragma optimize("s", on)

template <class T> const T& __fastcall GFLimitReturn(const T& value, const T& lo, const T& hi)
{
    return value < lo ? lo : value > hi ? hi : value;
}

template const float& __fastcall GFLimitReturn<float>(const float&, const float&, const float&);