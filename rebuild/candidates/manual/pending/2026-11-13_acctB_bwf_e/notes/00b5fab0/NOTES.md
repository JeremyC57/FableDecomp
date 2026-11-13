## 0x00B5FAB0 progress (author 1)
- grade ~25: DIFFER(5302v5302) 43 hunks from full draft b.cpp (implicit CRenderTarget dtor = no vptr store; CSE local `top`/`bottom` in loops).
- Residue: offsetX slot 0x18 (retail 0x28) shifts all C4DVector temps +4; x87 `fld scale; fmul off` order; reg swaps in inline setters. Tried: static placement, decl orders, named vectors, const static, named left/right/top/bottom -- no change.
- grade ~60: w2.cpp DIFFER(5302v5302) 6 hunks. What moved it: threshold as a C4DVector temporary per if/else arm passed straight to SetPixelShaderConstant (fixed every frame slot AND the x87 scale*offset order).
- Left: loop-only ties -- inline SetAlphaBlendEnable at loop top (mask edi/flags edx swapped) and mid-loop (load order), loop-end `++i`/N-load scheduling. Tried with no effect: while/do/for-increment forms, int/unsigned i, reversed compare, statics placement, named weights/intensity/motionBlur, scratch/backBuffer pointer forms, SetAlphaBlendEnable inside blocks, for-filled textures (worse).
- grade ~100: still w2 6 hunks. Probes: dead-name sweep at fn top flips only the x87 scale*offset order (record ids), never the setter/loop-end ties; address-escape of offsets/i, volatile N, QFE 4035 compiler, EARLY_RETURN SetRenderState, CPixelShader copy-ctor texts, CSurface/CRenderTarget inline forms: no effect on the 3 loop ties. Current lead: c2 81-tuple scheduling-region cut in the loop-B tail block (no-code grouping changes).
- grade ~140: na1.cpp DIFFER 4 hunks. What moved it: natural named locals in the two loop blocks (`CTexture* destination`, `const CTexture* source` for the textures array, `C4DVector weights`) -- ~5..16 extra symbol records in the loop body fix the mid-loop setter load order (dead-name probe window k=5..16 at loop top).
- Left: loop-top setter regs (mask edi / flags edx) and loop-end ++i store / NoBlurPasses load order; neither moves with dead names before the loop, at loop top, in loop B, or at loop end.

## 0x00B5FAB0 hand-back (author 1, ~170 grades) -- DIFFER(5302v5302), 4 hunks, best = differ/00b5fab0.cpp
- Headers: hdr/ overlay (README): GLOWRENDERER_CAPTUREBACKBUFFER, CDISPLAYMANAGER_SETRENDERTARGET, CGPUSCRATCHBUFFERTEXTUREMANAGER_STARTTEXTUREUSE,
  CPIXELSHADER_COPYCTOR; repo opt-ins CTEXTURE_SIZED (GetSurfaceLevel), CSURFACE(+COPYFROMMEMORY), CRENDERMANAGERCORE(+FORCEINLINE_HELPERS,
  DRAWASINDEXEDTRIS, _USER), CENGINERESOURCEMANAGER, CDISPLAYMANAGER(+BACKSCREEN), CENGINECAMERA(+REFRESHD3DTRANSFORMS). No flags.
  CRenderTarget = local model (no shared header): implicit destructor (an empty user dtor adds the vptr store retail lacks).
- What closed 90 -> 43 -> 6 -> 4 hunks: implicit CRenderTarget dtor; CSE locals in the loops (top/bottom, named left/right/top/bottom);
  threshold as a C4DVector temporary per if/else arm (fixed all frame slots + x87 order); named destination/source/weights locals in
  the two loop blocks (record count fixes mid-loop setter load order).
- Residue: (1) loop-top inline SetAlphaBlendEnable: retail mask->edi, flags->edx, ours swapped (lane gap noted in CRenderStateManager.h:
  bookmark test/or operand order varies block by block). (2) loop end: retail `inc; mov [i],eax; fstp X; fld s; mov ecx,[N]; cmp`, ours loads N
  right after `mov eax,[i]` and stores i after the cmp. Scheduler DAG dump (agent2031/tools/sched.sh, cfg pat [fld,fmul]): no alias edges
  i-store->N-load in ours; N-load prio 38912 > inc 7168, so retail must have a different DAG/priority for N-load or inc.
- Tried without effect on (1)/(2): for/while/do-while/guarded do/for(;;)break/for-increment multiplies/++i between multiplies, i=i+1, i++,
  int/unsigned i, reversed compare (worse), N as int/unsigned/extern/global/volatile(probe)/local copy (worse), statics placement, address escape
  of offsets/i (probe), dead names (fn top, before loop, loop top, loop B, loop end: only the mid-loop tie and x87 order move), casts/no-code
  grouping on multiplies, Draw-argument spellings, textures via cast, CRenderTarget ctor/dtor forms, CPixelShader copy-ctor texts, CSurface
  FORCEINLINE_DTOR on/off (same), SetRenderState EARLY_RETURN, QFE 4035 compiler (probe), named intensity/motionBlur/pass-2 weights.
- Best next idea: dump the retail-equivalent DAG question with sched.sh on the loop-end region (cfg pat ["fld","fmul","fstp","fld","fmul","fstp","mov","inc"])
  and find which IL shape gives `inc` priority over the N load (e.g. the compare reading i from memory: i spilled/address-taken in a way c2 keeps,
  or the loop condition in an inline predicate `while (MoreBlurPasses(i))`), then the loop-top register tie (try the loop-top setter inside an
  inline helper shared by both halves, e.g. a real per-pass helper if one is ever found in FableWin's COMDAT list).

## attempt 2 (author 2)
- grade ~20: vb1.cpp DIFFER 1 hunk. What moved it: `static volatile float BlurOffsetScale` + `static volatile long NoBlurPasses` (tweakable statics). DAG proof: retail orders fld scale(2) -> load N, a load->load edge only volatile gives; loop end now exact.
- No effect (attempt 2): address-taken i/offsets (dead &, by-ref inline helpers, escape to global), C-style decls at fn top, compare forms, region-break loop shapes, /G5 /G7 /Op /Oa /Ow /GA.
- grade ~45: still vb1 1 hunk (loop-top setter regs). KEY PROBE: 36..44 (loop top) or 40/80 (fn top) dead `float probeN = 1.0f;` locals give RELOCATION_MATCH (v2/lk36.cpp, kt3, kl1, kf80; copy in differ/lk36_probe.cpp) -- the loop-top tie is a record-id effect of the number of INITIALIZED locals in the function (uninit symbols / bare loads / includes do nothing; not monotone: 33 no, 150 no, 60-with-(void) no). Need a natural source of ~35+ more records.
- FableWin /Od sizes (fwsize FW_EXTRA=/EHsc): SetPixelShaderConstant FableWin 28i/79b = `PSConstTable[index] = value;` (hdr opt-in FABLE_RETAIL_CSHADERRENDERMANAGER_SETPSCONST_INDEX, proven by size); with it (ps1.cpp) the mid-loop setter flips back (2 hunks) and record windows move. Row body /Od 764i v FableWin 963i (+199i: asserts or missing text). Consistent destination/source/weights names in block 64 / block 16: no change.
- ps1 (FableWin-sized SetPixelShaderConstant text) + fn-top floats k=1..120: loop-top setter NEVER flips (only the mid-loop toggles) -> the repo text `*(PSConstTable.begin() + index)` is the retail one (FableWin is a later build); best source stays on the repo header.
- No effect on the loop-top tie (attempt 2): SetAlphaBlendEnable / SetRenderState text opt-ins (operand order, `!(..)`, `x = x | m`, `SetRenderState(enable)`), loop shapes on the volatile base (while, guarded do, for(;;) break, if N>0 do), int i, setter inside block A, renaming locals (offsets, target, scratch: names do not matter, only count), consistent destination/source/weights names in block 64 / 16, named motionBlur/weights/intensity vectors, const texelSize locals, extra TU includes, flags /G5 /G7 /Op /Oa /Ow /GA. Block-1 `destination` local breaks 13 hunks.
- Mechanism facts: dead const-init float locals flip the loop top at k>=36/37 (vb1, any position: fn top, loop top, fn END), k=80 too, k=150 not; uninit symbols, repeated stores to one local, bare loads, includes do not count. So retail's function carries ~36+ more initialized local records (inline-expansion params / named locals) than our text -- probably debug-only or assert-related text or helper layers we do not model.

## attempt 2 hand-back -- DIFFER(5302v5302), 1 hunk, best = differ/00b5fab0.cpp (= v2/vb1.cpp)
- What moved it 4 -> 1: `static volatile float BlurOffsetScale` and `static volatile long NoBlurPasses` (tweakable function statics). Evidence: retail's loop-end schedule needs a load->load edge fld BlurOffsetScale(2) -> mov ecx,[NoBlurPasses]; only volatile ordering gives it (DAG dump v2/sch_base.json: no such edge without volatile, load-N prio 38912 beat inc 7168).
- Residue: loop-top inline SetAlphaBlendEnable(false) 0x00B60665 mask->edx/flags->edi (retail edi/edx). A record-count effect: v2/lk36.cpp / differ/lk36_probe.cpp (36 dead floats) grades RELOCATION_MATCH, but that is a probe, not a source.
- Best next idea: find the natural text that adds ~36 initialized locals: an extra inline layer in the 12 render-state setter expansions (e.g. the real SetRenderState/AddStateToUpdateList with a helper or named locals that leave no code; check 0x00B70230 / 0x00B761D0 / 0x00B751F0, which have the same block-by-block tie, against the same count probe), or per-block helper inlines; check each candidate with the count probe (grade candidate + k dead floats to measure its weight).

## attempt 3 (author 3) -- RELOCATION_MATCH (v3/final.cpp = differ/00b5fab0.cpp = 00b5fab0.cpp; test 00b5fab0_test.cpp, vt PASS, strict headers MATCH)
- What made it match: the loop-top register tie is the lane's bookmark or-operand tie (c2prio: our OR wrote the mask temp #263, prio 80 -> edx;
  retail writes the BookmarkMask temp). It moves with the TOTAL count of named symbols (ruler: +36..44 dead floats anywhere). The lane's
  FITTED opt-in SETRENDERSTATE_DESIRED (chunk 2048) counts exactly 12 (ruler window moved to +24..32). Two more code-identical named locals
  per expansion (SETRENDERSTATE_BOOKMARK `bookmark_mask = state.BookmarkMask`, ADDSTATE_DIRTY `dirty = state.DirtyListFlag`) = 36 total fixed
  the loop top; the mid-loop setter's load order then needed one more symbol in the loop: a named `const float origin = -1.0f / 128.0f` tap in
  both loop blocks. Not on an edge (still matches with +1 / +3 diagnostic floats).
- FableWin hint for the count: debug SetRenderState +168i (2 asserts), AddStateToUpdateList +85i (1), StartCaptureBlock +166i (2), scoped
  capture block ctor/dtor +81/+83i (1 each) = 3 per setter expansion x 12 + 4 = 40, the middle of the 36..44 window: retail's release asserts
  probably left one symbol each. A release-assert macro with memory-reading conditions (`{ const bool c = (e); }`) changes scheduling (5 hunks);
  `__assume` changes code; `(void)(e)` adds nothing. Setter-level `const unsigned long value = enable ? 1 : 0` changes code (41 hunks).
  Named mask local (`bookmark_mask = CurrentBookmarkMask`) and `CStateInfo* const entry = &state` change load order (3 hunks).
- Headers: hdr/engine/CRenderStateManager.h (full copy of HEAD + the three opt-ins, README lines) on top of the earlier overlay; no flags.
- Made-up local names: origin, bookmark_mask, dirty (descriptive; no PDB local records). Names otherwise unchanged from attempt 2.

## repair (verifier findings) -- still RELOCATION_MATCH (also LANDVERIFY_STRICT_HEADERS=1), vt PASS; probes in r/
- Comments only (no code change): src header now has a `// Retail-volatile:` line (0x00B60C79..0x00B60C9C load order), a FITTED
  paragraph for the `origin` locals citing the mid-loop SetAlphaBlendEnable(false) before loop block B (0x00B6094A..0x00B60985; without
  them 0x00B6097C/0x00B60982 loads swap, r/noorig.cpp 2 hunks), and the FITTED record-count paragraph citing 0x00B60665 mov edi,[eax+0x2814]
  / 0x00B6066B mov edx,[ecx+8] (without BOOKMARK/DIRTY the registers swap, r/nobd.cpp 1 hunk). hdr CRenderStateManager.h opt-in
  comments (SETRENDERSTATE_BOOKMARK, ADDSTATE_DIRTY) carry the same citation.
- Made-up names (descriptive, no PDB record anywhere): locals `origin`, `bookmark_mask`, `dirty`; function statics `BlurOffsetScale`
  (0x0139A6B0), `NoBlurPasses` (0x0139A6B4), `QuadIndices` (0x0139A6B8) (DOSSIER: no PDB names; Fable PDBs not on this lane's disk).

## Verifier findings not applied
- Finding 1 (`volatile` on BlurOffsetScale / NoBlurPasses): NOT dropped, needs a COORDINATOR DECISION. Without it the row does not match:
  r/nv1.cpp (both non-volatile) and r/nv2.cpp (`*=` form) DIFFER 3 hunks at the loop end (retail 0x00B60C88 mov [i],eax / 0x00B60C96
  mov ecx,[0x0139A6B4] after the second fld [0x0139A6B0]; ours loads N right after `mov eax,[i]` and stores i after the cmp);
  r/v_n.cpp / r/v_s.cpp (volatile on only one static) same 3 hunks; r/nv3.cpp (N copied to a local + break form) 88 hunks.
  Prior attempts (~300 grades: loop shapes, int/unsigned, extern/global N, address escapes, flags, QFE compiler) found no other text.
  Honest status: no PDB LF_MODIFIER and no vendor source is available (Fable PDBs not on this disk); the only evidence is that retail's
  loop-end order needs the volatile load->load edge. No read is dropped without it, so ASM_RULES' first bullet is not literally met;
  the source now documents this in a `// Retail-volatile:` line. If the coordinator rejects it, the row is DIFFER(5302v5302) 3 hunks
  with r/nv1.cpp as the best non-volatile source.
