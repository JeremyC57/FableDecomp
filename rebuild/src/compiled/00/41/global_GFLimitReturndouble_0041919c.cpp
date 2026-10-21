// GFLimitReturn<double>(const double&, const double&, const double&) -- clamp by reference: the lower bound, the upper bound or the value itself
// Leaf: GFLimitReturn
// Retail: Fable.exe @ 0x0041919C, 37 bytes, __fastcall (value in ecx, lower in edx, upper on the stack, `ret 4`),
// returns a reference in eax. Two x87 compares (`test ah,5` for value < lower, `test ah,41h` for value <= upper).
// Callers 0x004170E7/0x004171BB (CMainGameComponent).
// Opt: size TU (unaligned entry). Shape: one nested `?:` return; the if-chain is the same length but orders the
// upper-bound arm differently, and /O2 is a byte longer.
#pragma optimize("s", on)
template <class T> const T& __fastcall GFLimitReturn(const T& value, const T& lower, const T& upper)
{
    return value < lower ? lower : value > upper ? upper : value;
}
template const double& __fastcall GFLimitReturn<double>(const double&, const double&, const double&);