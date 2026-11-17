# 2026-11-17 landing nf65: name-fix round 7, NF65 (16 rows: engine-side ports NF63 did not reach)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent
to speed up naming queue completion".

NF65 took the 57 engine-side rows NF63 had only drafted, plus three rows the coordinator had set aside by mistake (60 rows). It
ported 16 and ran out of time. Of the other 44, 12 were inspected (the opt-ins or header decisions each needs are in STATUS.tsv), 1
had a real attempt and 31 were not reached. The 39 that are not coordinator decisions are split between NF67 (21 rows) and the
next port agent. Brief, list and records: `naming/2026-11-17_agentNF65/`.

## Rows (60; STATUS.tsv)

| Status | Rows |
|---|---|
| ported | 16 |
| blocked | 44: 42 blocked/port, 2 blocked/authoring |

**Ported:** 0x0085F6F0, 0x008909F0, 0x008DF3A0, 0x00972490, 0x009EF750, 0x009EFB80, 0x00B3FF70, 0x00B42750, 0x00B86C00, 0x00C16D90,
0x00C17400, 0x00C1EEE0, 0x00C1F4E0, 0x00C2E190, 0x00C2E4D0 and 0x00C2E7A0.
- 0x009EF750 / 0x009EFB80 are `_M_insert` / `insert_unique` of `map<CCharString, CCountedPointer<CConsoleInputBase> >`. Their stand-in
  names were `CCacheInfo::InsertNode` and `CacheTree::std::_Rb_tree::insert_unique`. NF58 had blocked them on fable_string.h. NF65 matched
  0x009EFB80 by putting the real `CBasicString<char>::operator<` body (0x00429950's compare loop) in the row as an explicit
  specialisation, so no root-header change was needed. That row and 0x00429950 now both define the operator: queued, like the
  duplicate GetFOV and 0x0081F910 bodies.
- libpk: 0x00C1EEE0 / 0x00C1F4E0 are the CEventTriggerRef ctor and InsertDriver (were `CItemAction::CItemAction` / `Handle`). This
  completes the nf44 113 split binding NF53 queued: 0x00C2E4D0 and 0x00C2E7A0 now bind them too.
- 0x00C16D90 / 0x00C17400 were catalogued under propagated game labels (`ProcessEntry`, `DeleteTrackNodeCommand`). They are now
  `ProcessBankEntry` / `ReadMetadataBank`, descriptive libpk names (NOTES.md says which names are descriptive).
- Four access-only self renames (QAE to AAE): 0x00B3FF70, 0x00B42750, 0x008DF3A0 and 0x00C1F4E0's class move.

## Entries (APPLIED.tsv): 11 done, 1 rejected, 38 blocked
- **Rejected:** nf44:120. The retail trace string names PKSerialiseLoad, and 0x00C17400 now binds the landed self spelling.
- Two blocked entries, nf44:117 and nf41:e101, have every binder ported; only their self rows outside the slice are left.

## Rows and checks
- **Section 1: 10 self renames (12 file moves). Section 2: 50 lines, generated from relocation dumps of the original and ported
  objects.**
- **Coordinator correction:** NF65 gave 0x009EF750 / 0x009EFB80 the bare module `std::_Rb_tree`. The coordinator wrote out the full
  instance spelling, in the form NF58 used.
- **Funclets:** the renames moved the parents of five EH funclets: 0x01212915, 0x0121291D and 0x01212928 (parent 0x00C1EEE0),
  0x01211DC1 (0x00C16D90) and 0x01211E45 (0x00C17400). The coordinator repointed their includes and comment lines;
  funclet_includes_check reports 1,355 checked, 0 broken.
- **Recheck (`logs/recheck_part0..2.log`):** 21 graded from the repo after landing (16 rows + 5 funclets): 21 RELOCATION_MATCH, every
  test PASS. NF65's own final run agrees.
- **Checks:** `naming/rebind.py` removed 55 binding lines and added 30; bindings_multi_check is at its baseline (`NullCoord`).

## Headers (20 opt-ins in 11 files)
CAnimateThing, CEngineWorldMap (2), CStaticMapBankFile / CStaticMapBankStateBlock, CEngineMap, CEngineWaterRenderer,
CEngineScreenEffectDisplacementRenderer, CAIStateGroup_JackOfBladesStage2, and libpk CBankProcessor (6), CItem (4), CScriptHost.
Details: `naming/2026-11-17_agentNF65/hdr/README.md`.
- libpk/CItem.h's `CITEMTABLE_INSERT` opt-in sits inside an existing line of virtual-slot placeholders: `v03()` moved into the
  `#else` and `v04()`..`v08()` onto their own line. The coordinator checked that the default path preprocesses to the same tokens
  (`gcc -E -P`, whitespace-normalised).

## Coordinator decisions requested
- **libpk CItem identity:** libpk/CBankProcessor.h forward-declares `class CItem;` while libpk/CItem.h defines `struct CItem`, so
  0x00C101D0 binds both the PAV and PAU forms. CNamedSet and CItemDispatcher also model one registry object under two names. 0x00C08D40
  waits on this. 0x00C1EEE0 uses a `reinterpret_cast` (CNamedSet* to CItemDispatcher*) for one virtual Attach call because of it; this
  affects no relocation, but should go when the two models are merged.
- engine/NGameText_CDataBank.h: CGroupEntry's base would need to be CResource for 0x009C8890.
- libpk/CCodaFrameStreamer.h: vtable 0x012B80AC has no named base class (landed rows use the placeholder `CVtable_012b80ac`); 0x00C49820
  waits on this.
- 0x00C21C20 / 0x00C12940: the 16-parameter `CSoundEvent::Initialise` list is unsettled (an evidence gap).

## Name fixes queued (`land1117fu`, 11)
From NF65's NAME_FIXES.tsv (10) plus the coordinator's duplicate `operator<` line above: the 0x00C16CE0 self row (RunBankProcessor), caller 0x00C174A0, the CItemAction setters / ctor that become
CEventTriggerRef, 0x00B430A0 `DeleteData<CStaticMapBankStateBlock>` and others.

## Rule slips
- NF65 first wrote grade counts and its "21 ported" figure from memory; both were rebuilt from grades.log before hand-back.
- The CNamedSet-to-CItemDispatcher `reinterpret_cast` above is a judgement call the coordinator would not have made; it is queued with
  the identity decision.
