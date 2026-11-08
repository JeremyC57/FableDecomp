// test_CpuIdentify.cpp -- dual-path test of libpk/CpuIdentify.h: every CPUID block gives sane results on an x86 / x86-64
// host (a printable 12-byte vendor, at least leaf 1, a nonzero family, masked fields in range), the in-place masks hold,
// and with FABLE_DUAL_PATH_TEST the asm blocks and the portable twin read the same words (the leaf-1 EBX logical count
// and brand index can differ between cores only on hybrid parts, so they are compared masked as the macro masks them).
#include <stdio.h>
#include <string.h>
#include "libpk/CpuIdentify.h"
#include "fable_test_mode.h"
// (`type` is a MASM operator: an asm operand named so does not assemble, hence cpuType; every macro operand is a plain
// local or global, never a reference parameter, which an asm operand would address as the pointer slot.)

struct SCpuWords {
    unsigned long maxLeaf, extendedMax, features, stepping, brandIndex, logicalCount, model, family, type;
    unsigned long extendedFeatures, l2;
    char vendor[13];
    char brand[49];
};

static void ReadAll(SCpuWords& out)
{
    unsigned long maxLeaf = 0, extendedMax = 0, features = 0, stepping = 0, brandIndex = 0, logicalCount = 0;
    unsigned long model = 0, family = 0, cpuType = 0, extendedFeatures = 0, l2 = 0;
    char vendor[13];
    char brand[49];
    memset(vendor, 0, sizeof vendor);
    memset(brand, 0, sizeof brand);
    LIBPK_CPUID_VENDOR(maxLeaf, vendor);
    LIBPK_CPUID_SIGNATURE(features, stepping, brandIndex, logicalCount, model, family, cpuType);
    LIBPK_CPUID_EXTENDED_MAX(extendedMax);
    if (extendedMax >= 0x80000001UL) {
        LIBPK_CPUID_EXTENDED_FEATURES(extendedFeatures);
    }
    if (extendedMax >= 0x80000004UL) {
        LIBPK_CPUID_BRAND(brand);
    }
    if (extendedMax >= 0x80000006UL) {
        LIBPK_CPUID_AMD_L2(l2);
    }
    out.maxLeaf = maxLeaf; out.extendedMax = extendedMax; out.features = features; out.stepping = stepping;
    out.brandIndex = brandIndex; out.logicalCount = logicalCount; out.model = model; out.family = family; out.type = cpuType;
    out.extendedFeatures = extendedFeatures; out.l2 = l2;
    memcpy(out.vendor, vendor, sizeof vendor);
    memcpy(out.brand, brand, sizeof brand);
}

static unsigned long ReadDescriptorsAndSerial(unsigned long& signatureOut)
{
    unsigned long descriptors[4] = { 0, 0, 0, 0 };
    unsigned long signature = 0, serialLow = 0, serialMiddle = 0;     // plain locals: an asm operand names the variable itself
    LIBPK_CPUID_CACHE_DESCRIPTORS(descriptors);
    LIBPK_CPUID_SERIAL(signature, serialLow, serialMiddle);
    signatureOut = signature;
    return descriptors[0] & 0xff;                       // leaf 2 AL: the iteration count (1 on every part that has leaf 2)
}

int main()
{
    int bad = 0;
    SCpuWords w;
    ReadAll(w);
#if defined(_M_IX86) || defined(__i386__) || defined(__x86_64__) || defined(_M_X64)
    if (w.maxLeaf < 1) { printf("  max leaf\n"); ++bad; }
    for (int i = 0; i < 12; ++i)
        if (w.vendor[i] < 0x20 || w.vendor[i] > 0x7e) { printf("  vendor byte %d\n", i); ++bad; }
    if (w.family == 0 || w.stepping > 0xf || w.model > 0xf || w.family > 0xf || w.type > 3) { printf("  signature fields\n"); ++bad; }
    if (w.brandIndex > 0xff || w.logicalCount > 0xff || w.l2 > 0xffff) { printf("  masked fields\n"); ++bad; }
    if (w.extendedMax != 0 && w.extendedMax < 0x80000000UL) { printf("  extended max %lx\n", w.extendedMax); ++bad; }
    unsigned long signature = 0;
    if (w.maxLeaf >= 2) {
        unsigned long iterations = ReadDescriptorsAndSerial(signature);
        if (iterations > 1 && iterations != 0xff) { printf("  leaf 2 count %lu\n", iterations); ++bad; }
        if (((signature >> 8) & 0xf) != w.family || ((signature >> 4) & 0xf) != w.model || (signature & 0xf) != w.stepping) { printf("  serial signature %lx\n", signature); ++bad; }
    }
#endif
#ifdef FABLE_TEST_DUAL
    unsigned long r[4];
    fable_portable::LibpkCpuid(0, r);
    if (r[0] != w.maxLeaf || memcmp(&r[1], w.vendor, 4) || memcmp(&r[3], w.vendor + 4, 4) || memcmp(&r[2], w.vendor + 8, 4)) {
        printf("  leaf 0 differs\n"); ++bad;
    }
    fable_portable::LibpkCpuid(1, r);
    if (r[3] != w.features || (r[0] & 0xf) != w.stepping || ((r[0] >> 4) & 0xf) != w.model || ((r[0] >> 8) & 0xf) != w.family
        || ((r[0] >> 12) & 3) != w.type) {
        printf("  leaf 1 differs\n"); ++bad;
    }
    fable_portable::LibpkCpuid(0x80000000UL, r);
    if (r[0] != w.extendedMax) { printf("  leaf 0x80000000 differs\n"); ++bad; }
    if (w.extendedMax >= 0x80000004UL) {
        char twin[49];
        memset(twin, 0, sizeof twin);
        LIBPK_CPUID_BRAND(twin);                        // asm path
        for (unsigned long l = 0; l < 3; ++l) {
            fable_portable::LibpkCpuid(0x80000002UL + l, r);
            if (memcmp(r, twin + 16 * l, 16) && sizeof(unsigned long) == 4) { printf("  brand leaf %lu differs\n", l); ++bad; }
        }
    }
#endif
    printf("test_CpuIdentify %s: %s (%s, family %lu model %lu, max leaf %lu)\n", kFableTestMode, bad ? "FAIL" : "PASS",
           w.vendor, w.family, w.model, w.maxLeaf);
    return bad ? 1 : 0;
}
