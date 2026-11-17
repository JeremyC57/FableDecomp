# 2026-11-17 landing nf67: name-fix round 7, NF67 (15 rows: engine-side ports below 0x00970000)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent
to speed up naming queue completion".

NF67 took 21 engine-side rows that NF63 / NF65 had left `blocked/port` (rows whose wrong binding comes from a local model of a class
that has a shared header). It ported 15. Brief, list and records: `naming/2026-11-17_agentNF67/`.

## Rows (STATUS.tsv, 21)

| Status | Rows |
|---|---|
| ported | 15 |
| blocked after NF67 grades | 1: 0x008F95D0 |
| not reached (0 NF67 grades; earlier agents' blockers kept) | 5: 0x008DDAD0, 0x008E0060, 0x008C2550, 0x008F3630, 0x006A0150 |

**Ported:** 0x009627C0, 0x00961880, 0x0096C4F0, 0x0090FAE0, 0x0094B8B0, 0x00915190, 0x00921440, 0x00966290, 0x008AF850,
0x008F7950, 0x008D3DD0, 0x00869020, 0x00869CA0, 0x008B9E50 and 0x008A7980.
- Among the self renames: the CAIStateGroup `Update` rows were bound to invented `...Methods` classes and are now the protected
  virtual members (`MAEXXZ`); 0x008F7950 is `NCombatActionHelpers::CreateDustEffectForCreature` (Ego_r.pdb 0x0091BB60 +
  FableWin.pdb 0x02B0A440; was `_global::`); 0x00869CA0 is the `_M_insert` of `multimap<CDefPointer<const CArmourDef>, float>`.
- **0x008F95D0** (3 grades, DIFFER 276 v 263): the whole port on LSocket.h matches every binding and the frame. The header's
  `vector<char>` destructor tests the pointer before freeing it and retail does not, so `okay` stays in memory instead of in ebx.

## Entries (APPLIED.tsv): 9 done, 6 blocked
- **Done:** nf41:e80, nf41:e93, nf41:e132, nf44:10, nf44:11, nf47:land1011fv, nf58:0x00869CA0, q10436, q10463.
- **Blocked:** nf41:e106 (outside binder 0x00715560), nf41:e110 (0x008F3630; also the byte twin 0x008B9F20, queued), nf41:e2
  (0x008C2550), nf44:50 (0x008DDAD0, 0x008E0060), nf44:98 (0x008F95D0), q10502 (0x006A0150).
- **Rejected (REJECTED.tsv, 4):** among them NF49's and NF53's port blockers for 0x008B9E50 / 0x008A7980, which were measured on
  hybrid drafts; both rows ported.

## Rows and checks
- **Section 1: 13 self renames (4 file moves). Section 2: 72 respelled bindings.**
- **Coordinator corrections:**
  - 0x00869CA0's module was the bare `std::_Rb_tree`; it now uses the instance spelling of its sibling 0x00869BB0 (`_M_erase`).
  - engine/CQuestManager.h: NF67's overlay predated NF66's `FABLE_RETAIL_CQUESTMANAGER_ACTIVEQUEST_CONTAINSREGION`; merged three-way
    (`git merge-file`, no conflicts), so NF66's opt-in stays.
  - engine/CAIStateGroup_UsesSeat.h (new): NF67 put the whole class under an opt-in. The coordinator made it an ordinary shared
    header (the guard lines became comments, so the line count and the `FABLE_STATIC_ASSERT` line stay the same). Only NF67's rows
    include it.
- The prep's purity scan flagged `_emit` in 0x008F7950: it is the identifier `p_emitter`, not a byte bake.
- **Funclets:** no EH funclet includes a changed row. funclet_includes_check reports 1,355 checked, 0 broken.
- **Recheck (`logs/recheck_part0..2.log`):** all 15 rows graded from the repo after landing: 15 RELOCATION_MATCH, every test PASS.
  NF67's own final run agrees.
- **Checks:** `naming/rebind.py` removed 72 binding lines and added 20; bindings_multi_check is at its baseline (`NullCoord`).

## Headers (30 opt-ins in 27 files; lines added only, default path unchanged)
Details: `naming/2026-11-17_agentNF67/hdr/README.md`. engine/LHNetworkTypes.h is an unchanged copy (LSocket.h includes it by a
relative path). The two LSocket opt-ins (`SENDBLOCK`, `LADDRESS_CTOR_OUTOFLINE`) serve only the blocked 0x008F95D0 draft.

## Coordinator header work requested
- One shared header for `NEntityEvents::CIsEventOfType` / `CConvFilterToCP` / `CEventChildHurtGoToMother` / `CEventKilledCreature`
  (each is defined locally in the rows that use it), and one home for `CEventBase` (defined in several event headers).
- 0x008F95D0: a retail-shaped `vector<char>` destructor (no null test before the free) as an opt-in.

## Name fixes queued (`land1117fu`, 10)
NF67's NAME_FIXES.tsv (9: constructors and protected members its ported rows now bind, two `CTCEntityEvents::GetEvent`
instances, and 0x007BED60 `NThingShot::CTCShotDefinitionBase::IsAbleToBeDeflected`, from the retail accessor run against Ego_r),
plus the TeleportIn twin 0x008B9F20, to be ported with the same text as TeleportOut 0x008B9E50.

## Rule slips
- Five rows got no NF67 grade and 0x008F95D0 stopped at 3 of 15 (the "carry each row to a terminal grade" rule).
- One read-only `git status` ran inside a check command.
- A relative log path wrote `work/v_msg.log` into the repo's ignored `work/` directory; NF67 moved it out. Nothing was committed.
- A column slip overwrote four STATUS notes; NF67 restored them.
