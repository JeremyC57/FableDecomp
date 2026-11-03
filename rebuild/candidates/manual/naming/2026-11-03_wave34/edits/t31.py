# T31: undo two collisions the respell made (one address, one name): 0x005CC300 is a separate COMDAT from Transfer<long> 0x004110B0
# (frame-pointer body, 27 bytes vs 32; a different instantiation, unproven) and 0x00661EA0 toggles bit 1 of +0x15D, not CThing's
# +0x92 bit 5 like CThing::SetAsGameStatePersisted 0x004C7880. Both call sites keep their established spellings; queued w34.
A = "rebuild/src/compiled/00/5c/CTCInventoryWeapons_OnSerialise_005c3a95.cpp"; B = "rebuild/src/compiled/00/89/CGameScriptInterface_EntityTeleportToPosition_0089b780.cpp"
EDITS = [
    (A, "    template <class T> void Transfer(const char* name, T& value);   // 0x004107C0 <bool> / 0x00411010 <CCharString> / 0x005CC300 <long> (Ego_r ??$Transfer@J@)",
        "    template <class T> void Transfer(const char* name, T& value);   // 0x004107C0 <bool> / 0x00411010 <CCharString>\n    void Transfer(const char* name, long* value);       // 0x005CC300: not Transfer<long> 0x004110B0 (separate COMDAT); name unproven"),
    (A, 'context.Transfer("ActiveMeleeWeaponDefIndex2", MeleeDefIndex);', 'context.Transfer("ActiveMeleeWeaponDefIndex2", &MeleeDefIndex);'),
    (A, 'context.Transfer("ActiveRangedWeaponDefIndex2", RangedDefIndex);', 'context.Transfer("ActiveRangedWeaponDefIndex2", &RangedDefIndex);'),
    (A, "Transfer<long> 0x005CC300", "Transfer(name, long*) 0x005CC300 (name unproven)"),
    (B, "    void SetAsGameStatePersisted(bool persisted);  // 0x00661EA0\n", ""),
    (B, "class CThing { public:                             // the thing view the teleport code calls (SetAsGameStatePersisted / SetInLimbo are CThing members, Ego_r / FableWin)",
        "struct CTeleportThing {                            // 0x00661EA0 toggles bit 1 of +0x15D (CThing::SetAsGameStatePersisted 0x004C7880 is +0x92 bit 5): name unproven\n    void SetAsGameStatePersisted(bool persisted);  // 0x00661EA0\n};\nclass CThing { public:                             // the thing view the teleport code calls (SetInLimbo is a CThing member, Ego_r / FableWin)"),
    (B, "        T(object)->SetAsGameStatePersisted(false);", "        ((CTeleportThing*)object)->SetAsGameStatePersisted(false);"),
]
