// GF_ROR13(value) -- the engine's rotate-right-by-13 hash step on an unsigned long held in memory (GFROR13, FableWin.pdb
// ?GFROR13@@YIJK@Z, __fastcall long(unsigned long)): an inline-assembly `ror` on the argument's stack home.
// Retail-asm-helper: GF_ROR13
// Retail-asm: 0x00497891 `mov [esp],ecx` + 0x00497894 `ror dword ptr [esp],0Dh` (GFROR13 0x00497890): the fastcall argument
//   is homed in the `push ecx` word and rotated in memory, then reloaded (`mov eax,[esp]`). VC7.1 C++ (`_rotr` / `_lrotr`
//   with or without /Oi, `(v >> 13) | (v << 19)`, through a temporary or a pointer) rotates in a register (`mov eax,ecx ;
//   ror eax,0Dh`) and never homes the argument (the batch ATTEMPTS.tsv, family ROR): the original is `__asm ror value, 13`.
// Form: a macro on the caller's own variable (an asm operand that names a C variable is a memory operand: the homed word).
// Semantics: value = (value >> 13) | (value << 19) on 32 bits.
// Portable twin (FABLE_PORTABLE): plain C++ rotate; test rebuild/tests/portable/test_GFROR13.cpp.
// Use: write each macro call as a statement on a line of its own: the retail branch expands to `__asm` statements, and an
//   `__asm` statement runs to the end of its source line (anything after it on that line is read as assembler).
// Proven (2026-11-05 round 2): VC7.1 never emits a memory-operand rotate from C++ or `_rotr` / `_lrotr` / `_rotl`: even
//   `g = _lrotr(g, 13)` on a global, `*p = _lrotr(*p, 13)` through a pointer or the shift form under #pragma optimize("g",
//   off) load, rotate in a register and store (64 spellings, 32 plain C++ and 32 intrinsic: rebuild/candidates/manual/naming/2026-11-05_asm_helpers_r2/ATTEMPTS_GFROR13.tsv).
#ifndef FABLE_ENGINE_GFROR13_H
#define FABLE_ENGINE_GFROR13_H

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
namespace fable_portable
{
    inline unsigned long GFRotateRight13(unsigned long value)
    {
        unsigned int bits = (unsigned int)value;        // a 32-bit rotate, as `ror dword ptr [esp],0Dh`
        return (unsigned long)((bits >> 13) | (bits << 19));
    }
}
#endif

#ifdef FABLE_PORTABLE
#define GF_ROR13(value) ((value) = fable_portable::GFRotateRight13(value))
#else
#define GF_ROR13(value) \
    __asm ror value, 13
#endif

#endif  // FABLE_ENGINE_GFROR13_H
