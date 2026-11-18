# 2026-11-17 landing nf69: name-fix round 7, NF69 (33 rows: the names Ego_d.pdb settled)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent
to speed up naming queue completion". The evidence is the other account's Ego_d.pdb write-up, `docs/journal/2026-11/EGOD_FINDINGS.md`.

NF69 took the 40 queue entries `egod1117` (q10819..q10858) and the 33 rows that carry or bind them. Brief, entries, worklist and
records: `naming/2026-11-17_agentNF69/`.

## Rows (STATUS.tsv: 33 rows + 2 extra claims)

| Status | Rows |
|---|---|
| ported | 33: 31 from rows.txt, plus 0x004E0064 and 0x007F5D00 (they still called the targeting destructors under stand-in names) |
| not-a-binder | 2: 0x004E1AAF (already the STLport `~vector`), 0x0047EEE4 (already binds 0x009C8890 right; its local `CGroupEntry : CRefCounted` is queued for a port) |
| blocked | 0 |

**What changed:**
- **Hit locations:** `NHitLocations::CHitLocation::StatRegisterClass` 0x008162F0 (was the made-up `_global::RegisterThingHandlers`),
  the static map `RegisteredHitLocationCodelets` with its dynamic initializer 0x0121F4F0 and atexit destructor 0x01229F30, the tree
  `_M_insert` / `insert_unique` 0x00817D70 / 0x00818000, the reader `ProcessHitLocationCode` 0x00815E80 (now in its namespace), and
  `NHitLocationCodelets::HLCAutoKill` 0x008F5CF0 (was `_unattributed::CallWith_008f5cf0`). The four codelets now bind their PDB names.
- **Combat sequences:** `CCombatSequenceBase::StatRegisterClass` 0x008B08B0 (was the propagated label
  `CScreenEffectPriorityStack::Process`), the three static maps `IsValidPredicates` / `OnStartModules` / `OnStopModules` with
  their initializers and atexit destructors, the three tree pairs at 0x008B29A0..0x008B2F50, the readers
  `CCombatSequenceGeneric::IsValid` / `OnStart` / `OnStop`, and `CCombatManager::RegisterCombatSequences`. The nine
  `NCombatSequenceIsValidModules` / `NCombatSequenceOnStartModules` handlers now bind their PDB names.
- **Targeting destructors:** 0x004E37BF `CTCTargetingPlayer::~CTCTargetingPlayer` now uses the real members and base.
  0x004DA8B7 is `CTCTargetingBase::~CTCTargetingBase`, not CTCTargeted's (below), and 0x004E0080 is
  `CTCTargetingAI::~CTCTargetingAI` (was `_unattributed::~CAggregate_004e0080`; Ego_r layout, and its only caller is the
  CTCTargetingAI scalar deleting destructor).
- 0x00CD3A16 is `map<int, CCharString>::operator[]` (the catalog said `long`) and now binds the `int` insert 0x00CD36C2.
- 0x009C8890 `NGameText::CDataBank::GetPGroupEntry`: CGroupEntry derives CResource, as Ego_d and Ego_r show. The row also defines
  `BeginLoadingGroup` (0x009C8220), which grades RELOCATION_MATCH on its own at that address.

## Entries (APPLIED.tsv): 39 done, 1 rejected
- **Rejected:** q10855 claimed `??1CTCTargeted@@UAE@XZ` for 0x004DA8B7. The body destroys exactly Ego_r's CTCTargetingBase
  members, its callers are the destructors of CTCTargetingBase's RTTI children, and `??1CTCTargeted@@UAE@XZ` already belongs to the
  landed row 0x004DBF56. NF69 applied `??1CTCTargetingBase@@UAE@XZ` (Ego_r.pdb + FableWin.pdb) instead; all five binders call it.

## Rows and checks
- **Section 1: 28 self renames (46 file moves). Section 2: 54 respelled bindings.**
- **Coordinator correction:** NF69 gave the eight static-initializer rows a class module and a bare member
  (CCombatSequenceBase::`dynamic initializer for 'IsValidPredicates''). The coordinator respelled them to the PDB form the catalog
  already uses for class statics: `dynamic initializer for 'CCombatSequenceBase::IsValidPredicates'' with module `_global`.
- **Funclets:** no EH funclet includes a changed row. funclet_includes_check reports 1,355 checked, 0 broken.
- **Recheck (`logs/recheck_part0..2.log`):** all 33 rows graded from the repo after landing: 33 RELOCATION_MATCH, every test PASS.
  NF69's own final run agrees.
- **Checks:** `naming/rebind.py` removed 116 binding lines and added 112; bindings_multi_check is at its baseline (`NullCoord`).

## Headers (6 opt-ins in 6 files; default path unchanged)
NHitLocations_CHitLocation.h (`TREGISTEREDHITLOCATIONCODELETMAP`), CCombatManager.h (`REGISTERCOMBATSEQUENCES`), CCombatSequenceGeneric.h
(`MODULES`), CTCTargetingBase.h and CTCTargetingAI.h (`IMPLICIT_DTOR`: an `#ifndef` that drops the declared destructor so the
compiler makes it), NGameText_CDataBank.h (`GETPGROUPENTRY`: CGroupEntry : CResource, CDataBank's second base CResourceBank, per
RTTI). Details: `naming/2026-11-17_agentNF69/hdr/README.md`.

## Open
- The four scalar deleting destructors 0x004E37A3, 0x004E1023, 0x004E0064 and 0x007F5D00 keep their stand-in free-function names;
  only their callees were ported. The compiler's `??_G` needs a TU that emits the class vtable.
- CCombatSequenceBase and CCombatSequenceDef have no usable shared header, so these rows model them locally, as the landed rows
  0x008B0B60 / 0x006F1770 already do. A shared header for them is open.
- STLport tree spellings and the enum casts are descriptive-by-library (NOTES).

## Name fixes queued (`land1117fu`, 21)
NF69's NAME_FIXES.tsv (20: the four trees' `_M_erase`, `operator--` and member-template `find`, `~vector` / `~list` instances, the
int pair and `_M_lower_bound`, `AsyncUpdatePending`, and NGameText's `BeginLoading` / `BuildGroup` / `BeginLoadingGroup`), plus the
port of 0x0047EEE4 onto NGameText_CDataBank.h.

## Rule slips
- Two Bash calls exited non-zero, neither changed anything (a `cat` given a Source / TestSource pair as two paths; `g.sh` with a
  relative path, which failed before any compile).
