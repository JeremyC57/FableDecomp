# NF163 notes (round 10: NScript quest script naming, CQ_/CV_ N-Z)

## q4053 (0x00E01710 ~CDarkwoodTrader)
- Self row already right. Its one referencer 0x00E016F0 (exe scan) was a free stand-in; re-authored as
  `??_GCDarkwoodTrader@CQ_TraderEscortScript@NScript@@UAEPAXI@Z` on engine/CQ_TraderEscortScript.h (RELOCATION_MATCH, test PASS, test file
  unchanged: it still exercises its own standalone copy, as the landed sibling ??_G 0x00D6A0B0 does).
- Form: the PDB ctor (opt-in FABLE_RETAIL_CQ_TRADERESCORTSCRIPT_DARKWOODTRADER_CTOR) is defined `inline` in the row and referenced by an
  anonymous-namespace placement-new helper (`// Fidelity: scaffolding, not retail`, as 0x009566C0), so the vtable, ??_G, ??1 and ??0 are all
  pick-any COMDATs. A qualified call of the implicit dtor alone emits no vtable (tried, work/00e016f0_f5.cpp).
- **Coordinator decision needed (one definition of ??0CDarkwoodTrader):** VC7.1 does not inline the ctor into the helper, so the object
  emits ??0 as a pick-any COMDAT. The owner 0x00E01670 defines it strongly, so a link gives LNK2005 (link/ctl_e016f0.log). HEAD already
  has the same clash: 0x00E01710 also defines ??0 strongly (link/ctl_head.log: HEAD 0x00E01670 + HEAD 0x00E01710 -> LNK2005, exit 145).
  The fix is on the owner (nf88 / nf140: re-author 0x00E01670 on the shared body so it emits pick-any, and port 0x00E01710 onto the header
  the same way); not done here (0x00E01670 is outside my locks, 0x00E01710 needs no naming change).
- Pre-existing, not changed: 0x00E01710 and the wave-33 ??_G / Alloc siblings in 00/e0 still model CQ_TraderEscortScript locally although
  engine/CQ_TraderEscortScript.h exists.

## q4631 / q11756 / q11748 (CPersistContext::Transfer<ArcheryOpenState> family)
- 0x00CDCC20 (self) now instantiates the shared member template body of engine/CPersistContext.h (FABLE_RETAIL_CPERSISTCONTEXT_TRANSFER_BODY with
  the function-scope traits variant FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE, which is the shape the landed row had) for the PDB enum
  NScript::CQ_SunnyvaleMasterData::ArcheryOpenState; first such instance row on the shared body. RELOCATION_MATCH in one grade.
- Family (moved whole): 0x00CDCD00 TransferBinaryIn (outside my entries; it is 0x00CDCC20's binary-load callee, so leaving it would have left
  0x00CDCD00 bound under two spellings) and 0x00CDCDB0 TransferOut (q11748). 0x00CDCD90 TransferIn was already ported by NF136.
- Side effect, intended: 0x00CDCC20's object emits the component vtable 0x012C2ECC; at HEAD its slots named the EArcheryState stand-ins
  (TransferIn / TransferOut of a local model) while the landed 0x00CDCD90 defines the PDB TransferIn. The new vtable names the PDB TransferOut
  (0x00CDCDB0, this hand-back) and TransferIn (0x00CDCD90), so the vtable-slot split closes.
- 0x00CDCD00 keeps the row-local CDataInputStream / CMemoryDataInputStream read model it had at HEAD (same tokens; the landed 0x007AD7F0
  sibling has the same); porting the read onto engine/CMemoryDataInputStream.h is a separate shape question, not a naming one.
- 0x009E2560: the existing multi-spelling split (7 spellings in reloc_bindings) does not grow: 0x00CDCDB0 moves from the stand-in
  ?TransferOut_009e2560@@YIXPAVCPersistContext@@JPAJ@Z to the PDB ?TransferOut@CPersistTraitsSLONG@@SIXAAVCPersistContext@@AAVCCharString@@ABJ@Z.

## Resumed session (2026-11-20, after the container restart)
- The 8 rows the first session finished (rows/ 00cdcc20 00cdcd00 00cdcdb0 00dcf6e0 00dfc630 00dfc920 00e016f0 00edbd10) each had a
  STATUS line and a complete vland log (work/b1..b3.log, RELOCATION_MATCH PASS); not regraded before the final recheck. The overlay
  CQ_NewOakValeIntroScript.h had been copied but not edited. No landed EH funclet (rebuild/src/compiled/01/2*/*Unwind*) includes any
  of the 11 changed rows' files, so no funclet regrade and no funclet repointing is needed for the self renames.

## q6348 / q6463 / q6474 (AddBadDeed 0x00DAEA70: long -> EBadDeeds)
- Census (exe scan REFS.tsv): 10 landed referencers. 9 call `ParentClass.AddBadDeed(...)` on engine/CQ_NewOakValeIntroScript.h, whose
  only declaration is `AddBadDeed(EBadDeeds)`: already the PDB spelling. The 10th, 0x00DBCD00, was a July stand-in binding
  `?AddBadDeed@CQ_NewOakValeIntroScript@@QAEXJ@Z`.
- 0x00DAEA70 (self, signature change: both PDBs) ported onto the shared header; 1 grade, RELOCATION_MATCH. The row-local
  `CQ_NewOakValeIntroScript : CScriptBase` model (GSI +0x40) is gone; `Game` is CLevelScriptBase<T>::Game (+0x40).
- 0x00DBCD00 ported onto the header's existing FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_BULLY; 1 grade, RELOCATION_MATCH. This is also a
  self rename (`?GivenTeddy@CNOVI_Bully@@QAEXXZ` -> the PDB private nested `?GivenTeddy@CNOVI_Bully@CQ_NewOakValeIntroScript@NScript@@AAEXXZ`)
  whose only referencer 0x00DBB310 already binds the PDB spelling, so it closes an existing split. Slot 126 / 125 = GiveHeroGold(1) /
  TakeObjectFromHero (engine/CGameScriptInterfaceBase.h); +0x70 / +0x91 = SpokeAboutFindingTeddy / TeddyRuined (header layout).
  Catalog module for 0x00DBCD00 moves from `CNOVI_Bully` to `NScript::CQ_NewOakValeIntroScript::CNOVI_Bully` (RENAMES s1).

## q6349..q6364, q6475..q6490, q6502 (Alloc family) -> Main 0x00DABAC0
- All 16 Alloc self rows already define the PDB `?Alloc@CNOVI_<Name>@...` / `?Alloc@COVI_DeadFather@...` leaves. The only exe-scan
  referencer of each is Main 0x00DABAC0, which bound 16 stand-in nested structs (`ALLOC(CLiveFather)` ...).
- Renaming the stand-ins in place (CLiveFather -> CNOVI_LiveFather) would rename a stand-in to a header class (nf95b), so the row was
  ported onto fable_string.h + engine/CEntityScriptBinding.h + engine/CQ_NewOakValeIntroScript.h, with the preamble of the landed
  CV_ChapelOfEvilScript::Main 0x00E693F0. The five classes the header lacked go in the new overlay opt-in ENTITIES3 (hdr/README.md),
  not in the row (the 0x00E693F0 row-local nested-class definitions are doubtful under the 11-20 local-model rule).
- 1 HEAD control compile + 1 form: RELOCATION_MATCH; the object's defined-symbol set is identical to the HEAD object's (no new
  COMDAT, no one-definition change). Self leaf unchanged: HEAD already emitted `?Main@CQ_NewOakValeIntroScript@NScript@@UAEXXZ`
  (CScriptBase's virtual Main); a Leaf line is added. No landed row references 0x00DABAC0 (exe scan).

## q6472 (AddGoodDeed 0x00DB0660): done-already
- Self row already the PDB leaf; 0x00DB0600 probed by compile (binds the NScript spelling through the header); 0x00DB2B10 / 0x00DB5330
  / 0x00DBB310 call it through the header's one declaration. reloc_bindings.tsv no longer carries the non-NScript line the claim cites.

## Tests
- The tests of 0x00DAEA70, 0x00DBCD00 and 0x00DABAC0 are unchanged HEAD standalone behaviour models (they do not link the row);
  the 0x00DBCD00 and 0x00DABAC0 tests model CCharString / CScriptThing / the script locally (pre-existing, not new). Rewriting them on
  the shared headers is a separate test task, not a naming one.

## Pre-existing local models left in place (not naming)
- 0x00DB0660 (AddGoodDeed) and the 16 Alloc rows still model CQ_NewOakValeIntroScript locally although the header exists; each is
  already on its PDB spelling.

## Descriptive names
- None added. Every name in this hand-back is a PDB spelling.

## Flags lines
- None added (the three new rows take their catalog flags: none).

## q4134 (0x00DCF6E0 ??_GCTeamSpawn): coordinator decision needed (one definition of ??0CTeamSpawn)
- 0x00DCF6E0 uses the 0x00E016F0 recipe: the PDB ctor defined `inline` in the row and referenced by an anonymous-namespace placement-new
  helper (`// Fidelity: scaffolding, not retail`), so ??0 / ??1 / ??_7 / ??_G are pick-any COMDATs (objdump -h: the ??0 section is
  LINK_ONCE_DISCARD).
- The landed ??1 row 0x00DCF700 (NScriptCQOrchardFarmRaidScriptCTeamSpawn_CTeamSpawn_00dcf700.cpp, line 61) defines the same PDB
  `??0CTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z` out of line (strong), while the
  ctor's address owner 0x00DCD160 is still the global stand-in `??0CTeamSpawn@@...` (NAME_FIXES). So a link of the new 0x00DCF6E0 object
  with 0x00DCF700 would give LNK2005 (pick-any vs strong), the clash link/ctl_e016f0.log proves for the DarkwoodTrader pair. Unlike that
  pair, HEAD has no such clash here (only one strong PDB ??0 today). NOT link-proved for this pair (link/ covers 0x00E016F0 only).
- The fix is on 0x00DCF700 / 0x00DCD160 (outside my locks): port 0x00DCD160 onto the header under the PDB ctor and make 0x00DCF700's
  ctor copy pick-any (nf88 / nf140). If the coordinator prefers, hold 0x00DCF6E0 until then; its entry q4134 is otherwise done.

## q6266 (0x00DFC320 UpdateLiveEnemies)
- 0x00DFC630 and 0x00DFC920 were respelled in place inside the local CQ_TraderConflictGoodScript model they had at HEAD (same class
  token; the advisor's in-place rule). The vland WARN SHARED_HEADER_AVAILABLE lines (CQ_TraderConflictGoodScript, and CScriptBase on
  0x00DFC920) name those HEAD local models, not new ones (selfcheck: 0 NEWLOCAL). Porting the two rows onto
  engine/CQ_TraderConflictGoodScript.h is a separate shape task.

## q6269 (0x00EDBD10 WatchForQuestInfoDisplayConditions)
- Leaf-only self rename inside the HEAD local CV_TempleOfLightScript model (WARN SHARED_HEADER_AVAILABLE = that HEAD model).
- vland WARN STANDIN_LEAF ("catalog NScript::CV_TempleOfLightScript::WatchForQuestInfoDisplay; names another function") is the catalog
  label this hand-back renames (RENAMES s1); it clears when the coordinator applies the catalog rename. Catalog grep: no other address
  carries `CV_TempleOfLightScript::WatchForQuestInfoDisplayConditions` (the hits are 0x00E69F90 CV_ChapelOfEvilScript and 0x00D17DA0
  CQ_BountyHuntScript, other classes), so the rename creates no second address for the name.

## For whoever authors the parked Mains 0x00DB1F00 (CNOVI_AffairWoman) / 0x00DB3FA0 (CNOVI_BookTrader)
- The overlay opt-in FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_ENTITIES3 now defines both classes (and Barrel / CreatedBeetle / DeadFather);
  use it rather than a second definition once it lands.

## Rule slips
- q6472 (AddGoodDeed) was closed after the Alloc entries q6475..q6502 (queue order puts it before them); the Alloc entries were all
  served by one row (0x00DABAC0), worked as one family.
