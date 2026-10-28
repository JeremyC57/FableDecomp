# Chunk 529 lessons (GOTCHAS one-liners)

- A class holding a double (CEngine) pads its vptr slot to 8 under VC7.1; a local model with `void* vptr` + 4-byte pad is off by 4 everywhere -- start members at +0x08.
- Inline member functions with loops (CVectorMap::Insert, SetSlerpFromIdentity) need `__forceinline` under the lane's flags; plain `inline` stays a call.
- STLport rows only match with the same-TU noinline bodies of `_M_insert_overflow`, `__uninitialized_copy`, `__uninitialized_fill_n`, `__destroy_aux` (or `__copy` / `__copy_backward`) -- VC needs to see the address does not escape.
- `_Rb_tree::_M_insert` is private in STLport 4.6 _tree.h: spell it AAE, not QAE.
- `bool failed=false; failed |= a && !a->F(...);` reproduces retail's eax->bl accumulate; separate ifs give early returns.
- One `||` chain puts a shared `return false` at the end; separate ifs inline extra epilogues.
- A ternary final return keeps an earlier `return 0.0f` as its own epilogue; if/return pairs merge them.
- No NRVO in VC7.1: a named result local plus `return result` emits the copy retail has.
- Member access through an inline member (capture-block push) makes VC address the member through a register (`lea ecx,[eax+0x2808]`) as retail does.
- VC7.1 widens two adjacent bool reads into one `mov ax, word ptr` when the destination is a 2-bool struct local (0x00B27D90 residue).
- An inline helper with a named local reproduces retail's temporary destruction order (temporary dies before the conversion call).
- `fld v; fistp dword r` (the engine's FloatToLong) cannot be produced by VC7.1 C++ (`(long)x` -> __ftol2, /QIfist -> fistp qword): rows inlining GFFastArcCos / GFFastSin are retail_asm.
- Catalog labels like "ApplyPartialAnimationAlpha" were propagated across byte-identical bodies of different functions -- check PDB signatures, not labels.
- The cc.sh 200 s timeout can expire silently when the Wine lane is shared: no .obj and no error; rerun.
- Test variable names must not contain the forbidden substrings (`g_emitters` contains `_emit`).

## Why earlier attempts stalled
- Invented callee spellings and catalog labels (wrong class for 0x00B3BC90, propagated ApplyPartialAnimationAlpha, CParticleEmitterManager).
- Missing same-TU STLport bodies, so VC reloaded finish pointers.
- The CEngine vptr padding (every member 4 bytes low).
- Inline-asm FloatToLong rows not recognised as retail_asm.

## Review at landing
- See NOTES.md "Review at landing" (Calculate stand-in in 0x00A86CD0, 0x01436EA0 spelling choice, 0x0099B6B0 wchar_t mangling, 0x01436E38 type).
