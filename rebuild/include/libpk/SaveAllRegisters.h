// LIBPK_SAVE_ALL_REGISTERS() / LIBPK_RESTORE_ALL_REGISTERS() -- libpk's `pushad` / `popad` guard around its winmm driver
// queries (CAudioContext::GetPreferredDeviceName 0x00C55DA0; libpk has no PDB: descriptive names).
// Retail-asm-helper: LIBPK_SAVE_ALL_REGISTERS, LIBPK_RESTORE_ALL_REGISTERS
// Retail-asm: 0x00C55DB4 `pushad` and 0x00C55DDA `popad` bracket waveOutMessage / waveOutGetDevCapsA: VC7.1 never emits
//   pusha / popa from C or C++ (no intrinsic exists), so the original had two inline-asm statements.
// Semantics: every general register is saved and restored around the bracketed calls; no C++-visible state changes (the
//   calls are ordinary stdcall imports that already preserve ebx/esi/edi/ebp).
// Portable twin (FABLE_PORTABLE): nothing (the bracket has no effect a C++ program can observe). Test:
//   rebuild/tests/portable/test_SaveAllRegisters.cpp (a bracketed call returns the same values on both paths).
// Use: write each macro call as a statement on a line of its own: the retail branch expands to `__asm` statements, and an
//   `__asm` statement runs to the end of its source line (anything after it on that line is read as assembler).
#ifndef FABLE_LIBPK_SAVEALLREGISTERS_H
#define FABLE_LIBPK_SAVEALLREGISTERS_H

#ifdef FABLE_PORTABLE
#define LIBPK_SAVE_ALL_REGISTERS() ((void)0)
#define LIBPK_RESTORE_ALL_REGISTERS() ((void)0)
#else
#define LIBPK_SAVE_ALL_REGISTERS() \
    __asm pushad
#define LIBPK_RESTORE_ALL_REGISTERS() \
    __asm popad
#endif

#endif  // FABLE_LIBPK_SAVEALLREGISTERS_H
