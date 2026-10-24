// GFFloatToLongFloor(float value) -- round down to a long: round-to-nearest of (value - 0.5), corrected up by one when that lands exactly one below an integral value
// Grade: DIFFER(45v58) -- lane gap: retail inline asm. Retail keeps 0.5f and 1.0f in frame words and runs a bare
// `fld [value] ; fsub [half] ; fistp dword [result] ; fild dword [result] ; fadd [one] ; fstp [check]` sequence, then an
// INTEGER compare of value's bits with the stored check (`mov eax,[ebp+8] ; cmp eax,[ebp-10h] ; jne ; inc [result]`).
// VC7.1 cannot emit a dword `fistp` from C++ (a cast calls _ftol; /QIfist gives a qword fistp), so this C++ form is
// the behavioural reference only. Retail-asm candidate for the landing session (the owner's 2026-10-24 exception:
// a C++ body with the minimal `__asm { fld ; fsub ; fistp }` block and a `// Retail-asm:` evidence line); this
// chunk's brief forbids `__asm`, so it is parked here.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048D940, 58 bytes, `?GFFloatToLongFloor@@YIJM@Z` (Ego_r.pdb name set: __fastcall,
// the float on the stack, `ret 4`); ebp frame of four words (result -4, half -0xC, one -8, check -0x10); no call or
// absolute reference in the image (an unreferenced copy; the callers inline it).
// Leaf: GFFloatToLongFloor
long __fastcall GFFloatToLongFloor(float value)
{
    float half = 0.5f;
    float one = 1.0f;
    long result = (long)(value - half);     // retail: fistp in the current (round-to-nearest) mode; this cast truncates (_ftol)
    float check = (float)result + one;
    if (*(long*)&value == *(long*)&check)
        ++result;
    return result;
}
