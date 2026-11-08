// DetectCpu() -- libpk CPU identification: reset the CPU globals, and when the processor implements CPUID read the vendor, the signature and feature words, the Intel cache descriptors and serial words, the extended features, the brand string (copied without its leading spaces into the CPU name) and the AMD L2 size; name the vendor and model, measure the clock when there is a time-stamp counter, publish the feature flags (CMOV, MMX, FXSR, SSE, SSE2, FPU, 3DNow!), the L2 size and the logical-processor count, and mark the CPU as detected
// Leaf: ?DetectCpu@@YAXXZ
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C52E40, 1032 bytes of code (`ret` at 0x00C53247) + the four-entry vendor jump table
//   0x00C53248 inside the 1048-byte row; /GS cookie (__security_cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9).
// Identity: every libpk caller binds it as ?DetectCpu@@YAXXZ (`if (!g_cpuDetected) DetectCpu();` over 0x0143BC78: the mixers,
//   CAudioSource::CreateProcessor 0x00C55270, the static initialiser 0x01224C20); the catalog's "InitCPUCapabilities" is a
//   description. Callees: IsCpuidSupported's EFLAGS probe (the shared libpk/IsCpuidSupported.h), the landed
//   CCpuInfo::GetIntelModelName 0x00C52AD0 / GetCentaurModelName 0x00C52C90, the parked GetAMDModelName 0x00C52BB0 /
//   GetCyrixModelName 0x00C52C40, the landed MeasureCpuFrequency 0x00C52CE0 / RoundCpuSpeed 0x00C52DA0, _aulldiv 0x00F3FC50.
// Retail-asm: the EFLAGS.ID probe 0x00C52EA7 (libpk/IsCpuidSupported.h) and the eight CPUID blocks 0x00C52ED4, 0x00C52F5F,
//   0x00C52FB3, 0x00C53001, 0x00C5301E, 0x00C53039, 0x00C53055, 0x00C530EB (`cpuid`: hdr/libpk/CpuIdentify.h, family CPUID
//   C1: VC7.1 has no __cpuid intrinsic and no C++ form emits the instruction); the row has no asm of its own.
// Shape: the vendor string is compared with the strcmp intrinsic (`repe cmpsb` over strlen + 1 bytes); the vendor locals
//   share one struct so the leaf-2 descriptors and the vendor string keep separate frame slots (as retail: -0x24 / -0x14);
//   the last test is an inline helper with two returns: its return value is a register candidate (colour eax), so the
//   `setne / dec / and 4` result takes no rotating temporary and the L2 shift at 0x00C52FF2 gets eax, as retail (the plain
//   `else if ... = 4; else ... = 0;` arm or a single-return `? 4 : 0` uses one rotating temporary more: edx there; c2prio
//   --rotation).
// Layout: CCpuInfo (0x34 bytes, the model of the landed GetIntelModelName: Family +4, Stepping +8, Model +0xC, BrandIndex
//   +0x10, L2CacheKB +0x30) filled by the CPUID blocks; the vendor codes 1 Intel, 2 AMD, 3 Cyrix, 4 Centaur select the
//   model-name helper. Globals 0x0143BC30..0x0143BCAF: the CPU name (48 bytes), vendor / model name pointers, the leaf-1
//   feature word, the clock in MHz, the L2 size, the logical count, the detected flag and seven feature bytes
//   (g_cpuDetected 0x0143BC78, g_cpuHasSSE 0x0143BC7C, g_cpuHasSSE2 0x0143BC7D: the landed spellings), the brand string.
#include <string.h>
#include "libpk/IsCpuidSupported.h"
#include "libpk/CpuIdentify.h"

struct CCpuInfo {                                       // 0x34 bytes
    long          Vendor;                               // +0x00 1 Intel, 2 AMD, 3 Cyrix, 4 Centaur
    unsigned long Family;                               // +0x04
    unsigned long Stepping;                             // +0x08
    long          Model;                                // +0x0c
    long          BrandIndex;                           // +0x10
    long          Type;                                 // +0x14
    unsigned long Features;                             // +0x18 leaf 1 EDX
    unsigned long ExtendedFeatures;                     // +0x1c leaf 0x80000001 EDX
    unsigned long LogicalCount;                         // +0x20
    unsigned long Signature;                            // +0x24 leaf 1 EAX
    unsigned long SerialMiddle;                         // +0x28 leaf 3 EDX
    unsigned long SerialLow;                            // +0x2c leaf 3 ECX
    unsigned long L2CacheKB;                            // +0x30
    const char* GetIntelModelName() const;              // 0x00C52AD0
    const char* GetAMDModelName() const;                // 0x00C52BB0
    const char* GetCyrixModelName() const;              // 0x00C52C40
    const char* GetCentaurModelName() const;            // 0x00C52C90
};

unsigned __int64 MeasureCpuFrequency();                 // 0x00C52CE0
long RoundCpuSpeed(long mhz);                           // 0x00C52DA0

extern char          g_cpuName[48];                     // 0x0143BC30
extern const char*   g_cpuVendorName;                   // 0x0143BC60
extern const char*   g_cpuModelName;                    // 0x0143BC64
extern unsigned long g_cpuFeatures;                     // 0x0143BC68
extern long          g_cpuSpeedMHz;                     // 0x0143BC6C
extern unsigned long g_cpuL2CacheKB;                    // 0x0143BC70
extern unsigned long g_cpuLogicalCount;                 // 0x0143BC74
extern bool          g_cpuDetected;                     // 0x0143BC78
extern bool          g_cpuHasCMOV;                      // 0x0143BC79
extern bool          g_cpuHasMMX;                       // 0x0143BC7A
extern bool          g_cpuHasFXSR;                      // 0x0143BC7B
extern bool          g_cpuHasSSE;                       // 0x0143BC7C
extern bool          g_cpuHasSSE2;                      // 0x0143BC7D
extern bool          g_cpuHasFPU;                       // 0x0143BC7E
extern bool          g_cpuHas3DNow;                     // 0x0143BC7F
extern char          g_cpuBrand[48];                    // 0x0143BC80

// The last vendor test: 4 (Centaur) for "CentaurHauls", else 0 (an unknown vendor).
inline long GetCentaurVendorCode(const char* vendor)
{
    if (!strcmp(vendor, "CentaurHauls"))
        return 4;
    return 0;
}

void DetectCpu()
{
    CCpuInfo info;
    unsigned long value;
    struct { unsigned long descriptors[4]; char vendor[13]; } regs;
    g_cpuName[0] = 0;
    g_cpuVendorName = "<unknown>";
    g_cpuModelName = "<unknown>";
    g_cpuFeatures = 0;
    g_cpuSpeedMHz = 0;
    g_cpuHasCMOV = false;
    g_cpuHasMMX = false;
    g_cpuHasFXSR = false;
    g_cpuHasSSE = false;
    g_cpuHasSSE2 = false;
    g_cpuHasFPU = false;
    g_cpuHas3DNow = false;
    g_cpuL2CacheKB = 0;
    value = 0;
    LIBPK_EFLAGS_ID_PROBE(value);
    if (!value)
        return;
    memset(&info, 0, sizeof info);
    LIBPK_CPUID_VENDOR(value, regs.vendor);
    regs.vendor[12] = 0;
    if (!strcmp(regs.vendor, "GenuineIntel"))
        info.Vendor = 1;
    else if (!strcmp(regs.vendor, "AuthenticAMD"))
        info.Vendor = 2;
    else if (!strcmp(regs.vendor, "CyrixInstead"))
        info.Vendor = 3;
    else
        info.Vendor = GetCentaurVendorCode(regs.vendor);
    LIBPK_CPUID_SIGNATURE(info.Features, info.Stepping, info.BrandIndex, info.LogicalCount, info.Model, info.Family, info.Type);
    if (info.Vendor == 1) {
        if (value > 1) {
            LIBPK_CPUID_CACHE_DESCRIPTORS(regs.descriptors);
            for (unsigned long i = 1; i < 16; ++i) {
                unsigned long descriptor = ((unsigned char*)regs.descriptors)[i];
                if (descriptor == 0x40) {
                    info.L2CacheKB = 0;
                    break;
                }
                if (descriptor > 0x40) {
                    info.L2CacheKB = 0x80 << ((descriptor & 0xf) - 1);
                    break;
                }
            }
        }
        if (value > 2) {
            LIBPK_CPUID_SERIAL(info.Signature, info.SerialLow, info.SerialMiddle);
        }
    }
    LIBPK_CPUID_EXTENDED_MAX(value);
    if (value >= 0x80000001) {
        LIBPK_CPUID_EXTENDED_FEATURES(info.ExtendedFeatures);
    }
    if (value >= 0x80000004) {
        LIBPK_CPUID_BRAND(g_cpuBrand);
        const char* name = g_cpuBrand;
        while (*name == ' ')
            ++name;
        strcpy(g_cpuName, name);
    }
    if (info.Vendor == 2 && value >= 0x80000006) {
        LIBPK_CPUID_AMD_L2(info.L2CacheKB);
    }
    switch (info.Vendor) {
    case 1:
        g_cpuVendorName = "Intel";
        g_cpuModelName = info.GetIntelModelName();
        break;
    case 2:
        g_cpuVendorName = "AMD";
        g_cpuModelName = info.GetAMDModelName();
        break;
    case 3:
        if (info.Family == 6 && info.Model == 5)
            g_cpuVendorName = "Via";
        else
            g_cpuVendorName = "Cyrix";
        g_cpuModelName = info.GetCyrixModelName();
        break;
    case 4:
        if (info.Family > 5)
            g_cpuVendorName = "Via";
        else
            g_cpuVendorName = "IDT";
        g_cpuModelName = info.GetCentaurModelName();
        break;
    }
    g_cpuFeatures = info.Features;
    if (g_cpuFeatures & 0x10)
        g_cpuSpeedMHz = RoundCpuSpeed((long)(MeasureCpuFrequency() / 1000000));
    g_cpuHasCMOV = (g_cpuFeatures >> 15) & 1;
    g_cpuHasMMX = (g_cpuFeatures >> 23) & 1;
    g_cpuHasFXSR = (g_cpuFeatures >> 24) & 1;
    g_cpuHasSSE = (g_cpuFeatures >> 25) & 1;
    g_cpuHasSSE2 = (g_cpuFeatures >> 26) & 1;
    g_cpuHasFPU = g_cpuFeatures & 1;
    g_cpuHas3DNow = (info.ExtendedFeatures >> 31) & 1;
    g_cpuL2CacheKB = info.L2CacheKB;
    g_cpuLogicalCount = info.LogicalCount;
    g_cpuDetected = true;
}