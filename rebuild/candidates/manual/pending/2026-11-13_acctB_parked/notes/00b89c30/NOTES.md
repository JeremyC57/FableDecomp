## 0x00B89C30 ApplyOutline -- author session 1 (grades 1-28)
- best after 28 grades: DIFFER(6431v6431) 1 real hunk (v19): SetAlphaRef(1) block loads state.BookmarkMask before CurrentBookmarkMask. Moved it: scope `tex` (i/tex slot share), `static volatile float` (x87 copies), named `CRenderTarget current = GetRenderTarget()` (fixed SrcBlend block).
- did not move AlphaRef: /Op, EARLY_RETURN SetRenderState, 1UL, if-wrap, static decl positions, tex array, textures by stores, loop block, viewport ref, i outside for, final-section block, pass-0 consts in block.
- grades 29-50: AlphaRef load-order hunk is a flip point (retail: state-first for Mag0..AlphaFunc of section 2, eax-first from AlphaRef); named `current` moved it 1 block; a named viewport ref (v44) instead moved the SECTION-1 StencilEnable test-order flip the wrong way. Include order, static form ([2], file scope), arg types, named surfaces (code change), named source pointers (worse) did not help.
- grades 51-75: LOCAL_MASK SetRenderState opt-in (overlay, unused) makes every block eax-first (21 hunks); alias-reference knobs (+1 symbol: no change, +2: flip moves to the end), while-loop, file statics, const volatile, QuadIndices[] -- none fixes AlphaRef alone.
- grades 76-95: probes show section-2 'S band' (state-first blocks) starts at Mag0 and its END moves with upstream IL (v7 ends after SrcBlend, v19 after AlphaRef, retail after AlphaFunc; removing pass 0 or the loop moves the band later). Ref-bound/named GetSurfaceLevel temps change code (worse).
- grades 96-125: dead/extra locals (bool results of SetRenderTarget, aliases, dead longs): +1 no change, +2 -> every section-2 block state-first. No GetRenderTarget at all -> band 8; temp form (v7) 7; named (v19) 6; retail 5. Function-scope shared textures array (w1) moves the band later. Non-const arrays + casts: no change.
- grades 126-170: v7 + any one extra local -> band 6 (same as v19); probes rm1/rm2/rm4 (deleting 1/2/4 section-1 UV setters) move the band END later by 1/2/4 blocks => retail has about ONE SETTER BLOCK more IL upstream of section 2 than ours (or equivalent). Tried, no effect or code change: __assume asserts, texel-size float locals (1 local: none; 2: all-S), &QuadIndices[0] / &Verts[0], per-stage loops (not unrolled -> code), `if (BlurStepCount > 0)` guard (code change), /Op (none), /Ow /Oa (code), /G5 (none), /G7 (code), include order, statics placement/forms, scope shapes, alias refs, dead bools.

## Hand-back (session 1): 0x00B89C30 ApplyOutline -- DIFFER, 1 real hunk
- Best: differ/00b89c30.cpp (= c/v19.cpp + header). DIFFER(6431v6431): only the SetAlphaRef(1) block's two independent loads are swapped
  (ours `mov esi,[edx+8]` before `mov eax,[eax+0x2814]`; retail the other way). fg also prints 2 "replace" hunks of identical bytes
  (00b8a8af / 00b8b0d2) -- relocation-mask artefacts of fg's normaliser (an imm32 0 in ours), not real differences.
- What made it close: shared headers + overlay (hdr/README.md: new CRenderTarget.h, CDisplayManager SETRENDERTARGET/GETRENDERTARGET,
  CSurface ASSIGN, OutlineGlow MEMBERS); `static volatile float` blur offsets (x87 copies); `tex` in its own block (shares i's slot);
  named `CRenderTarget current = GetRenderTarget(); OldRenderTarget = current;` in a block (fixed the SrcBlend block).
- Model of the residue: in section 2 a run of blocks is "state-first" (retail Mag0..AlphaFunc = 5 blocks; ours Mag0..AlphaRef = 6).
  The run's end moves with upstream IL volume: deleting 1 section-1 setter moves it 1 block later; the GetRenderTarget spelling moved it
  (none 8 / temp 7 / named 6); +2 extra locals anywhere after section 1 makes every section-2 block state-first. So retail has a little
  MORE code-free IL between section 1 and section 2 (about one setter block) than this source, without extra locals.
- Best next ideas: (1) look for a code-free construct the original likely had between section 1 and the composite (an inline helper
  layer the PDB lists for this TU, e.g. a CRenderTarget / CDisplayManager inline accessor with more statements, or a CTexture accessor for
  GetSurfaceLevel); (2) try the passes' SetRenderTarget / PrepareForRender written through an inline helper the debug build lists; (3) an
  intermediate named object of a type with an inline no-op ctor/dtor (same effect class as the `current` fix) in pass 0 or the loop.
- Names: BlurOffsetU / BlurOffsetV are descriptive (function-local statics 0x0139AB54 / 0x0139AB58, no name anywhere); QuadIndices as the
  twin rows. No landed name proved wrong in this row (dossier side note: the landed 0x00B89080 "ReleaseTextures" is probably
  ?FinishWithDynamicTextures@CEngineScreenEffectOutlineGlow@@AAEXXZ -- one source only, not added to NAME_FIXES).
- session end: 129 grades; best differ/00b89c30.cpp (v19). Also tried after the hand-back block: CSurface(...) functional casts (none), chained textures assignment (store order changes), v23 viewport inside the current block (dtor order changes). FableWin COMDAT order puts CRenderTarget::GetPRenderToSurface right after this TU's functions (probably DrawDebugTextures' use, not ApplyOutline).

## Session (bwf rows 1-4 agent, 2026-11-13) -- still DIFFER(6431v6431), 1 hunk (AlphaRef block load order); best source unchanged
- New probe tools: c2/pat.py (per-SetRenderState block token: load order S/C + test/or operand roles m/b), c2/sw.sh (compile + token diff
  v retail, ~1.2 s per compile), c2/mk.sh (adds header opt-in defines). Retail tokens differ from ours ONLY at block 28 (AlphaRef).
- Overlay hdr/engine/CRenderStateManager.h restored to the session-1 copy (LOCAL_MASK probe opt-in); the bwf_e copy (DESIRED/BOOKMARK/DIRTY
  opt-ins) was tried: DESIRED alone 13 hunks, DESIRED+BOOKMARK+DIRTY 16, DIRTY alone / DESIRED+BOOKMARK / +DIRTY: 3..5 token diffs.
- Ruler grids (probes, not source): dead const floats k=0..40 at fn top / before loop / section 2 / before SetAlphaRef are position-
  independent (period ~34) and never give retail (k=34/35 fixes block 28 but flips the test/or roles of blocks 29..31); the same k=0..36
  grid on top of DESIRED, DESIRED+BOOKMARK, DIRTY, DESIRED+BOOKMARK+DIRTY: no exact point.
- KEY PROBE: 2..16 dead initialised locals INSIDE the SetAlphaRef inline body (before or after its SetRenderState) give RELOCATION_MATCH
  (c2/h/AR02..AR16, c2/g2/h01 `unsigned long a = 1; unsigned long b = 1;`, h03 bools, h06 floats); 1 is not enough; the same records in
  SetAlphaTestFunc (before or after its SetRenderState) or SetSourceAlphaBlendMode act like global outer names. So retail's AlphaRef
  expansion carries >=2 more records than `SetRenderState(ref, DeviceState.AlphaRef)` -- but FableWin's /Od SetAlphaRef is 14i/34b = ours.
  Natural forms tried, no effect: used local copy of ref, const local, CStateInfo& / CStateInfo* local, casts, `this->`, nested block,
  `return;`, `const` param, renamed param; call-site args (unsigned long)1 / true / static_cast / 1L / (unsigned char)1 / 1.0f / (long)1 /
  named block local; plain `inline` SetAlphaRef (VC7.1 then CALLS it: 6341 bytes). Same probe on row 0x00B8B780: no fix (other lever).
- Best next idea: find what natural text puts >=2 records into the AlphaRef expansion only (a different SetAlphaRef signature/body in the
  retail build than FableWin's, e.g. a DWORD-valued helper layer shared with SetStencilRef/Mask -- test that the same records in those
  two setters keep the match), or why plain-inline SetAlphaRef is refused (budget rule) while retail expands it.
- Follow-up probe: the same 2 dead records also in SetStencilRef / SetStencilMask (all raw-DWORD setters, c2/hv_st) still RELOCATION_MATCH,
  so a shared DWORD-setter text with >=2 more records would be consistent for this row (regrade 0x00B87870 ApplyDisplacement with it).
  No natural /Od-neutral text found (FableWin SetAlphaRef / SetStencilRef / SetStencilMask 14i/34b = the one-line form).

## Session 2 of the bwf agent (row 1 only, 2026-11-13) -- still DIFFER(6431v6431), 1 hunk; differ/ unchanged
- Landed / matched SetAlphaRef, SetStencilRef, SetStencilMask users (00bbc460, 00bf01e0, 00b5af80, 00bbf4a0, 00bb8750, 00bc25f0, 00ba6ff0,
  00b87870) all use the plain one-line setters with literal / variable arguments: no special spelling to copy.
- FableWin CRenderStateManager list: every raw-DWORD setter (AlphaRef, StencilRef/Mask/WriteMask, FogColour, TextureFactor, float FogStart/End,
  DepthOffset) is 34 bytes = one SetRenderState call; no intermediate helper (SetRenderStateValue-like) exists in the class.
- Probes, which records inside the AlphaRef expansion fix block 28 (c2/g4, overlay c2/hp PROBE_ARBODY): two chained used copies
  (`a = ref; b = a; SetRenderState(b, ..)`), `a + b - ref`, two dead initialised locals (any type), one used copy + one dead local:
  RELOCATION_MATCH. One local, a `CStateInfo&` / pointer local, a clamp (`v > 255`), `ref & 0xff`, a ternary clamp: no. So retail's
  expansion carries two more value records than ours; FableWin's 34-byte SetAlphaRef rules out a longer body there.
- Call-site forms (c2/a2, c2/w): inline identity / copy / max / float-convert helpers around the argument, `(unsigned long)1.0f`,
  constant float expression, enum constant, named const locals, a forced-inline wrapper with a ref parameter (ProbeAR(r)), a wrapper taking
  CRenderManager&: none moves block 28 (wrappers that also hold AlphaTestFunc shift every later block, like outer names). Plain-inline
  SetAlphaRef with /d2inlT200 expands it and gives the same residue (the inline path is not the cause).
- volatile check (owner: no volatile unless retail provably used it): without `volatile` on the two blur statics (c4/nv.cpp) the row is
  DIFFER(6408v6431), 14 hunks: VC7.1 copies the positive offsets with integer moves (`mov reg,[0139AB54h] ; push reg`), retail copies them
  through the x87 (`sub esp,8 ; fstp [esp+4] ; fld dword ptr [0139AB54h] ; fstp [esp]`, 0x00B8A913..0x00B8A91D, 0x00B8A927..0x00B8A936).
  Non-volatile spellings tried, all 14 hunks: `static float BlurOffset[2]`, `+x`, `x * 1.0f`, `(float)x`, flag /Op. So retail's bytes depend
  on the volatile reads here (ASM_RULES "retail's bytes depend on it" case); needs a `// Retail-volatile:` line if the owner accepts it.
- Out of real ideas for a natural source of the two AlphaRef records. Remaining guess: retail's SetAlphaRef (unlike FableWin's later build)
  copied the value through two locals or a two-parameter helper; a header opt-in with that text for the DWORD setters would need the owner's
  call (one text for SetAlphaRef / SetStencilRef / SetStencilMask; c2/hv_st shows the row still matches; regrade 0x00B87870 with it).
- Sibling check (c5/): giving the raw-DWORD setters (SetAlphaRef / SetStencilRef / SetStencilMask) the two-copy text, or 1..8 extra
  records, makes this row RELOCATION_MATCH (c5/row.cpp) but breaks the matched sibling 0x00B87870 ApplyDisplacement (DIFFER 4944v4944 for
  every variant; its baseline with its own overlay is RELOCATION_MATCH). One header text cannot serve both, so "retail's SetAlphaRef has more
  records" is NOT the source fact: the AR-internal probe only imitates some other per-block difference at block 28.
- Hand-over: differ/00b89c30.cpp unchanged (v19, 1 hunk, `static volatile float` statics -- see the volatile check above). Next level to try:
  what else is per-instance at block 28 (the AlphaRef statement's own scope / position relative to the volatile reads in the loop, the
  section-2 statement grouping into blocks or helper scopes that FableWin does not list), measured with c2/sw.sh against the AR probe.

## Session 3 of the bwf agent (row 1, coordinator resume) -- still DIFFER(6431v6431), 1 hunk
- Scheduler DAG dump (agent2031 sched.sh, c7/base.json v c7/ar02.json = the two-copy probe that matches): every bookmark test region is
  picked in the SAME order in both builds; the S/C load order is already fixed in the IL (the operand order of the inlined
  `state.BookmarkMask & CurrentBookmarkMask`), i.e. a c2 commutative-canonicalisation key over record ids, not a scheduler tie.
- Probes that fix block 28 alone: records inside the SetAlphaRef expansion (two value copies / two dead locals), or a forced-inline wrapper at
  the call that copies the value once more (`Wrap(r) { v = r; SetAlphaRef(v); }`, c8/x3 RELOCATION_MATCH); a wrapper passing only the value or
  the manager pointer fixes 28 but flips 29 (c8/x1, x2). No FableWin / Ego_r function wraps SetAlphaRef (no Alpha* helper outside the
  CRenderStateManager setters), so none of these is a natural source.
- No effect (block 28 stays S): grouping the section-2 setters in braces (8 groupings, c6/), AlphaTestFunc body spellings (table forms,
  named value / state ref, c8/f*), call-site casts / base-qualified calls / `(*p).` (c9/v*), duplicated setter calls (not folded: +110 b),
  extra functions before or after ApplyOutline in the TU (c8/t*), SetupAlphaBlendingStatesFromBlendMode(ALPHA) for the Src/Dest pair
  (14 token diffs).
- Volatile: per owner ruling the `static volatile float` blur offsets stay (retail x87 copies 0x00B8A913 / 0x00B8A927; without volatile 14
  hunks). The differ draft still needs the `// Retail-volatile:` line added at hand-back.

## Session 4 (author agent, 2026-11-13) -- still DIFFER(6431v6431), 1 hunk; differ/ unchanged
- Block 28 is untouchable from outside the AlphaRef statement: 70 combos of byte-neutral spellings (c11: current/direct/temp render-target
  copy, `textures[]`, 0x18 for sizeof, textures[1] for tex, int 0 for 0.0f args, file-scope statics, 0.00390625f) only toggle block 29
  (the direct `OldRenderTarget = GetRenderTarget()` form flips 29 alone); `__assume`/`__noop` near it, comma-joined setter statements
  (c14, c15): no effect. One-layer one-record wrappers (c12 w0..w3: value / manager ptr / manager ref / no param): no effect.
- What fixes it is >=2 records alive in an inline frame AROUND the SetAlphaRef expansion: c13/m4 `PW(ref, src, dst)` (AlphaRef + Src +
  Dest) and c13/m5 `PW(ref, src)` (AlphaRef + Src) grade RELOCATION_MATCH; frames that also hold AlphaTestFunc (m1..m3) shift 28..38.
  No PDB (FableWin / Ego_r / Ego_d) function takes ALPHA_BLEND_MODE / ALPHA_TEST_FUNC besides the setters, no local or anonymous helper
  in the Outline TU, no CRenderManager / CRenderManager2D setter layer -> still no natural source.
- Header, AlphaRef-only (c16): one copy keeps the sibling 0x00B87870 RELOCATION_MATCH but leaves this row DIFFER; 2..8 chained copies
  fix this row and break the sibling (DIFFER 4944v4944) every time. The header route is closed.
- Note: the sibling's overlay CRenderStateManager.h has a FABLE_RETAIL_CRENDERSTATEMANAGER_DISPLACEMENT_INLINE section ours lacks (merge).
- Sibling check of the CALL-SITE frames (c17): x3 `PW(r){ v = r; SetAlphaRef(v); }`, m4 `PW(ref, src, dst)` and m5 `PW(ref, src)` applied
  to 0x00B87870's identical AlphaRef/Src/Dest sequence keep it RELOCATION_MATCH (its own overlay). So a shared inline frame around the
  AlphaRef call (not a longer SetAlphaRef body) is consistent with BOTH rows; header bodies are not. x1/x2 (fix 28, flip 29) plus the
  direct OldRenderTarget copy (c18) give 28+29 wrong: no combination trick there.
- Not landed: every matching form is a made-up force-inline frame with no PDB / FableWin / string evidence (rule: no unexplained
  force-inline, never for a compiler residue). Owner decision needed: accept a descriptive shared helper (best: m4 = AlphaRef + Src + Dest,
  i.e. "set alpha ref and blend factors", or x3) for both screen-effect rows, or park the row at DIFFER(6431v6431) 1 hunk.
- Next idea if pursued: look for a retail-era header (EgoCore / FSE / sibling repos) with an inline that groups AlphaRef + Src + Dest or
  forwards SetAlphaRef through a by-value copy; also check the other matched SetAlphaRef users (00bbc460, 00bf01e0, 00b5af80, ...) with
  the m4 / x3 frame to see whether one shared helper keeps them all.
- Coordinator round (frame end points, c19..c21): a frame `PW(ref, src)` that starts at SetAlphaRef grades RELOCATION_MATCH for EVERY end
  point from Src through Dest, Z, cull, dither, the stencil setters and PrepareForRender(0) (c19/e01..e11); to the end of the function
  with (texture, verts, indices) params the pattern is exact but `&texture` moves a slot (c20/t2 DIFFER 6431v6431); (stencil_mask,
  stencil_ref) frame over the setters: RELOCATION_MATCH (c20/t3). Member frames (overlay c21/hdr adds the probe members): `this` only,
  even with the `tex` local, does nothing (p1, p2); `this` + `unsigned long ref` over the whole tail = RELOCATION_MATCH (c21/p3). So the
  rule is: >= 2 PARAMETER records of a frame entered at SetAlphaRef (locals declared later in the frame do not count).
- Real-function search: no member of CEngineScreenEffectOutlineGlow / Displacement / Glow / RadialBlur / ColourFilter / CEngineComponent
  (FableWin sizes listed in the session) is a composite / draw-to-back-buffer helper; no render-state helper class in struct_layouts_egor
  (only CScopedShadersEnable, CScopedVertexFormatShader, CScopedVertexShaderConstantLayout, CRenderStateScopedCaptureBlock). FableWin's
  ApplyOutline is LARGER than ours at /Od (2312b/619i v 2092b/523i with /EHsc), so the later debug build did not split the tail out either.
- State: differ/ unchanged (1 hunk). Best evidenced-free candidates: c21/p3 (private inline member taking the alpha ref, AR..end) or
  c13/m4. Next: step 4 (recs.py / cands.py on base v c21/p3 to name the two deciding records), or the owner's call on a descriptive helper.

## Session 5 (author agent, 2026-11-13) -- progress note at ~70 probes; differ/ unchanged
- Debug-size gap (Lead 1) explained away: sibling 0x00B87870 /Od+EHsc 220i/819b v FableWin 250i/888b (+30i/+69b); this row +96i/+220b.
  VC7.1 /Od pushes float literal args as `push imm` (seen in c22/_fw/base.dis); VC8 (FableWin is VC8: _Debug_pointer/_Is_checked
  symbols) pushes them as `push ecx ; fldz|fld [c] ; fstp [esp]` (+2i, +4/+5b each). This row has 38 literal float args to
  SetUserVertexShaderConstant (34 zero, 4 non-zero) -> +76i/+156b; the sibling 4 -> +8i/+16b. Residues 20i/64b v 22i/53b: the same common
  VC8 overhead. So the gap is NOT text specific to this row.
- No effect (all exactly base, 28:Smb): empty inline calls with 0..4 params (with or without (void) uses) between AlphaTestFunc and AlphaRef
  (c22/d*, u*); 12 argument spellings of SetAlphaRef(1) (c22/g3); the argument produced by an inline function with 1..3 params, incl.
  min/max/clamp templates (c22/g4); `if (true)` / `if (OutlineTexture)` / `do{}while(0)` / `{}` around 9 tail ranges (c22/g5, 36 probes).
- Frames ENDING before AlphaRef (Lead 2, c22/f*): 0/1-param frames over [TestFunc] = base; any frame over [TestEnable..] / [BlendEnable..] /
  [FilterMag(0)..] with a param flips 24..38; 2-3 param frames over [TestFunc] flip 28..38. None fixes 28.
- c2 records (agent3005 cands.py, c22/h): the AlphaRef block's temp is candidate 340 (id 4054 base, 4056 in the matching c8/x3) in the
  36-id-per-block series 2850..4455 (one temp per SetRenderState block); x3's frame records are freed after the frame, later blocks keep
  their ids. Partner record not identified (a mod-256 brute force points at section-1 setter locals 726/727, unproven).
- More no-effect families (all exactly base 28:Smb): plain `inline` instead of FABLE_FORCEINLINE (AddStateToUpdateList: no change; SetAlphaRef
  or SetRenderState: not inlined, thousands of bytes off); header SetRenderState operand / increment spellings (mask & state swapped,
  `!(..)`, `a = a | b`, `StateStack[StateStackSize++].Value`, `StateUpdateList[StateUpdateListSize++]`, postfix ++: c22/ov/b*, h*);
  the `tex` local opened at 8 points of the tail, in its own block or at function scope (c22/g6). A live `CRenderManager*` /
  `CRenderManager&` / `CRenderStateManager*&` local for the AlphaRef statement (c22/g7) changes code (6429..6253 bytes), never fixes 28.
- c2 IL at the block pass (agent3005 dumpblk.py, depth 0; c22/h/base_blk.json, x3_blk.json): the two loads of block 28 are already in
  final order there (base: `[edx+8]` node tagged 0x3d5 then `[eax+0x2814]` 0x3d9; x3: `[eax+0x2814]` 0x3dd then `[edx+8]` 0x3d9), so the
  operand order is decided before the block pass and is not the creation order of those memory nodes.
- Per-block state-load temps (cands 334/340/346/352/358 = blocks 27..31): base 4018/4054/4092/4128/4164; x3 4018/4056/4092/4128/4164
  (fixed); x1 4056/4090 (29 flips); k2 dead locals 4054/4090/4126/4162 (28..31 wrong); k34 4088/4124/4160/4196 (28 ok, 29..31 wrong).
  Blocks 29..31 sit exactly on their edge (2 lower flips them); block 28 is 2 below its edge. Only records inside / around the
  SetAlphaRef expansion move T28 alone.
- Overlay merged: $W/hdr/engine/CRenderStateManager.h is now the sibling's overlay copy (adds FABLE_RETAIL_CRENDERSTATEMANAGER_DISPLACEMENT_INLINE;
  drops this row's unused LOCAL_MASK probe opt-in, backup c22/CRenderStateManager.h.bak) + CEngineScreenEffectDisplacementRenderer.h copied.
  With it: this row DIFFER(6431v6431) 1 hunk (unchanged), sibling 0x00B87870 RELOCATION_MATCH (q.py, flags none).
- State at end of session 5: no evidenced source matches. Every matching text is an invented inline frame (c8/x3, c13/m4, c21/p3) or
  dead records in the shared SetAlphaRef body (breaks the sibling). Owner decision still needed. Best next idea if pursued: decode the
  c2 commutative key (which record pair at block 28; dumpblk tuple word 0x28 nodes / sib.py-style hook earlier than the block pass) to
  predict which REAL construct could move T28 by +2..+33 alone; or check the other SetAlphaRef users (00bbc460 / 00bf01e0 / 009a1720 /
  00ab83a0 / 00ba6240) for a context pattern shared with this row.
