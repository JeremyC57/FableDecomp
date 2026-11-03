# T27 (pass-12 queued_correct_open, small): NProfileTimer Start/EndProfile, GetAbilityMaximumLevel const (EHeroAbility),
# RemapUIDForSerialisation const, TransferVector<CCharString> + vector<CCharString>(size_type) in 0x00706F40.
P = "rebuild/src/compiled/"
W = P + "00/49/CWorld_ActivateWorld_0049f180.cpp"
U = P + "00/5e/CTCInventoryAbilities_UpgradeAbility_005eeee6.cpp"
F = P + "00/5e/CTCInventoryAbilities_ForceAbilityLevelTo_005eef84.cpp"
M = P + "00/5f/CTCInventoryMap_RemapUIDsForLoading_005fae90.cpp"
H = "rebuild/src/compiled/00/70/CTCHeroAttachableAppearanceModifiers_OnSerialise_00706f40.cpp"
CV = ("    SAbilityEntry& operator[](unsigned long n) { return first[n]; }\n",
      "    SAbilityEntry& operator[](unsigned long n) { return first[n]; }\n    const SAbilityEntry& operator[](unsigned long n) const { return first[n]; }\n};\nenum EHeroAbility { };\nstruct CAbilityEntryVector_End {\n")
EDITS = [
    (W, "void __fastcall LoadingStage(const CCharString& stage, long level);     // 0x009D8240 (empty in retail)\nvoid __fastcall LoadingStageDone(long level);                           // 0x009D8250 (empty in retail)",
        "namespace NProfileTimer {\nvoid __fastcall StartProfile(const CCharString& name, long level);      // 0x009D8240 (empty in retail)\nvoid __fastcall EndProfile(long level);                                 // 0x009D8250 (empty in retail)\n}"),
    (W, "    LoadingStage(", "    NProfileTimer::StartProfile(", 0),
    (W, "    LoadingStageDone(0);", "    NProfileTimer::EndProfile(0);"),
    (U, "    long GetAbilityMaximumLevel(long ability);       // 0x005EDEA3 (defined below)", "    long GetAbilityMaximumLevel(EHeroAbility ability) const;   // 0x005EDEA3 (defined below)"),
    (U, "    void UpgradeAbility(long ability);", "    void UpgradeAbility(EHeroAbility ability);"),
    (U, "long CTCInventoryAbilities::GetAbilityMaximumLevel(long ability)", "long CTCInventoryAbilities::GetAbilityMaximumLevel(EHeroAbility ability) const"),
    (U, "void CTCInventoryAbilities::UpgradeAbility(long ability)", "void CTCInventoryAbilities::UpgradeAbility(EHeroAbility ability)"),
    (U, "    SAbilityEntry& operator[](unsigned long n) { return first[n]; }\n};\n", "    SAbilityEntry& operator[](unsigned long n) { return first[n]; }\n    const SAbilityEntry& operator[](unsigned long n) const { return first[n]; }\n};\nenum EHeroAbility { };\n"),
    (F, "    SAbilityEntry& operator[](unsigned long n) { return first[n]; }\n};\n", "    SAbilityEntry& operator[](unsigned long n) { return first[n]; }\n    const SAbilityEntry& operator[](unsigned long n) const { return first[n]; }\n};\nenum EHeroAbility { };\n"),
    (F, "CTCInventoryAbilities_Retail", "CTCInventoryAbilities", 0),
    (F, "    long GetAbilityMaximumLevel(long ability);    // 0x005EDEA3 (defined below)", "    long GetAbilityMaximumLevel(EHeroAbility ability) const;   // 0x005EDEA3 (defined below)"),
    (F, "    void ForceAbilityLevelTo(long ability, long level);", "    void ForceAbilityLevelTo(EHeroAbility ability, long level);"),
    (F, "long CTCInventoryAbilities::GetAbilityMaximumLevel(long ability)", "long CTCInventoryAbilities::GetAbilityMaximumLevel(EHeroAbility ability) const"),
    (F, "void CTCInventoryAbilities::ForceAbilityLevelTo(long ability, long level)", "void CTCInventoryAbilities::ForceAbilityLevelTo(EHeroAbility ability, long level)"),
    (M, "    unsigned __int64 RemapUIDForSerialisation(bool saving, bool levelLess, long level, unsigned __int64 uid);   // 0x0051E210",
        "    unsigned __int64 RemapUIDForSerialisation(bool saving, bool levelLess, unsigned long level, unsigned __int64 uid) const;   // 0x0051E210"),

    (H, "    void Transfer(const char* name, std::vector<CCharString, std::allocator<CCharString> >& value);   // 0x0049B8D0",
        "    template <class T> void TransferVector(const char* name, std::vector<T, std::allocator<T> >& value);   // 0x0049B8D0 <CCharString>"),
    (H, 'context.Transfer("HairModifierNames", names);', 'context.TransferVector("HairModifierNames", names);', 0),
    (H, "CPersistContext::Transfer(const char*, vector<CCharString>&) @ 0x0049B8D0", "CPersistContext::TransferVector<CCharString>(const char*, vector<CCharString>&) @ 0x0049B8D0"),
]
