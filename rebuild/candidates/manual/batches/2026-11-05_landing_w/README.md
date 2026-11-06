# Landing 2026-11-05w: chunk 797 (3 of 6 rows landed, 3 parked)

One expert agent authored six script Mains still left in the groups ledger (sharing the lane with the chunk 796 agent). All three
landed rows are plain C++ and grade `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x00E18630 | NScript::CQ_WhiteBalverineWWScript::Main | RELOCATION_MATCH PASS (4 grades) |
| 0x00E46D30 | NScript::CV_BanditCampPathScript::Main | RELOCATION_MATCH PASS (2 grades) |
| 0x00F0CAA0 | NScript::CQ_AmbushTradersScript::CAT_ConvoyMember::Main | RELOCATION_MATCH PASS (2 grades) |

- **Key spellings:**
  - 0x00E18630: the CScriptThing implicit-assignment opt-in plus TU `__forceinline` operator= bodies, as landed sources use.
  - 0x00E46D30: the pause-entities ctor forceinline opt-in.
  - 0x00F0CAA0: GetPos spelled `if (PImp.IsNull()) return NullCoord; return PImp->GetPos();`.
- **Headers** (`hdr_README.md`): new CQ_WhiteBalverineWWScript.h, CV_BanditCampPathScript.h, CQ_NewOakValeIntroScript.h; additive
  opt-ins CQ_AmbushTradersScript.h (CONVOYMEMBER), CQ_BountyHuntScript.h (GANGDEPUTY2), CQ_MinionClifftopChaseScript.h (MAPKEEPER),
  CScriptGameResourceObjectMovieBase.h (FORCEINLINE, used only by the parked 0x00D18DF0). hdrcheck2 reads `default=same`; the DIFF
  macros are new. The agent rechecked 18 landed siblings and includers with the overlay (all MATCH / RELOCATION_MATCH PASS).
- **vland warnings:** two RELOC_NAME on the cast-map dtor 0x00CDBFB0 (catalogued as the placeholder
  `CRetailTree_erase00cd344a`) and one on the TeleportData ctor 0x00F0D470 (`SScriptQuery_00f0d470`): both queued.
- **Names.**
  - Labels: `ghidra_out/labels_landing_2026-11-05w.tsv` (the six rows, SpawnBalverines 0x00E19560, ~TeleportData 0x00CDBC50 and three
    entity vtables 0x012F7DF8 / 0x012D73E0 / 0x012D8678).
  - 24 queue lines (tag `land0511w`), among them: GetVillagerSpeechIndex 0x00DAEBF0 is nested in CQ_NewOakValeIntroScript and private;
    AddBadDeed 0x00DAEA70 takes EBadDeeds; the landed CQ_NewOakValeIntroScript::Main 0x00DABAC0 binds all 16 nested-script Allocs
    without their `CNOVI_` prefix; the TeleportData ctors 0x00F0D470 (AmbushTraders) and 0x00D15140 (BountyHunt, landed as
    CMainGameComponentInit); 0x00CDBC50 called `CLandscapeBackgroundPatch_dtor` in 0x00CDBD00; the cast-map dtor 0x00CDBFB0.
- **Landing:** `vland --land` (`logs/land.log`); labels applied; the manifest sync dry run prints 0.

## Parked (`pending/2026-11-05_linux_hand797/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00DA91A0 CQ_MinionClifftopChaseScript::CMapKeeper::Main | DIFFER(2765v2765), 10 grades | one instruction of scheduling: retail hoists `lea ecx,[esp+0x1c]` above the loop-head terminate check |
| 0x00DADF80 CQ_NewOakValeIntroScript::CNOVI_Villager::Main | DIFFER(2790v2787), 10 grades | the eight speech-list picks: `end() - begin()` gets retail's registers but folds a load; `.size()` is the right size with registers swapped |
| 0x00D18DF0 CQ_BountyHuntScript::CBH_GangDeputy2::Main | DIFFER(3337v3321), 14 grades | the two MovieBase dtor sites: a user-declared forceinline dtor resets the vtable at each site, retail's compiler-generated one does not |
