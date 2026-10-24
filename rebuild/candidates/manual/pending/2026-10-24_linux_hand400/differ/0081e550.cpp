// FtoL(float value) -- round a float to a long with the FPU's current rounding mode (the engine's out-of-line float-to-long helper)
// Grade: DIFFER(25v29) -- retail_asm candidate: retail rounds with a bare `fistp dword ptr [edx]` through a pointer to the result (inline assembly in the original); VC7.1 C++ `(long)x` calls __ftol2 (truncation) and /QIfist emits `fistp qword`
// Leaf: FtoL
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081E550, 29 bytes, `ret 4` (__stdcall, one float on the stack; ecx unused). No callees.
//   Retail: `sub esp,8 ; lea eax,[esp] ; mov [esp+4],eax ; fld dword ptr [esp+0Ch] ; mov edx,[esp+4] ; fistp dword ptr [edx] ;
//   mov eax,[esp] ; add esp,8 ; ret 4` -- the result local at [esp], the pointer to it homed at [esp+4] and reloaded into
//   edx for the store: the same inline-assembly FloatToLong(long* result, float value) helper as the landed retail_asm
//   CSprite_00b1b5e0::GetHeight 0x00B1B5E0 (its `fld value ; mov edx,result ; fistp dword ptr [edx]` body), here expanded
//   inside a standalone `long FtoL(float)`.
// Landing: re-author with the minimal three-instruction inline-assembly helper and a `// Retail-asm:` line (grade
//   retail_asm, crawl/purity.py) -- this lane's rules forbid it, so the C++ reference body below keeps the semantics only.
// Reference body: C++ cannot emit a bare dword `fistp`; the round-to-nearest (default FPU mode) is modelled with the
//   classic 1.5 * 2^23 magic-number trick so the behaviour test can check it.
long __stdcall FtoL(float value)
{
    long result;
    long* out = &result;
    double rounded = (double)value + 6755399441055744.0;   // 1.5 * 2^52: the low dword is the round-to-nearest-even integer
    *out = *(long*)&rounded;
    return result;
}
