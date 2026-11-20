# NF120 notes (round 10, PRIORITY reconciliation slice 7; 31 queue lines over 30 addresses)

Counts come from APPLIED.tsv / STATUS.tsv / the vland logs. Early hand-back (context): 7 entries are `open` with a probe census below.
- Every REFS row of every entry was compiled once from HEAD (work/probe.tsv, 383 row/target pairs, all RELOCATION_MATCH); work/selfsym.txt
  holds the HEAD self objects' defined symbols. Binder lists below come from that probe, not from WORKLIST (two WORKLIST lines were stale:
  0x00E11DC0 and 0x00E02950 already bind the UAE spellings).
- Claims settled from the evidence column: q6222 (CTCUnholyPower, private, CCharString/bool), q6326 (AddAllyThing is the claim; the
  CTCAllies_Retail spelling is the stand-in), q6450 (protected FinishFishing), q6453 / q6455 (CModeBase nested GetWorld), q6454 (nested,
  virtual OnActivate), q6408 (CCountedPointer<CTCCreatureModeManager::CModeBase> by value).

## Self renames that also rename a non-entry row
- 0x007B0CC0 (q6450 binder) was the generated placeholder CClass_007b0cc0::CallWith_007b0cc0. It is CTCFishing::CancelButtonPressed
  (Ego_r.pdb 0x007B2600 / FableWin.pdb 0x026D9B60): Ego_r orders FinishFishing, ReelInButtonPressed, CancelButtonPressed, GoFishing,
  FrameUpdate; retail has FinishFishing 0x007B0950, ReelInButtonPressed 0x007B0BA0, this 8-byte body (Ego_r's is 16 with padding),
  FrameUpdate 0x007B0CD0; the body is FinishFishing(false). Its only caller 0x00690B20 declared the made-up CastButtonPressed (in no PDB)
  and is respelled (row and test, in place).
- 0x004C7CF0 CThing::OnKill: QAE -> MAE (both PDBs, protected virtual). Its only REFS row is 0x00664370, which moved onto the shared
  headers and so binds MAE; the self row's local model is respelled in place (virtual, the vptr takes the first pad word).
- 0x00664370 CThingCreatureBase::OnKill: the HEAD object defined QAE; now UAE (both PDBs), the spelling its only REFS row 0x00830D80
  already binds.

## Callee bindings moved in passing (RENAMES section 2; each checked against REFS)
- 0x0099EAE0 (~CCharString), 0x00662110 (MakeNoise), 0x0066BD00 (PlayAttachedCriteria), 0x0075C280 (TransformVector), 0x0075F830
  (DeleteData; only REFS row 0x0075FA30, its self leaf is that spelling), 0x00760C40 (GetTrackPosAtLengthFraction QBE: both REFS rows
  0x00760DA0 / 0x0087B910 now bind its self leaf), 0x00751E80 (GetTurningAngles: 0x007525C0 now binds the QAE_NAAM0 the self row defines;
  both PDBs say AAE: NAME_FIXES).
- 0x0073E6A0: 0x0073AE70 now binds the _List_base clear spelling 0x0073AEA0 / 0x0073B0D0 already bind (4 spellings -> 3; nothing added).
  The self row stays staged (q3942 blocked).

## q3942 blocked (family)
- 0x00742260 and 0x00743220 are descriptive std::list_T_xxx destructors that call 0x0073E6A0 as `clear@list_T_xxx`. The two bodies are
  byte-identical; the Fable.exe scan finds no absolute or relative reference to either. Without a caller nothing proves which STLport name
  each carries, so neither can move onto stlport/fable_containers.h. The self row is staged: work/0073e6a0_ready.cpp (explicit instantiation
  of _List_base<CCountedPointer<CTCCreatureModeManager::CModeBase>>::clear, RELOCATION_MATCH).

## Open entries: probe census (resume here; no re-probe needed)
- q4747 0x00733480: self defines ?GetPMeleeFlourishAbility@CTCCombat_Retail@@QAE?AUCDefPointer_MeleeCombatAbilityDef@@XZ (stand-in). Its one
  REFS row 0x0064BE45 binds ?GetPMeleeFlourishAbility@CTCCombat@@QAE?AV?$CDefPointer@VCAbilityDef@@@@XZ over local CDefPointer / CTCCombat /
  CThing models. The same row also declares IsShowingBaseAbility (0x00733290) and GetPBaseAbility (0x007334B0), which are in no PDB, and a
  non-const IsMeleeFlourishValid (both PDBs: QBE). It is a port onto engine/CTCCombat.h + CDefPointer.h.
- q4802 0x0075B8D0: the self row already defines the claim. 21 REFS: 6 bind it (0075ba30 00a58130 00b13530 00b149d0 00b211b0 00b22b40);
  9 bind ??4?$CArray@VC3DVector@@@@QAEAAV0@ABV0@@Z (00a6cb20 00b20fd0 00b21090 00b21160 00b21dd0 00b21e90 00b22100 00b22310 00b22530: check
  whether that comes from engine/CArray.h (a fold, not-a-binder) or a local model); 4 bind ??4CElement12_0075b8d0@@ (00764e90 00bcd850
  00bcd8f0 00bcd950), 1 ??4CPointVector@@ (00762e10), 1 ??4CKeyVector@@ (00b20b60): stand-ins (the 0x007744A0 std:: transformer).
- q6326 0x0076D270: self and 0x0079ACC0 bind the claim. Work left: 0x0089C100 only (CTCAllies_Retail / CThing_Retail stand-ins; needs
  engine/CTCEnemy.h with AddAllyThing / RemoveAllyThing opt-ins; RemoveAlly 0x0076C850 is RemoveAllyThing, NAME_FIXES).
- q6408 0x0073ACC0: self defines ?AddMode@CTCCreatureModeManager@@QAE_NUCModePointer@@@Z (stand-in). 34 REFS: 9 bind the claim; 16 bind
  ?AddMode@CTCCreatureModeManager@@QAE_NV?$CCountedPointer@UCModeBase@@@@@Z (00848730 00848db0 008c0c00 008c0d10 008c1e80 008c1f70 008c48a0
  008c7600 008cb060 008cb1a0 008ccd70 008ed950 and 4 more in work/probe.tsv); 5 bind ..UCCountedModePointer@@ (008c1c30 008f47e0 008f4b00
  00950c30 00969cb0); 1 ..UCCountedPointer_CModeBase@@ (00848fd0); 1 ..CModeBase@NCreatureMode (008676a0); 1 CreatureModeManagerMethods
  (008a08a0); 1 CTCCreatureModeManager_Retail (008a1a10). A family: the self rename waits for all 25.
- q6453 / q6455 0x0073A680: the self row already defines the claim (binder-only). 18 REFS: 5 bind it (008db810 008dd7e0 008e0060 008e7f80
  008eacb0); 13 do not: 3 ?GetGlobalComponents@CModeBase@@ (00862ec0 008e7b90 008e9850), 3 ?GetWorld@CModeBase@@QBEPAVCWorld@@XZ (008d6580
  008d6ac0 008d8ea0), 2 ?GetWorld@CModeBase@@QAEPAVCWorld@@XZ (008621a0 008ea500), ModeIceTroll (008d7630), CModeDragonStrafeBurninate
  GetDisplayEngine (008dc900), CModeJackOfBladesFlash (008dcb60), NCreatureMode::CModeBase (008ddd70), ModeBriar (008e7cf0).
- q6454 0x0073AAC0: the self object defines ?OnActivate@CModeBase@CTCCreatureModeManager@@QAEXXZ (non-virtual; claim UAE). 15 REFS: 1 binds
  the claim (008eacb0); 10 ?OnActivate@CModeBase@@QAEXXZ (008d6580 008d89c0 008d9480 008d9e30 008da2e0 008dbb30 008dc600 008de6a0 008e7290
  008e7390); 2 ?OnActivate@CModeBase@@UAEXXZ (008621a0 008d8ce0); _ForwardTarget_0073aac0 (008d6760); BaseModeNative (008e9a30).
- The CModeBase rows overlap across q6408 / q6453 / q6454: port per row (recipe 0x008EACB0, engine/CTCCreatureModeManager.h CMODEBASE_VIRTUALS).

## Descriptive names
None new. Library spellings (descriptive-by-library, STLport 4.6): the staged 0x0073E6A0 _List_base::clear; 0x007744A0 binds the STLport
map<long, CShopItem>::operator[] the self row 0x00778280 defines.

## Flags lines
None added. Every row grades with its build entry's CompilerFlags (0x00690B20 keeps /GS from its entry).

## Funclets
No EH funclet row includes any changed row (grep of rebuild/src/compiled for every changed file name found none). Self renames move these
parent files, none with funclets: 0x0073E970, 0x00789A50, 0x00789A70, 0x007940F0, 0x0075C160 / 170, 0x0075CCB0 / CF0, 0x0074B900, 0x0075FA30,
0x007525C0, 0x00751B30, 0x007528A0, 0x0079F810, 0x007B0950, 0x007B0CC0, 0x00760DA0, 0x007E7360 / 90 / D0 / 7410, 0x0073AE70, 0x00664370,
0x004C7CF0.

## Tests
Only 0x00690B20's test changed (CastButtonPressed -> CancelButtonPressed in its own model). Every other changed row's test is a standalone
model that links nothing from the row; kept.

## Rule slips
- The first vland batch was started with a trailing `&` inside a background call; it ran to completion (log complete), no overlap with
  another compile.
- One RENAMES section-2 line (0x0073AE70 AddMode, old = new) was written by mistake and removed before hand-back.

## Final checks
- recheck_final.log: one vland --recheck-landed run over the 36 rows/ rows: 8 MATCH + 28 RELOCATION_MATCH, every test PASS.
  selfcheck.log: 0 problems.
- WARNs reviewed: 98 SHARED_HEADER_AVAILABLE on local models the landed rows already had (in-place respells); 2 STANDIN_LEAF on the
  section-1 renames whose catalog name changes at landing (0x0074B900, 0x007940F0); 9 RELOC_NAME: 0x00690B20 -> 0x007B0CC0 is the
  section-1 rename (CancelButtonPressed), the other 8 are callee bindings these rows had at HEAD (unchanged by NF120, pdiff); 2 RELOC_ALIAS
  (import thunks).
