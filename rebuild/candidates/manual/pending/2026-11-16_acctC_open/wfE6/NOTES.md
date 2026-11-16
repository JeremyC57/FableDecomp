# wfE6 NOTES (acctC lane, render rows; agent wfE6, 2026-11-14)
SUMMARY (wfE6 successor-8, latest): 5 of 6 rows RELOCATION_MATCH (unchanged; no hdr/ change, no regrade needed). 0x00B4D370 parked again (~130 grades + 5 lk): /d2inlT314 justified by the same-TU sibling 0x00B4FC40 (PostMultiply inline budget); lk shows the c2 id pool is chunked and non-monotone under deletion; S3-gauge sweeps (natural spellings, combos, deletions, inline thresholds, device d with inline-call / temporary conditions) never put SetInverse right; best differ/00b4d370.cpp unchanged. See succ-8 block.
SUMMARY (wfE6 successor-7): 5 of 6 rows RELOCATION_MATCH (unchanged; no hdr/ change, no regrade needed). 0x00B4D370 parked again (~75 grades): visible same-TU callee CalcLocalWorldToBaseShadowSpace byte-identical; device c flat; dead DrawFrustrumDebugScreen-style call moves only the frame (overshoots); truncation bisect: deleting the whole perspective branch makes SetInverse + frustum nearly exact (population), no single block carries it; scopes do not consume ids. See succ-7 section at the end.
SUMMARY (wfE6 successor-5, latest): 5 of 6 rows RELOCATION_MATCH (unchanged; no hdr/ change). 0x00B4D370 parked again (~40 grades): fwsize-evidence level (EMatrix34Init ZERO/IDENTITY switch ctor from the 360 PDB enum, SetInverse via GetDeterminant) gives candidate differ/00b4d370_e1.cpp nf5 316 v 330 at equal length/frame; Initialise-form operators and dead asserts move nothing usable; see succ-5 section at the end.
SUMMARY (wfE6 successor-4, latest): 5 of 6 rows RELOCATION_MATCH (unchanged; no hdr/ change). 0x00B4D370 parked again (~170 grades + 1 lk): differ/00b4d370.cpp unchanged best; new EVIDENCED SetInverse text fact (cofactor A `m.E22*m.E33 - m.E32*m.E23`, differ/00b4d370_SetInverse_S3.h: exact under the ruler diagnostic) and the residue reduced to one question, the c2 id population after the named pass (see succ-4 section at the end).
SUMMARY (wfE6 successor-3, latest): 5 of 6 rows RELOCATION_MATCH (unchanged, no header change this run). 0x00B4D370 parked again: new best differ/00b4d370.cpp DIFFER(7192v7176) nf2 1589/1760, 114 hunks (grid weights exact); lk-measured key model + ruler windows + frame facts in its succ-3 section at the end.
SUMMARY (wfE6 successor-2, latest): 5 of 6 rows RELOCATION_MATCH + test PASS + addmatch; 0x00B783F0 re-verified (q.py RELOCATION_MATCH, vt PASS, read-only vland: all 5 RELOCATION_MATCH PASS, 0 RELOC_IDENTITY). 0x00B4D370 now EQUAL LENGTH DIFFER(7176v7176), nf2 1580/1760 (from 1497), best differ/00b4d370.cpp; new opt-in FABLE_RETAIL_CMATRIX3X4_SETINVERSE_TRANSLATION_LOCAL (all 5 matched rows regraded: unchanged).
SUMMARY (wfE6-8, latest): 5 of 6 rows RELOCATION_MATCH + test PASS + addmatch + strict vland clean (0x00B783F0 closed this run); 0x00B4D370 parked (DIFFER 7160v7176). Older summary (wfE6-3): 2 matched (0x00B73760 ctor; 0x00B5CA70 CaptureBackBuffer, RELOCATION_MATCH + test PASS + addmatch + vland clean); 0x00B783F0 down to 1 real hunk (register pick) with rulers; 0x00B6EDF0, 0x00B70A90, 0x00B4D370 parked. Earlier summary:
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
  FIX-UP (wfE6, shared-header rule): the local `class CRenderTarget : public CBaseClass {...}` (comment wrongly said no shared header) is replaced by `#include "engine/CRenderTarget.h"` (same layout +0x04/+0x14, 0x24 bytes, inline default ctor, AttachTarget 0x00A10E60; needs FABLE_RETAIL_CSURFACE + _COPYFROMMEMORY, already defined). No new opt-in, no overlay change. Still RELOCATION_MATCH, vt.py PASS, addmatch redone, strict-header vland (LANDVERIFY_STRICT_HEADERS=1) RELOCATION_MATCH PASS with no HEADER_POLICY / SHARED_HEADER_AVAILABLE / RELOC_IDENTITY (only the 3 known WARNs). No other local class / struct in the source. Backup: tmp/00b5ca70.before_fixup.cpp.
- 0x00B783F0 (wfE6-3): sweeps with tools/swg.sh (~3 s a grade): opt-in combos x dead asserts 0..24 (only none / BOOKMARK keep length;
  DESIRED / DIRTY change code), then 2-D dead asserts k x unused named locals u at the top: k = 3..5 alone fixes pair site 0x00B7A635;
  k = 6..11 with u = 6 closes the quartet (0x00B7945D/C1/525/589) AND both pair sites -> 1 real hunk left, 0x00B79938 (mask/state register
  pick; agentC13: post-RA decision), which no k (4..14) x u (4..10) x BOOKMARK variant moves. Best (rulers, last-resort devices c + d):
  differ/00b783f0.cpp (k6 u6); the earlier best in differ/00b783f0_prev.cpp. 263 sweep grades this run.
- 0x00B70A90 (wfE6-3): opt-in combos x dead asserts 0..16 x with/without the fitted unused0 (272 grades): best stays the known 4 fg hunks
  (2 real, 0x00B716B7 / 0x00B71724); the last setter run's blocks never move (as the earlier agents found). Parked.

## Fourth author (wfE6-4, 2026-11-15) -- bit-B measurements (0x00B783F0, 0x00B70A90)
- Both rows' residue is bit B of the SetRenderState bookmark blocks (tools/ab.py: A = mask/flag load order, B = test / `or` operand
  order; the "register pick" is NOT a colouring choice: tools/c2prio_f.py shows the `or` destination candidate is always the 5-ref
  one -> edx, so the swap is which VALUE is the destination = bit B). Exact rule (wfB3 tools/lkb2.py on the comparator log, copied as
  tools/lk + tools/lka.sh, 7.7 min for 0x00B783F0, 2.5 min for 0x00B70A90): B = [2X mod 2048 >= 2Y mod 2048], X = first-pass CSE
  temp id, Y = late temp id (made after all inlining). tools/fsolve.py <lk.json> <first> <last> <flips> solves for uniform shifts
  (F = temps added before every site, T = temps added after the last site).
- MEASURED: unused named locals (1..84, initialised or not), dead FABLE_RETAIL_ASSERT(this) (any count, any position), renaming locals,
  live-initialised rulers at any statement, SCRS defined after the row: NONE moves X or Y (k5 lk run: identical ids; tmp/lk_b0 v
  tmp/lk_k5). Locals/asserts move only bit A (first-pass tree hash). So devices c/d cannot close a bit-B site here.
- 0x00B783F0: the bad site = comparator entry 76 (X 5984, Y 9059, d = -6), the 2nd SetTextureBumpEnvMatrix of the ELSE arm (zeros) of the
  2nd SetCommonRenderStates expansion (line 122). fsolve: the ONLY uniform fixes are T = -3..-7 (three to seven FEWER temps created after
  the last site, i.e. in the tail: Select*Buffer(0,0), FinishUsingDynamicTextures, the three destructors) with F = 0..6; nothing that only
  adds temps works (T would have to be ~+1018). Tried for the tail (probe only, reverted -- it would wrap an existing opt-in body): a nested opt-in SCOPED_EMPTY
  (PDB-shaped empty scoped class, dtor = one RestoreCaptureBlock): byte-identical, ids unchanged. Diagnostic tail deletions (probe/tl,
  NOT candidates): removing a Select call changes the code (no reading).
- 0x00B70A90: sites 11/12 of lk (UVModeU/V(0,CLAMP)) have d = -4 / +32; retail needs B 1/0, i.e. site 12's kX must WRAP past 2048 while
  kY does not: fsolve gives F = 43..78 temps added BEFORE the first bookmark site (T = -32..+67 tolerated). Unused locals 0..84 never move
  B (probe/a9 sweep, tmp/a9.txt) -- they are not temps. So retail has ~43..78 more inline-time temps before the first setter than ours
  (e.g. more inline helper layers / parameters in the sky / camera / colour preamble), or ours has extra temps after; not a ruler question.

## Fifth author (wfE6-5, 2026-11-15) -- 0x00B70A90 record-id measurements
- fsolve.py's printed T ranges hide gaps (prints first..last per F). Exact solution set for sites 4..12 (tmp/a90 analysis): with a uniform
  shift F of every X and Y and a Y-only shift T: F == 43..78 (mod 1024) AND (T <= -2 or T in 17..~110). F alone or T alone never works.
- Dead FABLE_RETAIL_ASSERT arms DO consume c2 record ids when they sit in an inlined helper or call inline functions: x1 (2 row asserts
  with inline calls in the preamble) shifts every site X and Y by +64; 3 simple asserts per SetRenderState expansion (probe overlay
  tmp/a90/hdrp, hooks RS_A*/RS_D*) shift sites 4..12 by +352 (= 11 expansions x 32) with per-site spacing unchanged => asserts give F,
  never T. FableWin /Od: SetRenderState +164i (2 asserts), AddStateToUpdateList +83i (1 assert) over our texts: retail's header
  inlines carried dead asserts (candidate real source of the FITTED DESIRED/BOOKMARK/DIRTY named-local opt-ins).
- FableWin COMDAT order after the row: PeekWindow, PeekHFOV, PeekVFOV (PeekWindow first used in this row in FableWin; an unused
  `const C2DBoxF& window = GPCamera->PeekWindow();` at 6 positions is byte-identical, hunks unchanged).

## Sixth author (wfE6-6, 2026-11-15) -- 0x00B783F0 / 0x00B4D370 (both parked again)
- 0x00B783F0 (differ/00b783f0.cpp unchanged, still 1 real hunk 0x00B79938 = bit B of lk entry 76). Exact insertion model re-solved on
  tmp/lk_b0.json (probe/r7): the ONLY fixes are a net change of -3..-7 (or +1017..+1021) first-pass records ANYWHERE after record 5984
  (X of entry 76); every insertion at or before it is neutral (it shifts X76 and the late Y76 alike). Late Y ids (8600..) follow the
  TOTAL record count, so any tail text change moves them: MEASURED with lk (tmp/lk_r7hd1.json, probe overlay probe/r7/hd1 = scoped
  capture block ctor/dtor calling StartCaptureBlock / RestoreCaptureBlock directly): X unchanged everywhere, every batch-2 late Y +2
  (wrong direction; d76 -6 -> -10). Grade-oracle probes (tools/rh.sh = fg + count of REAL hunks, OVL=<overlay> FLAGS=...): hd3 ctor
  direct, hd4 base dtor INLINE_DTOR-form, hd5 explicit Open=false dtor, hd6 empty scoped dtor -> byte-identical, hunk unchanged; hd2
  (dtor direct alone) changes code (+11 bytes). Inline-decision toggles (CSCOPEDSHADERSENABLE_FORCEINLINE off, CRENDERMANAGERCORE_
  FORCEINLINE_HELPERS off, SetCommonRenderStates plain inline, RESTORE_OPEN_FIRST off, DISPLACEMENT_INLINE off) all change code.
  A uniform "r records per setter expansion" model (dead asserts in SetRenderState) fixes no r in 1..79 (<=2 bad entries never
  reached). Conclusion: our helper texts are already minimal, so a natural fix would be MORE records in a FableWin-style pattern
  (FableWin /Od: SetRenderState +168i, AddStateToUpdateList +85i, SetTextureBumpEnvMatrix +332i, Select*Buffer +98/+171i, scoped
  block ctor/dtor +72i each -- dead asserts in the helper texts); that changes every site at once and the existing bit-A rulers would
  need re-fitting. Not pursued further (needs a whole-function lk re-solve per candidate assert set, 7.7 min each).
- fwsize (FW_EXTRA=/EHsc) for the row without rulers: Render 414i v FableWin 504i (+90i ~ one FableWin assert + ~18i);
  SetCommonRenderStates 75i v 106i (+24i float pushes, +7i still unexplained).
- 0x00B4D370 (differ/00b4d370.cpp unchanged): regraded DIFFER(7160v7176) with /d2inlT314; tools/nf.py (esp-frame + reloc masked
  difflib): same=1388/1760, 220 hunks. Device sweeps: k = 1..7 unused named locals at the top -> byte-identical; 1/2/4 dead
  FABLE_RETAIL_ASSERT(this) at the top or one before BuildDerived... -> byte-identical; 8 -> worse (1315). The final 26-loop: both
  builds keep the point in st; retail `fld E ; fmul st(i)` in Z,Y,X product order v ours `fld st(i) ; fmul E` in Y,X,Z -- operand
  order, not a memory/register question. Row is structurally far (frustum block, SetInverse, frame 0x3b4 v 0x3d0); parked.
- Grades this run: 00B783F0 ~16 + 1 lk run; 00B4D370 14.
- 0x00B70A90 MATCHED (wfE6-5): RELOCATION_MATCH, test PASS (mutant without the heading mirror FAILs), addmatch + read-only vland clean
  (WARN RELOC_NAME only). What closed it: (1) the made-up inline helper GFClamp01 replaced by the clamp written inline (an inline call
  takes a 32-record block: removing it is F -32, and it moved the Y-X gap by -5 = the needed T); (2) 3 dead FABLE_RETAIL_ASSERTs
  (`bank` after PeekGraphicBank, `frame.GetTexture()` before the texture fetch, `PeekHFOV() > 0.0f` before fovFraction; each +32);
  (3) `C2DVector direction(GPCamera->PeekLookVector().X, GPCamera->PeekLookVector().Y)` (the `look.X, look.Y` form flips bit B of the
  last block); (4) the FITTED unused0 local is no longer needed; DESIRED/BOOKMARK/DIRTY stay (bit A of blocks 9..12). Assert conditions
  matter only for bit A of one block (bank/skyTexture/texture/IsInitialised as the 2nd assert after AttachTextureToStage break site 9's A).
  Method: tools/00b70a90_abs.sh (q.py + ab.py one line per variant: A/B strings v retail) + lk runs (tools/00b70a90_lko.sh with OV=probe
  overlay) to measure X/Y; probe header tmp/a90/hdrp (RS_* hooks, never handed back). ~150 grades + 9 lk runs this author.
- 0x00B6EDF0 (wfE6-5): re-measured with the new devices in mind; the 9 real vertex-block lines are invariant to k = 1..80 nested parens round
  the vertex Lock's GetSize() argument and to 1..4 dead FABLE_RETAIL_ASSERTs after each of the three Locks (tmp/edf/P, tmp/edf/Q; 33
  grades, every variant byte-identical in the vertex block). The DAG dump (k/d1.json via k/pk.py) shows the real fact: v1's X/Y/Z stores
  all carry prio 83 (v0's 78, v1 UV 80, col 70) and go by IL index, so retail's Y-first needs a DAG/height difference for v1.Y, not a record
  count or region phase. Parked again.

## 0x00B6EDF0 CEngineWaterRenderer::BuildConstantBuffers -- RELOCATION_MATCH, test PASS, addmatch + read-only vland clean (wfE6 successor 3, 2026-11-15)
- src/00b6edf0.cpp, test/00b6edf0.cpp; no flags, no header change (overlay opt-ins as before: CENGINEWATERRENDERER, CENGINERESOURCEMANAGER,
  CTRIANGLESTRIPIFIER2, C3DTRIANGLE2_SETVERTEXINDEX, CTVERTEXCOLTEX1_SETTERS).
- What closed it: the vertex block's store order is decided by the c2 MEMORY-EDGE pattern, not by priorities of one setter. Model built from
  DAG dumps (tmp/edf2/00b6edf0_model.py / _gm.py, validated edge-for-edge on 3 dumps and order-for-order on 2 compiles): prio = outdeg + H,
  H(i) = max over succ (H(s) + 1 + edge latency); pick = highest prio among ready, ties by IL index. Alias classes: setters on `v[k]`
  (k != 0) give a `this` temp per setter call (stores of one setter independent, every other store dependent, except float -> col none
  from a precise store); plain `v[k].X = ..` stores are precise (no edges between them at all); setters on a plain pointer variable
  (v[0] = `v`) are precise. Retail needs each vertex's six stores mutually independent AND every store dependent on all earlier vertices:
  exactly what one pointer variable reassigned per vertex (`vertex = &v[k]; vertex->Set...`) gives (also separate pointers p0..p13, or an
  inline helper taking CTVertexColTex1&; neither is in the PDBs). With that, v6 / v12 / v13 schedule like retail at once; v1 then needs its
  stores in IL order Y, X, Z (all six have equal priority): FITTED `vertex->Y = 1; vertex->X = 1; vertex->Z = 1;`.
- Dead ends this run: CGenericVertex<CTVertexColTex1Base> template type (identical), direct member stores per group (fixes v6 only),
  sub-range pointers band = v + 6 / top = v + 12 (fix v6 / v12 only), `v += 6` (pointer register, 3040 bytes), DSE probes (c2 keeps
  overwritten stores: no dead-store elimination through pointers).
- Grades this run: ~40 + 8 DAG dumps. FITTED items marked in the source header (rowBase zero, v1 Y-first, v5 colour first).

## Seventh author (wfE6-7, 2026-11-15) -- 0x00B783F0 T-measurements (in progress)
- New fast probe: tools/lk/00b783f0_lkq.py + _lkq_gdb.py (native gdb ignore count K, then M logged comparator hits; ~4 min v 7.7 for a
  full lk), batch driver tools/00b783f0_qb.sh K M name=src[@overlay]..., reader tools/00b783f0_qy.py. With K=48900 M=700 the window holds
  entries 75/76/77 directly (baseline 5901/5966 5984/9059 5982/6017). tools/00b783f0_psolve.py (insertion point p x net change T) and
  tools/00b783f0_tflip.py (late-only T -> flipped entries): with F=0, T -7..-3 flips entry 76 alone (= MATCH); T<=-8 also flips 88,
  T>=12 flips 87; T in -2..11 is INVISIBLE in a grade, so a grade with the hunk unchanged only says T in -2..11.
- Measured T (Y76 shift, X76 fixed): capture-block probes hd6 (empty scoped dtor) +2, hd3/hd4/hd5 0; tail spellings nest2, bumpne,
  selnull, falseint, zeroint1 0 (c1-identical spellings move nothing).
- Grade-only (byte-identical, hunk unchanged): this1, nest1, bgblk, fin1, alphaRef, zerod, zeroint, attnull, restore2, seaelse; dead asserts in
  the capture block helpers (Scoped ctor/dtor, Capture, Restore; FableWin sizes 93i/92i/169i/169i suggest asserts there) -> unchanged
  (all four together also flip a bit-A site 0x00B7A565). Code-changing: bumpeq, factor, scrsrm, tailrm, tailscope, bgrm, attloop, hiblk,
  every `.empty()` -> size() form; begin()!=end() for one list flips bit A of 0x00B7A565 only.
- Deletion probes (diagnostic only, code-changing): removing SelectVertexBuffer(0,0) X76 -32 / Y76 -59; SelectIndexBuffer -32/-51;
  FinishUsingDynamicTextures() 0/0; the seaBg branch's two setters -15/-186; CScopedShadersEnable -36/-67. So the late ids count records
  of a WHOLE-FUNCTION pass (code after site 76 moves Y76): the lever is anywhere in the function, not only the tail.
- FOUND (dm7): drop the named `bool ice = iceFg || iceBg;` and test `!(iceFg || iceBg)` in the early return -> X76 -1, Y76 -4 (net T -3:
  the materialised || value costs 3 late records + 1 first-pass record). Entry 76 becomes a TIE (kX = kY = 1726) and the tie keeps ours'
  order, so the hunk stays; one more late record fewer (T -1..-3 at F -1) closes it. dm7 also flips the quartet's bit A (0x00B7945D);
  `bool created = CreateDynamicTextures(...); if (!created) return;` (cdt, ids identical to dm7) restores it -> b1+cdt = 1 real hunk again.
  Measured 0: capture dtor/base dtor texts D1 (`if (Open) StateManager->RestoreCaptureBlock();`), D2, SCRS defined after Render (aft_fi,
  FableWin order; byte-identical), De Morgan forms of the bools / early return / sea test (cA, cB), dropping component_params (cp1).
- Full lk on the dm7 base (tmp/lk_b1.json): entries 0..32 unchanged, every later entry X -1, late Y -4 (first-pass Y -1). Exact solution on
  it (tools/00b783f0_psolve.py, STRICT=1 = tie keeps ours): net -1..-3 records anywhere after record 5983 (first pass or late), nothing
  else. const bools (cbools) only swap X ids inside entries 63..75 (bit A churn, entry 76 untouched).
- FOUND the missing record, but only in a code-changing spelling: `bool bg = false; if (!BackgroundRenderList.empty() ||
  !AlphaedBackgroundRenderList.empty()) bg = true;` (if_bg, also f5/f7/ifr forms) -> Y76 9054 = bit B FIXED; the price is the bg=0 store
  hoisted from retail 0x00B78438 (after the 2nd cmp, inside the || lowering) to 0x00B7842A (entry block): 1 real hunk. So retail's bg is the
  plain `||` materialisation and the record is missing elsewhere. Saved as differ/00b783f0_ifbg.cpp (probe, not a candidate).
- Grade-screened on the dm7+cdt base (window T -1..-3 visible: the hunk vanishes) with no hit: if/ternary/!= forms of every bool (only bg's
  if-forms hit, with the code change; iceBg / seaMid if-forms change much more code), split `T x; x = e;` for every local, reassociated
  ||/&& chains (early return, sea test, CreateDynamicTextures arg, 3-term ifs), named / if-form CreateDynamicTextures args (code change),
  SCRS arm forms (!EnableBumpMap inverted arms change code; two ifs change code; this->, != 0 identical), k = 0..16 dead asserts x
  u = 0/3/6/8 unused locals (none removes 0x00B79938; k6..k11 u6 keep the quartet broken, u8 breaks 0x00B7A565 instead).
- New best differ/00b783f0.cpp = dm7 + cdt (1 real hunk, entry 76 a tie); the earlier best kept as differ/00b783f0_b0.cpp.
- Next ideas (not tried): a per-instance -1 inside SetCommonRenderStates (x3 instances, late records are whole-function) or any other
  code-neutral IL change worth -1..-3 late records on the dm7+cdt base -- screen by GRADE (3 s; the hunk vanishes) and confirm with the
  quick probe (tools/00b783f0_qb.sh 48900 700 name=src); header texts would need FableWin /Od evidence (fwsize lists the helpers).
- Grades this run: 00B783F0 ~230 + 31 quick probes + 2 full lk runs; 00B4D370 0 (not reopened: structurally far, no measured lead).

## Eighth author (wfE6-8, 2026-11-15) -- 0x00B783F0 bit B of entry 76 CLOSED by a code-neutral spelling (bit A of 0x00B7A565 open)
- FOUND: on the dm7+cdt base (differ/00b783f0.cpp), writing ONE render-list test as `L.begin() != L.end()` instead of `!L.empty()`
  (any one of the ten lists, tmp/e8/v1/be0..be9) removes the 0x00B79938 hunk (bit B of entry 76 = the T -1..-3 lever) and is
  otherwise byte-identical except bit A of the SetRenderState block at 0x00B7A565 (load order mask/flags) -> 2 real lines.
  `L.empty() == false` is c1-identical (no change). All ten lists as begin()!=end() (be_all) -> 5+ hunks at every k/u.
- Ruler sweep on be0 (fg list): k dead asserts x u unused locals (tmp/e8/o_v2f.txt): k6..12 x u6/u8 -> only 0x00B7A565; k4..11 u3 ->
  quartet sites 0x00B7945D/0x00B794C1 only; no 0-hunk cell in k 0..12 x u {0,3,6,8}. Next: finer u (1..10), multi-list be sets
  (tmp/e8/v3, v4), assert conditions / positions.
- Probe: swapping the operand order of the bookmark test / or in SetRenderState (probe overlays tmp/e8/ov1..3) is c1-canonicalised
  (byte-identical, entry 76 tie unchanged).
- test/00b783f0.cpp written (host model; host g++ PASS; vt.py not yet run).
- MATCHED (wfE6-8): 0x00B783F0 RELOCATION_MATCH = src/00b783f0.cpp (tmp/e8/v4/k06_u05.cpp + comments): be0 (fg list `begin() != end()`)
  + k = 6 dead FABLE_RETAIL_ASSERT(this) + u = 5 unused named locals (was 6) + FITTED `bool created` + no named ice. In the be0 k x u
  grid only k6 u5 is clean (k6 u4: quartet site 0x00B7945D; k4/5 u1/u2: 4 quartet hunks; u6/u8 k6..12: 0x00B7A565). vt.py PASS (host
  model; mutant without the high-end gate on the ice background pass FAILs). addmatch done; no flags; no header change (overlay unchanged).
  Read-only strict-header vland (LANDVERIFY_STRICT_HEADERS=1): all 5 payload rows RELOCATION_MATCH PASS; no RELOC_IDENTITY /
  HEADER_POLICY. One WARN for 0x00B783F0: RELOC_CONFLICT D3DAlphaFuncModeTable -> 0x01396DD8 here v 0x01396DE8 in landed 0x00BBC460
  (SetAlphaTestFunc(ALPHA_TEST_GREATER) reads element 4 = 0x01396DE8; the header documents the table base 0x01396DD8, so the landed
  row's binding is the element address, not the base -- coordinator: reloc_bindings.tsv entry for 0x00BBC460 looks off by +0x10).
- 0x00B4D370 (wfE6-8, ~16 grades, parked again; differ/00b4d370.cpp unchanged = best by nf metric). New facts / probes:
  * FableWin COMDATs: ?GetDeterminant@CMatrix3x4@@QBEMXZ (0x01B41130, 44i/118b) is emitted right after ?SetInverse@CMatrix3x4 (0x01B40BE0,
    332i/1085b) => FableWin's SetInverse calls m.GetDeterminant(); our SetInverse text is 205i at /Od (+127i unexplained: not just the
    det call). `float det = m.GetDeterminant();` with the same expression (probe overlays tmp/d3/ov_g0/g1) is byte-identical (term
    order of the det is canonicalised: g0 == g1); sign-cofactor forms g2/g3/g4 break the CSE (g3 7176 long but worse).
  * ?SetInverseNoTranslate@CMatrix3x4 exists (FableWin 0x02D840E0, 290i = SetInverse - 42i, next to the shadow TU's COMDAT cluster):
    probes with E4k = 0 (tmp/d3/nA/nB/nC: on CameraViewMatrix directly, on the copy, copy + SetTranslateOnly) are all SHORTER (6904..7048):
    retail computes the translation rows (frustum[0] = E4k), so the row's SetInverse-with-translation is right as far as length goes.
  * fwsize: C3DVector operator+ / operator- are +6i in FableWin (34i = the Initialise named-result form, cf. the existing
    FABLE_RETAIL_C3DVECTOR_INITIALISE_SUB); operator*(float) 29i v 31i. Probe opt-in INITIALISE_ADD (tmp/d3/ov_op, NOT in hdr/):
    length 7176 = retail, frame 0x3c0 (from 0x3b4; retail 0x3d0) but a block of the frustum/grid region moves (nf 1289) ->
    differ/00b4d370_a1_probe.cpp, a candidate base for the next author (the grid partial vectors in memory = retail 0x00B4D970..).
    INITIALISE_SUB alone: no length change (nf 1306); both: 7096; Initialise-form operator*: byte-identical.
  * Operand swaps / det term order are c2-id decisions (as acctB found); next step remains a c2 candidate-id trace on a1.
- 0x00B4D370 (wfE6 successor-2, in progress): NEW BASE differ/00b4d370_c1.cpp (DIFFER(7144v7176), nf2 metric 1547/1760 v 1497 for
  differ/00b4d370.cpp; metric x/nf2.py masks absolute [0x..] operands too). Two changes: (1) new opt-in
  FABLE_RETAIL_CMATRIX3X4_SETINVERSE_TRANSLATION_LOCAL (hdr CMatrix3x4.h: SetInverse with `C3DVector t(m.E41,m.E42,m.E43)` first,
  translation -(Ek1*t.X+...)): retail's cofactor order E12,E13,E11 (0x00B4D40A..62 exact) and column 3 kept in st; (2)
  TRANSFORMPOINT_LTR (FableWin 0x024543E0 text) instead of the row-made XZY: with (1) the frustum translation rows factor
  `(E31+E21+E11)*0` like retail 0x00B4D5C1 and the frustum block 0x00B4D660..0x00B4D8D0 aligns (rgdiff 225 -> 57).
  Diagnostic facts: TransformPoint text is canonicalised (LTR == ZYX_SUMS, SUMS == XZY); det text order canonicalised (48 forms
  identical); cofactor-local SetInverse forms give the right order but local-first det operands (x/sw1.py, sw2.py).
  * NEW BASE differ/00b4d370_c4.cpp (DIFFER(7192v7176), nf2 1570/1760, 127 hunks): c1 + the else-branch axes as NEW locals of the
    else block, `C2DVector x_axis(light_points[15] - light_points[11]); C2DVector y_axis(light_points[23] - light_points[3]);`
    (shadowing the outer pair; the outer x_axis/y_axis assignment forms unchanged): retail 0x00B4E580..0x00B4E6E0 now exact (was 34
    differing). Named-temporary forms (along/across copies) are worse; the outer `C2DVector x_axis(y.Y,-y.X)` ctor form is worse.
  * BEST = differ/00b4d370.cpp (= x/c5.cpp; old best kept as differ/00b4d370_prev_wfE6-8.cpp): c4 + the non-perspective scale loop as
    `light_points[k] *= scale` -> EQUAL LENGTH DIFFER(7176v7176), nf2 1580/1760, 117 hunks, frame 0x3b0 v 0x3d0. Region diff counts
    (x/rg.py): SetInverse 0x00B4D400.. 19, frustum ..0x00B4D8D0 69, grid/26-loop ..0x00B4DCC0 37, corner passes ..0x00B4E370 0,
    else branch ..0x00B4E700 3, scale/PostScale/final loop ..0x00B4EC00 71, end 30.
    Remaining residue is one systematic pattern: retail evaluates a commutative product/sum with the MEMORY operand first
    (`fld [mem] ; fmul st(i)` and `fld [mem] ; fadd st(1) ; fstp ; fstp st(0)`), ours `fld st(i) ; fmul [mem]` / `fadd [mem]`:
    SetInverse det (E31*c13, E21*c12), grid weights (u*v), final 26-loop PTP (rot element first), PostTranslate(0.5) rows. Text order is
    canonicalised (det 48 forms, grid operand swaps, TransformPoint forms all byte-identical); only symbol population moves it
    (cofactor locals, a `const CMatrix3x4& m = mm` alias, removing the named `u` flip single sites but shift others). Devices on c1:
    asserts 1/2/4 tiny change, 3/6/8 worse; k unused locals 1..5 identical, 7 = +6.
    Still open ideas: (1) c2 id trace of one product (det `m.E31*c13`) to learn which population fact puts the memory operand first;
    (2) frame: retail has 3 more scalar slots below the view copy (view at 0x8C v ours 0x80) and 8+16 bytes around light_points;
    (3) -1e30 min/max init stores scheduled before PostScale in ours (retail after PostTranslate, 0x00B4E962).
- Grades this run (succ-2): ~150 (x/ = scratch; every variant file kept there with its .q).
  * Also measured on c5 (no gain): min/max declaration orders (3 orders byte-identical); frustum[5] initialised before the view copy /
    before `CMatrix3x4 inverse` / before local_to_base (7208..7304, worse: retail interleaves the MaxShadowDistance stores into the
    SetInverse schedule, so the array is initialised after SetInverse as now); combination sweep of {no named u, vector *= loop, 7
    unused locals, indexed final loop} on c1: 1521..1555, noise.

## OWNER GUIDANCE 2026-11-15 (coordinator note, for every agent in this dir, read now)
Sometimes hand-authoring is more likely to get a match than automatic tools. Read the retail disassembly and the matched same-class / same-TU siblings, reason about the statement structure a programmer would have written, and write it by hand. Use sweeps and generators only to answer one specific measured question; stop sweeping when two sweeps in a row move nothing. Full text: /tmp/claude-0/-home-user-FableDecomp/923afea2-3bc0-53db-a7ad-a8bbc78e03e2/scratchpad/w22/wfcommon/WF_RULES.md (last section).
- 0x00B4D370 (wfE6 successor-3, ~60 grades + 2 lk runs, parked again). NEW BEST differ/00b4d370.cpp (= y/a1.cpp) DIFFER(7192v7176),
  nf2 1589/1760, 114 hunks; old best kept as differ/00b4d370_prev_succ2.cpp (7176v7176, 1580/117). Scratch in y/ (every variant + .q).
  * WARNING on the metric: nf2 masks esp offsets, so it reports wrong-ELEMENT sums as equal. Real state at SetInverse (view copy
    retail 0x8C / ours 0x80): retail computes the E21-cofactor E13*E32-E12*E33 first, then E12*E23-E13*E22, then E22*E33-..; ours
    the first two swapped (the succ-2 claim "0x00B4D40A..62 exact" was masked). Translation sum 0x00B4D5C1: retail E31+E21+E11, ours
    E11+E31+E21. Light-loop X row 0x00B4DB2D: retail (E31*z + E21*y) + E11*x + E41, ours (E11*x + E31*z) + E21*y. Final 26-loop PTP
    0x00B4E988: retail (E31*z + E21*y) + E11*x memory-first, ours (y*E21 + x*E11) + z*E31. tools/nf3.py = nf2 without the esp mask.
  * CLOSED a block: grid weights 0x00B4D8F0..0x00B4D93E exact with u / one_minus_u defined per point inside the j loop AFTER v /
    one_minus_v (lk: base ids u 27, 1-u 28, v 30, 1-v 31 -> keys id*32, larger first; retail needs u, 1-u above v, 1-v). EVIDENCED.
  * c2 key model measured (tools/lk/cm.py = readable comparator log; tmp/lk_base.json, tmp/lk_r720.json): the +4.. fields of local
    aggregates (view, inverse, local_to_base, rot, perspective, frustum elements' Y/Z) get records in a LATE pass (ids 2133.. in base),
    after the named-local pass (0..~866) and the "+0" pass (867..~2120). Ids >= 2048 key as rotl16(id,5), i.e. ~ (id mod 2048)*32, so
    those fields key LOW and our st-held temps / point components sort first; retail's systematic memory-first pattern = those field
    records keying HIGH. Ruler diagnostic (N dead `float rK = MaxShadowDistance;` at the END, 2 ids each, numbered before the +0 pass):
    N=720/790/810..840 make the SetInverse region exact (inv=0), grid best (17..18) at N>=800, else best at 830, scl best 27 at 810,
    frustum stuck at 32 for every N: NOT a single uniform shift (region windows differ); N=1..32 (end or top) nothing (chunked).
    So retail's record population differs by ~+1440 or ~-600 ids (mod 2048) before the late pass, plus local differences.
  * Measured, no gain: corner passes as nested i,j loops (VC7.1 does not unroll: 6152..6696 bytes, so the manual unroll is right);
    FABLE_RETAIL_CMATRIX3X4_DATA_UNION (PDB union arm) byte-identical; two-arg TransformPoint(points[k], light_points[k]) with
    SOURCE_DEST_SUMS (retail keeps the p copy at 0x78: worse); light-loop forms `C3DVector p;` outside (7176 long but grid 83), in-place
    on the array, copy-ctor, rebuilt copy (all worse); min/max declaration orders / chained init (no change); a non-callee function
    defined above the row (byte-identical: c2 ids are per function); CMatrix4x4(EMatrix34Init) if/else text (probe overlay tmp/ovI:
    worse, 38i at /Od v FableWin 75i -- the identity ctor text is still unknown, +37i); k = 1..7 initialised unused locals at the top
    (k=1 +2 same, k>=2 identical).
  * fwsize (a1): SAME for TransformPoint, PerspectiveTransformPoint, Initialise(16), C3DVector ops except +/- (+6i); PostMultiply +6i,
    PostScale +3i, PostTranslate +1i (loops plausible); CMatrix4x4(EMatrix34Init) 36i v 75i; SetInverse 207i v 332i; row 1058i v 1202i.
  * Frame: retail bottom+104..+116 is the light-loop C3DVector p (+ $T5892, one_over_max, result$6044 share it); ours packs them at
    +12 with the grid p / corner q's. Every length drift (incl. a1's +16) is disp8/disp32 from that layout (tools/drift.py,
    tools/slotmap.py, tools/efm.py fixed for short listings). Packer replay tools exist: rebuild/candidates/manual/pending/
    2026-11-08_linux_hand886_wip/tools8/pack.py + phook.py (not run yet; the next level for this row).
  * Third source: the 360 Test PDB has no CEngineShadowRenderer functions (UE3 renderer replaced it): no original local names.
  * Next author: (1) run phook/pack on a1 to see why the light-loop p shares the +12 slot (conflict set / weights); (2) find the
    natural text that changes the named-local count by the lk-measured window (helper texts whose FableWin /Od size disagrees:
    identity ctor +37i, SetInverse +125i, row +144i); (3) re-measure each region's best ruler window after every structural change.
- 0x00B4D370 (wfE6 successor-4, ~60 grades + 1 lk run (10 min each), parked again; differ/00b4d370.cpp unchanged = best). Scratch z/
  (every variant + .q; z/lk_base.json = lk of the current best). New tools: tools/nf5.py (nf2 alignment + majority slot map: counts
  wrong-ELEMENT operands per region that nf2 hides), tools/lk/recs2.py (records by id: `recs2.py lk.json lo:hi`), tools/mkassert.py.
  * Late-pass ORDER (not only offset) differs. lk of the best: view field records E41..E43 #2123..2128 (from SetInverse's t), then
    E22 #2133, E33 2134, E23 2135, E32 2136, E13 2140, E12 2141, E21 2143, E31 2240; E11 = view+0 #873 (+0 pass). Larger key is loaded
    first. Retail SetInverse (0x00B4D40A..0x00B4D481) needs key order E13>E32, E12>E33, E12>E23, E13>E22, E33>E22, E23>E32 and det/sum
    order E31>E21>E11. E31>E21>E11 = all three un-wrapped (ids mod 2048 above 873): any uniform shift s of the late pass in
    [-1269,-193] or [779,1855] gives it (the succ-3 ruler window 720..840 pairs = s 1440..1680 is inside). But E23>E32 contradicts our
    late order (2135<2136) for EVERY s except s=-88 (wrap between them), which breaks E12>E33: so retail's late pass numbers the view
    fields in a different order (E32 before E23), i.e. retail's SetInverse/GetDeterminant traversal differs, not only the population
    before it. Ids come in 32-blocks (jumps 895->1056, 1151->1216, 1247->1440, 1471->1760, 1791->1856, 1887->2112, 2143->2240).
  * Dead asserts (device d): FABLE_RETAIL_ASSERT x k at 10 positions (top .. before the final compose), k=1..3: k=1 byte-identical
    to base everywhere, k>=2 identical to each other everywhere (inv 24+17, worse): a single global effect, position-independent; not a
    shifter of the c2 ids here. Device d does not help this row.
  * /d2inlT314 is replaceable: FABLE_FORCEINLINE on CMatrix4x4(const CMatrix3x4&) / Initialise(const CMatrix3x4&) / PostMultiply
    (probe overlay z/hdrp, NOT in hdr/) gives byte-identical code with and without the flag (an opt-in instead of a flag, if ever landed).
  * Diagnostics (truncations, not candidates): delete the perspective + else blocks (z/d3) -> SetInverse 4+3 and frustum 5+16 by nf5
    (from 16+15 / 57+20): the frustum block's 32-stuck residue is key population, not structure; delete the tail (d2) worse; both (d4)
    worse again: non-monotone (32-block chunked).
  * Measured, no gain: frustum by assignments (7345), via a `d` local (7112), by direct field stores (L3: equal length, inv 14+8 but
    frustum 60+49); grid point component-wise (6872: the C3DVector operators are right, partial vectors in memory); projected as two
    floats (corner/persp structure breaks); one reused corner q (7144).
  * Next level (untried): find a SetInverse/GetDeterminant text whose c2 traversal numbers E32 before E23 and E22<E33<E12 while keeping
    retail's cofactor CSE (FableWin SetInverse calls GetDeterminant, 332i /Od): one lk run per candidate (recs2.py 2100:2300).
  * succ-4 later: SetInverse TEXT fact (EVIDENCED under the ruler diagnostic): late field records follow FIRST TOUCH in the inline body;
    only cofactor A's two product texts matter (64-way sweep of the 6 det products: only bits P1 `E22*E33`/P2 `E23*E32` change code; the
    12 non-det element products are canonical). With cofactor A = `m.E22 * m.E33 - m.E32 * m.E23` (probe body saved as
    differ/00b4d370_SetInverse_S3.h; z/hdrp + -DSIPROBE harness z/gp.sh, z/gr.sh) + 720 end rulers, SetInverse 0x00B4D40A..0x00B4D5C1 is
    exact (inv 0+0); the current text gives 0+2 there. Element statement order E11,E21,E31,E12.. is right (E11,E12,E13.. order: inv 43+7).
    Translation operand order canonical. So SetInverse = S3 text + population; the population (late field ids mod 2048 above view+0's
    #873, i.e. s in [-1269,-193] or [779,1855]) is the only open question there. Frustum under ruler 720 stays 31+32 for every TP form
    (LTR/PARTICLE_POINT/ZYX canonical; SUMS/XZY/default longer), frustum init form, unrolled calls; truncation d3 + S3 gives inv 4+1,
    frustum 5+16 (the E11 term still first: late keys below #873) -> retail's population is more likely FEWER ids after the named pass
    than the ruler's +1440 (the ruler also wraps the +0 pass).
- 0x00B4D370 (wfE6 successor-5, 2026-11-16, ~40 grades + ~15 fwsize probes, parked again; differ/00b4d370.cpp unchanged = best). Level
  changed to FableWin /Od helper evidence (fwsize-SAME base), as the advisor proposed. Scratch v/ (probe overlay v/hdrp, tools/gp2.sh =
  one-line grade with #defines + probe overlay, tools/fwp.sh = fwsize with defines, tools/odis.py = /Od disassembly of one symbol).
  * fwsize of the best (FW_EXTRA none / /EHsc same for the row): SAME for 18 helpers; DIFF: PostTranslate +1i/+7b, GetSquaredMagnitude
    +2i/+8b, operator*(float) +2i, GFLimit +2i, PostScale +3i, Initialise(3x4) +4i/-8b, C3DVector +/- +6i, PostMultiply +6i (its retail
    out-of-line copy 0x00B507E0 is LANDED with this text, so the /Od gap is FableWin-only), CMatrix4x4(EMatrix34Init) +39i, SetInverse
    +125i, row +144i.
  * NEW EVIDENCE: EMatrix34Init = { MATRIX34_INIT_ZERO = 0, MATRIX34_INIT_IDENTITY = 1 } (360 Test PDB type 0x110C; the repo headers and
    the landed _$E1 row 0x01224340 declare a one-value enum, IDENTITY = 0 -- descriptive, code-neutral). FableWin
    ??0CMatrix3x4@@QAE@W4EMatrix34Init@@@Z is 83i "two 12-store blocks behind an enum test" (acctB r3010 note); a CMatrix4x4 ctor with a
    switch (ZERO: zero loop, IDENTITY: the retail (i==j)?1:0 loop) is 65i/208b at /Od v FableWin 75i/209b (current loop text 36i/112b);
    if/else forms 60i/191b, 71i/283b. Graded: A/B/D all identical, nf5 330 -> 318 (inv 16+15 -> 14+8), length / frame unchanged.
  * SetInverse calling `m.GetDeterminant()` (FableWin COMDAT 0x01B41130 right after SetInverse 0x01B40BE0): GetDeterminant 42i v 44i
    (named `float det` result 43i/115b v 44i/118b); SetInverse 174i v 332i (+158i ~ two FableWin asserts of ~79i). With the S3 cofactor
    text + t local: nf5 316 (inv 14+6). Dead asserts INSIDE the inline SetInverse (det != 0, &m != this; 1 or 2): byte-identical.
  * Initialise-form C3DVector operators (+, -, *(float): 34i/92b v 34i/90b, 31i/84b v 31i/82b, i.e. instruction counts SAME): operator*
    byte-identical; + and/or - move the population: 7208 long, frame 0x3bc (nearer 0x3d0) but nf5 455..464 (every region worse); on the
    older bases c1/c4/c5/prev_succ2 also 7032..7208, nf5 464..562. Row dead asserts on the A+GD base: k=1,2 identical, 3: 310, 4: 305
    (corner pass only), 6: 361, 8: 7112 long -- noise, no direction.
  * Candidate kept as differ/00b4d370_e1.cpp (= best + #defines of the probe texts; overlay = hdr/ + differ/00b4d370_hdrp/engine/{CMatrix4x4,
    CMatrix3x4,C3DVector}.h, i.e. v/hdrp): DIFFER(7192v7176), frame 0x3b0, nf5 316 v 330, nf2 1591 v 1589. Not promoted (length and
    frame unchanged; probe opt-ins not in hdr/, so the matched rows need no regrade).
  * Frame fact (efm map / sbs): in the grid block retail 0x00B4D95C..0x00B4DA96 our code is instruction-identical except slot numbers;
    retail scatters the operator temporaries into holes (0x144/0x148/0x14C/0x154 below light_points, 0x298/0x2A4 = a 24-byte gap between
    light_points 0x158 and points 0x2A8, 0x7C/0x80, 0xCC..0xD4); ours packs them all above frustum. The 0x20-byte frame gap is a packer
    partition question (replay tools: $S/w22/wfE3/tools/ph/ph_host.py + sim.py, not run on this row: one trace of a 7 KB function).
  * Next author: (1) a phook trace of the best + sim.py to learn which conflict / order puts the grid temporaries above frustum (retail's
    holes say they join light_points-adjacent slots); (2) the FableWin assert texts (row +144i, SetInverse +158i) are unknowable without
    the binary -- do not sweep assert counts further (measured flat / noisy twice).
- 0x00B4D370 (wfE6 successor-6, 2026-11-16) -- FRAME PARTITION level (phook trace tools/ph/ph_host.py + tools/ph/simall.py, sim
  validated: slot members = efm built slots, slot sizes sum = frame). Retail facts read from the bytes: (R1) the LAST PostMultiply(rot)
  result is built at 0x100 = local_to_base's home (retail 0x00B4EE98 lea edx,[esp+0x104]; 0x00B4EF4C copy from 0x100) -> local_to_base's
  slot is 64 bytes (0x130..0x13F never touched), i.e. local_to_base (address-taken: NRV return slot) does NOT interfere with that
  result; in ours it does. Measured rule: a FUNCTION-SCOPE address-taken local conflicts with every later slot member; inside any nested
  block it does not (rot hoisted alone = 944; one block from local_to_base to after PostMultiply(rot), rot unhoisted = joins, 980;
  the first y_axis in a block also shares with later members). (R2) else-branch y_axis lands at 0x84 = the first y_axis's home
  (either the same variable reassigned, or a new local after the first y_axis's block ended; the new-local form reproduces retail's
  else code exactly (compare reads the temp at 0x24/0x28), so the reassigned form is disfavoured, see below).
  (R3) grid temps result(f4w3) 0x140, sumA $T 0x14C, f3w2 0x290, f2w1 0x29C are exclusive; (R4) sumB 0xCC shares with the PostScale
  1/(max-min) and PostTranslate(-CameraPos) temps; (R5) light-loop p at 0x78 with f1w0 result and the x_axis C2DVector temps, grid p at
  0x24 with the persp-branch q's and the else lp[23]-lp[3] temp.
  RESULT: differ/00b4d370_f1.cpp (R1 via `CMatrix4x4 rot;` hoisted + block {local_to_base .. PostMultiply(CMatrix4x4(local_to_base))},
  R2 y_axis reassigned) = DIFFER(7176v7176), frame 0x3d0 = retail (first equal length AND frame), nf5 327 v 330. The R1 block is a
  measurement spelling (unnatural); the natural statement shape that ends local_to_base's scope is still to find.
  succ-6 continued: the else branch's NEW-local `C2DVector y_axis(lp[23]-lp[3])` form is right after all (retail reads the temp at
  0x24/0x28 for the magnitude compare and the x_axis arm; the reassigned form reads y_axis): f1 is refuted there and kept only as a
  frame probe (disfavoured, not refuted). NEW BEST differ/00b4d370.cpp (old best -> differ/00b4d370_prev_succ5.cpp) = R1 block + a block around the first
  y_axis / x_axis / rot.Initialise / four-corner y range (retail's else x_axis shares the first x_axis home esp+0x18): DIFFER(7162v7176),
  frame 0x3c8, nf5 328, else 3+16 (instruction-identical to retail apart from slots). With the succ-5 probe helper texts
  (differ/00b4d370_p3.cpp + differ/00b4d370_hdrp overlay): DIFFER(7176v7176) frame 0x3c8 nf5 313 (lowest nf5 of any variant).
  Remaining partition facts (sim checker tools/ph/facts.py: lp+f1 f3alone gp+persq yy sumB) need several coordinated changes: no single
  sim edit (a:/x:/order swap, tools/ph/srch3.py) satisfies more than two; the 4-byte scalar partition differs too (retail esp+0x18 =
  j counter + a grid weight + both x_axis; esp+0x84 = both y_axis alone; esp+0x24 = grid p + ALL eight corner q's + final q + else temp;
  esp+0x78 = light-loop p + f1w0 result + the three C2DVector arm/axis temps; esp+0xCC = sumB + PostScale + PostTranslate temps).
  Rulers on the new structure (end floats N=720/830): SetInverse exact again, frustum 31+24..32 stuck as before: the scope work does not
  move the x87 residue (frustum TransformPoint term order (x*E11+z*E31)+y*E21 v ours (z*E31+y*E21)+x*E11 at 0x00B4D654..).
  Tools: tools/ph/{ph_host.py,sim.py,simall.py (prints all slots + TOTAL),pt.sh (trace+sim one line),inter.sh,facts.py,srch3.py}.
  Grades this run: ~20 full grades + ~16 packer traces (11 s each).
  succ-6 final: least-fitted spelling promoted (= e3 code byte-identical, md5 of the build): ONE block from local_to_base to after
  PostMultiply(rot) (rot NOT hoisted) + a block {first y_axis, x_axis, rot.Initialise} with `CMatrix4x4 rot;` just before it (near_y
  not hoisted). Alternative with only the outer block: u/t1.cpp DIFFER(7192v7176), frame 0x3d4, nf5 323, nf2 1590 (better nf5/nf2,
  but the x_axis sharing fact fails). Retail small-slot shape (verified sum 148 = 976 - 816 big - 12 light_vector): twelve 4-byte
  slots (esp+0x10,0x14,0x20,0x30,0x74, 0xD8..0xF0 x7), two 8-byte (0x18 x_axis's + scalars, 0x84 y_axis's), seven 12-byte (0x24,
  0x78, 0xCC, 0x140, 0x14C, 0x290, 0x29C). Base already had the two 8-byte slots (x_axis in a scalar slot, y_axis alone); every block
  variant loses the y_axis one (it joins a scalar slot), which is the 8 missing frame bytes.
- 0x00B4D370 (wfE6 successor-7, 2026-11-16; scratch n/, one-line grader tools/gn.sh <src> [overlay]) -- new levels measured, best unchanged:
  * VISIBLE SAME-TU CALLEE: CalcLocalWorldToBaseShadowSpace 0x00B4B8B0 (421 b, ret 10h, not landed; hand263 / hand759 draft 421v421) defined
    above the row (n/b1.cpp): BYTE-IDENTICAL to the best (q output cmp-equal). c2's NRV/escape view of local_to_base does not depend on it.
  * DEVICE c on the current best (k = 1..7 `long unusedN = 0` at 4 positions, n/k_*.cpp): position-independent; k=1 nf5 316 (inv 14+8,
    fru 56+21), k=2 322, k=3 328 (= base), k>=4 375..382. Length / frame never move. Not a closer.
  * DEAD DEBUG CALL (new evidence: Ego_r.pdb / FableWin.pdb have private ?DrawFrustrumDebugScreen@CEngineShadowRenderer@@AAEXPBVC3DVector@@
    JJABVCMatrix4x4@@@Z, Ego_r 0x009EF770 right before UpdateShadowSpace 0x009F0370; NOT in retail (no catalog / coverage / reloc entry);
    UNVERIFIED hypothesis that the source calls it under a compile-time condition -- caller unknown; Ego_r span deltas prove nothing (Ego_r is
    a different compile: CalcLocalWorld... 400 b v retail 421, BuildDerived 0x320 v 0x260). Probe `if (0) DbgDraw(arr, 26, 0, mat);` (stand-in decl):
    dead code DOES reach the frame partition (address-taken is computed before dead-code removal) but never the x87 keys: arrays already
    exposed (light_points / points: no change); frustum exposed +0x20 frame, rot exposed +0x30 (7293 long); same on prev_succ5 / p3 / e1
    (0x3ec / 0x3e0 / 0x3e8 ...). No arg set gives retail's +8 (every exposure overshoots 0x3d0). Dead `if (0)` is position-independent. NEGATIVE result.
  * TRUNCATION BISECT (diagnostic, n/t_*.cpp; inv / fru = regions before the cut): whole perspective branch body deleted (t_noP) -> inv 0+2,
    fru 4+16 (SetInverse and frustum nearly exact); else body deleted 18+19 / 74+19; both 4+3 / 5+16; perspective pieces alone are
    NON-MONOTONE: corners deleted 16+5 / 139+13, tail (scale loop + perspective matrix + PostMultiply) deleted 14+10 / 56+22, head+corners
    replaced 21+8 / 97+24. Reading: the perspective branch's instructions are right (corn region 0 real diffs) but retail's function has
    fewer c2 records before the late field pass than ours (consistent with succ-4's s in [-1269,-193]); no single block carries it.
  * Scopes do not consume ids: the four perspective corner blocks and the four y-range blocks without braces (numbered q1..q4,
    projected1.. etc., n/u_*.cpp) are BYTE-IDENTICAL to the best, frame included.
  * Parked again after ~75 grades (best differ/00b4d370.cpp unchanged, DIFFER(7162v7176) nf5 328). Next level for a successor: find which
    construct retail lacks by an lk record census per construct (tools/lk/recs2.py on a fresh lk of the best; the population is 1792
    records in 32-id blocks): candidates are the 16-parameter Initialise expansions (rot x2, perspective) v direct member stores, the
    PostMultiply copy `CMatrix4x4 a(*this)`, the nine PerspectiveTransformPoint expansions.
  * Last probe (n/i_*.cpp): the three 16-float Initialise calls as direct member stores: perspective 327, rot#1 336 (7160), else rot
    7176-long 317 (inv 14+8, fru 56+22), all three 389 -- noise, never near t_noP's inv 0+2 / fru 4+16. Stopped (~79 grades this run).
- 0x00B4D370 (wfE6 successor-8, 2026-11-16; scratch s/, one-line grader tools/gn.sh <src> [overlay]; S3 gauge overlay s/hdrS = hdr/ + cofactor A
  `m.E22 * m.E33 - m.E32 * m.E23` in the SETINVERSE_TRANSLATION_LOCAL body, probe only) -- best differ/00b4d370.cpp unchanged, parked again:
  * /d2inlT314 IS JUSTIFIED: flagless the row calls PostMultiply out of line 3x (9 calls v retail 5); PostMultiply inlines from T~185..190 up
    (callee-only cost: t_noP has the same threshold), and T 190..1000 give byte-identical code. The landed same-TU sibling 0x00B4FC40
    SetupShadowFuncRenderStates carries `// Flags: /d2inlT314` with an Inline-budget line for exactly this callee (PostMultiply scores 313 v
    default 74; unreferenced retail COMDAT 0x00B507E0). Copy that Inline-budget line if the row ever lands.
  * lk census (s/lk_*.json; comparator key = rotl16(id, 5), i.e. id mod 2048 in the high bits, bit 0 set for ids >= 2048 -- checked on
    873->0x6d20, 2133->0x0aa1, 2240->0x1801, 1619->0xca60; same rule as LESSONS_wfB4): base view+0 #873 (key 0x6d20), late view fields #2123..2136
    + E31 #2240 (keys 0x0aa1..0x1801, wrapped below 0x6d20); t_noP view+0 #593, late #1609..1622 (0xca60.., above 0x4a20) -> with S3 text
    inv 0+0, fru 4+16. Ids come from a shared pool in 32-id chunks interleaved with invisible consumers (gaps of 192 / 288 ids), so the late
    start is NON-MONOTONE in deletions: t_noPtail (tail deleted) puts the late pass LATER (#2145, view+0 #965); t_noCorn #1591 (unwrapped)
    but inv 16+3 under S3 (late view records split across a chunk boundary 1599 -> 1792). Initialise(16 floats) x3 as member stores: view+0
    #871 (id-neutral), late #1919/#2240 (worse).
  * S3-gauge sweep (inv under s/hdrS; 1.2 s per grade): inv takes only four values over ~70 variants (16+14, 14+6, 16+7, 23+18), never 0:
    12 single natural spellings (x_axis ctor, light loop direct, t without start/end, final loop index, grid sum division, near_y default,
    fade without cast, ext_sum `C3DVector edge`, persp without candidate / n1 / limit / f locals) + 40 random combinations (s/cb/); D1..D4
    deletions (PostMultiply x3, the six PostScale/PostTranslate, identity ctor, frustum loop) all keep inv wrong. Best nf5 of the sweep 309
    (fin_index+near_else+fade_f+ext_sum, 7172 long, frame 0x3c4) -- noise, not promoted.
  * Negative: y-range corners as a 4-trip loop stay a loop (6980); BuildDerived... landed text defined above the row: byte-identical;
    /d2inlT sweep 190..1000 byte-identical; fwsize probes of SetIdentity / EMatrix34Init / PostScale / PostTranslate / operator texts find
    no FableWin-size text (FableWin SetIdentity 55i/138b, ctor 75i/209b unmatched by loop / switch / store forms; only the YX
    GetSquaredMagnitude reaches 18i, 44b v 43b).
  * FableWin TU COMDAT list (first instantiated in this TU, after DrawFrustrumDebugScreen 0x02D78080): CArray<C3DVector>(unsigned) (vector:
    heap, so not in this row), GFLimit<float,int,int>, CPixelFormat::IsValid, five CRenderStateManager setters, CPlane(3 vectors),
    CMatrix3x4::PreTranslate(C3DVector) / SetTranslateOnly(MMM) / PostTranslate(MMM) / PostScale(C3DVector) / PostScale(MMM),
    C3DBoundingBox::CalcFurthestDistanceToPointSquared, CMatrix4x4 ctor() / ctor(EMatrix34Init) / ctor(CMatrix3x4) / SetIdentity /
    Initialise(3x4) / Initialise(16) / PostMultiply / PerspectiveTransformPoint / PostTranslate(C3DVector) / PostScale(C3DVector). Retail
    builds the final identity in a stack temp (0x00B4EC84 loop, `(i==j)?1:0`) and copies it: CMatrix4x4(MATRIX34_INIT_IDENTITY) is right.
  * Device d with id-consuming conditions (k = 1..8 at the row top, s/ad/): `light_vector.GetMagnitude() > 0` (inline call) and
    `(light_vector - SceneData.ShadowLightVector).GetMagnitude() < 1` (class temporary) both move code (7002..7162 long) but inv stays
    12+6 .. 24+16 under s/hdrS; best tmp1 nf5 319 (inv 12+6). Device d closed for this row (scalar forms were flat in succ-4/5).
  * Lessons dir checked (LESSONS_wfB4 id-map regimes / rotl16 wrap, wfB3 helper-internal rulers, C19/C28 IL-size helper texts): the
    FableWin-larger helpers here are the float-argument artefacts wfB4 lists (operator* 31i, +/- 34i default bodies) plus SetIdentity /
    EMatrix34Init / SetInverse / the row itself (asserts per wfB4's sizing rule); no FableWin-backed helper text was found that moves ids.
  * Grades this run: ~130 full grades (incl. ~70 sweep, 16 device d) + 5 lk runs (5 min each). Reading for a successor: the row needs a source whose c2 id
    population puts the late view-field records below id 2048 in one chunk (as t_noP does); no single natural spelling or deletion short of
    the whole perspective body does it. Without the original source text this is a lottery; park unless new evidence (another build's bytes).
