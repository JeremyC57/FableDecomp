// CCpuInfo::GetAMDModelName() -- names an AMD processor from its CPUID family / model: 486 or 5x86, the K5 / K6 / K6-2 / K6-III family 5 parts and the K7 / Athlon / Duron / Athlon XP family 6 parts; everything else "<unknown>"
// Grade: DIFFER(140v140) -- 4 masked lines: the family-5 K5 test `cmp eax,3 ; ja` (ours `cmp ecx,3` with `<= 3`, or `cmp eax,4 ; jae` with `< 4`); chunk 864 (3 grades, 7 probes): deciding quantity = colouring order of `this` (prio 18 = 12 in the switch block (K=3: this, the Family temp, const 4) -1 -1 +4 (family-6 Model load) +4 (K5 load)) against the K5 Model temp #10 (16 = 8 (K=2) + 2+2+2+2); `Model < 4` links the compare to the const-4 record the switch `sub eax,4` created (K=3 in the K5 block -> 20 v 20, the later-defined #10 wins the tie -> eax) but emits `cmp 4 ; jae`; compare constants alone never create a record (const 3 / 6 / 7 / 8 all refs 0), so `<= 3` keeps K=2; diagnostic D1 (`== 5` for the family-6 `== 4`, semantics-changing) drops the const-4 record (this 14) and gives retail bytes
// Next idea: need #10 >= 18 or this <= 16 with the retail compares: a third candidate referenced in the K5 block, or one fewer `this` reference in the switch / family-6 blocks; none found in plain C++ (the const-4 pairing of the switch and the family-6 `== 4` is inherent)
// Leaf: ?GetAMDModelName@CCpuInfo@@
// Earlier grade: DIFFER(140v140) (wave-2 sec6, IMPROVED from 7 lines to 2): `Model < 4` makes VC7.1 keep Model in eax in the
//   family-5 arm as retail; left only the compare form (ours cmp eax,4 ; jae, retail cmp eax,3 ; ja). `<= 3`, `3 >=
//   Model`, `!(Model > 3)` all go back to ecx; a signed `Model >= 0 && Model <= 3` keeps eax but is not merged (148).
//   Next: another unsigned spelling that VC canonicalises to <=3 while keeping eax.
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
        if (Model < 4)
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
