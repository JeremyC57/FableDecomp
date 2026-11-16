# 2026-11-16 landing gn, follow-up 2: 0x007C2AB0 row, 0x00B22B40 port, callers of 0x00686800 / 0x007448F0

Owner: "fix these now" on the three items still open after follow-up 1: the 0x007C2AB0 placeholder row, 0x00B22B40's own CArray
copy, and old rows binding wrong names to 0x00686800 (CTCBase::CTCBase(CThing&)) and 0x007448F0 (CTCPerceiveThing::GetSightRadius).

## Done (this commit)
- **0x007C2AB0** is now STLport `std::vector<C3DVector>::vector(size_type)` itself:
  `template std::vector<C3DVector, std::allocator<C3DVector> >::vector(size_type);` over the shared stlport/fable_containers.h body.
  It matches (115 bytes, no flags), strict, test PASS. File, test, catalog and module are renamed to the 0x00AD30C0 convention
  (`stdvectorC3DVectorstdallocatorC3DVector_vector_007c2ab0`). The two failed tries logged in FINDINGS_LOG had wrapped it in a
  CArray body, which VC7.1 compiles as a call.
- **0x00B22B40** (and 0x00AE1730 / 0x00AE18C0, found by the relocation scan) now use engine/CArray.h's retail branch instead of a
  local CArray / std::vector copy. CArray(n) forwards to vector(n), and all three now call 0x007C2AB0 as
  `??0?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@QAE@I@Z`. 0x00B22B40 also calls 0x0075B8D0 as vector::operator= and
  0x00464931 as `?resize@?$vector@KV?$allocator@K@std@@@std@@QAEXIABK@Z` (`FABLE_RETAIL_STLP_VECTOR_C` and
  `FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE`). The new opt-in `FABLE_RETAIL_CARRAY_GETBYTESIZE` (FableWin.pdb
  `?GetByteSize@?$CArray@..@@QBEIXZ`, inline) is body-only. All three are RELOCATION_MATCH with test PASS non-strict; strict still
  fails on their other, older local models (CDataInputStream / CCardinalSpline / CCountedPointer / CPSCBase in 0x00B22B40;
  C4DVector / CParticleSimulation in the two others).
- **0x007448F0:** 0x00933B30, 0x00936210, 0x009207A0 (made-up `CTCUndergroundMover::GetSurfaceRange`), 0x00935AD0 and 0x00665190
  are respelled `GetSightRadius() const`. The first three use engine/CTCPerceiveThing.h through the new declaration-only opt-in
  `FABLE_RETAIL_CTCPERCEIVETHING_GETSIGHTRADIUS`. All 17 retail calls into 0x007448F0 from landed rows now bind
  `?GetSightRadius@CTCPerceiveThing@@QBEMXZ`.
- **0x00686800**, first part: the 12 component constructors whose bindings named a local base, all re-authored on engine/CTCBase.h
  (`FABLE_RETAIL_CTCBASE`) with the PDB `(CThing&)` signature: 0x004D49DD, 0x004D7E46, 0x007E08E0, 0x00749810, 0x0075C180,
  0x007CC7D0, 0x007E1AC0, 0x004DDAED, 0x006BF8A0, 0x007C6760, 0x007A09B0, and 0x004C00E0 (`NGraphicAppearance::CTCGraphicAppearance`).
  Re-signing them changes their own symbols, so their callers were re-authored too:
  - the nine `X::Alloc(CThing&)` registry factories, 0x004D87DF, 0x004D5D23, 0x004D3658, 0x004D5866, 0x004D5E74, 0x004E0105,
    0x004D31EB, 0x004D5690, 0x004D4CB8 (`?Alloc@X@@SIPAVCTCBase@@AAVCThing@@@Z`);
  - CTCCameraPointBuilding 0x004D8193 and CTCCameraPointScripted 0x0075CC60.

  reloc_bindings.tsv lines for those callers name the retail symbols.

Grades: 30 changed rows (all from `git status`) RELOCATION_MATCH with test PASS (`logs/vland_recheck_changed30_nonstrict.log`), plus
the two CArray rows (above). 19 + 0x007C2AB0 pass strict; the rest fail strict only on local models they already had.
Header equivalence (`logs/equivalence_changed_input_rows.tsv`, HEAD include tree vs this one, all 56k landed rows): only the five
rows that use the two new opt-ins and the five known `__FILE__`-only rows differ. Every `FABLE_STATIC_ASSERT` below both header
insertions is pinned `_AT`.

## Not done: 181 rows still bind wrong names (`callers_wrong_symbol.tsv`)
reloc_bindings.tsv keeps only the first caller of each symbol, so it showed one row per wrong spelling. A scan of the retail
bytes is complete: `tools/callers.py` finds every E8/E9 into a target and maps it to the landed row that holds the call site
(`retail_calls_into_targets.tsv`). `tools/xrow.sh` compiles that row and resolves each relocation of its function against the
retail bytes at the same offset (`tools/xref.py`). It finds:

| Target | Rows | Wrong symbols they bind |
|---|---|---|
| 0x00686800 | 175 | 106 `??0CTCBase@@QAE@PAVCThing@@@Z`, 29+ `??0CTCBase_Calls@@QAE@PAVCThing@@@Z`, 14 `??0CEngineSubPrimitive2DClockSprite@@..`, `??0CThingComponent@@..` |
| 0x0075C180 / 0x0075CC60 / 0x004D8193 / 0x004C00E0 / 0x004D7E46 | 6 | the old pointer-form spellings of constructors re-signed above (CTCCameraPointScriptedSpline 0x0075DD10, CTCCameraPointShapeActivationBase 0x004D3A03, NGraphicAppearance::CTCGraphicAppearance::Alloc 0x004D44E8, CTCOwnedEntity::Alloc 0x004DAF68, ...) |

Each needs the same port as the 12 above, worked base-first because each re-signed constructor pulls its own callers in.
Queue line 2272 (0x00686800) stays open and points here.

## Other findings
- 0x00665190 has no `// Leaf:` line, and its own signature `IsThingInFOV(CThing*, bool)` differs from the PDB
  `?IsThingInFOV@CThingCreatureBase@@QBE_NABVCThing@@_N@Z`. It also calls 0x007445D0 as a placeholder `GetFieldOfView_007445d0`.
  Both are older naming debt, left as they are.
- 0x0075B8D0 (vector<C3DVector>::operator=) is bound under five names in reloc_bindings.tsv (CElement12_0075b8d0, CKeyVector,
  CPointVector, CArray<C3DVector>, vector). Same class of problem; not in this request.
- 0x00AD30C0 (vector<C3DVector>(n, value)) is still a local model with a `__forceinline` fill helper. The explicit-instantiation
  form used for 0x007C2AB0 is worth trying there.
