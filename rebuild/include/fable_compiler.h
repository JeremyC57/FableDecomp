// fable_compiler.h -- compiler-specific spellings behind portable names.
//
// FABLE_FORCEINLINE: VC7.1's `__forceinline` when the compiler is MSVC (the retail build: the token reaches cl exactly as if it
// were written out, so the bytes are unchanged), plain `inline` elsewhere (same meaning for the program: forceinline is an
// inlining request plus `inline` linkage; GCC/Clang decide inlining themselves). Rows and shared headers spell it this way so a
// GCC/Clang build of the reconstruction does not trip over the MSVC keyword. Never spell `__forceinline` bare in rebuild/
// (tools/decomp_pipeline/check_portable_spellings.py rejects it; an `#ifdef _MSC_VER`-guarded one is tolerated).
#ifndef FABLE_COMPILER_H
#define FABLE_COMPILER_H

#if defined(_MSC_VER)
#define FABLE_FORCEINLINE __forceinline
#else
#define FABLE_FORCEINLINE inline
#endif

#endif // FABLE_COMPILER_H
