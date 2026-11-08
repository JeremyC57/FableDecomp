// CCpuInfo::GetAMDModelName() -- names an AMD processor from its CPUID family / model: 486 or 5x86, the K5 / K6 / K6-2 / K6-III family 5 parts and the K7 / Athlon / Duron / Athlon XP family 6 parts; everything else "<unknown>"
// Grade: DIFFER(140v140) -- md=4: the K5 test `cmp eax,3 ; ja` v ours `cmp eax,4 ; jae` (`Model < 4`; `<= 3` gives `cmp ecx,3`); chunk 868 (5 grades + probe): colouring order `this` v the K5 Model temp #10: with `<= 3` this 18 (12 in the switch block, K=3 = {this, the Family temp, the const-4 record the `sub eax,4` lowering creates and the family-6 `== 4` compare links to}, -1 -1 live-through, +4 family-6 load, +4 K5 load) v #10 16 (8 with K=2, then 2+2+2+2), so `this` takes ecx first and #10 follows the dying-source preference into ecx; `< 4` links the K5 compare to the record (K=3 -> 20 v 20, #10 first by key) but emits `cmp 4 ; jae`; new this chunk: `switch (Family - 4)` (c1: an extra `sub eax,0`), `== 4u` (c2), case order 6/5/4 (c6), `!(Model >> 2)` (c7, 147 bytes), an enum constant for 3 (c8) all keep the record linked (md 12-22); earlier: v1-v16, r1/r4 (signed range check, 148)
// Next idea: the named quantity is c2's compare-constant linking (a compare links to an existing arithmetic const record, never creates one): need the family-6 `== 4` not to link, or a third zero-cost candidate in the K5 block; none found in plain C++
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
