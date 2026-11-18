# 2026-11-17 landing nf74: name-fix round 7, NF74 (5 rows: engine-side ports left blocked by NF68, NF70 and NF71)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done" and "add another agent
to speed up naming queue completion".

NF74 took seven engine-side rows that earlier agents had left blocked, including the port of 0x006C9B70 that the coordinator held at
nf70. It ported 5. Brief, list and records: `naming/2026-11-17_agentNF74/`.

## Rows (STATUS.tsv, 7)

| Status | Rows |
|---|---|
| ported | 5: 0x006C9B70 (3 grades), 0x008C2550 (8), 0x008DDAD0 (15), 0x008E0060 (7), 0x008F3630 (10) |
| blocked/port | 2: 0x00B9E950, 0x00BD1F70 (one NF74 grade each) |

- **0x006C9B70** (`std::__copy<...>`, q10747) is now on stlport/fable_containers.h and fable_map.h with opt-ins the headers already had,
  in place of NF70's local `_Nonconst_traits`; its test was rewritten on the same headers and links the row's own code. The catalog
  name keeps its template arguments (NF74 wrote a bare `std::__copy`; the coordinator restored the full name and the file names).
- **0x008DDAD0** `NCreatureMode::CModeThunderLightningStorm::CreateLightningBeams` needs `// Flags: /d2inlT65` (`// Inline-budget:`
  line in the row; 61-64 were not tried, so 65 is not shown to be the smallest). It also uses a FITTED opt-in,
  `FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_NAMED_ARGS`: the string length and text are named locals before the CRC call. Ego_d.pdb
  lists only `it` in this function, so the source had no such locals; without them one register order differs (661 v 661 bytes). Its
  self leaf moves from public `QAE` to the PDBs' private `AAE` (no landed callers).
- **0x008F3630** uses a FITTED `const C3DVector& creature_pos` reference local (not an Ego_d local) that gives retail's single GetPos
  call.
- Both FITTED forms are marked in the row and pass the owner's 2026-11-14 acceptance test (byte match, same behaviour, portable C++);
  they are listed here so a later pass can look for the source's own form.
- **0x008C2550** includes a GetValidRushPosition body written from the landed 0x008C1600 row; it was not graded at 0x008C1600 itself,
  and the class of its second filter is unproven (NF74 NOTES).
- **Blocked:** 0x00B9E950 / 0x00BD1F70 need real class headers: engine/CEngineInternalPrimitive2DStaticMesh.h and its animated
  counterpart are flattened layouts with no base class or methods. NF74 drafted CEnginePrimitiveManager2DStaticMeshes.h /
  CEnginePrimitiveManager2DAnimatedMeshes.h in its overlay; they have not compiled in a passing build, so they are not landed (kept in
  `naming/2026-11-17_agentNF74/hdr/`).

## Entries (APPLIED.tsv): 4 done, 6 blocked
Done: nf41:e2, nf44:50, nf41:e110 (for this slice; 0x008B9F20 and the other callers of 0x00669580 not checked), q10747. Blocked:
q10478, nf41:e120, nf41:e188, nf44:70, nf44:71, nf44:73 (the two render rows).

## Rows and checks
- **Section 1: 2 self renames** (0x006C9B70 leaf, catalog name kept; 0x008DDAD0 access only).
- **Headers (8 opt-ins in 7 files, lines added only):** CCombatAction_GenericStrikeResponseBase.h, CCombatAction_StrikeResponseJackOfBladesIntoRush.h,
  CTCBoss.h, CSpecialEffectsDef.h (2; one existing `#ifdef _MSC_VER` widened to `&& !defined(...GETEFFECT_INLINE)`),
  CCreatureAction_CastPhysicalShieldSpell.h, CCreatureAction_DragonRearUpFull.h, NThingShot_CTCShotDefinitionBase.h. NF74 pinned the
  asserts its opt-ins moved (1, 8 and 4); `lane_merge/assert_names_check.py` reports 0 mismatches against 49b943df65.
- The prep's purity scan flagged `_emit` in three rows: it is the identifier `p_emitter_thing`, not a byte bake.
- **Funclets:** none affected (1,355 checked, 0 broken).
- **Recheck (`logs/recheck.log`):** all 5 rows graded from the repo after landing: 5 RELOCATION_MATCH, every test PASS.
- **Checks:** `naming/rebind.py` removed 19 binding lines and added 4; bindings_multi_check is at its baseline (`NullCoord`).

## Name fixes queued (`land1117fu`, 3)
0x008C1600 (the protected `C3DVector&` spelling), 0x007C0FB0 `AddThingToAvoid` (both PDBs and Ego_d; landed descriptive name
AddHitThing), 0x008AC060 (its 0x008C1600 caller still uses a local stand-in). NF74 withdrew two drafted fixes (REJECTED.tsv):
the landed 0x008AC060 filter tests an XY radius while the shared header's body and 0x008C2ED0 test 3D length.

## Rule slips
- One STATUS note with a backticked word was passed in double quotes and lost the word; repaired.
