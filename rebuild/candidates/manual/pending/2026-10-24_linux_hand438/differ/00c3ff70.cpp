// _global::RoundHalfDownToLong(float value) -- subtract one half and convert to an integer with the FPU's current rounding (an inline-asm `fld ; fistp` in retail), then bias negative results up by one
// Grade: DIFFER(21v32) -- retail_asm candidate: bare `fld [value] ; fistp dword [esp]` after storing value-0.5f back to the argument slot (an inline-asm block); the C++ reference body converts with a truncating cast instead. 1 grade.
// Leaf: ?RoundHalfDownToLong@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C3FF70, 32 bytes, __cdecl, long in eax.
// Library: libpk audio (no PDB/RTTI); descriptive name. Called once, from 0x00C3FFF3. Float constant 0x0122F59C =
//   0.5f. The manifest's "GFFloatToLongFloor" is propagated.
// Retail-asm candidate: `value -= 0.5f` is stored back to the argument slot (the asm block reads it), then a bare
//   `fld dword [value] ; fistp dword [result]` (no _ftol call, no control-word switch) -- an inline-asm block in the
//   original. This C++ reference body (truncating cast, _ftol) is NOT byte-exact.
long __cdecl RoundHalfDownToLong(float value)
{
    value -= 0.5f;
    long result = (long)value;
    if (result < 0)
        ++result;
    return result;
}
