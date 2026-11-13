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
