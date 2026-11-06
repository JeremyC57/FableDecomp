# Chunk 797 header overlay

Seven headers. Three are new and four are fresh copies of the repo header with an opt-in branch added. Every change is additive:
`diff rebuild/include/engine/<h> hdr/engine/<h>` has no removed line, and every added line is inside `#ifdef FABLE_RETAIL_<...>`.
Only chunk 797 rows define those macros, so existing includers compile the default text unchanged. I rechecked the 18 landed
siblings and includers with this overlay: all MATCH or RELOCATION_MATCH plus PASS (SIBLINGS.tsv, log tmp/recheck.log, deleted at
cleanup).

## New headers (no includer before chunk 797)

All three need FABLE_RETAIL_NSCRIPT, else `#error`.

| header | class | size | evidence |
|---|---|---|---|
| engine/CQ_WhiteBalverineWWScript.h | NScript::CQ_WhiteBalverineWWScript : CLevelScriptBase<CQ_SunnyvaleMasterData> | 0x5c | Ego_r.pdb layout (112 bytes) at PDB - 0x14. The landed ctor 0x00E18440, Init 0x00E18480 and MonitorBalverine 0x00E18EB0 use these offsets, and so does Main 0x00E18630 (RELOCATION_MATCH). Vtable 0x012E0CE4. |
| engine/CV_BanditCampPathScript.h | NScript::CV_BanditCampPathScript : CLevelScriptBase<CQ_SunnyvaleMasterData> | 0x70 | Ego_r.pdb (136 bytes) at PDB - 0x14, then a further - 4 after the 12-byte STLport vector Bandits. Main 0x00E46D30 is RELOCATION_MATCH. |
| engine/CQ_NewOakValeIntroScript.h | NScript::CQ_NewOakValeIntroScript plus the modelled nested CNOVI_Villager (0x20) | 0x10c | See the details below. |

CQ_NewOakValeIntroScript.h in more detail:
- Layout is Ego_r.pdb (320 bytes) at PDB - 0x14, with a further - 4 after each of the eight STLport vectors.
- The class is `#pragma pack(push, 1)` with the PDB's alignment gaps written out (_pad49 / _pad97 / _pad101). The reason is that the PDB puts the two C3DVectors at odd offsets (retail +0x76 / +0x85) while the longs stay 4-aligned.
- These retail functions use the offsets: the ctor 0x00DAAC00, AddBadDeed 0x00DAEA70, GetVillagerSpeechIndex 0x00DAEBF0 and the 00DADF80 row. The factory 0x00DBEF70 allocates 0x10C.
- `enum EBadDeeds {}`: its enumerator names are not in the PDB set, so callers cast the value.

## Opt-in branches added to repo headers

**engine/CQ_AmbushTradersScript.h: FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_CONVOYMEMBER** (with ..._MAIN for the class part)
- The retail TeleportData struct (0x1c) with its ctor 0x00F0D470 / dtor 0x00CDBC50 declared only.
- The nested CAT_ConvoyMember entity script (0x1c): Alloc 0x00F04280, GetParentScript 0x00F04320, Main 0x00F0CAA0, Init 0x00F08C30.
- Evidence: Ego_r.pdb CQ_AmbushTradersScript::TeleportData and CAT_ConvoyMember, entity vtable 0x012F7DF8, and the Main 0x00F0CAA0 bytes.
- Includers rechecked: 00cf9610, 00cf9a50, 00f024c0 (default path).

**engine/CQ_BountyHuntScript.h: FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_GANGDEPUTY2**
- TeleportData() 0x00D15140 / ~TeleportData() 0x00CD4550 declared.
- WatchForDeputy2GruntsKilled 0x00D19AF0.
- Class CBH_GangDeputy2 (0x1c): Alloc 0x00D161A0, GetParentScript 0x00D16240, Main 0x00D18DF0, Init 0x00D169E0, OnPersist 0x00D16250, OnPredicateFail 0x00D16260.
- Evidence: Ego_r.pdb names, plus the vtable stored by the landed Alloc.
- Includers rechecked: 00d17470, 00d13130.

**engine/CQ_MinionClifftopChaseScript.h: FABLE_RETAIL_CQ_MINIONCLIFFTOPCHASESCRIPT_MAPKEEPER**
- Class CMapKeeper (0x2c): Alloc 0x00DA4250, GetParentScript 0x00DA42F0, Main 0x00DA91A0, Init 0x00DA4350, OnPredicateFail 0x00DA4300.
- Members: ParentClass +0x14, PMasterData +0x18, DoneIntro +0x1c, ImprisonedFX +0x20.
- Evidence: Ego_r.pdb CMapKeeper (44 bytes), and the offsets the landed Init and OnPredicateFail use. Entity vtable 0x012D73E0.
- Includer rechecked: 00da34b0.

**engine/CScriptGameResourceObjectMovieBase.h: FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE**
- Adds a `__forceinline` empty ctor and a `__forceinline virtual` empty dtor, under `_MSC_VER` only.
- Only the parked row 00D18DF0 uses it. In that 3.3 KB body VC7.1 otherwise calls the implicit pair's COMDAT copies 0x006E7B60 / 0x006E7B80, while retail expands both.
- The ctor half is byte-exact.
- The dtor half is the parked residue: a user-declared dtor resets the MovieBase vtable, and retail's compiler-generated one does not. Landing may keep only the ctor half (then the row grades 14 diff lines, not 10) or hold the opt-in until 00D18DF0 matches.
- 75 landed includers. None defines the macro, so they are not rechecked one by one.
