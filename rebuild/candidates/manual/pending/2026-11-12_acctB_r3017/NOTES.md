# Hand chunk 3017 (CEngineClothRenderer): 1 of 1 byte-exact

Grade with `export FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent3017/hdr` (gr.sh exports it). Read-only verifier
`vland.py payload.json oracle.tsv` (no --land): RELOCATION_MATCH + behaviour PASS, no RELOC_IDENTITY / RELOC_NAME lines; work dir deleted.
Tests: `00bb1f50_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py). Purity: `retail_asm` (crawl/purity.py on src/00bb1f50.cpp).

## What matched (1)
- **CEngineClothRenderer** 0x00BB1F50 `?CalculateVertexNormals@CEngineClothRenderer@@AAEXABVCParticleSimulation@@ABVCParticleProgram@@@Z`
  (1508 bytes) RELOCATION_MATCH. Started from chunk 2008's parked source (420/425). What closed it:
  1. **Prologue register (0x00BB1FDB `mov edi,[esi+98h]` / 0x00BB1FED `sub edx,edi`)**: read `PositionBuffer` into its named local BEFORE
     `GetRenderTriangleCount()` (statement order of the four fetches after GFClear4DVectors). One grade of a 6-way order sweep (c06). PROVEN by
     the grade only (no second source; the order is the natural "pointers, then counts" one).
  2. **Triangle-arm scheduling (0x00BB21CC..0x00BB221B, the W self-copy store and the next fld)**: c2's 81-tuple scheduling region of the
     x87 arm. Traced with the c2 scheduler DAG dump (tools/sched, copied from agent2031): with the prior CROSS_X + SUBTRACT_XY opt-ins
     the arm is 95 tuples, cut after corner 2's W load (corner-2 W store fell into the next region). A diagnostic paren ruler (dg*/dk*
     probes, never a hand-in) showed retail needs exactly 2 fewer no-code tuples before the cut (+12 puts corners 2+3 in one fresh region and
     still misorders; -1 fixes corner 2 but breaks corner 3). Since the quad loop shares the helpers (triangle 2 subtractions + 1 cross,
     quad 4 + 2), the per-helper paren distribution must give triangle T = 3: subtraction plain (the repo default) and the cross with each
     component grouped -> **FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP**, which also keeps the whole quad loop. FITTED (PAREN tuples are no-code;
     FableWin /Od sizes cannot see parentheses), but: it is the one distribution of per-component groups that fits both loops, and it
     regrades all 6 landed operator^ users unchanged (below), so it can serve as the single shared cross text.
  3. Corner indices read through FableWin's inline accessors `C3DTriangle2::GetA/GetB/GetC` and `C3DQuad2::GetA..GetD` (FableWin debug
     COMDATs 0x02E77990..0x02E77A50, listed in the cloth TU's COMDAT run after CalculateVertexNormals 0x02E76000; fwsize SAME 9i/17-18b).
     Byte-neutral here (graded both ways), chosen on the PDB evidence (prefer the PDB's inline accessors).
- An operator+= with W stored before Z (`X; Y; W; Z`) also matched (c4_XYWZ), but retail's uncalled out-of-line copy of
  `C4DVector::operator+=` at 0x00A4B9B0 (and 0x00885F20) is X, Y, Z, W: the XYZW body grades MATCH against 0x00A4B9B0 (qx.py,
  QLEN=0x2a), XYWZ does not -- so that spelling was rejected as contradicted by retail.

## Shared-inline regrade (CROSS_XYZ_GROUP as a candidate single text)
Opt-in only (default text unchanged, no landed row changes). Every landed row that expands C3DVector::operator^ graded with
CROSS_XYZ_GROUP forced on (probe/xyz, qx.py against the catalog oracle) -- all unchanged:
0x00A42170 CPlane::Initialise (uses CROSS_XY_GROUP today) RELOCATION_MATCH -> RELOCATION_MATCH; 0x00E64210 CKickedChicken::Main RM -> RM;
0x007BC6FA CTCShotDefinitionProjectilePredictive::InitMovement RM -> RM; 0x00BEBAA0 CalcObjectMatrix MATCH -> MATCH;
0x0063AB90 CWorldMap::GetThemeFaceNormalAt RM -> RM; 0x00AC8C80 COptimisedPrimitive::ComputeVertexCost RM -> RM.
So the landing session can make CROSS_XYZ the default operator^ body and retire CROSS_X_GROUP / CROSS_XY_GROUP (review at landing).

## Asm helper (the SSE block 0x00BB2079..0x00BB20ED)
- `engine/CEngineClothRendererSSE.h` GF_SSE_ADD_TRIANGLE_NORMAL (chunk 2008's helper, unchanged; descriptive name; FABLE_PORTABLE twin;
  dual-path test hdr/tests_portable/test_CEngineClothRendererSSE.cpp, portable PASS re-run here with g++). Evidence: the block re-reads
  positions / normals / the three corner indices from frame words (0x00BB2079..0x00BB208F) that 0x00BB205E..0x00BB206C stored just before
  the g_SSEAvailable test while the x87 arm keeps them in registers, and the function has no `and esp,-16` frame.
- **ASM_RULES rule 4 per-site table: DONE this session** (the debt chunk 2008 left): probe/asm/gen.py writes 31 plain-C++ + 12 intrinsic
  spellings plus a positive control, each in the row's context (triangle loop, indices through the remap, `if (g_SSEAvailable)`);
  run.py compiles at the row flags and with /arch:SSE and /arch:SSE2 and records what VC7.1 emitted; probe/asm/table.md is the table
  (45 rows x 3 flag sets). Result: no plain-C++ spelling emits packed SSE (x87 at row flags; scalar movss/mulss/subss/addss under
  /arch:SSE[2]); every intrinsic spelling emits packed xmm code but with an aligned frame (`and esp,-16`), its own xmm numbering /
  instruction order and no frame re-read of the operands; only the control (the helper) emits retail's block, under all three flag sets.
  The reached site is unchanged from chunk 2008's helper.

## Names
- Proven (two sources; LABELS.tsv): the row (Ego_r 0x00A67E10 + FableWin 0x02E76000), GetNormalBuffer 0x00BB1E90, NVectorMath::GFClear4DVectors
  0x00A5C300 / GFNormalise3DVectors 0x00A5C360 (bound spellings), g_SSEAvailable 0x013D2880 (reloc_bindings), C3DTriangle2 / C3DQuad2
  GetA..GetD (FableWin ?GetA@C3DTriangle2@@QBEKXZ etc.; inline, no retail address).
- Descriptive: GF_SSE_ADD_TRIANGLE_NORMAL / fable_portable::AddTriangleNormalSSE (inline asm in retail, no symbol); all local names.
- No landed spelling found wrong (NAME_FIXES.tsv header only).

## Header debt / overlay
See hdr/README.md. New this session: FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP (C3DVector.h copy of the repo header),
FABLE_RETAIL_C3DPRIMITIVE_CORNER_ACCESSORS (C3DTriangle2.h / C3DQuad2.h copies). The rest of hdr/ is chunk 2008's overlay as handed over.

## Grades
47 full-row grades (grades.log; 11 of them diagnostic ruler probes dg*/dk*/s0c3 that are not hand-ins), plus free compiles: 6 c2 scheduler
DAG dumps, ~10 fwsize runs, 2 qx checks of operator+= against 0x00A4B9B0, 12 qx regrades of landed operator^ users, 3 asm-table compiles.
