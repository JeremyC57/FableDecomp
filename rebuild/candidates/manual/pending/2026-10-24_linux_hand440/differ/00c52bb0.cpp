// CCpuInfo::GetAMDModelName() -- names an AMD processor from its CPUID family / model: 486 or 5x86, the K5 / K6 / K6-2 / K6-III family 5 parts and the K7 / Athlon / Duron / Athlon XP family 6 parts; everything else "<unknown>"
// Leaf: ?GetAMDModelName@CCpuInfo@@
// Grade: DIFFER(140v140) -- register choice only: the family-5 arm loads Model into ecx (`mov ecx,[ecx+0Ch] ; cmp ecx,..`) where retail uses eax; every other byte and the literal pairing match
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C52BB0, length 140 bytes (+ int3 padding). No callees. Caller: the CPU
// identification 0x00C5313A (vendor "AuthenticAMD"). String literals: "486 or 5x86" 0x012B88BC, "K5" 0x012B88DC, "K6"
// 0x012B88C8, "K6-2" 0x012B88D4, "K6-III" 0x012B88CC, "K7" 0x012B88FC, "Athlon" 0x012B88E0, "Duron" 0x012B88E8,
// "Athlon XP" 0x012B88F0, "<unknown>" 0x012B8830.
// Layout (the landed GetCentaurModelName 0x00C52C90 model): Family +4, Stepping +8, Model +0xc. Model is unsigned here:
// the K5 test is `cmp eax,3 ; ja`. The catalog label "CPU_GetAMDBrandName" is descriptive; the class name follows the
// landed row.
// Shape: the three-case family switch lowers to `sub eax,4 ; je ; dec eax ; je ; dec eax ; jne` with the family 6 arm
// falling through; the model tests are if-chains in source order.
struct CCpuInfo {
    unsigned char _pad[4];
    long Family;                                         // +4
    unsigned long Stepping;                              // +8
    unsigned long Model;                                 // +0xc
    const char* GetAMDModelName() const;
};

const char* CCpuInfo::GetAMDModelName() const
{
    switch (Family) {
    case 4:
        return "486 or 5x86";
    case 5:
        if (Model <= 3)
            return "K5";
        if (Model == 6 || Model == 7)
            return "K6";
        if (Model == 8)
            return "K6-2";
        if (Model == 9)
            return "K6-III";
        break;
    case 6:
        if (Model == 1)
            return "K7";
        if (Model == 2 || Model == 4)
            return "Athlon";
        if (Model == 3 || Model == 7)
            return "Duron";
        if (Model == 6 || Model == 8)
            return "Athlon XP";
        break;
    }
    return "<unknown>";
}
