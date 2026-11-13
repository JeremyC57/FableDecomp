# Chunk 3017 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00BB1F50) A one-register prologue residue (`mov edi,[esi+98h]` v ours eax for a CArray begin load) closed by the ORDER of the
  plain fetches after a call: the member pointer (`positions = PositionBuffer`) before the inline count (`GetRenderTriangleCount()`).
  Sweep the order of adjacent named fetches first; it costs one grade per permutation.
- (0x00BB1F50) Scheduling residue inside a long straight block = c2's 81-tuple region cut. Measure it, do not guess: dump the region
  list with the scheduler DAG hook (agent2031 tools/sched/t13.py, pattern ["zzz"] lists every region's size), then use a DIAGNOSTIC paren
  ruler (extra grouping parentheses in the shared helper, never handed in) to find how many no-code PAREN tuples retail has before the
  cut. Here retail needed 2 fewer than the parked CROSS_X + SUBTRACT_XY opt-ins; a fresh region starting exactly at a corner add
  misorders it (`fadd` before the W load), so "move the cut to a statement boundary" is not the rule -- count.
- (0x00BB1F50) When two loops of one row share helper texts, solve the paren count per helper call: triangle arm = 2 subtractions +
  1 cross, quad = 4 + 2; only "subtraction plain, cross grouped per component" (FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP) fits both.
- (0x00BB1F50) A fitted helper order (C4DVector::operator+= storing W before Z) matched the row but is contradicted by retail's uncalled
  out-of-line copies 0x00A4B9B0 / 0x00885F20 (X, Y, Z, W; XYZW grades MATCH against 0x00A4B9B0 with qx.py QLEN=0x2a). Scan retail .text
  for uncalled copies of a helper before accepting a helper-text fit.
- (0x00BB1F50) The FableWin COMDAT run after a row names the accessors it used: C3DTriangle2 / C3DQuad2 GetA..GetD (not GetVertexIndex).
- (purity) A row's `// Retail-asm:` tag line must cite at least one of the helper's mnemonics (e.g. `movaps xmm2,[esi+eax]`) with a
  retail address, or crawl/purity.py grades the row naked_asm and vland prints NOT_GENUINE.
- (lane) tools/sched/host.py runs with the repo as cwd: give sched.sh an ABSOLUTE output path, or the json lands in the repo's tmp/.

Review at landing:
- FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP regrades all 6 landed operator^ users unchanged (NOTES.md list, incl. CPlane::Initialise which
  uses CROSS_XY_GROUP today): consider making it the default operator^ body and retiring CROSS_X_GROUP / CROSS_XY_GROUP.
- The SSE helper engine/CEngineClothRendererSSE.h now has its ASM_RULES per-site table (probe/asm/table.md, gen.py / run.py / table.py);
  copy the table into the batch README with the helper and put tests_portable/test_CEngineClothRendererSSE.cpp in rebuild/tests/portable/.
- hdr/ carries chunk 2008's overlay (re-base on HEAD before landing).
