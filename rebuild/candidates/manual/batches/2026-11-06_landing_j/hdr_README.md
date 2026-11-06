# chunk 809 header overlay

There are four headers. Every edit is additive and opt-in: for each of the two repo headers, `diff rebuild/include/engine/<h>
hdr/engine/<h>` shows only `>` lines, so the default path is textually unchanged. Each diff was re-checked against the repo copy
just before hand-back.

As a belt-and-braces check, all 8 landed includers of the two edited repo headers were rechecked read-only with this overlay using
`vland.py --recheck-landed`. All 8 are RELOCATION_MATCH + PASS (tmp/recheck_vland.log, listed in SIBLINGS.tsv):
- CQ_AmbushTradersScript.h: 00CF9A50, 00CF9610, 00F0CAA0, 00F024C0.
- CQ_NewOakValeIntroScript.h: 00DB09E0, 00DB2B10, 00DB6C60, 00DBCD60.

## engine/CQ_AmbushTradersScript.h (repo header, opt-in additions)
- `FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_BANDITALLY` adds:
  - Member declarations: AddBanditHealthBar 0x00F07A80, AskBanditHireQuestion 0x00F07ED0, BanditHireSetup 0x00F081E0,
    WatchForBanditSpeechMarkers 0x00F07B80.
  - With `..._MAIN` also defined, the nested `CAT_BanditAlly` class:
    - Layout from Ego_r.pdb, 44 bytes: ParentClass +0x14, PMasterData +0x18, six bools +0x1c..+0x21, BanditHealthBarID +0x24,
      InvalidRegionTimer +0x28. 0x00F063B0 reads each member at those offsets.
    - Entity vtable 0x012F7D88: dtor 0x00F03870, Main, Init 0x00F05F30, GetParentScript 0x00F03840, OnPersist 0x00F0A8E0,
      OnPredicateFail 0x00F03850.
    - SwitchBanditAllegience 0x00F079D0.
- `FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_CONVOYSPY` adds:
  - Member declarations: RefreshSpyAllegiences 0x00F05A10, WatchForSpyDeath 0x00F0EDA0, WatchForHeroProximity 0x00F0EE90,
    WatchForSpySpeechMarkers 0x00F0EF60, ProcessInitialSpyExtraction 0x00F0F680.
  - With `..._MAIN` also defined, the nested `CAT_ConvoySpy` class: 28 bytes, vtable 0x012F7E14 (dtor 0x00F044F0, Main, Init
    0x00F04410, GetParentScript 0x00F04400, OnPersist 0x00F04470, OnPredicateFail 0x00F04480).
- A forward declaration of CScriptGameResourceObjectScriptedThingBase sits inside the opt-in.
- Evidence: the Ego_r.pdb / FableWin.pdb name sets (exact manglings in the header comments), the vtables stored by the landed
  Allocs 0x00F037A0 / 0x00F04360, and the member offsets read by the two Mains.
- Used by 0x00F063B0 and 0x00F0D630.

## engine/CQ_NewOakValeIntroScript.h (repo header, opt-in addition)
- `FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_GUARD` adds the nested `CNOVI_Guard`:
  - 28 bytes: ParentClass +0x14, PMasterData +0x18. The landed Alloc 0x00DAC580 news 0x1c.
  - Vtable 0x012D83C0: dtor 0x00DAC630, Main 0x00DAC760, Init 0x00DAC650, GetParentScript 0x00DAC620.
  - Main / Init are private, which matches the `EAE` manglings in both PDBs.
- Used by the parked 0x00DAC760.

## engine/CQ_MinionCampScript.h (new, behind `#ifdef FABLE_RETAIL_NSCRIPT`, no #error branch)
- NScript::CQ_MinionCampScript : CLevelScriptBase<CQ_SunnyvaleMasterData>, sizeof 0xAC.
  - Retail offsets are the PDB offsets minus 0x14 (retail CLevelScriptBase is 0x48) up to CurrentStage +0x74.
  - The two STLport lists are 4 bytes each, so ShouldSpawn is at +0x98, SpawnPointToSpawn +0x9c and BriarState +0xa8.
  - Those offsets agree with the landed AddNewHeroAttacker 0x00DA0B30 / GetRandomSpawnPoint 0x00DA0E40 and with every member
    0x00DA0EF0 reads.
- Nested CBriarRose, 0x50 bytes:
  - Members: ParentClass, PMasterData, seh_me +0x1c, MyDestination, Target, NumHitsBetweenForcePushes +0x44, SpeechTimer +0x48,
    IsPlayingChantAnim +0x4c.
  - Vtable 0x012D6D0C (stored by the landed ctor 0x00D9E930).
  - Helpers: DoSpeech 0x00DA2610, DoOnHitSpeech 0x00DA26B0, DoOnSpeakToSpeech 0x00DA27B0, UpdateTarget 0x00DA2A20,
    PlayCutscene 0x00DA2CA0. DoChant and DoOnHeroHitSpeech are declared but have no retail copy.
- Parent members used by the row: RunChantTimer, PauseChantTimer and CheckBriarHit, plus the landed members' addresses.
- Header debt: LeftSpawnPoints / RightSpawnPoints are opaque `void*` until a 4-byte STLport std::list model exists (NOTES.md).
- Used by 0x00DA0EF0. No landed includer.

## engine/CV_IntroductionToTrophiesScript.h (new, behind `#ifdef FABLE_RETAIL_NSCRIPT`, no #error branch)
- NScript::CV_IntroductionToTrophiesScript : CLevelScriptBase<CQ_SunnyvaleMasterData>, sizeof 0x6C.
  - Retail offsets are the PDB offsets minus 0x14; the class has no STLport member.
  - The offsets were checked against the reads in 0x00E9E420: +0x48..+0x60, with OpinionCurrent / OpinionRequired at
    +0x58 / +0x5c.
- Nested CBriarRose, 0x20 bytes: AnimTimer +0x1c. Vtable 0x012ED67C; the landed Alloc 0x00E9D9D0 news 0x20.
- The landed parent methods are declared at their addresses.
- Used by the parked 0x00E9E420. No landed includer.
