// LIBPK_EFLAGS_ID_PROBE(supported) -- libpk's CPUID-support probe (the body of IsCpuidSupported 0x00C52AA0; libpk has no PDB:
// a descriptive name): toggle the ID bit (bit 21) of EFLAGS and set the long `supported` to 1 when the change sticks.
// Retail-asm-helper: LIBPK_EFLAGS_ID_PROBE
// Retail-asm: 0x00C52AAB `pushfd` .. 0x00C52AC3 `mov dword ptr [ebp-4],1` (`pushfd ; pop eax ; mov ecx,eax ; xor eax,200000h
//   ; push eax ; popfd ; pushfd ; pop eax ; xor eax,ecx ; je`): VC7.1 has no pushfd / popfd intrinsic (__readeflags /
//   __writeeflags arrived with VC8) and no C++ form emits them, so the original is the classic inline-asm EFLAGS.ID probe.
// Semantics: supported = 1 when EFLAGS.ID can be toggled (every CPUID-capable processor: all i486 SL and later, every
//   x86-64); otherwise supported keeps its value. The probe restores nothing but the bit it toggled (retail leaves it
//   toggled, as here).
// Portable twin (FABLE_PORTABLE): supported = 1 on an x86 / x86-64 host (every such host implements CPUID), unchanged
//   elsewhere. Test: rebuild/tests/portable/test_IsCpuidSupported.cpp (both paths report the same on the test host).
// Use: write each macro call as a statement on a line of its own: the retail branch expands to `__asm` statements, and an
//   `__asm` statement runs to the end of its source line (anything after it on that line is read as assembler).
#ifndef FABLE_LIBPK_ISCPUIDSUPPORTED_H
#define FABLE_LIBPK_ISCPUIDSUPPORTED_H

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
namespace fable_portable
{
    inline void EflagsIdProbe(long& supported)
    {
#if defined(_M_IX86) || defined(__i386__) || defined(__x86_64__) || defined(_M_X64)
        supported = 1;
#else
        (void)supported;
#endif
    }
}
#endif

#ifdef FABLE_PORTABLE
#define LIBPK_EFLAGS_ID_PROBE(supported) fable_portable::EflagsIdProbe(supported)
#else
#define LIBPK_EFLAGS_ID_PROBE(supported) \
    __asm pushfd \
    __asm pop eax \
    __asm mov ecx, eax \
    __asm xor eax, 200000h \
    __asm push eax \
    __asm popfd \
    __asm pushfd \
    __asm pop eax \
    __asm xor eax, ecx \
    __asm je libpk_eflags_id_done \
    __asm mov supported, 1 \
    __asm libpk_eflags_id_done:
#endif

#endif  // FABLE_LIBPK_ISCPUIDSUPPORTED_H
