# NF68 notes (slice: 18 engine-side rows NF63 / NF65 drafted or inspected)

All counts here come from STATUS.tsv, APPLIED.tsv and recheck_final.log. Grade counts come from grades.log: one per g.sh line for the address, with vland runs excluded.

## Status
- **Rows ported: 12.** Each is RELOCATION_MATCH with test PASS in a single vland run (recheck_final.log): 0x009790F0, 0x00979F70, 0x0097A630, 0x009CEAE0,
  0x009F6890, 0x00A25BD0, 0x00A42980, 0x00A79A30, 0x00AD85D0, 0x00B32AD0, 0x00B7FA20, 0x00EDA8C0.
- **Rows blocked: 6.** 0x00B76BC0 (9 grades), 0x00BB8090 (10), 0x00E12580 (12), 0x00EE2490 (8), 0x00B9E950 (2), 0x00BD1F70 (1).
- **Not-a-binder:** 0. **Deferred:** 0.
- **Entries done: 9.** nf41:e102, nf41:e177, nf44:16, nf44:83, q10459, q10475 (a judgement call, see below), nf48:land1113fy:e28, nf48:land1011fk:e1, nf46:34.
- **Entries blocked: 9.**
  - nf41:e120, nf41:e188, nf44:70, nf44:71, nf44:73: these wait on 0x00BD1F70.
  - q10478: waits on 0x00B9E950.
  - nf41:e161: waits on 0x00EE2490.
  - q10541: waits on 0x00E12580, plus 0x00EE2A80, which is outside this slice.
  - nf45:29: waits on 0x00B76BC0 and 0x00BB8090, plus four rows outside this slice that are not ported (0x00BAD2E0, 0x00BCB800, 0x00BF4570, 0x00BE6F70).

## Method
Each port deletes the row's local models of classes that have a shared header and reuses the macro block of a landed sibling on the same headers:

| Row(s) | Sibling block |
|---|---|
| 0x00B7FA20 | 0x00B7C510 |
| 0x00A79A30 | caller 0x00A7AF60 |
| 0x00EDA8C0 | 0x00ED94D0 |
| Cover chain | 0x009711D0 / 0x009710D0 |
| Bank rows | 0x00B42750 / 0x00AF1360 |

Members the headers lack are added as `#ifdef` opt-ins in hdr/ (listed in hdr/README.md). Local models remain only for classes with no shared header:
- CLipSyncDataBankEntry and its CData
- CBankFileAsyncEntry
- CPhysicsMeshInfo and its CMeshData

0x00EDA8C0 carries the owner-allowed `// Flags: /d2inlT59` with an `// Inline-budget:` line. 59 is the smallest matching value: 57 and 58 grade DIFFER.

RENAMES section 1 holds the self renames: 0x009CEAE0, 0x00B7FA20, 0x00AD85D0, 0x009F6890, 0x00A79A30, 0x00A42980, 0x00A25BD0, 0x009790F0, 0x0097A630.
Section 2 holds the callee spelling changes. Every line pairs an old and a new relocation at the same target, and none is unpaired.

## Rows blocked
- **0x00BB8090 and 0x00B76BC0 (blocked/port).** Stop reason: every form inside the shared headers is exhausted. What remains is the CRenderStateManager
  bookmark test/or operand order at one or two tracked-state sites, a c2 record-id effect. Forms tried:
  - the PASS22 method order and inline variants
  - FITTED named locals
  - RESIZE_OUTOFLINE
  - the C3DVector component constructor

  Unblock: a per-site-faithful setter form in engine/CRenderStateManager.h (coordinator), or the FableWin.pdb debug bodies.
- **0x00E12580 (blocked/port, 12 grades).**
  - Finding: NF60's local-model draft, with its `#pragma inline_depth` regions removed, is RELOCATION_MATCH under `/d2inlT62`. The inline budget, not the
    inline_depth regions, reproduces this "Revisit: inline-depth" row.
  - My port onto engine/NScript_CQ_WaspBossScript.h compiles. It uses the overlay opt-in FABLE_RETAIL_CQ_WASPBOSSSCRIPT_DOMISSION, calls EndMission, and has
    every local model deleted. Results: `/d2inlT100` gives DIFFER(1446v1446), with register and frame-slot order differing throughout. At `/d2inlT62` it is
    DIFFER(1411v1446), and at the default budget DIFFER(1346v1446).
  - Stop reason: the remaining difference is a c2 record-order effect of the header class shapes.
- **0x00EE2490 (blocked/port, 8 grades).** The port onto engine/CV_TravellingHeroesScript.h compiles. It uses the overlay opt-in
  FABLE_RETAIL_CV_TRAVELLINGHEROESSCRIPT_THUNDER with the PDB names and binds only PDB spellings. Best result: DIFFER(1321v1334).
  - Retail calls the CScriptThing copy out of line for the predicate member but expands it at the by-value argument. No per-TU copy form in the headers
    gives both in one TU; the landed row used a fitted `InlineCopy` form.
- **0x00B9E950 (2 grades) and 0x00BD1F70 (1 grade) (blocked/port).** The row classes have no shared header. The body is written against local-model member
  names, which the shared C3DMeshInfo / C3DMesh2 / CEngineInternalPrimitive2DStaticMesh / CDisplayManager headers spell differently. The exact errors are in
  STATUS.tsv. A full respelling of the 901-byte and 24-class bodies was not reached.

## Coordinator header changes proposed (I did not touch any header default path)
- **engine/CBankFileAsync.h.** Retail 0x009D58D0 is the PDB `GetAsyncEntry`, and neither PDB has `CreateEntry`. The existing CREATEENTRY opt-in declares the
  wrong name. My overlay adds GETASYNCENTRY; see NAME_FIXES.tsv.
- **engine/CBankFile.h.** CRuntimeData is a `class` in the PDBs but a `struct` on the default path. My opt-in CRUNTIMEDATA_CTOR switches it.
- **engine/CRenderStateManager.h.** Add the per-site setter form described above.
- **Retail-class opt-ins worth promoting to the default.** The flattened generated defaults cannot be reached otherwise:
  - CLipSyncDataBank.h: CLipSyncDataBank : CBankFileAsync
  - CCreatureAction_ProjectileCrouch.h: CCombatActionBase base; FacePos byte-packed at +0x115 (Ego_r +301), class aligned 4, sizeof 0x124. This matches the
    landed constructor 0x008B8310.
- **CScriptThing copy forms for NScript TUs.** Needed for 0x00EE2490, see above.
- **One address, one name.** 0x00E13310 is EndMission in the landed self row and the header, but WaspMenace in the landed DoMission binder. Both names are
  made up.

## Judgement call: q10475
The only binder, 0x009F6890 (vector<CInputEvent>::operator=), is ported and passes (RM + PASS). Ported onto stlport/fable_containers.h, it binds 0x009F6720 as the
STLport spelling `??$_Destroy@PAVCInputEvent@@@std@@YIXPAVCInputEvent@@0@Z`, not the queued descriptive `?_Destroy_Record34@@YIXPAVCInputEvent@@0@Z`.
- I counted the entry as done.
- The queued spelling is in REJECTED.tsv.
- The STLport spelling of the landed self row 0x009F6720 is in NAME_FIXES.tsv.

## Descriptive names kept (no PDB name found)
- LipSyncLoadingPriority (0x01397700)
- g_PhysicsMeshReadPriority (0x0139723C)
- `mutable` on CPhysicsMeshInfo::MeshData. The PDB function is const and stores the member; the bytes cannot show whether the source used `mutable` or a const_cast.

## Funclets
None:
- No ported row has an __ehhandler or frame-handler relocation in its xref dump (work/x_*.txt).
- No landed funclet names any ported row as its parent.

## Rule slips
- Two Bash calls exited 1, from a trailing `grep -c` / `grep -n` with no match.
- One `rm -rf "$L/$a"` was refused by the safety check. It was rerun as `rm -r "${L:?}/${a:?}"`.
- g.sh's `head -40` truncated one original relocation dump (0x0097A630), so my first Section 2 for that row dropped pairs. I redumped it with xref2.py directly
  and regenerated it.
- I first spelled 0x009D58D0 as the header's CreateEntry. I fixed this to the PDB GetAsyncEntry and regraded both bank rows to RM.
- I first gave ProjectileCrouch a +0x118 FacePos. I fixed this to the retail +0x115 and regraded both crouch rows to RM.
- Clean checks:
  - The purity grep over rows/ is clean. The only hits in hdr/ are repo lines inherited unchanged.
  - No cast addresses, no `#error`, no git, nothing written under the repo.
- Overlays:
  - Every overlay was re-copied from, or rebased on, the current repo header. stlport/fable_containers.h was rebased on NF67's new list remove_if opt-in.
  - They are `#ifdef` opt-ins only. The default path and four opt-in configurations preprocess identically to the repo, apart from fable_containers.h's one
    rewritten `#if`, which is documented in hdr/README.md.

## Locks
- The 12 ported rows keep their NF68 locks.
- The locks of the 6 unchanged blocked rows are released.
