// NavMath::Sign(float value) -- step direction of a coordinate delta: -1 when negative, else +1 (zero and NaN give +1)
// Leaf: ?Sign@@YGHM@Z
// Retail: Fable.exe @ 0x00437160, 30 bytes, callee-pops `ret 4`, no `this` use; 0.0f is the float at 0x0122DEDC.
// Body: return value < 0.0f ? -1 : 1 (eax = -1 first, `test ah,5; jnp` keeps it, else eax = 1).
// Layout: none.
// Names: the catalog label CDefaultEngineMemoryInit::ApplyCustomValue is wrong (BSim 0.69; the real
// ?ApplyCustomValue@CDefaultEngineMemoryInit@@AAEXAAMM@Z takes (float&, float), `ret 8`). The six callers are
// CNavigatorAStar::IsRouteClear-area line walkers (0x00A1A455, 0x00A1A463, 0x00A1E9C0, ...), stepping along X and Y.
// Real name unknown; `Sign` is descriptive.
// Shape: `value >= 0.0f ? 1 : -1` lays out a jp/or-eax tail instead; the `< 0 ? -1 : 1` ternary gives retail.
// Opt: /O2.
int __stdcall Sign(float value)
{
    return value < 0.0f ? -1 : 1;
}