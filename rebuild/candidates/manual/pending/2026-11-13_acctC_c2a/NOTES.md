# agentC2 NOTES (w22 acctC lane)

## 0x00651B04 NPlayerGui::CDrawAbilityButtons::Draw -- RELOCATION_MATCH, test PASS (g++ + VC7.1), vland read-only RELOCATION_MATCH PASS
Source src/00651b04.cpp, test test/00651b04.cpp, flags none (`#pragma optimize("s", on)` in the row). ~130 full-row grades + ~40 toy/probe compiles.
Started from the bwf1 draft (DIFFER 26 hunks) ported onto today's headers (12 opt-ins re-applied in hdr/, CPlayerGuiDef via the existing
FABLE_RETAIL_CPLAYERGUIDEF_CLASS opt-in). What closed it, in order:
1. Callee body visibility (biggest lever): DrawButton 0x00645099's landed body defined out of line (`#pragma auto_inline(off)`) ahead of
   Draw. c2 scheduler trace (tools/sched, from pending/2026-11-09_linux_hand892) showed the loop-1 `cmp [eax+ecx+3Ch]` had DAG edges from
   the drawPos stores (escape -> may-alias); with the body visible the edges go, and the uncastable / special / switch sums become
   X-first direct stores (26 -> 17 hunks).
2. Branch target check (bt.py; fg's diff hides branch targets): retail's `if (normal)` false jump 0x0065260B goes to the border
   loop's own size test 0x006528DA -> the interact-border loop is AFTER `if (normal) {...}`, not inside it (behaviour: border drawn in
   special mode too).
3. Middle-ring position (6 missing bytes): retail X-first + `fld st(1)` copy after the uv zero stores = the in-argument
   `C2DVector(C2DVector(GFAnchorToBottomRight(ringsPos) + def.ABXYMiddleOffset))` (outer copy not elided). Found with toy probes
   (probe/toy/s_f4.cpp); named middlePos moves the call before the uv stores, the single C2DVector(...) is elided.
4. Operand order of the anchor+offset sums (c2 record-id keyed; dead-local ruler period 8): a named `const C2DVector& offset` at the
   uncastable site plus two natural names in the special loop (`const CAbilityButtonGuiDef& buttonDef = def.SpecialAbilityButtons[k];`
   for GetAssignableSpecialMoveIndexForButtonDef, `const C2DVector& effectPos = ...Position;` for DrawEffects). Window: ruler k=1 or 3
   breaks it (narrow, as expected for record-id orders).
Reviewer notes: the double functional cast in (3) is the only unusual spelling; it is code-bearing (retail's copy). Made-up name:
HUDRingAlpha (0x01375744) descriptive. NAME_FIXES.tsv carries the bwf1 name proofs (+ GetWeaponType v ICF label).
Things that did NOT work (do not retry): NAMED_RESULT_ADD operator+ (breaks the tooltip sums: retail is the unnamed text there),
`#pragma optimize("y", off)` (390 hunks), /Oa, /Ow, PeekTC without GETTC_EXPANDED (size 4921), operator+ texts with named component
floats / member stores / += (toys), anchor via named const reference (frame addressing instead of retail's [eax]).

## 0x00BB3E80 CEngineClothRenderer::RenderClothPrimitive -- IN PROGRESS, best DIFFER(4540v4540) 20 fg hunks (differ/00bb3e80.cpp = src/)
Ported the bwf1 draft (37 hunks) onto today's headers (14 opt-ins re-applied; CEngineSubPrimitiveMeshBase accessors go into the
EXISTING class in engine/CEngineSubPrimitiveRepeatedStaticMesh.h, not the stale second header; the 2060 RenderUtil content is a new
opt-in _MESHPRIMITIVE because _MESHRENDER is now chunk 2028's getters). What moved it (37 -> 20):
- FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD (existing repo opt-in; out-of-line copy 0x00B66EB0) + `SetEnableFadedShadowFactor(true)`
  at both sites: retail's `cmp [eax+4694h],1 ; mov cl,1 ; je ; xor cl,cl` is the setter's own guard (34 hunks).
- FableWin's first-user COMDAT list after the row: the RenderUtil static wrappers ApplyVertexShader / ApplyPixelShader / GetCurrentCamera
  (existing FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL decls, landed bodies) and the GetPixelShaderXxx getters (new opt-in _CLOTHSHADERS; all
  fwsize SAME) -> 28 hunks, fixed the GetMagnitude X,Z,Y order and the loop pad.
- FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_SUMS (existing) fixes all TransformPoint row orders (24).
- SETRENDERSTATE_BOOKMARK + ADDSTATE_DIRTY (existing) -> 20.
- ApplyTriVertexShader with a named `const CVertexShader& shader` per arm (fwsize 67i v FableWin 68i, was 61i): code-neutral, kept.
Residue (all other fg lines are relocation artefacts: `mov [..],0` / movzx [eax+0x0143B990] table): (1) 0x00BB49F2 SetAlphaRef(0) expansion:
retail `test esi,eax ; or eax,esi ; mov [edx+8],eax`, ours `test eax,esi ; or esi,eax ; mov [edx+8],esi`; (2) 0x00BB4C15 (SetAlphaTestEnable(true)
end of the blend arm) and 0x00BB4C5C (SetZBufferWriteEnable(true) first of the else arm): retail loads CurrentBookmarkMask first; the c2
scheduler trace (tools/sched t13.py) shows those regions with a 1->2 dependency between the two loads in ours.
Did NOT move them: dead-local rulers (outer, mipmap, SetupAlphaBlending, ApplyTri, inside SetRenderState), SetRenderState mask-read texts
(current named / both named / operands swapped: probe opt-ins in hdr/CRenderStateManager.h, diagnostic only -- remove before hand-back),
arm swaps / bool for render_flags & 1, alpha-ref spellings.
Next ideas: GetAppropriateMipmap text to FableWin 81i (ours 75i), SetTextureFactor(RenderUtil) 35i v ours 39i, callee body visibility
(RenderPolygons 0x00BB2540 same file; not landed).
## 0x00B783F0 CEngineWaterRenderer::Render -- not started
- Row 2 hand-over: context ended; continue from differ/00bb3e80.cpp (20 fg hunks, 3 real sites). Probe header for SetRenderState texts was removed from hdr/.
