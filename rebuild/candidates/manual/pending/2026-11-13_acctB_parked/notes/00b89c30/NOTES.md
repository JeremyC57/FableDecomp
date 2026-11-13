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
