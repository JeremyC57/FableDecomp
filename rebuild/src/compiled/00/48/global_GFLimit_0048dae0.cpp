// _global::GFLimit(float& value, const float& minimum, const float& maximum) -- clamp a float in place to [minimum, maximum]
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048DAE0, 40 bytes, `ret 4`.
// Callees: none.
// Shape: `if (value < minimum) value = minimum; else if (value > maximum) value = maximum;` -- `__fastcall`
// (value in ecx, minimum in edx, maximum by reference on the stack). Each compare is `fld [value] ; fcomp
// [bound]`: `test ah,5 ; jp` is the ordered `<`, `test ah,41h ; jne` the `>`; the stores are dword moves (a
// float assignment through references is an integer copy) and each arm returns on its own `ret 4`.
// The landed caller CSubtitleRenderer::FormatTextForVWindow (0x0048F50C) declares it exactly so; the
// manifest label "GFLimitReturn<double>" is propagated (the operands are dword floats, nothing is returned).
// Leaf: GFLimit

template <class T> void __fastcall GFLimit(T& value, const T& minimum, const T& maximum)
{
    if (value < minimum)
        value = minimum;
    else if (value > maximum)
        value = maximum;
}

template void __fastcall GFLimit<float>(float&, const float&, const float&);