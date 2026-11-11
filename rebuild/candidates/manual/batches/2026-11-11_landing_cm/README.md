# 2026-11-11 landing cm -- chunk Q8 (parked near misses, Opus 5.5): 3 rows, two new shared headers, two landed helpers given their PDB names

vland --land: 3 wins (RELOCATION_MATCH), every test PASS.
- **0x006A4FB0 CThingPhysical::GetFirstCollision.** Moved off a local model onto the shared headers (CThingPhysical, CTCPhysicsBase,
  CPhysicsMeshObject, CMeshIntersectionData, C3DVector). The last residue was the epilogue pops interleaving with the final Normalise
  multiplies. The fix is the new FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_GROUPED opt-in, which groups the Newton step fully, plus the
  existing FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER.
- **0x0095D0E0 CAIStateGroup_RangedCombatUsePreferredFirePoint::IsStillRunnable.** On the shared AI headers. The levers: build the event
  filter straight from the member, `CIsEventCreatedAfter(TimeOutCounter)`, and use a named local for the elapsed frames.
- **0x009F5690 CInputEventPackage::CInputEventPackage(CAFile&).** The catalog label "Save" was wrong: the body fills `this` from the file.
  The lever: redundant parentheses `(float)((GFGetTime() - age))` bring back retail's memory-form `cmp ecx,[esi+8]`.

**Headers (coordinator)**
- The coordinator added engine/CChecksum.h and engine/CInputEventPackage.h with gen_class_headers.py, so 0x009F5690 needs no local class
  declarations.
  - The generated flat layout stays the default.
  - The typed Ego_r.pdb layout sits behind opt-ins: FABLE_RETAIL_CINPUTEVENTPACKAGE, with a vector<CInputEvent>, CChecksum, C2DVector and
    the constructor, and FABLE_RETAIL_CCHECKSUM_CTOR, CChecksum's inline zeroing constructor.
  - 0x009F5690 regraded RELOCATION_MATCH on the new headers before it was landed.
- Q8's overlay: hdrcheck2 reports default=same on all seven headers. The only opt-ins that DIFF are new ones:
  - FABLE_RETAIL_C3DBOUNDINGSPHERE_FASTBUILD (used only by a differ row);
  - FABLE_RETAIL_CPHYSICSMESHOBJECT_SPHEREINTERSECTION.

**Names**

0x006A4FB0 binds two callees under their PDB names, so the coordinator respelled the landed code that used other spellings.
`RENAMES_respell.tsv` lists the respells. The 4 touched rows recheck MATCH or RELOCATION_MATCH with tests PASS
(`logs/vland_recheck_respell.log`).
- 0x00A442E0 `?GFGetShortestDistanceSquaredBetweenPointAndLine@@YIMABVC3DVector@@00AAM@Z` (Ego_r 0x00B999B0, FableWin 0x0320C740).
  - The row was landed as the descriptive `GFGetDistanceSquaredFromPointToLine(..., float*)`. Its definition is respelled, with the
    out parameter now `float&` as the PDB has it.
  - Its caller 0x006A7050 used a third spelling, `GetSquaredDistanceToInfiniteLine`.
- 0x00A443A0, the 2D twin, is `?GFGetShortestDistance2DSquaredBetweenPointAndLine@@YIMABVC2DVector@@00AAM@Z` (Ego_r 0x00B99AC0, FableWin 0x0320C970).
- 0x00A120D0 GetSphereIntersectionFromTo takes `CMeshIntersectionData&`. Landed 0x006A5460 called it `CPhysicsHit`, which is in neither PDB.
  That row's local hit struct is now `class CMeshIntersectionData` with its PDB member names.
- Still open in NAME_FIXES_QUEUE (tag land1011cm): 0x00989830 InitialiseLight takes `const CShaderLight&`. Landed 0x00B44F20 uses the
  pointer spelling.
- Bindings were re-derived (-4 / +2). bindings_multi_check is at its NullCoord baseline.
- Labels: ghidra_out/labels_landing_2026-11-11cm.tsv.

**Header debt:** 0x006A5460 still uses local models of C3DVector, CPhysicsMeshObject, CTCPhysicsBase and CThingPhysical. It is the
sibling of 0x006A4FB0, so it can move onto the same headers.

The chunk record is in pending/2026-11-11_linux_handQ8 (37 near misses in differ/, NOTES, LESSONS).
