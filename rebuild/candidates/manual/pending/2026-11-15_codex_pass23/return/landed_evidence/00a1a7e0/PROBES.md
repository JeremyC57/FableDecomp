# Pass23 GetIntersectionPointsBetweenNodes

Prior history: hand533895/862; retry2_sec5871/862; hand721862/862 after13 source probes, one x87 scheduling exchange remaining. Read all3 saved sources, hand721 notes and all6 supplied neighbors. Previous13 variants are summarized by the supplied notes; their individual source files were not included, so no fabricated per-variant record is claimed.

- baseline_package_headers: the provided local-model source reproduces862/862 with two diff hunks representing one fcomp/test ordering exchange. Baseline measurement only.
- rebase_shared_classes_01: remove every local game/container model; use real CNavigatorAStar, inherited CNavigatorBase, CANavQuadTreeNode, C2DVector, C2DBoxF, and STLport pair. Add only the proven private const declaration behind a new opt-in, pinning the moved assert. Keep the entire function body unchanged. Result862/862,253/253 instructions, masked match. One genuine new reconstruction strategy; no pragma or forced expansion change.
- final_header_declarations_02: add a separately guarded original private constructor declaration needed only to instantiate an actual derived fixture. Production flag does not select it; regrade remains862/862. Test-only external operations replace no class models.
- native_behavior_01: VC7.1 /Od /W3, no /EHsc, PASS; GCC64 C++03 PASS.4,900 cases cover all4edge directions,7by7node levels,5center offsets and5strip distances. Check both returned points against an independent line-parameter oracle and check GetSize call counts.
- RELOCATION_AUDIT:26 operands independently pass:10calls to original GetSize,10literal0.5f references and6 original epsilon globals. Native whole extent ends ret16; xref independently confirms sole caller00a1cd30.

The shared-header rebase changed several compiler inputs together; this record does not claim to isolate which one changed instruction scheduling. No unmatched row is parked here and no10-attempt claim is made.
