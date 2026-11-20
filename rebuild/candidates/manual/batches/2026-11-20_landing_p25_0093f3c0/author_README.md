# 0x0093F3C0 CAIStateGroup_VillageTaskPatrol::Update

**Grade:** RELOCATION_MATCH (1185/1185). No flag, no pragma, no device.
- **Strict vland:** passes (`vland.log`). It ran with LANDVERIFY_STRICT_HEADERS=1, overlay `hdr/` and a fresh LANDVERIFY_WORK.
  - The one warning is RELOC_NAME for 0x00658E90: the labels DB still says `_unattributed::ApplyToMember_00658e90`. It is in NAME_FIXES.tsv.
- **Undefined symbols:** every one is a PDB spelling.
- **Where the draft went:** p25K's local-model draft graded DIFFER(1182v1185) (ebx/ebp swap, frame 0x160 v 0x150). The rewrite on shared headers matched on its first grade.
  - It uses the Torch 0x0093EA90 and pending SecuritySweep 0x0093FCD0 define sets, Ego_d's locals and scopes, and the early `{ Terminate(); return; }` shape.
  - No dummy sweep was needed.

## Text and evidence
- **Ego_d.pdb** (`egod_lines/`) gives the locals and scopes: pvillage; block { track_node_target; block { ret, current_dir, wait_till, desired_dir; block { nav_route_calculated } } }.
  - Lines 68..161 have /Od sizes that fit the statements. With `this` at bprel -496, `Terminate()` is 11 B: lines 75 / 76 are 11 / 5 B, and the in-block pairs are 11 / 20 B, the 20 including the track_node_target destructor.
  - 106 (4 B) is nav = false, 109 (10 B) the do-while test, 120 (21 B) SetTaskAsLocked, 133 / 134 / 135 (45 / 5 / 8 B) desired_dir / Z = 0 / Normalise, and 149 (14 B) the idle-loop test.
  - Lines 79, 95, 113 and 122 (46 / 89 / 213 / 215 B) have no retail counterpart I could place. They were not fitted.
- **Callee bindings, two sources each** (in the source's `// Bindings:` lines):
  - **0x00658E90 = CTCVillage::GetBestPatrolTaskForThing.**
    - The PDB name has one reference argument, and retail ends `ret 4`.
    - Its body calls GetBestTaskForThing<CVillageTaskPatrol> 0x0065F856 (manifest) on PatrolTasks +0x54 (shared retail layout).
    - GetBestCrateTaskForThing 0x00658EA0 follows it, as in Ego_r.
  - **0x00836DA0 = PeekTrackStartNode:** the PDB `QBEPBV1@XZ` and its body; the landed 0x0093CB20 / 0x0093CAB0 use the same spelling.
  - **0x0065852D:** the PDB name and the catalog.
  - **0x00860A50 GetNearestNode:** the PDB `QBEPBVCThingTrackNode@@ABVC3DVector@@@Z` and the catalog.
  - **Task vtable 0x0125A900 slot 7 = 0x0065EEDD:** a compare with CreatureAssignedToTask +0x10, i.e. IsCreatureAssignedToTask.
  - **Physics slot 72 (+0x120) = GetRHSet() const:** the shared header's order. Forward is at +0xC.
- **p25K candidate corrected.** 0x008B8590 is the existing ONE-argument ctor (`ret 4`). The `push 0` at 0x0093F466 is AIAction_PerformActionAndWait's `false`, so no Unsheathe header change is needed.

## Header change (`hdr/`, `hdr.diff`; add-only; pin_asserts.py pinned 6, all in CVillageTaskPatrol.h)
- **NEW engine/CAIStateGroup_VillageTaskPatrol.h** comes from Ego_r's layout:
  - PTCVillageMember +0x14 and CIntelligentPointer<CVillageTaskPatrol> PTask +0x18, sizeof 0x20;
  - the FableWin.pdb method set.
- **engine/CTCVillage.h FABLE_RETAIL_CTCVILLAGE_PATROLTASK:** declares GetBestPatrolTaskForThing 0x00658E90 and SetPatrolTaskAsAcceptedByThing 0x0065852D.
- **engine/CThingTrackNode.h FABLE_RETAIL_CTHINGTRACKNODE_PEEK_START:** declares `const CThingTrackNode* PeekTrackStartNode() const`, 0x00836DA0.
- **engine/CVillageTaskPatrol.h FABLE_RETAIL_CVILLAGETASKPATROL_SETLOCKED_INLINE:** the inline `TaskLocked = true` (0x0093F5D7). The default `void SetTaskAsLocked();` stays under `#else`.
- **newly_flagged_rows.tsv:** six landed rows that define a local CAIStateGroup_VillageTaskPatrol now meet the new header under strict headers: 0x008FD923, 0x008FFA3C, 0x0093EDF0, 0x0093EE40, 0x0093EED0 and 0x0121F86F. The default path of each changed header is untouched.

## Tests
- **`test/0093f3c0.cpp`** is the real-object test. It links the REAL row object; vland falls back to linking the source object, and it passes.
  - Every callee is a `/alternatename` stand-in on its PDB symbol, using retail layouts.
  - It runs 21 scenarios, each compared event by event with a reference model:
    - no task; refused;
    - never assigned;
    - carrying or not;
    - in-progress repeats;
    - a move failure;
    - LookAt terminate;
    - a dead node;
    - idle failure;
    - unassigned at the Nth check, for N = 1, 3, 50 and 4..37 in steps of 3.
  - Node 2 is an end node, so the track wraps through PeekTrackStartNode. The look-at fires only when the next node lies ahead.
- **Mutants:** 21/22 KILLED (`mutants/0093f3c0.out`).
  - The one survivor is removing `desired_dir.Normalise()`. That mutant is equivalent: only the sign of the dot product is used, and normalising scales by a positive factor.

## Ablation (`evidence/ablations/`, preflighted by ablq.py; undefined symbols compared)
These were measured on the first draft, which had seven more defines. Dropping those seven together grades RELOCATION_MATCH with the same undefined symbols (`results_groups.txt` drop7), and the final source is drop7.

| change | grade |
|---|---|
| final source | RELOCATION_MATCH |
| no FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN (implicit copy for `PTask = ...` / the node steps) | DIFFER(1150v1185) |
| no FABLE_RETAIL_CVILLAGETASKPATROL_SETLOCKED_INLINE (SetTaskAsLocked called) | DIFFER(1188v1185) |
| no FABLE_RETAIL_CAIBRAIN_GETCREATURE | DIFFER(1213v1185) |
| `track_node_target == 0 \|\| track_node_target->IsDead()` instead of `!IS_THING_ALIVE(...)` | DIFFER(1176v1185) |
| no inner `{ }` block around track_node_target (Ego_d's S_BLOCK32) | DIFFER(1176v1185) |
| `IS_THING_DEAD(track_node_target)` | RELOCATION_MATCH (kept IS_THING_ALIVE, as the SecuritySweep / Torch family) |
| `current_dir.Dot(desired_dir)` | RELOCATION_MATCH (Inferred receiver) |
| `ret` declared outside the loop | RELOCATION_MATCH (kept in the loop body, Ego_d's scope) |
| FPS-first wait expression | RELOCATION_MATCH |
| no CCHARSTRING / CCLASS / DEF_CLASS_BASE / CAISTATEGROUPBASE_AIACTIONS / CAIBRAIN_CREATURE_REFERENCE / CMAINGAMECOMPONENT / CWORLD | RELOCATION_MATCH each and together (dropped) |
| no CTHING / CRIGHTHANDEDSET / CCOMBATACTIONBASE_MELEE | RELOCATION_MATCH each and with the seven (kept: they select the retail layouts the row reads, as in the Torch / 0x006F2DE0 precedent) |
| any other define (including each overlay opt-in) | SRC_FAIL |

## For the coordinator
- **NAME_FIXES.tsv: 0x00658E90 → ?GetBestPatrolTaskForThing@CTCVillage@@...** Respell it in:
  - the labels DB / manifest (`_unattributed::ApplyToMember_00658e90`);
  - its landed row file `unattributed_ApplyToMember00658e90_00658e90.cpp`;
  - the landed 0x0093EDF0's local `HasPatroller` declaration.
- **Migration candidates:** the six newly flagged rows can move to the new header later. This is not needed for this row.
