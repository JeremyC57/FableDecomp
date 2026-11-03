# 00dd35d0 final handoff

Final status: **VERIFIED_MATCH**. Selected grade: 4. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/00dd35d0/NAMING_EVIDENCE.md

# 00dd35d0 identity and verification

The exact leaf is `?WatchHeroEscapeStatus@CQ_PrisonEscapeRescueMotherScript@NScript@@QAEXXZ`: a public nonvirtual void member with no arguments. Both original PDB name sets and Ego_r type0x10c3e prove its spelling/access/type. Native owning Main00dd2ce0 stores this address in the CSpawnedFunc member-function pointer at00dd33f1, paired with the exact `WatchHeroEscapeStatus` thread literal at012db9f0. Owner vtable012db86c slot2 points to that Main. The assigned span is the complete1454-byte function through ret00dd3b7d, followed by two padding bytes and another function.

New shared header CQ_PrisonEscapeRescueMotherScript.h preserves the actual CLevelScriptBase<CQ_SunnyvaleMasterData> inheritance and every original owner data member. Native constructor00dd26e0, OnPersist00dd2c80 and member references confirm base0x48 and size0x80, compared with donor base0x5c/size0x94. HeroHasFoundWeapons is+0x65, MotherDied+0x68 and MotherRescuedFromPrisonAutoSaveCheckPoint+0x7a; the complete master header establishes PrisonKeyStolenByHero+0xee. No local owner stand-in is used.

Both original procedure records recover TortureChamberReset, jump_guard, jump_guard_pos, MotherSpawnPos and MotherInCell. FableWin additionally recovers HLStyleConversationIndex and the two lexical scopes. The PDB calls MotherInCell a CScriptThing object. Binding its returned temporary to const CScriptThing& is a reconstruction justified by native virtual dispatch and exact stack layout, not a claim that the donor contains that reference declaration. The original name and owned temporary lifetime are retained.

All78 target relocation operands are independently checked in RELOCATIONS.json, including each exact terminated literal and NullCoord0143e8e0. CCharString ctor/dtor99ebf0/99eae0, CScriptThing dtor4aa840 and termination predicatecb7940 use their exact original names. Both PDBs spell the wide constructor `??0CWideString@@QAE@PB_W@Z`; their distinct wchar_t mode differs from the retail unsigned-short ABI explicitly required by README_CODEX. The existing canonical header under default VC7.1 yields `??0CWideString@@QAE@PBG@Z`, calls99b6b0 and receives the verified UTF-16 literal. This ABI translation is documented rather than falsely claiming that PBG occurs in the donor name sets. The wide destructor is99b510. Wrong older bound spellings are in NAME_FIXES.json/root NAME_FIXES.tsv.

Sixteen Game virtual calls were checked against original PDB names and native vtable01260f0c. Handle IsAlive is outer CScriptThing slot75; inline GetPos uses its real PImp at+4, NullCoord fallback, and interface slot6. No vtable is fabricated in this object; returned handles are native constructions. The selected leaf is present in the actual COFF symbol table, and the entire extent matches with only relocation operands masked.

The standalone behavior fixture pastes the exact selected function. All1951 scenarios pass GNU C++03 and actual VC7.1 /GR-. They cover240 independent native event plans: five initial key/region/autosave cases, repeated loop iterations, equipment branches, alive/dead mother, absent/immediate/delayed chamber, courtyard wait, real/null marker coordinates, every reached termination guard, exact arguments, and lifetime cleanup. Dependency doubles supply external state; this is not a claim to test game engine implementations. The final wait ends only on thread termination, as in native code.
