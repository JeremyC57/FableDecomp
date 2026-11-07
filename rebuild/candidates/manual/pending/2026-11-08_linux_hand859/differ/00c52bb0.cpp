// CCpuInfo::GetAMDModelName() -- names an AMD processor from its CPUID family / model: 486 or 5x86, the K5 / K6 / K6-2 / K6-III family 5 parts and the K7 / Athlon / Duron / Athlon XP family 6 parts; everything else "<unknown>"
// Grade: DIFFER(140v140) -- 4 masked diff lines: the family-5 K5 test (retail `mov eax,[ecx+0Ch]; cmp eax,3; ja`); 51 attempts (chunks 757/840/844). Chunk 844 diagnostics: in the family-5 arm the Model load takes eax only when the arm's first compare constant is 4 (`Model < 4` gives `cmp eax,4; jae`, `== 4` / `<= 4` likewise eax), and ecx with any other constant (`<= 3`, `> 3`, `== 3`, `<= 5`, `< 3`: `cmp ecx,...`); the family-6 arm takes eax with 1, 4 or 5; case order, local copies, no-op nodes (+0, *1, casts), signed Model with casts, range checks, nested switch, ternary chain, default labels and the QFE 4035 cl change nothing.; chunk 859: 1 regrade (52 total): a register tie that only the constant 4 steers (chunk 844 grid); documented residue
// Next idea: an expression that compares Model with 4 in the IR but lowers to `cmp eax,3; ja` -- VC7.1 emits what is written, so look for a different arm structure instead (a `switch (Model)` whose first range is 0..4 with case 4 folded elsewhere is not retail); otherwise this is a register-allocation tie the oracle cannot steer and the row stays a 4-line park.
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
