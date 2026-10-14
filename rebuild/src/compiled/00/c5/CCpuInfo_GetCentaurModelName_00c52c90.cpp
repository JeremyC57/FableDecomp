// CCpuInfo::GetCentaurModelName() -- names a VIA/Centaur processor from its CPUID family / model / stepping:
// family 5 model 4 is the "WinChip C6"; family 6 model 6 "C3 Samuel", model 7 "C3 Samuel 2" (stepping below 8)
// or "C3 Ezra", model 8 "C3 Ezra-T"; everything else "<unknown>".
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C52C90, length 76 bytes. No callees; string literals @ 0x012B8954
// ("C3 Samuel"), 0x012B894C ("C3 Ezra"), 0x012B8940 ("C3 Samuel 2"), 0x012B8934 ("C3 Ezra-T"), 0x012B8928
// ("WinChip C6"), 0x012B8830 ("<unknown>").
// The manifest label (CAppearanceModifierDef::GetModifierScalingType) is propagated garbage: the strings identify
// a CPU-identification helper. Layout from the reads: Family +4, Stepping +8 (unsigned, `jb`), Model +0xc.
// The two-case outer switch (5, 6) lowers to `sub eax,5 ; je ; dec eax ; jne`; the inner tests are an if-chain, and
// the stepping test is two return statements (a ternary would share the final `ret` and lose one byte).
struct CCpuInfo {
    unsigned char _pad[4];
    long Family;                                         // +4
    unsigned long Stepping;                              // +8
    long Model;                                          // +0xc
    const char* GetCentaurModelName() const;
};

const char* CCpuInfo::GetCentaurModelName() const
{
    switch (Family) {
    case 5:
        if (Model == 4)
            return "WinChip C6";
        break;
    case 6:
        if (Model == 6)
            return "C3 Samuel";
        if (Model == 7) {
            if (Stepping >= 8)
                return "C3 Ezra";
            return "C3 Samuel 2";
        }
        if (Model == 8)
            return "C3 Ezra-T";
        break;
    }
    return "<unknown>";
}