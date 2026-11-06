# chunk 804 header hand-back

All three headers are in hdr/engine/ and are NEW files. rebuild/include/engine has no copy of any of them (checked at hand-back), so
there is no repo text to diff against and no default path to keep. Overlay: FABLE_HDR_OVERLAY=$S/w22/agent804/hdr. No existing repo
header was copied or changed.

Each header is wrapped in `#ifdef FABLE_RETAIL_NSCRIPT`. There is no `#error` branch, because VC7.1 under Wine spins forever on `#error`.
Layouts come from ghidra_out/struct_layouts_egor.tsv at PDB - 0x14, minus 4 after each STLport vector or map, and are checked against the
retail [reg+off] accesses. Member names are the Ego_r.pdb names. Method addresses come from the vtables and the landed rows.

## engine/CQ_ArenaHoldingScriptScript.h (new)
- NScript::CQ_ArenaHoldingScriptScript : CLevelScriptBase<CQ_SunnyvaleMasterData>, sizeof 0x50.
  - Methods: RegisterMain 0x00CF06F0, Main 0x00CF0790, Init 0x00CF06D0.
  - Members: PlayerAttackedFans +0x48, FansCanLeaveNow +0x49, OakvaleInsult +0x4a, ArenaOver +0x4b, InHitCutsceneAlready +0x4c.
  - Size: the landed Alloc 0x00CF8860 news 0x50 bytes. The three fan Mains read/write exactly +0x48/+0x49/+0x4b/+0x4c.
- Nested classes COakvaleFan, CBowerstoneFan and CHookCoastFan, 0x1c each: ParentClass reference +0x14, PMasterData +0x18.
  - Vtables 0x012C6260 / 0x012C6384 / 0x012C6480, slots sdd / Main / Init / GetParentScript.
  - Alloc 0x00CF17F0 / 0x00CF2A90 / 0x00CF3C40 (landed rows).
- CEntranceGuard, CEntranceGuard2 and CFanMarker are forward-declared only.
- Includers: the chunk 804 differ rows 00cf18d0, 00cf2b70 and 00cf3d20. No landed row includes it; the 29 landed sibling rows keep their
  local models.
- Header debt: the parked codex candidate rebuild/candidates/manual/pending/2026-11-02_codex_pass13/unmatched/00cf0d20 (CEntranceGuard2::Main)
  `#include`s this path. When it is revived it needs CEntranceGuard2 defined as a nested entity class (Ego_r.pdb layout; Alloc 0x00CF0C40;
  vtable 0x012C6174).

## engine/CV_SingingStonesScript.h (new)
- NScript::CV_SingingStonesScript, sizeof 0x9c.
  - Methods: RegisterMain 0x00ED0F50, Main 0x00ED10A0 (landed), Init 0x00ED0EC0, OnPersist 0x00ED1010.
  - Members: CurrentPlayListIndex +0x48, PlayList[4] +0x4c, RudePlayList +0x5c, YourPersonalPlayList +0x6c.
  - Then `std::vector<CScriptThing> EffectList` at +0x7c: STLport is 12 bytes, the PDB says 16.
  - Then `CCharString ListText[4]` +0x88 and DoorManIntroComplete / DoorManAttackedByHero / DoorManHasBribe / DoorManComplete at
    +0x98 .. +0x9b.
  - Cross-checks: the OnPersist 0x00ED1010 literals name the four DoorMan members. CManWithDoorName::Main reads and writes +0x98 .. +0x9b.
- Nested CManWithDoorName (0x1c): Alloc 0x00ED16E0, GetParentScript 0x00ED1780, Init 0x00ED1790, Main 0x00ED17D0; vtable 0x012F2C98.
- Includes stlport/fable_containers.h for the vector.
- Includers: the chunk 804 differ row 00ed17d0. The landed CV_SingingStonesScript::Main 0x00ED10A0 keeps its local model; it could move to
  this header later (header debt, optional).

## engine/CQ_HerosOldHouseScript.h (new)
- NScript::CQ_HerosOldHouseScript, sizeof 0x50.
  - Methods: Main 0x00D89E60, Init 0x00D89DA0.
  - Members: Helping +0x48, Helped +0x49, MissionAborted +0x4a, WifeAttacked +0x4b, WifeAttackedAndLeft +0x4c, BootyDugUp +0x4d.
- Nested CGhostFisherman (0x20): GhostGoing +0x1c, HaveTalked +0x1d.
  - Vtable 0x012D50C0: sdd 0x00D8A630, Main 0x00D8ABF0, Init 0x00D8A560, GetParentScript 0x00D8A550, OnPersist 0x00D8AB70.
  - Alloc 0x00D8A4B0.
- CFishermansWife and CExtraBooty are forward-declared only.
- Includers: src/00d8abf0.cpp (RELOCATION_MATCH with this overlay). No landed row includes it.

## engine/CQ_OpeningGraveyardSecretPassageScript.h
Repo header, used unchanged by differ/00dc4d70.cpp (not copied into hdr/).
