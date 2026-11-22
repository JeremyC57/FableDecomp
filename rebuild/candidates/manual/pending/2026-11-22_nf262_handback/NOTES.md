# NF262 notes (2026-11-22, round 24): L129 units 3 + 4 done (0x01436EA8: 4 more rows on the PDB spelling); the rest handed back as a plan

Handed back after two finished L129 units (brief: "hand back after any finished unit"; context). 4 rows changed, all RELOCATION_MATCH +
test PASS (recheck_final.log: one `vland --recheck-landed` run over the 4 rows plus the 4 witnesses of the target spellings 0x00BFC140 /
0x00BF6530 / 0x00BEB4D0 / 0x00BEAE50: 8 RELOCATION_MATCH PASS, 0 RELOC_CONFLICT). No header change (hdr/ holds only a README, no orig/: both units use
the landed `FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER` and the landed retail branch of engine/CEngineLandscapeMemoryManager.h),
so no equiv_check was needed. No new split, no new CompilerFlags, no new local model. Repo root (`ls -A`) before / after: identical.

## Unit 3: `?g_LandscapeRenderer@@3PAUNativeLandscapeRenderer@@A` (0x00BFBE40 / 0x00BFBF40 / 0x00BFC040, the CEdgeStipVertexArray<T>::Resize trio)
- The stand-in pointee held a `CEngineLandscapeMemoryManager_Methods*` (a derived stand-in with `AllocVirtual(int, ...)` / `FreeVirtual`), so
  moving the global onto CEngineLandscapeRenderer also moves the two callees. The `_Methods` and `NativeLandscapeRenderer` spellings are used by
  these three rows only (source grep of rebuild/src, rebuild/include, rebuild/tests: the three rows, their self-contained tests, and 0x00BFC140's self-contained test,
  which also models `NativeLandscapeRenderer` without linking any row object), so all
  three addresses drop one spelling: 0x01436EA8 (table lines 16 -> 15), 0x00BD9AF0 (table lines 6 -> 5), 0x00BD9B90 (table lines 3 -> 2). The
  "minus one" is proven; the absolute counts are table lines only (untabled stand-ins exist, e.g. `LandscapeMemoryMethods::FreeAlignedVirtual`
  for 0x00BD9B90 in 0x00BFAAE0).
- Callee spellings, two sources: `?AllocVirtual@CEngineLandscapeMemoryManager@@QAEPAXJPAVCMovableResource@@@Z` and
  `?FreeVirtual@CEngineLandscapeMemoryManager@@QAEXPAX@Z` are in both PDB name sets (Ego_r 0x009C2260 / 0x009C2300, FableWin 0x02CB7A80 /
  0x02CB6E00), and the table already binds them at 0x00BD9AF0 / 0x00BD9B90 (witnesses 0x00BF6530 / 0x00BEB4D0). The header's retail branch
  (`FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER`, landed; precedent 0x00BEAE50) declares exactly these.
- Recipe (tools/u1p.py, applied to all three; each row keeps its own statement shape and include order): define the two macros before the first
  include, delete the two stand-in structs and the extern, `NEngineInternal::GPLandscapeRenderer->MemoryManager` at the three uses. Did NOT copy
  sibling 0x00BFC140's text: it carries a row-local `class CEngineLandscapeMemoryManager` (HEAD debt), which would be a new local model here.
- Accepted by symbol: each object differs from HEAD in exactly three leaf relocation symbols (the three above, same counts) and the same three
  UND names; DEF list identical (only the leaf; pk.py: no other function copy in either object); no vtable; the bare `// Leaf: Resize` matches
  exactly one symbol. Link (the 3 unit-3 objects, LINK cleared, /FORCE /NODEFAULTLIB): 0 duplicates; control (the same 3 + a second copy of 0x00BFBE40): 1
  LNK4006. The 4-object link with 0x00BDFA10 added: 0 duplicates (no control on that run).

## Unit 4: `?gEngine@@3PAUEngineGlobal@@A` (0x00BDFA10, CLocalDetailCacheMap::CQuadTreeElement scalar deleting destructor)
- One binder (source grep). The row already includes engine/CEngineLandscapeRenderer.h, so it takes the header opt-in; the EngineGlobal /
  LandscapeMemory stand-ins go. Its 0x00BD9B90 call already bound the PDB FreeVirtual through the row's own `class CEngineLandscapeMemoryManager :
  CEngineLandscapeMemoryManagerLayout` (renamed-include local model: HEAD debt, untouched; the renderer header only forward-declares the class, so
  `GPLandscapeRenderer->MemoryManager` is that class and the callee spelling stays). Object differs from HEAD in one relocation symbol / one UND
  name; DEF list identical; no vtable. 0x01436EA8 table 15 -> 14.

## Recheck WARN lines (recheck_final.log)
- The three unit-3 rows: none. 0x00BDFA10: SHARED_HEADER_AVAILABLE CEngineLandscapeMemoryManager and CLocalDetailCacheMap (its renamed-include
  local models: HEAD debt, untouched) and STANDIN_LEAF on its own leaf `?ScalarDeletingDestructor@CQuadTreeElement@CLocalDetailCacheMap@@QAEPAXI@Z`
  (HEAD state). None is about 0x01436EA8.

## Tests
- All four tests are self-contained host models (each inlines its own copy of the row body and defines its own global); they do not link the
  row object and PASS unchanged. Their rewrite onto the shared headers is queued with the other header-based tests.

## Plan for what is left of L129 (14 stand-in spellings: 13 tabled + `?g_obj_01436ea8@@3PAUCGlobal@@A`; 25 rows; none changed)
Found while choosing units (read, nothing compiled):
- **GB, 0x00BF36A0 / 0x00BF9920 (`?g_00bf36a0`) and 0x00BF36E0 (`?g_00bf36e0`)**: three-line July rows whose leaves are `fn_<addr>` placeholders
  (0x00BF9920's source is 0x00BF36A0's text, defining the same `?fn_00bf36a0@@YIPAXH@Z`). Retail bodies (xdis) are class operator deletes:
  `mov eax,[GPLandscapeRenderer] ; push ecx ; mov ecx,[eax+6B0h] ; call FreeVirtual ; ret`, and their binders spell them
  `??3CEngineLandscapePatch@@SIXPAXJ@Z` (0x00BDD3E0) / `??_VCEngineLandscapePatch@@SIXPAXJ@Z` / `?CEngineSubPrimitiveGameMesh_dealloc@@...`
  (0x00BDFD60). FableWin's name set has `??3CEngineLandscapePatch@@SIXPAX@Z`, `??_VCEngineLandscapePatch@@SIXPAX@Z` and
  `??3CLocalDetailPrimitiveMesh@@SIXPAX@Z` (Ego_r only the placement forms `...SIXPAXPAVCMovableResource@@@Z`, which have the same bytes under
  __fastcall). So the global cannot move without the self rows moving: this unit is a self-rename family (3 self rows + their deleting-destructor
  binders, the 0x00BD9B90 `?m@That2@@QAEPAXH@Z` stand-in with them), with the plain-v-placement choice settled from the callers (whether edx is
  set before the call). Catalog names (`CEngineSubPrimitiveBase::operator_new`) are wrong for all three. Not a naming-only unit.
- **`?g_obj_01436ea8` (0x00BE5C30) + `?gRenderer@Native` (0x00BE6090)**: both call 0x00BDA110 through stand-ins (`?ReleaseTexture@CTexMgr@@QAEXPAX@Z`,
  `?DeleteTexture@LandscapeMemory@Native@@QAEXPAVCTexture@@@Z`). Candidate `?DeleteTexture@CEngineLandscapeMemoryManager@@QAEXPAVCTexture@@@Z`
  (Ego_r name set; needs its second source and a header declaration opt-in). One unit of two rows, 0x00BDA110 2 -> 1.
- **LocalDetailEngine (0x00BFAAE0 / 0x00BFB590)**: large local-model rows (CMovableResource / CIVCountedPointeeBase / CEnginePersistentPrimitiveBase
  included under renaming macros and re-modelled; a `LandscapeMemoryMethods::FreeAlignedVirtual` stand-in for 0x00BD9B90). A port, not a rename.
- **0x00B684F0 (`?g_pEngineGlobal_01436EA8`)**: CallView stand-ins on CEngineLandscapeMap / MemoryManager / Renderer (5 callee spellings);
  the global's struct carries them. A port.
- **CEngineGlobal (0x00BE61B0 / 0x00BE79D0 / 0x00BF8030 / 0x00BF84C0)**: two of them fail the clang precheck at HEAD (nf260); last.
- The rest as NF260 listed them (`DAT_01436ea8` x6 on July candidates headers, the singles 0x00B3B1A0 / 0x00B3B4A0, 0x00BFB210, 0x00BDE860,
  0x00BE3660, 0x00BEAA30, 0x00B428E0), each with its own callee stand-ins at 0x00BD9AF0 / 0x00BD9B90 / other manager members; check each
  callee spelling's binders before drawing the unit, as unit 3 needed.

## Entry 2 (0x009B0AC0): open, not attempted
- NF260's plan (its NOTES "Entry 2") stands unchanged: prove the 0x009B9490 sort spelling first, then the CDefinitionManager.h opt-in splitting
  `unsigned char NameCRCToIndexMap[16],DefClassInfo[16];` (equiv_check), then the port. Locks 0x009B0AC0 / 0x009AD390 read `released`; I did
  not claim them.

## Decisions for the coordinator
- D1: unit 3 also moves two callee spellings (0x00BD9AF0 / 0x00BD9B90 off `CEngineLandscapeMemoryManager_Methods`): needed because the stand-in
  pointee carried them, count-reducing at both addresses. Accept as one unit?
- D3: in unit 4 the header's `NEngineInternal::GPLandscapeRenderer->MemoryManager` resolves to 0x00BDFA10's own row-local
  `class CEngineLandscapeMemoryManager : public CEngineLandscapeMemoryManagerLayout` (a renamed-include local model of a class that has a header:
  HEAD debt, disclosed, untouched; the callee spelling is the PDB one either way). Accept the global move on top of it, or hold the row until the
  model is ported onto the header's retail branch?
- D2: the GB rows (0x00BF36A0 / 0x00BF36E0 / 0x00BF9920) are class operator deletes with placeholder self leaves and wrong catalog names; their
  0x01436EA8 spelling can only move with a self-rename family (plain `SIXPAX@Z` v placement form to settle). Queue it as its own item?

## Flags / funclets / descriptive names / locks / slips
- Flags: none new (the four rows have no CompilerFlags and none were added). Funclets: none (include grep of the four sources: 0 hits).
- Descriptive names: none added; two stand-in structs per row removed.
- Locks (rows.txt, NF262): the 4 changed rows and the 4 witnesses compiled as controls (0x00BFC140, 0x00BF6530, 0x00BEB4D0, 0x00BEAE50).
  Release with the landing.
- Order: every compile ran after its lock was NF262 and its clang precheck (row source and test) passed, in a later tool call, under an outer
  `timeout 900`. Clang-only precheck failures reproduced by two-line VC7.1-header controls and graded anyway: 0x00BEAE50's test (`<cwchar>`) and
  0x00BDFA10's test (`<assert.h>`); both PASS under VC7.1.
- Reading: the chained briefs NF260 back to NF215 and NF214's read-first list (NF92, NF44, NF90 / NF89 / NF88 lessons, ASM_RULES, EXTRA_RULES,
  COORDINATION), the nf260 / nf258 / nf256 READMEs and the nf260 hand-back, and GOTCHAS.md end to end (all 501 lines).
- Slip: one background launch line contained `sleep 0` (a no-op, no wait), which the "never any foreground sleep" rule forbids literally; jobs
  were otherwise waited on by PID with kill -0 in later calls.
