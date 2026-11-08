// Behaviour test for DetectCpu (0x00C52E40): with the CPUID leaves supplied by a fake processor, the vendor string picks the
// vendor code and the model-name helper, the Intel leaf-2 descriptors give the L2 size (0x40 = none, 0x41..0x4F =
// 0x80 << (low nibble - 1) KB), the brand string is copied without its leading spaces, AMD reads leaf 0x80000006, Cyrix
// family 6 model 5 and Centaur family > 5 report "Via", the clock is measured only with a time-stamp counter, the feature
// bytes come from the leaf-1 / 0x80000001 EDX bits, and a processor without CPUID leaves everything reset.
#include <cstdio>
#include <cstring>

struct FakeCpu { bool cpuid; unsigned long leaf[8][4]; const char* vendor; };   // leaf 0,1,2,3,0x80000000,1,2..4 collapsed below
static FakeCpu g_fake;
static unsigned long g_regs[0x10][4], g_ext[0x10][4];
static void Cpuid(unsigned long leaf, unsigned long r[4])
{
    const unsigned long* s = leaf >= 0x80000000UL ? g_ext[leaf - 0x80000000UL] : g_regs[leaf];
    std::memcpy(r, s, sizeof(unsigned long) * 4);
}
struct CCpuInfo {
    long Vendor; unsigned long Family, Stepping; long Model, BrandIndex, Type;
    unsigned long Features, ExtendedFeatures, LogicalCount, Signature, SerialMiddle, SerialLow, L2CacheKB;
    const char* GetIntelModelName() const { return "IntelModel"; }
    const char* GetAMDModelName() const { return "AMDModel"; }
    const char* GetCyrixModelName() const { return "CyrixModel"; }
    const char* GetCentaurModelName() const { return "CentaurModel"; }
};
static int g_measured;
static unsigned long long MeasureCpuFrequency() { ++g_measured; return 2400000000ULL; }
static long RoundCpuSpeed(long mhz) { return mhz; }
static char g_cpuName[48]; static const char* g_cpuVendorName; static const char* g_cpuModelName;
static unsigned long g_cpuFeatures; static long g_cpuSpeedMHz; static unsigned long g_cpuL2CacheKB, g_cpuLogicalCount;
static bool g_cpuDetected, g_cpuHasCMOV, g_cpuHasMMX, g_cpuHasFXSR, g_cpuHasSSE, g_cpuHasSSE2, g_cpuHasFPU, g_cpuHas3DNow;
static char g_cpuBrand[48];

static void DetectCpu()
{
    CCpuInfo info; unsigned long value, r[4];
    struct { unsigned long descriptors[4]; char vendor[13]; } regs;
    g_cpuName[0] = 0; g_cpuVendorName = "<unknown>"; g_cpuModelName = "<unknown>"; g_cpuFeatures = 0; g_cpuSpeedMHz = 0;
    g_cpuHasCMOV = g_cpuHasMMX = g_cpuHasFXSR = g_cpuHasSSE = g_cpuHasSSE2 = g_cpuHasFPU = g_cpuHas3DNow = false;
    g_cpuL2CacheKB = 0;
    value = g_fake.cpuid ? 1 : 0;
    if (!value) return;
    std::memset(&info, 0, sizeof info);
    Cpuid(0, r); value = r[0];
    std::memcpy(regs.vendor, &r[1], 4); std::memcpy(regs.vendor + 4, &r[3], 4); std::memcpy(regs.vendor + 8, &r[2], 4);
    regs.vendor[12] = 0;
    if (!std::strcmp(regs.vendor, "GenuineIntel")) info.Vendor = 1;
    else if (!std::strcmp(regs.vendor, "AuthenticAMD")) info.Vendor = 2;
    else if (!std::strcmp(regs.vendor, "CyrixInstead")) info.Vendor = 3;
    else if (!std::strcmp(regs.vendor, "CentaurHauls")) info.Vendor = 4;
    else info.Vendor = 0;
    Cpuid(1, r);
    info.Features = r[3]; info.Stepping = r[0] & 0xf; info.BrandIndex = r[1] & 0xff; info.LogicalCount = (r[1] >> 16) & 0xff;
    info.Model = (r[0] >> 4) & 0xf; info.Family = (r[0] >> 8) & 0xf; info.Type = (r[0] >> 12) & 3;
    if (info.Vendor == 1) {
        if (value > 1) {
            Cpuid(2, r); for (int k = 0; k < 4; ++k) std::memcpy((char*)regs.descriptors + 4 * k, &r[k], 4);
            for (unsigned long i = 1; i < 16; ++i) {
                unsigned long descriptor = ((unsigned char*)regs.descriptors)[i];
                if (descriptor == 0x40) { info.L2CacheKB = 0; break; }
                if (descriptor > 0x40) { info.L2CacheKB = 0x80 << ((descriptor & 0xf) - 1); break; }
            }
        }
        if (value > 2) { Cpuid(1, r); info.Signature = r[0]; Cpuid(3, r); info.SerialLow = r[2]; info.SerialMiddle = r[3]; }
    }
    Cpuid(0x80000000UL, r); value = r[0];
    if (value >= 0x80000001UL) { Cpuid(0x80000001UL, r); info.ExtendedFeatures = r[3]; }
    if (value >= 0x80000004UL) {
        for (unsigned long l = 0; l < 3; ++l) { Cpuid(0x80000002UL + l, r); for (int k = 0; k < 4; ++k) std::memcpy(g_cpuBrand + 16 * l + 4 * k, &r[k], 4); }
        const char* name = g_cpuBrand; while (*name == ' ') ++name;
        std::strcpy(g_cpuName, name);
    }
    if (info.Vendor == 2 && value >= 0x80000006UL) { Cpuid(0x80000006UL, r); info.L2CacheKB = (r[2] >> 16) & 0xffff; }
    switch (info.Vendor) {
    case 1: g_cpuVendorName = "Intel"; g_cpuModelName = info.GetIntelModelName(); break;
    case 2: g_cpuVendorName = "AMD"; g_cpuModelName = info.GetAMDModelName(); break;
    case 3: g_cpuVendorName = info.Family == 6 && info.Model == 5 ? "Via" : "Cyrix"; g_cpuModelName = info.GetCyrixModelName(); break;
    case 4: g_cpuVendorName = info.Family > 5 ? "Via" : "IDT"; g_cpuModelName = info.GetCentaurModelName(); break;
    }
    g_cpuFeatures = info.Features;
    if (g_cpuFeatures & 0x10) g_cpuSpeedMHz = RoundCpuSpeed((long)(MeasureCpuFrequency() / 1000000));
    g_cpuHasCMOV = (g_cpuFeatures >> 15) & 1; g_cpuHasMMX = (g_cpuFeatures >> 23) & 1; g_cpuHasFXSR = (g_cpuFeatures >> 24) & 1;
    g_cpuHasSSE = (g_cpuFeatures >> 25) & 1; g_cpuHasSSE2 = (g_cpuFeatures >> 26) & 1; g_cpuHasFPU = g_cpuFeatures & 1;
    g_cpuHas3DNow = (info.ExtendedFeatures >> 31) & 1;
    g_cpuL2CacheKB = info.L2CacheKB; g_cpuLogicalCount = info.LogicalCount; g_cpuDetected = true;
}

static void SetVendor(const char* v, unsigned long maxLeaf)
{
    std::memset(g_regs, 0, sizeof g_regs); std::memset(g_ext, 0, sizeof g_ext);
    g_regs[0][0] = maxLeaf; std::memcpy(&g_regs[0][1], v, 4); std::memcpy(&g_regs[0][3], v + 4, 4); std::memcpy(&g_regs[0][2], v + 8, 4);
}
static void SetBrand(const char* b) { char buf[48] = {0}; std::strncpy(buf, b, 47);
    for (int l = 0; l < 3; ++l) for (int k = 0; k < 4; ++k) std::memcpy(&g_ext[2 + l][k], buf + 16 * l + 4 * k, 4); }

int main()
{
    int bad = 0;
    // 1. Intel P4: family 15, descriptors 0x01 0x7C? -> first byte past AL that is 0x40 or above decides: 0x43 -> 0x80 << 2 = 512
    g_fake.cpuid = true; SetVendor("GenuineIntel", 3);
    g_regs[1][0] = 0xF29; g_regs[1][1] = 0x00020000 | 0x07; g_regs[1][3] = (1u << 0) | (1u << 4) | (1u << 15) | (1u << 23) | (1u << 25) | (1u << 26);
    g_regs[2][0] = 0x00433B01;                          // AL 01, then 0x3B (below 0x40), then 0x43
    g_regs[3][2] = 0x1111; g_regs[3][3] = 0x2222;
    g_ext[0][0] = 0x80000004UL; g_ext[1][3] = 0; SetBrand("    Intel(R) Pentium(R) 4 CPU 2.40GHz");
    g_measured = 0; DetectCpu();
    if (std::strcmp(g_cpuVendorName, "Intel") || std::strcmp(g_cpuModelName, "IntelModel")) ++bad;
    if (g_cpuL2CacheKB != 512 || g_cpuLogicalCount != 2) ++bad;
    if (std::strcmp(g_cpuName, "Intel(R) Pentium(R) 4 CPU 2.40GHz")) ++bad;
    if (g_measured != 1 || g_cpuSpeedMHz != 2400) ++bad;
    if (!g_cpuHasCMOV || !g_cpuHasMMX || g_cpuHasFXSR || !g_cpuHasSSE || !g_cpuHasSSE2 || !g_cpuHasFPU || g_cpuHas3DNow || !g_cpuDetected) ++bad;
    // 2. Intel with descriptor 0x40 (no L2), no TSC
    SetVendor("GenuineIntel", 2); g_regs[1][0] = 0x6B1; g_regs[2][0] = 0x00004001;
    g_measured = 0; DetectCpu();
    if (g_cpuL2CacheKB != 0 || g_measured != 0 || g_cpuSpeedMHz != 0 || g_cpuName[0]) ++bad;
    // 3. AMD: L2 from 0x80000006 ECX 31:16, 3DNow! from 0x80000001 EDX bit 31
    SetVendor("AuthenticAMD", 1); g_regs[1][0] = 0x681; g_ext[0][0] = 0x80000006UL; g_ext[1][3] = 0x80000000UL;
    g_ext[6][2] = 256u << 16; SetBrand("AMD Athlon(tm) XP");
    DetectCpu();
    if (std::strcmp(g_cpuVendorName, "AMD") || std::strcmp(g_cpuModelName, "AMDModel") || g_cpuL2CacheKB != 256 || !g_cpuHas3DNow) ++bad;
    if (std::strcmp(g_cpuName, "AMD Athlon(tm) XP")) ++bad;
    // 4. Cyrix family 6 model 5 -> Via; family 5 -> Cyrix
    SetVendor("CyrixInstead", 1); g_regs[1][0] = 0x650; DetectCpu();
    if (std::strcmp(g_cpuVendorName, "Via") || std::strcmp(g_cpuModelName, "CyrixModel")) ++bad;
    SetVendor("CyrixInstead", 1); g_regs[1][0] = 0x520; DetectCpu();
    if (std::strcmp(g_cpuVendorName, "Cyrix")) ++bad;
    // 5. Centaur family 6 -> Via, family 5 -> IDT
    SetVendor("CentaurHauls", 1); g_regs[1][0] = 0x670; DetectCpu();
    if (std::strcmp(g_cpuVendorName, "Via") || std::strcmp(g_cpuModelName, "CentaurModel")) ++bad;
    SetVendor("CentaurHauls", 1); g_regs[1][0] = 0x540; DetectCpu();
    if (std::strcmp(g_cpuVendorName, "IDT")) ++bad;
    // 6. unknown vendor: names stay "<unknown>"
    SetVendor("SomeOtherCPU", 1); DetectCpu();
    if (std::strcmp(g_cpuVendorName, "<unknown>") || std::strcmp(g_cpuModelName, "<unknown>")) ++bad;
    // 7. no CPUID: everything reset, detected flag untouched
    g_cpuDetected = false; g_fake.cpuid = false; g_cpuL2CacheKB = 99; DetectCpu();
    if (g_cpuDetected || g_cpuL2CacheKB != 0 || g_cpuHasFPU || std::strcmp(g_cpuVendorName, "<unknown>")) ++bad;
    std::printf("00c52e40_TEST %s\n", bad ? "FAIL" : "PASS");
    return bad;
}