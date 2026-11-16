# 2026-11-16 landing gj -- 0x00BF0740 SetupOutlineStencilStates, and 0x00BF0B20 respelled SetupOutlineFillStates

Agent w24C ported 0x00BF0740 CEnginePrimitiveRenderUtil::SetupOutlineStencilStates onto the shared headers in the shape of its landed
sibling 0x00BF0B20 (RELOCATION_MATCH + test PASS, strict headers). It could not land while the landed 0x00BF0B20 carried the same
symbol: 0x00BF0B20 is SetupOutlineFillStates (render pass 13; 0x00BF0740 is pass 12; queued land1113fy / land1115ga, and
reloc_bindings already bound both names this way from the caller 0x00BB3E80).

Respelling of 0x00BF0B20: source and test renamed to `CEnginePrimitiveRenderUtil_SetupOutlineFillStates_00bf0b20[_test].cpp`, the
definition, Leaf and test spelled SetupOutlineFillStates, build_candidates.ps1 paths and the catalog name updated. The nested opt-in
`FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_OUTLINESTENCILSTATES` in engine/CEnginePrimitiveRenderUtil.h now declares both setters (the
header's only assert is already pinned AT(110)). `--recheck-landed` of 0x00BF0B20 and its caller 0x00BB3E80: both RELOCATION_MATCH +
PASS (`logs/vland_recheck.log`). Whole-tree equivalence (old header v new): every landed row identical input except 0x00CBFB7D
(`__FILE__` path only); the nested block is inside an outer opt-in only 0x00BF0740 enables.
