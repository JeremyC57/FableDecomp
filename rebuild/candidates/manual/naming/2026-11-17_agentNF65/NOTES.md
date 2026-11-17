# NF65 notes (round 7, finishing NF63's blocked/port slice: 60 rows)

Work dir: $S/w22/agentNF65 (grades in pS/, logs in work/, grade ledger grades.log). Helpers copied from NF63 (g.sh private q.py grade
with the overlay and catalog flags + cpp precheck; vl.sh / pay.py vland recheck over rows/; fvl.sh / fpay.py EH-funclet recheck with
the parent staged under src/compiled/), plus st.py (STATUS.tsv upsert, APPLIED.tsv rebuilt from entries.tsv), readme_gen.py
(hdr/README.md with a default-path cpp token check), mkren.sh (RENAMES.tsv = ren1.tsv + `# Section 2` + ren2.tsv).

## Summary (counts from STATUS.tsv / APPLIED.tsv / recheck_final.log)
- Rows: 16 ported (recheck_final.log: 21 RELOCATION_MATCH + PASS = the 16 rows + 5 EH funclets of changed parents), 42 blocked/port,
  2 blocked/authoring (0x00C21C20 / 0x00C12940: an evidence gap, not a port), 0 not-a-binder, 0 deferred.
- The 44 blocked rows are NOT all terminal in the brief's sense (15 grades or a header default-path blocker). Three groups:
  - real NF65 attempt: 0x00869CA0 (NF58's ready re-author graded RM, held for its caller 0x00869020);
  - inspected, with the concrete opt-ins / header decisions they need written into STATUS.tsv but 0 NF65 port grades (12): 0x008F7950,
    0x00915190, 0x00979F70, 0x009C8890, 0x009F6890, 0x00A79A30, 0x00B9E950, 0x00C08D40, 0x00C49820, 0x00869020, and 0x00C21C20 /
    0x00C12940 (evidence gap); plus 0x00921440 / 0x00966290 (2, not attempted: they share 0x00915190's chain);
  - not reached (29): their STATUS line repeats the earlier agent's blocker and NF63's draft error; they did not get the 15-grade treatment.
- Entries (APPLIED.tsv): 11 done, 1 rejected (nf44:120), 38 blocked (nf44:117 / nf41:e101: binders done, only the self row outside the
  slice is left).
- Self renames (RENAMES.tsv section 1, 10): 0x00B3FF70, 0x00C1F4E0, 0x00C1EEE0, 0x00C16D90, 0x00C17400, 0x00B42750, 0x009EFB80,
  0x009EF750, 0x00972490, 0x008DF3A0 (access-only, catalog name unchanged). Section 2 (50 lines) is generated from relocation dumps of the
  original and the ported row objects (work/xo_*.txt / work/x_*.txt), not written by hand.
- Locks: the 16 ported rows stay claimed (NF65); the 44 unchanged rows' locks were released.

## Rows ported (method: find the landed sibling already on the shared headers and reuse its macro block)
- 0x008909F0 CGameScriptInterface::EntityGetOpinionOfPlayer: onto CScriptThing.h / CWorld.h / COpinionReactionManager.h
  (FABLE_RETAIL_GREETING_PASS23 declares GetThingOpinionOfPlayer); no overlay needed. nf44:95 done.
- 0x008DF3A0 CModeScorpionKingVulnerable::Recover: the sibling FrameUpdate 0x008E0A10's block; self now private AAEXXZ (q10496: the Leaf
  line was right, the row did not emit it). CCreatureAction_ScorpionTailPlunge has no shared header: a local class with the PDB ctor.
  0x0070D510 is declared through overlay opt-in FABLE_RETAIL_CANIMATETHING_CLEARQUEUES under the landed catalog name ClearQueues
  (DESCRIPTIVE: retail 0x0070D510 clears CCompleteAnimation's TransitionAnimSets (+0x58) then CombinationAnims (+0x64); Ego_r has
  ClearTransitionAnims 0x00577D10 / ClearCombinationAnims 0x00577CF0 separately, and no Ego_r / FableWin binary is on disk to prove
  which one the retail body is). The landed self row 0x0070D510 still emits the placeholder Pair::Destroy (not a NAME_FIX: no proven name).
- 0x00B3FF70 CEngineWorldMap::CreateNewBankMap: [self rename, access-only] private AAE in both PDBs; overlay opt-ins type the retail
  StaticMapFile member and the CStaticMapBankFile / CStaticMapBankStateBlock classes; the counted pointer is the FableWin pair
  (CCountedPointer<CStaticMapBankStateBlock> local, converted to the by-value CCountedPointer<CBankStateBlock>); NAME_FIXES 0x00B430A0.
- libpk event triggers (CItemAction = libpk/CBankProcessor.h CEventTriggerRef): 0x00C1F4E0 [self rename InsertDriver], 0x00C1EEE0
  [self rename ctor], binders 0x00C2E7A0 / 0x00C2E4D0; 0x00C2E190 AddSample (q10484). q10488 / q10489 / q10484 done.

- 0x0085F6F0 NCreatureMode::GetUpFromSleeping: the 0x0092DA20 macro block (CThing PeekTC, CCreatureAction_GetOutOfBed by reference);
  CModeSleepBed / CBrainOutput stay local (no shared header). nf41:e101 done.
- 0x00B42750 CEngineWorldMap::OpenStaticMaps [self rename, private]: overlay opt-ins on CEngineWorldMap.h (typed StaticMapBankName /
  StaticMapOpenState, private members), CEngineMap.h (OpenDevelopmentStaticMap), CEngineWaterRenderer.h (a declaration on the flattened
  default class). 0x00B420F0 is Ego_r.pdb ?OpenStaticMapsUsingCommonRetailHeader@CEngineWorldMap@@AAEXJ@Z (private, long; Ego_r 0x009E75F0):
  OpenStaticMaps calls it with the index of CommonStaticMapHeaderEntryName; was the manifest OpenMapFile (0x00B420F0 is not a landed row).
  q10425 done.
- 0x00B86C00 CEngineScreenEffectDisplacementRenderer ctor: overlay opt-in types OldRenderTarget on engine/CRenderTarget.h (exists now; the
  earlier note said none); FABLE_RETAIL_CSURFACE + CRenderStateCaptureBlock INLINE_CTOR give retail's inline member construction. q10423 done.
- 0x009EFB80 / 0x009EF750 (map<CCharString, CCountedPointer<CConsoleInputBase> >): NF58's ready 0x009EF750 re-author is still on disk
  ($S/w22/agentNF58/work/ready/) and grades RM; 0x009EFB80 went RM with the untried form once the real 0x00429950 body was written as an
  explicit specialisation under auto_inline(off) WITH the compare loop inline (a body that calls the out-of-line static Compare stays
  DIFFER 202v254: the call makes the key look clobbered). No fable_string.h change is needed. nf58:0x009EF750 done.
- 0x00972490 CAIStateGroup_JackOfBladesStage2::OnActivate [self rename from the JackMethods stand-in]: the sibling Update 0x00974AA0 block
  + CWorld COMBATMANAGER + CThing GETTC_EXPANDED + overlay FABLE_RETAIL_JACK_STAGE2_ONACTIVATE. nf41:e15 done.

## EH funclets of changed rows (self renames move the parent file: repoint these includes)
- 0x01212915 / 0x0121291D / 0x01212928 -> 0x00C1EEE0, 0x01211DC1 -> 0x00C16D90, 0x01211E45 -> 0x00C17400 (all RM + PASS in
  recheck_final.log against the rows/ parents). Not changed (their parents stay blocked): 0x0121176F (0x00C12940), 0x01210A94..0x01210AF0
  (0x00C08D40), 0x01215BDD..0x01215C19 (0x00C49820).

## Header issues for the coordinator (not changed here)
- 0x00C101D0 is bound under three spellings: ?Find@CNamedSet@@QAEPAVCItem@@ABVCString@@PAJ@Z (landed 0x00C2E610, sees only
  libpk/CBankProcessor.h's `class CItem;`), ?Find@CNamedSet@@QAEPAUCItem@@ABVCString@@PAJ@Z (my 0x00C1EEE0 / 0x00C2E7A0, which see the
  libpk/CItem.h definition `struct CItem`; VC7.1 mangles by the definition's class key) and the old ?Find@CItemDispatcher@@ forms.
  libpk/CBankProcessor.h's forward declaration should read `struct CItem;` (then 0x00C2E610 follows). Separately, CNamedSet
  (CBankProcessor.h) and CItemDispatcher (libpk/CItem.h) model the same registry object (Find by name 0x00C101D0 reads the +0x10 name
  index; FindItem 0x00C10140 the id list): one class should own both. Until then 0x00C1EEE0 passes its CNamedSet* to CItem::Attach
  (slot 6, CItemDispatcher*) through a reinterpret_cast (a virtual call: no relocation). Listed under rule slips as a judgement call.
- Staging proof (advisor check, no `#error`): a kept vland work dir for 0x01212915 showed the funclet object 01212915.obj carrying
  CEventTriggerRef symbols (2) and no CItemAction (the staged parent compiled), while the standalone test object carries the old name.
- 0x00C16D90 / 0x00C17400 (same TU): the rows ARE now the landed 0x00C17640 OpenBankFile TU (on libpk headers), with two changes:
  0x00C17940 is called as the landed PKSerialiseLoad<CBankData> template (the TU's ReadMetadataData is the rejected nf44:120 claim), and
  the real 0x00C17740 CBankDataOwner::GetChecked body is defined in the TU (VC7.1 then keeps the owned pointer in edi across the call,
  as retail; the landed 0x00C17640 TU does not need it). Self names follow that TU: ProcessBankEntry / ReadMetadataBank (RENAMES 1).
  Funclets 0x01211DC1 -> 0x00C16D90 and 0x01211E45 -> 0x00C17400 regraded RM + PASS (repoint their includes).
  0x00C16CE0's own row (RunProcessor, outside the slice) and the caller 0x00C174A0 follow via NAME_FIXES.tsv.

## Header changes the coordinator must make (blocked rows that wait on a header default path or an identity decision)
- libpk/CBankProcessor.h: `class CItem;` should read `struct CItem;` (libpk/CItem.h defines it as a struct; see above), and one class should own
  the registry object that CBankProcessor.h calls CNamedSet and libpk/CItem.h calls CItemDispatcher (needed before 0x00C08D40, whose lock
  pairs are this 0x20-byte object, can move).
- engine/NGameText_CDataBank.h FABLE_RETAIL_NGAMETEXT_GROUPENTRY: CGroupEntry derives CIVCountedPointeeBase, but 0x009C8890 touches the group
  as a CResource (LRU list walk); a base change there is the coordinator's (0x009C8890 blocked).
- libpk/CCodaFrameStreamer.h: no base class for vtable 0x012B80AC (landed rows call it CVtable_012b80ac, a placeholder): a descriptive base name
  is an identity decision (0x00C49820 blocked).

## Blocked rows
STATUS.tsv carries each blocked row's exact blocker. Rows inspected this pass got an NF65 inspection note with the concrete header opt-ins
they need (0x00C49820, 0x00C08D40, 0x00A79A30, 0x00B9E950, 0x009C8890, 0x008F7950, 0x00979F70, 0x00915190 (+ 0x00921440 / 0x00966290),
0x009F6890, 0x00869020 / 0x00869CA0). 29 rows were not reached: their STATUS line repeats the earlier agent's blocker and NF63's draft error,
with 0 NF65 grades (each has an earlier grade: NF63's draft, or for 0x006A0150 NF59's probe and for 0x00C08D40 NF53's probe).
Nearest next ports: 0x009627C0 / 0x00961880 (nf41:e106: a retail CAIStateGroup_IceTrollIcicleAssault / BanditKingLunge class opt-in over
CAIStateGroupBase, CTCStealth::SetAsInDanger, CCreatureAction_IceTrollIcicleAssaultInto.h exist), the TakeCover trio 0x00979F70 / 0x009790F0 /
0x0097A630, and 0x00C49820 once its base class is named.

## Rule slips / judgement calls
- 0x00C1EEE0 passes its CNamedSet* to CItem::Attach (CItemDispatcher*) through a reinterpret_cast: two shared headers model the same object
  under two names; a virtual call, so no relocation depends on it. Listed for the coordinator, not hidden.
- A note I wrote mid-pass said "21 ported rows" (21 counted the funclets); corrected to 16 in STATUS.tsv before hand-back. Grade counts in
  STATUS.tsv were first written from memory; rebuilt from grades.log before hand-back.
- No #error, no __asm / naked / _emit, no git, nothing written under /home/user/FableDecomp; every compile ran under timeout, one at a time.
