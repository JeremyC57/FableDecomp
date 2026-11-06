# Landing 2026-11-06h: chunk 807 (2 of 6 rows landed, 4 parked)

One expert agent authored the rows (sharing the lane with the chunk 806, then 808, agents). The six rows were the smallest unnamed
gap-scan Mains left (4.2-4.9 KB). The agent proved every name from the vtable slot (with landed neighbours) and both PDB name sets.
The landed rows are plain C++ and grade `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x00E0B160 | NScript::CQ_UndeadRisingScript::Main | RELOCATION_MATCH PASS (2 attempts; a row-local inline `CScriptThing::IsAlive` body, which retail expands; `CCharString("")` written out as the spawn-name argument) |
| 0x00DBCD60 | NScript::CQ_NewOakValeIntroScript::CNOVI_Victim::Main | RELOCATION_MATCH PASS (first attempt, on the landed CNOVI_AffairMan model) |

- **Headers** (`hdr_README.md`): the default paths are textually unchanged; the NewOakValeIntro overlay already carried chunk 806's
  `_ENTITIES2` branch (no line dropped).
  - New engine/CQ_UndeadRisingScript.h, behind `#ifdef FABLE_RETAIL_NSCRIPT` with no `#error` branch (the `quest_status`
    enumerator names are descriptive: the PDB type is unnamed).
  - engine/CQ_NewOakValeIntroScript.h: FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_VICTIM_LIVEFATHER (CNOVI_Victim, CNOVI_LiveFather).
  - engine/CV_TourGuideScript.h: FABLE_RETAIL_CV_TOURGUIDESCRIPT_GUIDE (four script methods and CTourGuideGuide).
  - The agent rechecked the landed includers 0x00EE42A0, 0x00DB09E0, 0x00DB2B10 and 0x00DB6C60 with the overlay: all
    RELOCATION_MATCH PASS (SIBLINGS.tsv).
- **vland warnings:** RELOC_NAME on 0x00CDBFB0 (the placeholder map-dtor label, already queued by chunks 796-804) and on 0x00E0C350
  (landed as `CV_LostTraderScript::WatchForQuestFailureState`; it is `CQ_UndeadRisingScript::WatchForTermination`).
- **Names.** Labels: `ghidra_out/labels_landing_2026-11-06h.tsv` (the six Mains). 19 queue lines (tag `land0611h`), all on landed rows:
  - 16 entity Allocs that the landed CQ_NewOakValeIntroScript::Main 0x00DABAC0 binds as `C<Name>`; the PDBs spell them
    `CNOVI_<Name>` (and `COVI_DeadFather` for 0x00DB81B0);
  - 0x00E0C350 WatchForTermination (above);
  - 0x00411570 `?Compare@?$CBasicString@D@@KIJPBD0@Z`, landed as `strcmp_signed`; reloc_bindings.tsv binds 11 spellings to it;
  - 0x00CDBFB0 the `std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>` dtor (again).
- **Landing:** `vland --land` (`logs/land.log`); labels applied; the manifest sync dry run prints 0; bindings_multi_check prints only
  the known NullCoord case; the 8 new bindings add no second symbol at a target and no symbol at a second address.

## Parked (`pending/2026-11-06_linux_hand807/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00DB86B0 NScript::CQ_NewOakValeIntroScript::CNOVI_LiveFather::Main | DIFFER(4325v4325), 10 attempts | return-pad tail merging after the two hint loops only |
| 0x00ECF7F0 NScript::CV_SickChildScript::CWomanToAttract::Main | DIFFER(4835v4885), 10 attempts | retail expands the by-value CScriptThing copy passed to SetIsPushableByHero but calls the predicate's copy; `~CTimer` expanded only at the natural loop exit |
| 0x00DC93F0 NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageSkeleton::Main | DIFFER(4536v4507), 10 attempts | the same copy split (CSpawnedFunc1 Par1 copy called, by-value temporary expanded) and a movie dtor split at the end pad |
| 0x00EE57B0 NScript::CV_TourGuideScript::CTourGuideGuide::Main | DIFFER(4356v4246), 12 attempts | per-exit `~CTimer` split (called at the common pad, expanded at two); retail also inlines the CTimer ctor and keeps both timer indices in ebx / ebp |
