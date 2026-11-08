// libpk CPU identification blocks -- the inline-assembly CPUID queries of libpk's CPU detection DetectCpu (0x00C52E40; libpk
// has no PDB: the macro names are descriptive, after the CPUID leaf each one reads). Each macro is one retail block: it zeroes
// ebx / ecx / edx where retail does, loads the leaf into eax, runs `cpuid` and stores the registers straight into the
// caller's variables (some masked or shifted in place, as retail does).
// Retail-asm-helper: LIBPK_CPUID_VENDOR, LIBPK_CPUID_SIGNATURE, LIBPK_CPUID_CACHE_DESCRIPTORS, LIBPK_CPUID_SERIAL, LIBPK_CPUID_EXTENDED_MAX, LIBPK_CPUID_EXTENDED_FEATURES, LIBPK_CPUID_BRAND, LIBPK_CPUID_AMD_L2
// Retail-asm: all eight blocks are in DetectCpu 0x00C52E40, each `xor`/`mov eax,<leaf>` ; `cpuid` ; `mov` / `and` / `shr` stores:
//   0x00C52ED4 `xor ebx,ebx ; xor ecx,ecx ; xor edx,edx ; xor eax,eax ; cpuid ; mov [ebp-28h],eax ; mov [ebp-14h],ebx ;
//     mov [ebp-10h],edx ; mov [ebp-0Ch],ecx` (leaf 0: highest leaf, vendor string)
//   0x00C52F5F `mov eax,1 ; cpuid ; mov [ebp-44h],edx ; mov [ebp-54h],eax ; and dword ptr [ebp-54h],0Fh ; ... ; shr eax,4 ;
//     mov [ebp-48h],eax ; and dword ptr [ebp-48h],3` (leaf 1: features, stepping, brand index, logical count, model,
//     family, type, masked in memory)
//   0x00C52FB3 `xor ebx,ebx ; xor ecx,ecx ; xor edx,edx ; mov eax,2 ; cpuid ; mov [ebp-24h],eax .. mov [ebp-18h],edx`
//   0x00C53001 `... mov eax,1 ; cpuid ; mov [ebp-38h],eax ; mov eax,3 ; cpuid ; mov [ebp-30h],ecx ; mov [ebp-34h],edx`
//   0x00C5301E `... mov eax,80000000h ; cpuid ; mov [ebp-28h],eax`, 0x00C53039 `... mov eax,80000001h ; cpuid ;
//     mov [ebp-40h],edx`, 0x00C53055 `... mov eax,80000002h ; cpuid ; mov [0143BC80h],eax ..` (three leaves into the brand
//     string), 0x00C530EB `... mov eax,80000006h ; cpuid ; shr ecx,10h ; mov [ebp-2Ch],ecx ; and dword ptr [ebp-2Ch],0FFFFh`
//   VC7.1 has no __cpuid intrinsic (it arrived with VC8's <intrin.h>; no $VC71/include header declares one) and no C++ form
//   emits cpuid (family CPUID, rebuild/candidates/manual/naming/2026-11-05_asm_helpers/w4/ATTEMPTS_FAMILIES.tsv C1; the
//   landed engine/GFInitVectorMathCPUID.h), so each block is the original's inline asm; the stores and in-place masks run
//   on the registers cpuid leaves, inside the same blocks.
// Semantics: each macro sets its outputs from the named CPUID leaf (leaf 0: the highest standard leaf and the 12-byte vendor
//   string; leaf 1: EDX features, EAX stepping 3:0 / model 7:4 / family 11:8 / type 13:12, EBX brand index 7:0 / logical
//   processor count 23:16; leaf 2: the 16 cache-descriptor bytes; leaf 1 EAX + leaf 3 ECX / EDX: the serial-number words;
//   0x80000000: the highest extended leaf; 0x80000001: EDX extended features; 0x80000002..4: the 48-byte brand string;
//   0x80000006: the L2 size in KB, ECX 31:16). Outputs are unsigned 32-bit words (char arrays for the strings).
// Portable twin (FABLE_PORTABLE): Strategy C, as engine/GFInitVectorMathCPUID.h -- no plain C++ runs CPUID: __get_cpuid_count
//   (GCC / Clang <cpuid.h>) or __cpuidex (MSVC >= VC8 <intrin.h>) on x86 / x86-64, VC7.1's own `__asm` CPUID inside the twin
//   (as engine/GFInitVectorMathCPUID.h) so the dual-path build compares two routes; elsewhere every register reads 0 (leaf 0
//   then reports no leaves and an empty vendor). Dual-path test: hdr/tests/portable/test_CpuIdentify.cpp (to land as
//   rebuild/tests/portable/test_CpuIdentify.cpp).
// Use: write each macro call as a statement on a line of its own: the retail branch expands to `__asm` statements, and an
//   `__asm` statement runs to the end of its source line.
#ifndef FABLE_LIBPK_CPUIDENTIFY_H
#define FABLE_LIBPK_CPUIDENTIFY_H

#if defined(FABLE_PORTABLE) || defined(FABLE_DUAL_PATH_TEST)
#if defined(_MSC_VER) && _MSC_VER >= 1400
#include <intrin.h>
#elif !defined(_MSC_VER) && (defined(__i386__) || defined(__x86_64__))
#include <cpuid.h>
#endif
#include <string.h>
namespace fable_portable
{
    // regs = { eax, ebx, ecx, edx } of CPUID `leaf` (ecx = 0), all 0 where the host cannot run CPUID.
    inline void LibpkCpuid(unsigned long leaf, unsigned long regs[4])
    {
#if defined(_MSC_VER) && _MSC_VER >= 1400
        int r[4];
        __cpuidex(r, (int)leaf, 0);
        regs[0] = (unsigned long)(unsigned int)r[0]; regs[1] = (unsigned long)(unsigned int)r[1];
        regs[2] = (unsigned long)(unsigned int)r[2]; regs[3] = (unsigned long)(unsigned int)r[3];
#elif !defined(_MSC_VER) && (defined(__i386__) || defined(__x86_64__))
        unsigned int a = 0, b = 0, c = 0, d = 0;
        __cpuid_count((unsigned int)leaf, 0, a, b, c, d);
        regs[0] = a; regs[1] = b; regs[2] = c; regs[3] = d;
#elif defined(_MSC_VER) && defined(_M_IX86)
        unsigned long a, b, c, d;
        __asm {                                         // VC7.1: no intrinsic, no other route to CPUID (see above)
            mov eax, leaf
            xor ecx, ecx
            cpuid
            mov a, eax
            mov b, ebx
            mov c, ecx
            mov d, edx
        }
        regs[0] = a; regs[1] = b; regs[2] = c; regs[3] = d;
#else
        (void)leaf;
        regs[0] = regs[1] = regs[2] = regs[3] = 0;
#endif
    }
    inline void LibpkStoreWord(void* destination, unsigned long value)
    {
        unsigned int word = (unsigned int)value;
        memcpy(destination, &word, 4);
    }
}
#endif

#ifdef FABLE_PORTABLE
#define LIBPK_CPUID_VENDOR(maxLeaf, vendor) \
    do { unsigned long fable_r[4]; fable_portable::LibpkCpuid(0, fable_r); (maxLeaf) = fable_r[0]; \
         fable_portable::LibpkStoreWord((char*)(vendor) + 0, fable_r[1]); fable_portable::LibpkStoreWord((char*)(vendor) + 4, fable_r[3]); \
         fable_portable::LibpkStoreWord((char*)(vendor) + 8, fable_r[2]); } while (0)
#define LIBPK_CPUID_SIGNATURE(features, stepping, brandIndex, logicalCount, model, family, type) \
    do { unsigned long fable_r[4]; fable_portable::LibpkCpuid(1, fable_r); (features) = fable_r[3]; \
         (stepping) = fable_r[0] & 0xf; (brandIndex) = fable_r[1] & 0xff; (logicalCount) = (fable_r[1] >> 16) & 0xff; \
         (model) = (fable_r[0] >> 4) & 0xf; (family) = (fable_r[0] >> 8) & 0xf; (type) = (fable_r[0] >> 12) & 3; } while (0)
#define LIBPK_CPUID_CACHE_DESCRIPTORS(descriptors) \
    do { unsigned long fable_r[4]; fable_portable::LibpkCpuid(2, fable_r); \
         for (int fable_i = 0; fable_i < 4; ++fable_i) fable_portable::LibpkStoreWord((char*)(descriptors) + 4 * fable_i, fable_r[fable_i]); } while (0)
#define LIBPK_CPUID_SERIAL(signature, serialLow, serialMiddle) \
    do { unsigned long fable_r[4]; fable_portable::LibpkCpuid(1, fable_r); (signature) = fable_r[0]; \
         fable_portable::LibpkCpuid(3, fable_r); (serialLow) = fable_r[2]; (serialMiddle) = fable_r[3]; } while (0)
#define LIBPK_CPUID_EXTENDED_MAX(maxLeaf) \
    do { unsigned long fable_r[4]; fable_portable::LibpkCpuid(0x80000000UL, fable_r); (maxLeaf) = fable_r[0]; } while (0)
#define LIBPK_CPUID_EXTENDED_FEATURES(features) \
    do { unsigned long fable_r[4]; fable_portable::LibpkCpuid(0x80000001UL, fable_r); (features) = fable_r[3]; } while (0)
#define LIBPK_CPUID_BRAND(brand) \
    do { for (unsigned long fable_l = 0; fable_l < 3; ++fable_l) { unsigned long fable_r[4]; \
         fable_portable::LibpkCpuid(0x80000002UL + fable_l, fable_r); \
         for (int fable_i = 0; fable_i < 4; ++fable_i) fable_portable::LibpkStoreWord((char*)(brand) + 16 * fable_l + 4 * fable_i, fable_r[fable_i]); } } while (0)
#define LIBPK_CPUID_AMD_L2(l2SizeKB) \
    do { unsigned long fable_r[4]; fable_portable::LibpkCpuid(0x80000006UL, fable_r); (l2SizeKB) = (fable_r[2] >> 16) & 0xffff; } while (0)
#else
#define LIBPK_CPUID_VENDOR(maxLeaf, vendor) \
    __asm xor ebx, ebx \
    __asm xor ecx, ecx \
    __asm xor edx, edx \
    __asm xor eax, eax \
    __asm cpuid \
    __asm mov maxLeaf, eax \
    __asm mov dword ptr vendor[0], ebx \
    __asm mov dword ptr vendor[4], edx \
    __asm mov dword ptr vendor[8], ecx
#define LIBPK_CPUID_SIGNATURE(features, stepping, brandIndex, logicalCount, model, family, type) \
    __asm mov eax, 1 \
    __asm cpuid \
    __asm mov features, edx \
    __asm mov stepping, eax \
    __asm and stepping, 0Fh \
    __asm mov brandIndex, ebx \
    __asm and brandIndex, 0FFh \
    __asm shr ebx, 10h \
    __asm mov logicalCount, ebx \
    __asm and logicalCount, 0FFh \
    __asm shr eax, 4 \
    __asm mov model, eax \
    __asm and model, 0Fh \
    __asm shr eax, 4 \
    __asm mov family, eax \
    __asm and family, 0Fh \
    __asm shr eax, 4 \
    __asm mov type, eax \
    __asm and type, 3
#define LIBPK_CPUID_CACHE_DESCRIPTORS(descriptors) \
    __asm xor ebx, ebx \
    __asm xor ecx, ecx \
    __asm xor edx, edx \
    __asm mov eax, 2 \
    __asm cpuid \
    __asm mov dword ptr descriptors[0], eax \
    __asm mov dword ptr descriptors[4], ebx \
    __asm mov dword ptr descriptors[8], ecx \
    __asm mov dword ptr descriptors[12], edx
#define LIBPK_CPUID_SERIAL(signature, serialLow, serialMiddle) \
    __asm xor ebx, ebx \
    __asm xor ecx, ecx \
    __asm xor edx, edx \
    __asm mov eax, 1 \
    __asm cpuid \
    __asm mov signature, eax \
    __asm mov eax, 3 \
    __asm cpuid \
    __asm mov serialLow, ecx \
    __asm mov serialMiddle, edx
#define LIBPK_CPUID_EXTENDED_MAX(maxLeaf) \
    __asm xor ebx, ebx \
    __asm xor ecx, ecx \
    __asm xor edx, edx \
    __asm mov eax, 80000000h \
    __asm cpuid \
    __asm mov maxLeaf, eax
#define LIBPK_CPUID_EXTENDED_FEATURES(features) \
    __asm xor ebx, ebx \
    __asm xor ecx, ecx \
    __asm xor edx, edx \
    __asm mov eax, 80000001h \
    __asm cpuid \
    __asm mov features, edx
#define LIBPK_CPUID_BRAND(brand) \
    __asm xor ebx, ebx \
    __asm xor ecx, ecx \
    __asm xor edx, edx \
    __asm mov eax, 80000002h \
    __asm cpuid \
    __asm mov dword ptr brand[0], eax \
    __asm mov dword ptr brand[4], ebx \
    __asm mov dword ptr brand[8], ecx \
    __asm mov dword ptr brand[12], edx \
    __asm mov eax, 80000003h \
    __asm cpuid \
    __asm mov dword ptr brand[16], eax \
    __asm mov dword ptr brand[20], ebx \
    __asm mov dword ptr brand[24], ecx \
    __asm mov dword ptr brand[28], edx \
    __asm mov eax, 80000004h \
    __asm cpuid \
    __asm mov dword ptr brand[32], eax \
    __asm mov dword ptr brand[36], ebx \
    __asm mov dword ptr brand[40], ecx \
    __asm mov dword ptr brand[44], edx
#define LIBPK_CPUID_AMD_L2(l2SizeKB) \
    __asm xor ebx, ebx \
    __asm xor ecx, ecx \
    __asm xor edx, edx \
    __asm mov eax, 80000006h \
    __asm cpuid \
    __asm shr ecx, 10h \
    __asm mov l2SizeKB, ecx \
    __asm and l2SizeKB, 0FFFFh
#endif

#endif  // FABLE_LIBPK_CPUIDENTIFY_H
