// CCpuInfo::GetAMDModelName() -- names an AMD processor from its CPUID family / model: 486 or 5x86, the K5 / K6 / K6-2 / K6-III family 5 parts and the K7 / Athlon / Duron / Athlon XP family 6 parts; everything else "<unknown>"
// Leaf: ?GetAMDModelName@CCpuInfo@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C52BB0, length 140 bytes (+ int3 padding). No callees. Caller: the CPU
// identification 0x00C5313A (vendor "AuthenticAMD"). String literals: "486 or 5x86" 0x012B88BC, "K5" 0x012B88DC, "K6"
// 0x012B88C8, "K6-2" 0x012B88D4, "K6-III" 0x012B88CC, "K7" 0x012B88FC, "Athlon" 0x012B88E0, "Duron" 0x012B88E8,
// "Athlon XP" 0x012B88F0, "<unknown>" 0x012B8830.
// Layout (the landed GetCentaurModelName 0x00C52C90 model): Family +4, Stepping +8, Model +0xc. Model is unsigned here:
// the K5 test is `cmp eax,3 ; ja`.
// Name: descriptive (no PDB, FSE, EgoCore or sibling-repo source names it; the strings exist only in retail Fable.exe, right after
//   the OpenAL entry-point names at 0x012B8780, so this is vendor CPU-identification code); as the landed GetIntelModelName 0x00C52AD0 /
//   GetCentaurModelName 0x00C52C90. The catalog label "CPU_GetAMDBrandName" is descriptive.
// Inferred: the family-5 arm's final `if (Model > 9) return "<unknown>";` is fitted. It is behaviour-neutral (the arm falls through
//   to the same "<unknown>" return) and VC7.1 removes the compare, so it emits no bytes. Its extra Model reference (by hand864's c2 priority model) raises the
//   family-5 Model temporary over `this` in the allocator, so the load takes eax as in retail (0x00C52BFC `mov eax,[ecx+0Ch]`).
//   Without it, `Model <= 3` gives `mov ecx,[ecx+0Ch] ; cmp ecx,3` (6 bytes differ) and `Model < 4` gives eax but `cmp eax,4 ; jae`
//   (2 bytes; hand864 D1: the switch's const-4 record pairs with the family-6 `== 4`). Other placements measured:
//   `Model >= 10` or `Model == 4 || Model == 5` at the arm's end also match; `== 4 || == 5` right after the K5 test is 154 bytes;
//   a ternary tail differs in 42 bytes.
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
        if (Model > 9)
            return "<unknown>";
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