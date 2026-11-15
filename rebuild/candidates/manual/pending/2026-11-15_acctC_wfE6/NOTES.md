# wfE6 NOTES (acctC lane, render rows; agent wfE6, 2026-11-14)
SUMMARY (wfE6-3, latest): 2 matched (0x00B73760 ctor; 0x00B5CA70 CaptureBackBuffer, RELOCATION_MATCH + test PASS + addmatch + vland clean); 0x00B783F0 down to 1 real hunk (register pick) with rulers; 0x00B6EDF0, 0x00B70A90, 0x00B4D370 parked. Earlier summary:
headers (0x00B6EDF0 9 real lines after the loop-head fix, 0x00B70A90 2 real blocks, 0x00B5CA70 38 hunks, 0x00B4D370 frame/x87, 0x00B783F0
quartet/pair). Overlay additions listed in hdr/README.md (wfE6 section). Tools: tools/ce.sh (compile errors), tools/sw.sh (variant sweep).

Global names (two sources each: anniversary PDB `NEngineInternal::GP*` globals list in tmp/gp_globals.txt + rebuild/oracles/reloc_bindings.tsv
bound spellings / landed rows): GPResourceManager 0x01436E98 (reloc_bindings + landed 0x00BB5040), GPRenderManager 0x01436E18 (CRenderManager*),
GPShaderRenderManager 0x01436E14, GPCamera 0x01436EA0, GPDisplayManager 0x01436E1C, GPEngineWorldMap 0x01436E8C (anniv PDB only +
type CEngineWorldMap* from the landed `g_pEngineWorldMap` spelling; reloc_bindings has only descriptive names there -> NAME_FIXES).

## 0x00B6EDF0 CEngineWaterRenderer::BuildConstantBuffers -- DIFFER(3144v3144), parked (differ/00b6edf0.cpp)
- Name: Ego_r + FableWin ?BuildConstantBuffers@CEngineWaterRenderer@@AAEXXZ; only caller the ctor 0x00B73760. Extent 3144 confirmed (ret at 0x00B6FA37).
- Ported chunk-723 draft onto today's headers: new overlay opt-in FABLE_RETAIL_C3DTRIANGLE2_SETVERTEXINDEX (FableWin ?SetVertexIndex@C3DTriangle2@@QAEXKG@Z,
  fwsize SAME 11i), the chunk-723 FABLE_RETAIL_CTRIANGLESTRIPIFIER2 block re-applied to a copy of today's header (default text identical),
  the shared CTVertexColTex1.h (FABLE_RETAIL_CTVERTEXCOLTEX1_SETTERS, already in the overlay; fwsize SAME for all three setters) instead of the
  draft's local CGenericVertex model, GPResourceManager respelled.
- Residue (13 fg hunks, all scheduling, equal length): (1) loop head: retail stores x=0 before rowBase=y*17 AND loads x before rowBase for
  `x + rowBase`; every form tried (rowBase named before/after x, comma init, hoisted y*17, x + 17*y, index two-step, declarations split)
  gives one of the two: the store order follows first IL appearance, the commutative load order follows the same symbol order.
  (2) vertex block: v1 Y stored before X; v6 / v12 -0.1f (ebp) stored late; v13 Z/V stored before `mov ecx,esi` (Unlock's this).
- LOOP HEAD FIXED (grade ~20): `long rowBase = 0;` declared before the y loop, then `long x = 0; rowBase = y * 17; for (; x < 16; x += 2)`
  gives both retail orders (store x=0 first, load x first): the dead zero gives rowBase the lower candidate id while x = 0 stays first in
  the block (playbook: commutative operands follow candidate ids, higher first). FITTED. Without the zero (h2) or with the x-loop init (h3): no.
  Now 12 fg hunks, 9 real (vertex block only).
- Grades: ~25 (this run) + ~100 (chunk 723).

## 0x00B70A90 CEngineWaterRenderer::RenderEnvironmentMap -- DIFFER(3567v3567), parked (differ/00b70a90.cpp)
- Name: Ego_r + FableWin ?RenderEnvironmentMap@CEngineWaterRenderer@@AAEXXZ (private). Extent 3567 (plain ret).
- Re-ported the chunk-723 draft onto SHARED headers (its local CResource / CGraphicFrame / CFrameData / GetFrame models removed): the 00BCC800 opt-in
  set for CGraphicInfoFrame::GetFrame (STATICBLOCKS_TEXTURE_HELPERS, CGRAPHICINFOFRAME_GETFRAME, ...), in-row FABLE_FORCEINLINE PeekGraphic /
  PeekGraphicInfoFrame (00BCC800 text), CEngineCamera POSITION branch + NEW overlay opt-in FABLE_RETAIL_CENGINECAMERA_LOOKFOV (PeekLookVector /
  PeekHFOV / PeekVFOV; PeekVFOV text `Use2DFOV ? VerticalFOV : HorizontalFOV * (1.0f / AspectRatio)` gives retail's `fld 1 ; fdiv ; fmul`, the
  `1/a*h` form folds to h/a and is 6 bytes short), CENGINEWORLDMAP_GAMEWORLD (PeekGameWorld), CENGINEWATERSETTINGS_COLOUR, C2DVECTOR_ROTATE +
  _NORMALISE (AccurateNormalise sits inside the NORMALISE block), CDISPLAYMANAGER_BACKSCREEN, enum names TEXTURE_UV_* / ALPHA_BLEND_*.
- Residue: bookmark test/or register pick only. Port = 9 hunks; SetRenderState DESIRED+BOOKMARK+DIRTY = 6; + 1 unused named local = 4 (2 real):
  0x00B716B7 (UVModeU(0,CLAMP), stage-0 state +0x2EE4) and 0x00B71724 (UVModeV(0,CLAMP), +0x2F84): flag/mask registers swapped.
  Tried, no change on those two: k = 1..8 unused locals at the top, 1..4 before the second pass / before the UV setters, 1..8 dead asserts top /
  middle (8 breaks five more blocks), the four other SetRenderState opt-in combinations with 1-2 locals. EARLY_RETURN opt-in does not compile alone (cl hang).
- fwsize: row 399i v FableWin 461i (+62i: ~one assert, or the later compiler's float-argument pushes); helpers SAME except the FableWin-later ones.
- Further (all no change on the two blocks): unused-local counts k = 1..32, 40, 48, 64 at the top (k = 22..31 break earlier blocks, the last two
  never move), PAREN-tuple float groupings in the camera / fov / width-height arguments (p1..p6), back-buffer pass spellings (pointer local,
  named float conversions, functional casts; a named C2DExtentsI changes length). The last setter run seems to draw its ids from a free list
  that symbol counts do not reach: next level = the c2 comparator trace (wfB4 k/cmp.py) or the post-RA flag analysis of agentC13 (row 6).
- Grades: ~110.

## 0x00B73760 CEngineWaterRenderer::CEngineWaterRenderer(const CEngineInit&) -- RELOCATION_MATCH, test PASS, vland clean, addmatch done
- Name: Ego_r + FableWin ??0CEngineWaterRenderer@@QAE@ABVCEngineInit@@@Z; extent 6799 confirmed (`ret 4` at 0x00B751EC end).
- Hand-authored in one pass from the acctB OUTLINE (statement list + bank numbers verified against the call sequence), in the idiom of the
  landed 0x00BC14E0 (Member = GPResourceManager->GetVShaderResource((EShaderBankNo)n)->GetVertexShader("..."), GetPShaderResource()->GetPixelShader).
  Matched on the FIRST grade: no flags, no fitted spelling, member construction implicit from the shared retail class.
- Headers (overlay, all additive opt-ins, hdr/README.md): FABLE_RETAIL_CVERTEXSHADER_COPYCTOR (FableWin ??0CVertexShader@@QAE@ABV0@@Z),
  FABLE_RETAIL_CLANDSCAPETESSELATORBASE_INLINE_CTOR, FABLE_RETAIL_CWATERPATCHTESSELATORBASE_INLINE_CTOR (FableWin ??0?$CWaterPatchTesselatorBase@..),
  FABLE_RETAIL_CENGINEWATERRENDERER_CTOR (InitialiseDynamicTextures decl); existing: CARRAY_PUSHBACK, CENGINEWATERSETTINGS_CTOR,
  CDISPLAYMANAGER_PIXELSHADERVERSION, CCHARSTRING, CARRAY, CENGINERESOURCEMANAGER.
- NAME_FIX: 0x00B6D510 is ?InitialiseDynamicTextures@CEngineWaterRenderer@@AAEXXZ (landed leaf says CWaterDynamicTextures).

## 0x00B5CA70 CEngineScreenEffectRadialBlurRenderer::CaptureBackBuffer -- DIFFER(5724v5748), parked (differ/00b5ca70.cpp)
- acctB best draft re-ported onto today's headers: SETPSCONSTANT_SUBSCRIPT -> the repo's identical CSHADERRENDERMANAGER_PSCONST_INDEX,
  CGPUSCRATCHBUFFERTEXTUREMANAGER_TEXTUREUSE -> the repo's CGPUSCRATCHBUFFERTEXTUREMANAGER_TEXTURES (same StartTextureUse / EndTextureUse decls),
  new overlay opt-in FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_CAPTUREBACKBUFFER (the acctB declaration). Same grade as acctB.
- Residue (38 fg hunks): first-pass setter bookmark test/or operand orders (0x00B5CD17.. six blocks), pass B tap-4 fsin position, the pow ->
  SetPixelShaderConstant temp-store scheduling in passes B/C/D (retail's `mov/or/mov` UpdateFlags form = the 24 missing bytes). The five
  SetRenderState fitted-opt-in combinations leave the hunk count at 38. acctB's 150 grades: c2 81-tuple scheduling regions (needs ~+18 IL
  tuples between angle2 and tap 4). Structure fact: passes B/C/D are the same code with step 1 / 4 / 16 (angle = twist * i*step, length = 1 - Length*i*step/256,
  offset = (centre - 0.5) * Length * (i*step/256.0) as a double, intensity = pow(IntensityR, i*step)); no PDB method takes a pass or tap index
  (Ego_r / FableWin method list checked) and FableWin's /Od body is 1514i (written out, no loop), so the repetition is textual -- most likely a
  MACRO with parenthesised parameters, whose grouping parens are exactly the no-code PAREN tuples the acctB probe needed (+18 before tap 4).
  Next step: write passes B/C/D through one plausible macro text (parameters wrapped in parens, e.g. `(1.0f - Length * ((I) * (STEP) / 256.0f))`)
  and grade, instead of scanning parens per line.
- Grades: 6.

## 0x00B4D370 CEngineShadowRenderer::UpdateShadowSpace -- DIFFER(7160v7176), parked (differ/00b4d370.cpp)
- acctB best draft (`// Flags: /d2inlT314`) re-ported: nine stale opt-ins re-applied to copies of today's headers (reapply.py + by-hand
  #elif / #else fixes, hdr/README.md); C2DVECTOR_ACCURATENORMALISE replaced by the existing ROTATE+NORMALISE pair, C3DVECTOR_SCALE_RESULT_LOCAL
  by the existing identical SCALE_NAMED_RESULT; GPLightingManager respelled. Grade = acctB's (frame 0x3B4 v 0x3D0; residues listed in
  ref/00b4d370/notes/NOTES.md: frame slots, x87 operand order `fld E ; fmul st(i)`, corner-pass CSE, SetInverse det order, chaotic frustum block).
- Grades: 1.

## 0x00B783F0 CEngineWaterRenderer::Render -- DIFFER(9343v9343), parked (differ/00b783f0.cpp)
- agentC8 / c19a open source regraded with this overlay: same residue (quartet 0x00B7945D/0x00B794C1/0x00B79525/0x00B79589, pair
  0x00B7A5D0/0x00B7A635, 0x00B79938; the other fg hunks are identical-text display hunks). sky renderer global respelled
  NEngineInternal::GPSkyRenderer (still DIFFER 9343v9343 same hunks). agentC13: decided in the post-RA scheduling pass (node flag 4).
- fwsize: in-row SetCommonRenderStates 75i v FableWin 106i: +24i is the later compiler's 3-instruction float-constant pushes (12 zero
  arguments), ~+7i unexplained (one more setter call in the later build?).
- Grades: 2.

### 0x00B5CA70 further (this run): a macro text for the three passes' tap set-up with parenthesised constant parameters is byte-identical
(constant parens fold, no PAREN tuple); grouping parens on the length lines `(1.0f - L * c)` change the schedule but worse (41..59 hunks,
any pass subset); on the offset lines no change. Row stays at 38.
### 0x00B6EDF0 further: named const corner vectors, unused locals (1..8 at top / before the vertex block) are byte-identical in the vertex
block; v1 setter permutations move v1's hunk but never give retail's Y-before-X. The vertex-store order is not the setters' IL order.

## Successor (wfE6-2, 2026-11-15) measurements
- 0x00B6EDF0 scheduler DAG dump (k/dag.sh = agentC6 t13 hook on regions > 60 nodes; k/pk.py prints IL order + picks): the vertex block
  is split across c2 81-tuple regions counted from the block start (region 2 = Initialise/Lock + first 69 vertex tuples, the rest + Unlock
  in the next region). Memory edges: every store has a latency-0 edge to every later store of ANOTHER vertex (each setter's `this` =
  &v[i] is its own base: no cross-vertex disambiguation); stores of the SAME vertex are independent, and float stores do not depend on
  the DWORD colour store of their own vertex (type split). So all freedom is inside one vertex; v1's X/Y/Z have equal prio (83) and go
  by IL idx (X first). Retail's Y-first needs either a different IL order or a different prio (outdeg/height) for v1.Y: not a count
  or spelling question. Uniform setter orders (6 permutations, tools/ve.sh): PUC natural best (15 vertex tokens off), fitted v5 = 13.
  Pointer walk `++v` keeps a pointer register (3021 bytes); `(v + i)->` byte-identical.
- 0x00B70A90 comparator trace (k/run.sh = wfB4 cmp.py; k/a0.ids): 9 bookmark blocks, first-pass k ids 1319..1336 / 1634..1660, m ids
  1957..1965 then 2274/2275/2292/2293 (m wrapped past 2048 from block 6 on, = the S load-order run). The load order (S/C) pattern equals
  retail (CCCCCSSSS); the residue is the OR destination register: retail MMMMMMMTM v ours MMMMMMMMT (T = `or` into the state-mask
  register) -- blocks 8 and 9 have the same first-pass orientation in ours yet differ, so it is a register-allocation pick, not the
  first-pass key (tools/bk.py prints both patterns).
- 0x00B5CA70 measured lead (k/reg.sh = region-ops dump, k/r0.json): the 24 missing bytes are exactly three UNFUSED `UpdateFlags |= 2`
  of SetPixelShaderConstant(7, ...) in passes B / C / D (retail mov ecx,[eax+3D8h] / or ecx,edi / mov [eax+3D8h],ecx at 0x00B5D496 and
  0x00B5D9A0 (+ pass D), 14 bytes v our fused `or [eax+3D8h],edi`, 6 bytes; pass A's const 7 is unfused in both builds). Retail's
  scheduler then puts const 8's temp read `mov edx,[esp+14h]` between the load and the or, and stores intensity3's C4DVector temp
  (fst x3 / fstp, shared slot esp+14h..20h) inside const 7. Fusion is chosen BEFORE scheduling, so the IL around const 7 / const 8
  differs: look for the source fact that keeps const 7's RMW split (pass A has it) -- pass B's const 7..8 fall in c2 region 66 of 81
  tuples (regions 64/65/66 = pass B, 88/89/90 = C, 112/113/114 = D; const 8's `or` falls into the NEXT region in C and D but not B).
- 0x00B783F0: regraded unchanged (DIFFER 9343v9343). tools/bk.py: 77 bookmark blocks; load-order (S/C) pattern differs only at the
  quartet (blocks 32..35: retail CCCC, ours SSSS) and the pair (72/73: retail SS, ours CC) -- the second-pass id wrap falls 4 / 2 blocks
  later in ours; agentC13's only closer (3 dead SetRenderState expansions before exp2) is a dead guard, not acceptable.
- Grades this successor run: 00B6EDF0 10 + 2 DAG traces, 00B70A90 1 + 1 comparator trace, 00B5CA70 1 + 1 region trace, 00B783F0 1.
- 0x00B5CA70 fusion probes (same row source otherwise): (c1) const 8's vector as a named C4DVector before const 5: 5793 bytes, 1 unfused;
  (c2, probe/00b5ca70_pow3_after_const7_PROBE.cpp, NOT retail structure: retail calls pow(IntensityR, 0/1/2/3) all four before const 5,
  exponents read at 0x0122ED70/0x01237F00/0x01244480/0x012368F0) moving `intensity3 = pow(...)` after const 7: 5760 bytes and const 7's
  RMW becomes UNFUSED in passes B/C/D exactly like retail (sites 0x00B5D481/0x00B5D9DA/0x00B5DF3A v retail 0x00B5D4A2/0x00B5D9AC/
  0x00B5DF04). So the fusion follows what is live / pending across const 7 (intensity3 in ST0 in both builds): the source fact sits in
  the const 7 -> const 8 IL. (c3) intensity3 as a double converted at const 8: byte-identical to the best; (c4) all four as doubles: 5922.
- Successor grades: 00B5CA70 +4 (total 11 this run).

## Third author (wfE6-3, 2026-11-15)
- 0x00B5CA70 NEW LEVEL (helper visibility): the TU has a static helper `GetRotationVector` (undecorated S_LPROC32 name in Ego_r.pdb
  0x00A0DD00 and FableWin.pdb 0x02D20240, both between OnReleaseDefaultPoolResources and FinishWithDynamicTextures; retail copy at
  0x00B5BD10, never called (rel32 scan of .text): `fld [esp+4]; fcos; fstp [eax]; fld [esp+4]; fsin; fstp [ecx]; ret 4` = cos -> *pX,
  sin -> *pY). Written as `static void GetRotationVector(float angle, float* pX, float* pY) { *pX = (float)cos(angle); *pY = (float)sin(angle); }`
  and called per tap (`GetRotationVector(angleN, &x, &y); Set...(n, x * lengthN, y * lengthN, ...)`): DIFFER(5748v5748) -- equal length,
  the six first-pass bookmark hunks (0x00B5CD17..0x00B5CEC0) are gone. Now in differ/00b5ca70.cpp. Remaining (44 fg hunks: 4 identical-text
  hunks at 0x00B5CC57..83, 1 bookmark register pick at 0x00B5D0F3 (SetAlphaBlendEnable before pass B, mask in edi v edx), and the c2
  81-tuple region phase in passes B/C/D: tap-4 fsin position + const 7 RMW split + const 8 temp stores).
  - region dump (k/reg.sh on g1 -> k/rg1.json): 0x269 IS the PAREN tuple; taps 1..3 carry 3 PARENs after each fsin/fcos (math.h
    cos(float)->cosf chain), tap 0 one. Ours: region 65 ends after tap 4's `fsin 0x269 0x269 0x269`; region 66 ends right after const 7's
    `or` (+2 fst), so const 8's first read cannot interleave with const 7's RMW as in retail.
  - tried: body spellings (float cos overload = same; `(float)cos((double)a)` 5724; cosf 5740; named c/s = same; C2DVector inside 5868);
    per-tap locals / `*=` / decl position / `length * x` = identical; C2DVector rot (address taken) 5918; pow(float,float) (FableWin
    ?pow@@YAMMM@Z + @powf@8 COMDATs follow CaptureBackBuffer in FableWin) on top: 5732 (pass B const fusions change) -- keep double pow;
    diagnostic nested parens k=1..30 on length1 (shifts all cuts): best B10 at k=12 but the frame grows; no single shift closes pass B.
- 0x00B5CA70 MATCHED (RELOCATION_MATCH, test PASS, addmatch, read-only vland clean; src/00b5ca70.cpp, test/00b5ca70.cpp). Steps, each measured:
  (1) GetRotationVector (above) -> equal length. (2) pow(float, float) (EVIDENCED: FableWin ?pow@@YAMMM@Z / @powf@8 follow this function)
  makes pass B's const 7 RMW split as retail (its 2 PAREN tuples per pow move cut 3). (3) region diagnostic: +4 tuples after the angle
  block and before tap 4 closed pass B; the natural source of exactly +4 is tap 0 written like taps 1..3: `float length0 = 1.0f;` and
  `x * length0, y * length0` (2 PARENs more per sin/cos) -> DIFFER 5748v5748 with 3 real hunks (FITTED). (4) the three repo FITTED
  SetRenderState opt-ins DESIRED + BOOKMARK + ADDSTATE_DIRTY fixed the pass-D bookmark (0x00B5DB81); (5) the pass-B bookmark
  (0x00B5D0F3, SetAlphaBlendEnable after `twist`) needs 8..12 dead release asserts (FABLE_RETAIL_ASSERT_DEAD_ARM, overlay fable_assert.h);
  placed naturally (2 textures free before StartTextureUse, 2 set after, 1 per render block before AttachTarget) -> RELOCATION_MATCH.
  0..7 asserts or other opt-in combos do not close it; unused locals do nothing. FableWin's body is 291 instructions longer than our /Od
  (asserts). fg.py shows 4 false hunks at 0x00B5CC57..83 (`mov [esp+x], 0`: its norm() turns ours' literal 0 into REL): ignore them.
  vland WARNs (not errors): StartTextureUse 0x00F36EE0 labelled StartSurfaceUse in the DB (both names are in Ego_r/FableWin; ours is the
  texture form the code needs), D3DTextureUVModeTable labelled g_textureUVModeToD3D, __CIpow import alias.
  Tools: tools/swp.sh (per-pass hunk counts; ~1 s a grade).
- 0x00B783F0 (wfE6-3): sweeps with tools/swg.sh (~3 s a grade): opt-in combos x dead asserts 0..24 (only none / BOOKMARK keep length;
  DESIRED / DIRTY change code), then 2-D dead asserts k x unused named locals u at the top: k = 3..5 alone fixes pair site 0x00B7A635;
  k = 6..11 with u = 6 closes the quartet (0x00B7945D/C1/525/589) AND both pair sites -> 1 real hunk left, 0x00B79938 (mask/state register
  pick; agentC13: post-RA decision), which no k (4..14) x u (4..10) x BOOKMARK variant moves. Best (rulers, last-resort devices c + d):
  differ/00b783f0.cpp (k6 u6); the earlier best in differ/00b783f0_prev.cpp. 263 sweep grades this run.
- 0x00B70A90 (wfE6-3): opt-in combos x dead asserts 0..16 x with/without the fitted unused0 (272 grades): best stays the known 4 fg hunks
  (2 real, 0x00B716B7 / 0x00B71724); the last setter run's blocks never move (as the earlier agents found). Parked.
