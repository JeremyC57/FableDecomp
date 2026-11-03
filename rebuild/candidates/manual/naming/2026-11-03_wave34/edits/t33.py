# T33 (KEEP follow-ups of the pass-12 rows done in T27): NProfileTimer::StartProfile / EndProfile in 0x00416953;
# CTCInventoryAbilities::GetAbilityMaximumLevel / IsAbilityPotentiallyAvailable const (EHeroAbility) (Ego_r 0x0057CB0E / 0x0057CBBB),
# ForceAbilityLevelTo(EHeroAbility, long) (Ego_r 0x0057DCB8) in 0x007A9FE0 / 0x005EE887 / 0x005F3E7D.
A = "rebuild/src/compiled/00/41/CMainGameComponent_PostInit_00416953.cpp"; B = "rebuild/src/compiled/00/7a/CTCAssassinRush_CreateTrailingEffect_007a9fe0.cpp"; C = "rebuild/src/compiled/00/5e/CTCInventoryAbilities_UpdateWillMorph_005ee887.cpp"; D = "rebuild/src/compiled/00/5f/CTCInventoryAbilities_ForceAllAbilitiesToMaxLevel_005f3e7d.cpp"
EDITS = [
    (A, "void __fastcall LoadingStage(const CCharString& stage, long level);     // 0x009D8240\nvoid __fastcall LoadingStageDone(long level);                           // 0x009D8250",
        "namespace NProfileTimer {\nvoid __fastcall StartProfile(const CCharString& name, long level);      // 0x009D8240\nvoid __fastcall EndProfile(long level);                                 // 0x009D8250\n}"),
    (A, '        LoadingStage("Activate Initial Quests", 0);', '        NProfileTimer::StartProfile("Activate Initial Quests", 0);'),
    (A, "        LoadingStageDone(0);", "        NProfileTimer::EndProfile(0);"),
    (B, "class CTCInventoryAbilities { public: long GetAbilityMaximumLevel(long ability); };   // 0x005EDEA3 (interface 0x6F)",
        "enum EHeroAbility { };\nclass CTCInventoryAbilities { public: long GetAbilityMaximumLevel(EHeroAbility ability) const; };   // 0x005EDEA3 (interface 0x6F)"),
    (B, "abilities->GetAbilityMaximumLevel(13);", "abilities->GetAbilityMaximumLevel((EHeroAbility)13);"),
    (C, "    bool IsAbilityPotentiallyAvailable(long ability);   // 0x005EE02E\n    long GetAbilityMaximumLevel(long ability);          // 0x005EDEA3\n",
        "    bool IsAbilityPotentiallyAvailable(EHeroAbility ability) const;   // 0x005EE02E\n    long GetAbilityMaximumLevel(EHeroAbility ability) const;          // 0x005EDEA3\n"),
    (C, "        if (IsAbilityPotentiallyAvailable(i)) {\n            possible += GetAbilityMaximumLevel(i) + 1;",
        "        if (IsAbilityPotentiallyAvailable((EHeroAbility)i)) {\n            possible += GetAbilityMaximumLevel((EHeroAbility)i) + 1;"),
    (D, "CTCInventoryAbilities_Retail", "CTCInventoryAbilities", 0),
    (D, "    bool IsAbilityPotentiallyAvailable(long ability);     // 0x005EE02E\n    void ForceAbilityLevelTo(long ability, long level);   // 0x005EEF84\n    long GetAbilityMaximumLevel(long ability);            // 0x005EDEA3\n",
        "    bool IsAbilityPotentiallyAvailable(EHeroAbility ability) const;   // 0x005EE02E\n    void ForceAbilityLevelTo(EHeroAbility ability, long level);       // 0x005EEF84\n    long GetAbilityMaximumLevel(EHeroAbility ability) const;          // 0x005EDEA3\n"),
    (D, "        if (IsAbilityPotentiallyAvailable(ability))\n            ForceAbilityLevelTo(ability, GetAbilityMaximumLevel(ability));",
        "        if (IsAbilityPotentiallyAvailable((EHeroAbility)ability))\n            ForceAbilityLevelTo((EHeroAbility)ability, GetAbilityMaximumLevel((EHeroAbility)ability));"),
    (D, "struct CTCInventoryAbilities {", "enum EHeroAbility { };\n\nstruct CTCInventoryAbilities {"),
]
