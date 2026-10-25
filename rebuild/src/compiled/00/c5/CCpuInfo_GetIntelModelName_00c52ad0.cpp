// CCpuInfo::GetIntelModelName() -- names an Intel processor from its CPUID family / model / stepping, the L2 cache size and the brand index: 486, Pentium, the P6 family (Pro, II, II Xeon, Celeron, III, III Xeon) and Pentium 4; everything else "<unknown>"
// Leaf: ?GetIntelModelName@CCpuInfo@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C52AD0, length 186 bytes plus its jump table (0x00C52B8C, five
// entries) and byte index table (0x00C52BA0, twelve entries) inside the row. No callees. Caller: the CPU identification
// 0x00C53126 (vendor "GenuineIntel"). String literals: "80486" 0x012B88B4, "Pentium" 0x012B88AC, "Pentium Pro" 0x012B88A0,
// "Pentium II" 0x012B8884, "Pentium II Xeon" 0x012B8890, "Celeron" 0x012B8868, "Pentium III" 0x012B885C, "Pentium III Xeon"
// 0x012B8870, "Pentium 4" 0x012B883C, "Pentium 4 (sample)" 0x012B8848, "<unknown>" 0x012B8830.
// Jump-table check (by hand, relocations are masked): index = family - 4; bytes 00 01 02 04 04 04 04 04 04 04 04 03;
// entries 0 -> 0x00C52AED "80486" (family 4), 1 -> 0x00C52AF3 "Pentium" (5), 2 -> 0x00C52AF9 the model chain (6),
// 3 -> 0x00C52B72 Pentium 4 (15), 4 -> 0x00C52B84 "<unknown>" (7..14).
// Layout from the reads (the landed GetCentaurModelName 0x00C52C90 model): Family +4, Stepping +8, Model +0xc, BrandIndex
// +0x10, L2CacheKB +0x30. The catalog label "CPU_GetIntelBrandName" is descriptive; the class name follows the landed row.
// Shape: the outer switch is a two-level table; the model tests are an if-chain, the brand index a small switch
// (`dec ecx` chain).
struct CCpuInfo {
    unsigned char _pad[4];
    long Family;                                         // +4
    unsigned long Stepping;                              // +8
    long Model;                                          // +0xc
    long BrandIndex;                                     // +0x10
    unsigned char _pad14[0x1c];
    unsigned long L2CacheKB;                             // +0x30
    const char* GetIntelModelName() const;
};

const char* CCpuInfo::GetIntelModelName() const
{
    switch (Family) {
    case 4:
        return "80486";
    case 5:
        return "Pentium";
    case 6:
        if (Model == 1)
            return "Pentium Pro";
        if (Model == 3)
            return "Pentium II";
        if (Model == 5) {
            if (L2CacheKB == 0)
                return "Celeron";
            if (L2CacheKB >= 1024)
                return "Pentium II Xeon";
            return "Pentium II";
        }
        if (Model == 6)
            return "Celeron";
        if (Model == 7) {
            if (L2CacheKB >= 1024)
                return "Pentium III Xeon";
            return "Pentium III";
        }
        if (Model == 8) {
            switch (BrandIndex) {
            case 1:
                return "Celeron";
            case 2:
                return "Pentium III";
            case 3:
                return "Pentium III Xeon";
            }
            break;
        }
        if (Model == 10)
            return "Pentium III Xeon";
        if (Model == 11)
            return "Pentium III";
        break;
    case 15:
        if (Stepping == 2)
            return "Pentium 4 (sample)";
        return "Pentium 4";
    }
    return "<unknown>";
}