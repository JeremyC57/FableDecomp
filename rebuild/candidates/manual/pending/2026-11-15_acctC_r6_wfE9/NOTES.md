# wfE9 NOTES (acctC lane)

## 0x005A4CDD CTCInventoryTrade::ConstructActualSellInventoryPC -- DIFFER(3664v3668), 1 hunk (differ/005a4cdd.cpp = earlier draft, ports unchanged onto today's headers + wfB2 overlay)
Residue: 0x005A4E49 retail `mov esi,[ebp+4] ; cmp esi,[ebp+8] ; je 0x005A4F19` (loop-2 head test duplicated, its empty exit
threaded past the ItemsToBuy insert loop); ours `mov esi,[ebp+4] ; jmp <bottom test>`. Everything else identical (incl. edi=0 reuse as index).
Mechanism (my reading): c2 duplicates every loop test at the head, then replaces a head copy that is equivalent to `jmp test` by the jmp
(the cross-jump). Retail kept loop 2's copy because its exit edge was threaded past loop 3; ours did not thread.
Tried this session (all no change unless said): dead-arm asserts (FABLE_RETAIL_ASSERT) at 3 positions; shared iterator across loops 2-3
(V1: +2 x87-free operand-order hunks at 0x005A54AC) and across 1-3; `buyableItems.end() != it`; while-loop; `&*begin()`;
guard `if (!buyableItems.empty()) { loop2; loop3 }` (probe/005a4cdd_s7a_guard.cpp): control flow = retail, residue
`mov eax,[ebp+4] ; cmp eax,[ebp+8] ; ... mov esi,eax` (begin temp not coalesced with `it`), 3670v3668; same with begin()!=end(); size() worse.
## 0x005AC10E CTCInventoryTrade::ConstructActualBuyInventoryPC -- DIFFER(3538v3532), 4 hunks (differ/005ac10e.cpp = earlier draft)
Residue: (1) 0x005AC189 retail `mov esi,[ebp+28h] ; jmp test` (head copy cross-jumped because begin temp is coalesced with `it` = esi),
ours `mov eax,[ebp+28h] ; cmp eax,[ebp+2Ch] ; mov esi,eax ; je` (begin temp in eax); (2) 0x005AC373 `!buyableItems.empty()` operand
order (retail loads end first); (3) one `mov edi,[ebp+4Ch]` scheduled one slot apart near 0x005ACB5B.
The SAME begin-temp-v-`it` colouring is the residue of the Sell guard variant: one fix should close both twins.
Tried: while loop with outside iterator; inverted condition (worse); continue form; dead asserts after the loop / before Initialise;
iterator declared at top / shared with the two later buyableItems loops; `end() != it` (worse); size() tests (worse); k=1..3 unused locals (no move).
More twin probes (all no change): Sell loop 3 as while / for-break / guarded do-while (sW1-3: byte-identical to the for form: c2
canonicalises every loop shape, so the source loop form is NOT the lever); guard around loop 2 only (s7g: identical to plain);
`it` declared before a guard on `it != end()` (s7e: threaded flow, residue `mov eax,[ebp+4] ; cmp eax ; mov esi,eax`); braces around
loop 3; index incremented in the for header (worse); 10-40 dead-arm asserts (no move: dead arms vanish before this decision).
c2prio (wfcommon/tools/C28/c2prio_f.py, copied to tools/c2prio.py; docs/pipeline/C2_ALLOCATOR_PROBE.md):
- Buy: begin temp #6 (refs in the Init-call / loop-head block 7, cost 4, K 5) prio 20 is coloured eax before `it` #26 prio 13
  (+5 block 7, +15 +15 in the body, -1..-4 live through the || temporary-destructor blocks). K(block 7) counts this, the hoisted
  loop constants 8 and 1, `it` and the temp. Retail needs `it` > temp (or K(block 7) <= 2) so the head copy coalesces into esi and is
  cross-jumped to `jmp test`.
- Sell: loop-2 begin temp #9 spans the loop-2 head AND the loop-3 init (lines 119,126; CSE of the begin() load on the empty path),
  prio 22, split esi; loop-2 `it` #37 prio 14 esi. So the colouring is already right in plain Sell; only the threading is missing.
Next ideas (not tried): find which IL fact lets retail thread the empty edge (value of t9 on that edge) -- compare a c2 --trace of
s7e (threads) against the plain form; for Buy, anything that removes `this` or the hoisted constants from the head block's K.

## 0x00648820 NPlayerGui::CDrawMiniMap::Draw -- DIFFER(3419v3464), 209 hunks (differ/ = stale draft regraded, unchanged)
Frame 0x324 v 0x334 and the two side borders (draft line 3 has the idea list). Still uses local C2DVector / C2DBoxF / CThing models:
must be ported to engine/ headers before it is landable. Not worked further this session.
## 0x009A25D0 CStreamingFontBank::Render -- DIFFER(3715v3722), 91 hunks (differ/ = stale draft)
Ported: re-applied FABLE_RETAIL_CSTREAMINGFONTBANK_GLYPHLOOKUP (stale hdr) onto today's engine/CStreamingFontBank.h in hdr/
(asserts pinned). esp-masked structural diff (tools/sde.py): 38 hunks, all register choice / x87 scheduling in the glyph-metric
block 0x009A2BF5..0x009A2EE6 plus local slot order. Not worked further.
## 0x00879900 NCameraModes::CProjectileWeapon::Update -- BLOCKED as cut
The draft includes engine/NCameraModes_CProjectileWeapon.h, which no longer exists in rebuild/include (nor _quarantine, nor the stale
hdr). struct_layouts_egor.tsv has CProjectileWeapon (304 bytes, 17 members); gen_class_headers.py writes only into the repo, which is
read-only for this lane. The coordinator must generate the shared header first. Earlier residue: frame 0x164 v 0x130 + x87 blocks.
## 0x00931390 CAIStateGroup_MinionReactToCriminal::Update -- DIFFER(3836v3863), 262 hunks (differ/ = stale draft)
Stale draft uses local class models (C3DVector, CTCTalk, ...): needs a full port to engine/ headers before any residue work.
Residue per its notes: colouring (result ebx / this edi) and an inline binary-search CTCTalk lookup in the cower branch. Not worked.

## Names
All six leaves are in BOTH ghidra_out/egor_pdb_names.tsv and fablewin_pdb_names.tsv (exact spelling); none of the six addresses is a
relocation target in reloc_bindings.tsv. No made-up names introduced.
Also tried on both twins: `#pragma optimize("y", off)` as the landed Special sibling 0x005A8C66 carries -> far worse (3754v3668 /
3601v3532, ~270 hunks): the twins' TU is ("s", on) only, as the drafts have it.
## Hand-back state
No row matched: payload.json / oracle.tsv deliberately not created (addmatch never ran). differ/ holds the best source of 5 rows;
row 0x00879900 is blocked (no shared class header). grades used this session ~45 (ATTEMPTS.log).
## (successor) 0x00879900 -- still BLOCKED, now measured
Wrote a PDB-based NCameraModes_CProjectileWeapon.h (all 29 Ego_r/FableWin member manglings + hand725 retail layout, sizeof 0x108):
probe/hdr_proposed/engine/ (NOT in hdr/). With it + FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER + FABLE_RETAIL_FIRE_RANGED_WEAPON the
draft (probe/00879900_port1.cpp) still fails on: no engine/CCameraModeDef.h in rebuild/include (the only copy is the Codex
pass-12 model in pending hand724 round3, 22 lines, not trustworthy), CUserProfileManager::GetCameraSensitivity and
CTCTargetingPlayer::GetMaxTargetingDistance (0x00802390) undeclared, CTCHeroRangedAttacks::ProjectileWeaponZLockPressure private.
Coordinator: generate CCameraModeDef.h + NCameraModes_CProjectileWeapon.h as shared headers first.
## (successor) 0x009A25D0 CStreamingFontBank::Render -- DIFFER(3722v3722), 62 hunks, 0 STRUCTURAL (differ/009a25d0.cpp)
Closed this session (each measured): (1) `unsigned short cell = glyph->cachePos;` moved between `weight` and `weight_scale`
(retail 0x009A2BF8 `mov ax,[ecx]` right after the weight lea; the old order put the lea result in eax and cost 7 bytes) ->
3722v3722; (2) EXISTING repo opt-in FABLE_RETAIL_CRENDERSTATEMANAGER_SETRENDERSTATE_DESIRED (FITTED upstream, chunk 2048) fixes the
bookmark-test load order of the SetTexture* expansions at 0x009A2985 / 0x009A29ED (DESIRED+BOOKMARK reintroduces one at 0x009A291F;
EARLY_RETURN made cl spin -> TIMEOUT); (3) inv_half_width declared before inv_half_height (-1 hunk, FITTED).
Residue: only esp slot order of the scalar locals (frame packer). tools/pk.py <src> Render@CStreamingFontBank runs the hand886
phook frame-packer trace (tools/phook.py) and prints every node: name, size, IL weight, offset (F = off + 9728 = final esp offset).
Ours F: cursor_x 0x20, line_x 0x24, x0 0x28, y0 0x2c, v 0x30, x1 0x34, vertex_count 0x38, y1 0x3c, u 0x40, col/block 0x44, new_line 0x48,
stride 0x4c, cursor_y 0x50, verts 0x54, tex_verts 0x58, inv_half_width 0x5c, line_y 0x60, inv_half_height 0x64, extents 0x68.
Retail (instruction-paired, main loop, 4 pushes): vertex_count 0x2c, line_x? 0x30, y1 0x34, x1 0x3c, cursor_y 0x44, col 0x48, 0x4c (read
by the centring floor at 0x009A2683 AND by the y0/x0 term at 0x009A2D4A -- ours reads line_x resp. line_y there), new_line 0x50,
verts 0x58, tex_verts 0x5c, inv_half_height 0x60, inv_half_width 0x64, extents 0x6c; the centring result goes to F 0x44 (0x009A268F).
So retail's IL weights differ (vertex_count > line_x; line_y / cursor_y heavier) and the floor reads/writes different variables
than ours: next step = re-derive the pos.X / pos.Y / line / cursor variable roles from 0x009A2610..0x009A2696 and 0x009A2B41..0x009A2BC4
(which copy the centring floor reads and which it writes), then weight-match with pk.py.
## (successor) 0x009A25D0 CStreamingFontBank::Render -- RELOCATION_MATCH, test PASS, addmatch + read-only vland done (supersedes the DIFFER note above)
src/009a25d0.cpp + test/009a25d0.cpp. What closed it (in order): `cell` read between `weight` and `weight_scale` (length);
the BEHAVIOUR fix found through the frame packer (tools/pk.py): flag 8 centres the block VERTICALLY, `cursor_y = floor(line_y -
extents.Y * 0.5f)` (retail 0x009A2676 `fld [extents+4]`, 0x009A2683 fsubr line_y, 0x009A268F store to cursor_y), the old draft did
`line_x = floor(line_x - extents.X * 0.5f)` (62 -> 8 hunks); verts declared before tex_verts and inv_half_width before
inv_half_height (FITTED, packer ties); local CRGBColour / CShaderEnableView models replaced by engine/CRGBColour.h (`IntValue`) and
engine/CShaderRenderManager.h (CShaderRenderManager::Get().Get{Vertex,Pixel}ShadersEnabled(), FABLE_FORCEINLINE out-of-class
definitions in the row file exactly as the landed row 0x00BA1B00 does). With those, the default SetRenderState text matches (the
DESIRED opt-in is no longer needed). No overlay header change, no flags. wchar_t respelled unsigned short.
Open for the coordinator: the font-manager read is still `(*(CFontManagerView**)((char*)GFGetSystemManager() + 0x84))->BlockOnLoad`
over a local padded view (no header class of that name, so vland does not flag it). The real spelling is
`GFGetSystemManager()->GetFontManager()->GetLoadFontGlyphsAsynchronously()` (Ego_r ?GetLoadFontGlyphsAsynchronously@CFontManager@@QBE_NXZ,
needs FABLE_RETAIL_CSYSTEMMANAGER_FONTMANAGER + FABLE_RETAIL_CFONTMANAGER_ASYNC + a NEW opt-in FABLE_RETAIL_CFONTMANAGER_GETASYNC:
probe/hdr_proposed/engine/CFontManager.h, source probe/009a25d0_realfontmgr.cpp); it compiles to the same instructions except one
SetRenderState bookmark-load scheduling tie at 0x009A291F (`mov ecx,[eax+8]` one slot early), not moved by DESIRED / DESIRED+BOOKMARK /
unnamed or fontManager-local spellings (6 grades). Decide: accept the view model, or adopt the real accessor + find the tie closer.
## (3rd author) twins 0x005A4CDD / 0x005AC10E -- new measurements, still open
- Sell, `long index = 0;` moved before loop 2 (3 placements, p3/s1-s3): 3667v3668 but index loses edi (frame slot, ~90 hunks) and the
  loop-2 head is still `jmp test`: the loop-3 preheader's `index = 0` copy is NOT what blocks the threading.
- Sell guard (both loops) + shared iterator (p3/g1-g3: inside guard, before guard, one iterator for all three loops): 2 hunks, the same
  `mov eax,[ebp+4] ; cmp eax ; mov esi,eax`. Guard + loop 2 as do-while (p3/d1-d3; d2 = `it = begin(); if (it != end()) do {...} while`)
  same residue (d2: 1 hunk, copy moved before je).
- c2prio --blocks on the guard form (g0): begin local #9 = 3 x K4 (Init-call block 32: #9, const 4, &buyableItems temp, this) + 2 = 14,
  loop-2 `it` #26 = 10 (+2 +15 +15 +6, -29 live through the 13 || temporary-destructor blocks). d2 (do-while): #9 4 x K5 = 20, it 13.
- c2prio --trace on Buy: begin local #6 (refs 2) IS checked for 2-reference copy forwarding (0x107820b7) and NOT forwarded (only #82,
  a PeekTC local at line 275, is). `it` then sees cost eax -4 (copy relation to #6 in eax) but eax is not allowed (live across calls) ->
  esi. So the residue = #6 coloured eax before `it`; fixes would be: #6 forwarded, #6 live across a call (callee-saved only -> esi),
  or `it` prio > 20. K(preheader) <= 2 is impossible with the hoisted const 8 / const 1 in every preheader (Sell loop 1 join block K 4).
- Reading: the head test copy is not in the allocator's IL (block 7 holds `#6 = [b] ; it = #6` only: #6 cost 4 = 2 refs, it cost 1);
  the test is duplicated into the head after allocation and kept only when its registers differ from the bottom test.
## (3rd author) 0x00931390 -- measured lead (inline decisions + loop alignment), draft still on local models
- The "inline binary search in the cower branch" is the non-const CVectorMap::LowerBound expanded at ONE GetTC(0x0F) site only
  (retail 0x00932070..0x009320AA, bitset bit tested as `test ch,ch ; jns`), called (0x0040F020, bool kept at [esp+17h]) at the other four
  non-const sites. Adding `#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND` to the stale draft (p9/m1.cpp): 3896v3863, 165 hunks
  (from 262): c2 now expands TWO sites, the first scream site (ours 0x00931567) and the cower site.
- c2 inline trace (tools/inl.sh = C26 inl tools, FUNC='?Update@CAIStateGroup_MinionReactToCriminal'): LowerBound sz 60, threshold 74;
  site 1 (first scream) f=2.3438 score 71 -> expanded (retail: called, so retail's f there is lower, score >= 75); sites 2-5 f=0.5
  score 100 -> called (= retail); cower f=2.6402 score 69 -> expanded (= retail).
- Loop alignment evidence that ours over-weights frequencies: retail pads loop heads only at 0x0093152C (scream do-while 1),
  0x0093158A (its spoken loop), 0x00931B59, 0x00931F7E, 0x009320BE (cower for(;;)); ours also pads the OUTER do-while (0x009313B9,
  retail has no padding at 0x009313B4 = loop top after `mov ebx,3`), the second scream loop (0x0093186C) and four more. Next: find
  the source fact that lowers block frequency in retail (outer loop shape, the order of the first block's && terms / IS_ALIVE
  spelling) with the inline trace as the cheap meter (site-1 score must reach >= 75 while cower stays <= 74), AFTER porting
  the draft to engine/ headers (CAIStateGroup_MinionReactToCriminal.h has FABLE_RETAIL_CAISTATEGROUP_MINIONREACTTOCRIMINAL; style
  row wfcommon/style_small/src/00945060.cpp has the CAIStateGroupBase opt-in set; no shared header exists for CTCBuilding,
  CTCBuildingSize, CTCHouseOwner, CThingFilter_IsNotBeingExclusivelyUsed). Callee sizes change the scores, so port first.
## (fix-up 2026-11-15) 0x009A25D0 CStreamingFontBank::Render -- local CFontManagerView removed, still RELOCATION_MATCH, test PASS, strict vland clean
- Now `if (!Preload(text, GFGetSystemManager()->GetFontManager()->GetLoadFontGlyphsAsynchronously()))` over the shared headers
  (FABLE_RETAIL_CSYSTEMMANAGER_FONTMANAGER + FABLE_RETAIL_CFONTMANAGER_ASYNC + NEW overlay opt-in FABLE_RETAIL_CFONTMANAGER_GETASYNC, hdr/README.md).
  Backup of the pre-fix-up source: tmp/009a25d0.before_fixup.cpp.
- The 0x009A291F tie (SetRenderState bookmark `mov edx,[ebp+2814h]` / `mov ecx,[eax+8]` order, site 4 of the 7 SetTexture* expansions
  0x009A27E9/284F/28B7/291F/2985/29ED/2A55) closed by a natural spelling: the quad-index fill writes `(unsigned short)(i * 4 + n)`
  directly instead of through a named `unsigned short base` (FITTED, chosen by grading; same instructions otherwise).
- Measured map (tools/fx_k.sh, fx_def.sh, fx_sw.sh; ~250 grades): every ADDED record moves the wrong-site set backwards from 291F
  (k initialised unused locals: k=0 291F; 2-3 +28B7; 4-5 +284F; 6 +27E9; >=7 wraps to 2985/29ED/2A55); uninitialised unused ints do
  nothing; the DESIRED/BOOKMARK/ADDSTATE_DIRTY combos only match at k=20 (DBA) / k=22 (DB); dead-arm asserts in GetFontManager /
  GetDisplayManager (FableWin 345-byte getters) also only add. So the fix had to REMOVE a record before the block: dropping `base`
  does it; dropping `col` or the named flag-8 `extents` changes structure (r1/r4: 89-99 hunks). Accessor spellings: unnamed and a
  named bool both match; a named `CFontManager* font_manager` local (0x00494900's idiom) re-opens 291F.

## (wfE9-4, 2026-11-15) 0x00879900 NCameraModes::CProjectileWeapon::Update -- UNBLOCKED: DIFFER(3400v3415), frame 0x130 = retail (differ/00879900.cpp)
- Headers built in hdr/ (all additive; hdr/README.md "wfE9-4" block): NEW engine/CCameraModeDef.h (Ego_r members, retail = Ego_r - 4
  on the 0x3C CParentDefClassBase base; 5 retail reads cited), NEW engine/NCameraModes_CProjectileWeapon.h (the successor's
  probe/hdr_proposed text, unchanged), opt-ins NCAMERAMODES_CBASE_INLINE_LEFTSTICK / _INLINE_SETTERS (SetPosition / SetObservedPos /
  SetFOV inline: FableWin-only names, retail stores in place), CUSERPROFILEMANAGER_CAMERAINPUT (IsCameraFPBowXY/YZInverted,
  GetCameraSensitivity inline + CameraSensitivityNewProfile +0x130 split out of the opaque pad), CTCHERORANGEDATTACKS_ZLOCKPRESSURE_INLINE,
  CTCTARGETINGPLAYER_MAXTARGETINGDISTANCE (decl, 0x00802390). Row also uses the existing CBASE_INLINE_ACCESSORS (GetCameraModeDef inline).
- Row fixes: local CThingFilter_IsNot model dropped (engine/CThingFilter_IsNot.h); `ProjectileWeaponZLockPressure` via the PDB accessor.
- Grade path: v2 3422v3415 167h -> +inline accessors/setters 3403v3415 22 struct hunks (frame 0x164) -> rightPos built IN the targetPos
  slot (`targetPos = targetPos - up*r + side*r`, read from retail 0x00879CA2..0x00879CF5: (tP - up) + sideR stored straight into [74])
  3415v3415 frame 0x158 -> the if-branch test-LOS section (ignore .. the hitThing if/else) in its own `{ }` block: frame 0x130 (s1).
  Blocking the FINAL LOS section instead shrinks the frame below retail (0x110 / 0x11c): retail shares if-branch slots with it only partly.
- C3DVector operator-text opt-ins swept (18 combos of NAMED_RESULT_SUB / SUBTRACT_XY_GROUP / ADD_XY_GROUP / SCALE_*): default text best.
- Open: frame-slot permutation (150 hunks incl. disp8/disp32 length -15); tools/fm.sh <src> prints ours->retail slot votes (efm).
  x87: zoom block 0x0087A2B0..0x0087A340 (retail `fld [S] ; fadd st1`), sway block 0x0087A4AA, `xor ebx,ebx` placement 0x0087A500.
- Global 0x013BAF98 keeps the landed binding name g_CUnprovenCameraInputFlag_013baf98 (reloc_bindings; a placeholder name: NAME_FIXES
  candidate for the coordinator, no PDB name found).

## (wfE9-4) twins 0x005A4CDD / 0x005AC10E -- new levels tried, still open (parked)
- Escape facts: InitialiseBuyableItems body visible above the row (DIAG stub, p4/e1 p5/e1) and vector::erase declaration-only
  (FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE): byte-identical to baseline in both twins.
- const_iterator for the read loops (NormalShopList idiom): identical. Index declaration at function top / split decl: no threading.
- k = 1..7 unused named locals (Buy): identical. 1..6 dead FABLE_RETAIL_ASSERT(pShop): identical. Guard spellings end-first: identical.
- A FORCEINLINE file-local RemoveCards(vector&) helper (DIAG only, p5/rc3): Buy 4 -> 2 hunks (the 0x005ACB5B `mov edi` scheduling hunk
  goes), Sell unchanged; a plain static / inline helper is CALLED (3366 / 3390 bytes), so this is not the retail source.
- (wfE9-4, 0x00879900 slot work) efm (tools/fm.sh <src>; fixed to resolve the path before cd) on s1: 57 of ~75 frame symbols in a different
  slot; retail's bottom word [esp+10h] holds lookDistance (0x0087A1F4) and the if-branch up*r X temp (0x00879C24), [esp+14h] radius.
  Declaration-order probes: lookDistance at function top = 3415v3415 length-exact but frame 0x134 and 68 slot mismatches (p8/t2.cpp);
  lookDistance before position / position after lookDirection: 3400v3415, 151 hunks. Next: replay the packer (wfE3 tools/ph, pk.py
  weights in tmp/pk_s1.txt) against retail's partition before more declaration moves; then the two x87 blocks (zoom 0x0087A2B0:
  retail `fld [pos.Y] ; fadd st1`, ours `fadd [S]`; sway 0x0087A4AA).
## (wfE9-5, 2026-11-15) twins 0x005AC10E / 0x005A4CDD + 0x00879900 -- new levels, still open (parked)
- Buy 0x005AC10E, c2prio --blocks re-read: ALL 116 blocks have w = 1 (no loop-depth weighting in this /Os function, so loop shape cannot
  re-weight `it`). Block 7 (Init call + head) K = 5 {#6 begin local, const 8, const 1, it, this}; #6 = 4*5 = 20, spill 1, refs 2 -> coloured
  first, eax. `it` = +5 (b7) +15 +15 (the two GetXCardDef blocks) +2 +2 +3 (erase / ++ / test) -29 (live through the 13 ||-temp dtor
  blocks) = 13. Arithmetic: `it` can never pass #6 (needs K(b7) <= 2). What makes Sell's loop-2 begin local (#9) follow `it` is NOT
  priority: #9 has spill -2 (it is live from the loop-2 head to the loop-3 init, i.e. live through the loop-2 body blocks) -> `skip
  (not worth a register)` -> `it` gets esi first -> #9's piece follows by the copy relation (cost -1). So the Buy residue needs #6 with
  spill <= 0 (a use of the begin value after the loop on the empty path) or #6 copy-forwarded; Sell loop 1 (#5, join head block, spill 1)
  is the retail eax form, consistent with this rule.
- Buy probes this session (all byte-identical to baseline, 4 hunks, unless said): `!empty()` as begin()!=end() / end()!=begin() /
  !(a.empty()&&b.empty()); size()!=0 3543, size()>0 3541 (loads finish first like retail but keeps sub/sar), end()-begin()!=0 / >0 3543;
  named `bool isCard` condition; ternary erase (177 hunks); reference alias `items`; dead check loop / dead-arm assert after the filter loop
  (6 hunks: shifts a later operand order at 0x005AC864, hunk 1 unchanged); k = 1..7 INITIALISED `long uN = 0;` and 1..3 `float uN = 0.0f;`
  at the function top (the landed E-19 re-key spelling): identical (row insensitive to symbol counts here).
- Sell 0x005A4CDD probes: loop-3 index increment as its own statement (4 hunks), named `entry` pair (identical), const_iterator (identical).
  c2prio --trace on the plain form: #9 spill -2 skipped, `it` #37 esi (costs eax -5), #9 piece #4 forwarded/esi; #97 (end value, lines
  119,126) memory. The colouring is already retail's; the loop-2 head copy is cross-jumped because its exit is not threaded to 0x005A4F19.
- 0x00879900 (new LEVEL: frame-masked structural diff, tools/sde2.py = sd.py masking [esp+..] and reloc [0]): differ/00879900.cpp has only
  30 structural hunks under the 150 slot hunks. probe/00879900_r1_rightpos.cpp (`C3DVector rightPos = targetPos - up*r + side*r;` as a
  NEW local, GetPhi/GetHeight on rightPos) -> 22 structural hunks: the 0x00879B5A and 0x00879CE9..CFC int-copy hunks go (the assignment form
  builds a temp and copies it; retail stores the x87 results straight). Still open in r1: &leftPos push scheduled ~3 tuples later than retail
  0x00879C4B; push ebp 0x00879F43; lookAt Y 0x0087A2B0 (retail `fld [pos.Y] ; fadd st1` keeps v.Y live, ours `fadd [mem]`); zoom block
  0x0087A30B: retail STORES `distance - zoomOffset` back into distance ([esp+18h]) and reloads it three times (distance kept in memory:
  live after or address-taken in retail), ours folds `fsubr [distance]` (tried `distance -= zoomOffset; zoom += lookDirection * distance;`,
  `(distance -= zoomOffset)` inline, explicit C3DVector: all folded the same); sway 0x0087A4AA; `xor ebx,ebx` 0x0087A500.
  Worse: `targetPos -= up*r; targetPos += side*r` (SUBTRACT_ASSIGN opt-in, 77), `+= side*r - up*r` (77), named up / sideR vectors (31/77).
  r1 frame 0x130, length 3370 (more disp32 slots): the slot permutation is the packer (weights tmp/pk_s1.txt) and still needs a replay.
- (wfE9-5, 0x00879900 on top of r1, all 22 structural hunks = no move): lookAt spellings (`v * d + position`, positionXY after lookAt,
  named `look` vector) keep `fadd [pos.Y]` (retail `fld [pos.Y] ; fadd st1 ; ... fstp st0`); k = 1..7 `float uN = 0.0f;` at the top:
  identical; zoom block with a named `float extraZoom = distance - zoomOffset;` (inside / before the if) or a named `extra` vector: c2 still
  forwards the difference on the x87 stack. Retail's distance ([esp+18h]) is NOT address-taken ([esp+14h] is radius, the only lea), so its
  stored-and-reloaded `distance - zoomOffset` (0x0087A30B..0x0087A32B; the product Y/Z also go through frame temps [60h]/[E8h]) is a
  memory-resident value our build forwards: next idea = which IL fact keeps it in memory (an inline callee taking the float by
  const reference -- the C3DVector scale opt-ins were swept globally by wfE9-4 but not per site / with r1).
- (wfE9-5, 0x00879900 on r1) C3DVector SCALE_NAMED_RESULT: 25 structural hunks, 3417 bytes (zoomOffset clamp becomes an int store,
  zoom block still forwarded); SCALE_X_PAREN: identical to r1. r1 stays the best structural base; differ/00879900.cpp is unchanged
  (DIFFER(3400v3415), closer in length, 30 structural hunks). ~19 grades this session on this row.

## (wfE9-5, 2026-11-15) 0x00931390 CAIStateGroup_MinionReactToCriminal::Update -- PORTED to shared headers: DIFFER(3863v3863) under /d2inlT70, equal call sequence (differ/00931390.cpp)
- Port: no local class model left. New overlay opt-ins (hdr/README.md "wfE9-5"): CThingBuilding _LOCKSTATE (IsLockable 0x0082DD90 / IsLocked
  0x0082DE20: Ego_r donor order after LockAllDoors + bodies; the catalog/reloc names HasOccupants / HasBusyOccupants are wrong -> NAME_FIXES),
  CTCCreatureNavigation _DIRECTEDPOSITION, NCreatureAI _AWAREINDIRECTLY, CAIStateGroupBase _PERFORMACTIONTARGETFACE,
  CAIStateGroup_MinionReactToCriminal _UPDATE; NEW header engine/CThingFilter_IsNotBeingExclusivelyUsed.h. The interfaces are the real
  ones: TCI_THING_OWNER (CTCThingOwner::GetObjectList; draft "CTCBuilding"), TCI_PHYSICS (CTCPhysicsBase::GetRadius; draft "CTCBuildingSize"),
  TCI_VOLUME_CONTAINMENT_TRACKER on the hero (GetPThingInside; draft "CTCHouseOwner"), TCI_DOOR filter for the doors list.
  Sort comparator 0x00930D90 = Ego_r ?OrderByHeight2@@YI_NPBVCThing@@0@Z (vector<const CThing*>; draft "CompareOccupants").
- Inline decisions (tools: tmp/00931390_calls.py call-sequence diff, tmp/00931390_inl.sh c2 inline trace): retail calls LowerBound at the
  first scream site (ours sc 71), IS_THING_ALIVE in the marker loop, and CIntelligentPointer<CThing>(CThing*) 0x004A96F0 for the marker temp.
  Measured: c2 for-loop body = 5x entry and the code AFTER a for keeps 5x (MATCHING_PLAYBOOK); retail's marker loop needs body/entry < 1.7:
  only a guard does it -> FITTED `it = begin(); if (it != end()) for (; it != end(); ++it)` (g3; `if (!empty())` g1 = same calls but
  `cmp [eax],eax` + re-read). Then every retail-called site sits at sc 71-72 and the cower LB (expanded in retail) at 69: /d2inlT70 or 71
  gives the exact call sequence; no plain threshold-74 structure found (while(true) outer loop halves everything: too cold).
- FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN needed (retail `marker = ...` is GetPItem + SetPItem, not the implicit copy); CSoundPair() default.
- `if (markers.size() != 0)` (FITTED; retail tests `cmp start,finish` = empty()): with `!markers.empty()` the vector `this` local (#180,
  c2prio) outranks result (108 v 89) and takes ebx; with size() result keeps ebx and length becomes equal.
- Remaining 84 struct hunks: (1) &PHome: ours one candidate (#31, lines 93..136) split eax/esi/memory, retail lea-rematerialised in esi at
  0x009313BF / 0x009316CD and ebp at 0x00931725 / esi 0x009319C2; (2) doors vector zero-init (retail xor esi + xor ebp); (3) x87 order of
  GetNearbyWanderPosition args 0x009317EA; (4) else-if (PHero) branch esi/ebp/ebx permutation 0x00931DA5..0x00931E40; (5) markers test.
- (wfE9-5 cont.) Flag no longer needed: the DOORS loop written `it = begin(); while (it != end()) { ...; ++it; }` (MATCHING_PLAYBOOK:
  for->while over a std::list is code-identical and cools all code after the loop by 5x) cools block 1's scream loop, block 2 and
  block 3 by ~15%: at the default threshold 74 site-1 LB / marker-loop IS_THING_ALIVE / marker IPctor are called and the cower LB
  expanded = retail, 3863v3863, 68 hunks (fg), no /d2inlT. Also FABLE_RETAIL_CTCPHYSICSBASE_RADIUS_ACCESS + `physics->Radius * 0.5f`
  (retail reads +0x34 after PHome's GetPItem, 0x009317F5; the inline GetRadius() is evaluated first) took 74 -> 68 hunks.
- Tried, no move: one-argument GetTC(&talk)/PeekTC(&owner) template forms (identical code), k = 1..7 initialised unused locals at the
  top (identical), filter temporary instead of `not_used` (worse), push_back((*it).Get()) (identical), outer loop for(;;)/break
  (identical), while(true) outer loop (halves all frequencies: too cold), const LowerBound declaration-only (diagnostic: 244 hunks).
- Retail loop padding (alignment) differs: retail pads the marker loop 0x00931B59 and not block 2's spoken loop 0x009318B1 nor
  0x00931C83 / 0x00931D30; ours the reverse -- a remaining frequency difference (block 2 colder, marker loop hotter in retail).
- (wfE9-5 hand-back) differ/00931390.cpp = best (no flags, 68 fg hunks, equal length + calls). Probes in probe/00931390/ (g*/h*/e*/w* variants;
  best_src.cpp = the same source without the grade line). Tools: tmp/00931390_g.sh (grade + call diff), tmp/00931390_calls.py,
  tmp/00931390_inl.sh (inline trace summary), tmp/00931390_sdf.py (non-offset struct hunks). Next ideas for the register residue: c2prio
  --trace on the block-1 condition (#31 &PHome candidate: why its condition piece goes to memory -- retail gives it esi), the TCI_TALK
  creature temp tie {'eax': -1, 'esi': -1} at 0x00931541 / 0x00931868 / 0x00931F35 (retail esi + `add esi,44h`), the doors-vector
  zero init (retail start via a second zero register esi), the frame order of the four `arrived` bools v `doors` (retail doors lowest).
  A stray /home/user/FableDecomp/pch.pch (13:07, root) is NOT from this row's tools as far as known; my one stray 00931390.asm was moved out.
## (wfE9-5) 0x00648820 NPlayerGui::CDrawMiniMap::Draw -- not reopened this session (still DIFFER(3419v3464), ~25 local class models to port first).
## (wfE9-6, 2026-11-15) 0x00931390 -- successor: 25 grades, no move (still DIFFER(3863v3863), 80 sd.py struct hunks; differ/ unchanged)
- c2prio --trace on differ/ (tmp/00931390_prio_at.txt): &PHome is ONE CSE temp #182 (lines 93..147, prio -278: live through the doors /
  move loops) split into ebp #218 (block-2 cond+body = retail ebp, matches), esi #51 (FINISHED arm = retail esi, matches), ecx #79
  (PeekTC remat = retail lea ecx) and THREE per-block 2-ref pieces #47/#46/#45 for the block-1 condition (blocks 6/7/8: copy-forwarded
  into `lea ecx,[edi+1Ch]`); retail keeps one esi piece over the whole condition (0x009313BF..0x009313E5). Same for block 3 (#183,
  173..190: cond per-use ecx, esi piece only from the tracker compare; retail esi from 0x009319C2 to 0x00931AC2, so the tracker map
  takes ebp in retail and esi in ours). #46's allowed set at colouring is acds..p (esi free), so the per-block cut is in piece
  FORMATION (0x1078B51C), not register availability. Not closed: no source fact found that changes piece formation.
- TCI_TALK creature temp: #90 (line 111), #108 (152), #167 (231) choose with costs {eax:-1, esi:-1} -> eax (retail esi at 0x00931541 /
  0x00931868 / 0x00931F35); the cower site #168 has {esi:-1} only -> esi = retail. The extra eax relation is not from the brain load
  (block-2 brain is in ecx and still eax -1); c2prio prints final costs only, the relation source is unknown.
- Tried, byte-identical (80 hunks): block-1 cond as bool / nested if / negated with empty then / `CIntelligentPointer<CThingBuilding>&
  home = PHome` / pointer to PHome; FABLE_RETAIL_STLP_ALLOC_PROXY_REBIND; OrderByHeight2 0x00930D90 body defined above Update (real body
  from 0x00930D90: `return a->GetPos().Z < b->GetPos().Z;`, fld [a+8] / fcomp [b+8] / test ah,5 / jp); doors loop as `for` + /d2inlT70.
  Worse: marker loop `while` without guard (101: IS_THING_ALIVE expanded, operator= called), plain `for` (114), `!markers.empty()` (181).
- Parked: pure piece-formation / relation-tie residue with equal calls and length (WF_RULES "parked on sight" class). Next level if
  reopened: a c2 probe of the split pass (0x1078B51C) printing each piece's block set for #182, to learn why blocks 6-8 are cut apart.
- (wfE9-6 cont.) Also byte-identical: a `CThingAICreature& creature = GetCreature();` local at the talk site, explicit GetTC<CTCTalk>,
  `CTCTalk* talk = 0;` (dead store removed). TU flags: the landed same-TU sibling OnReplace 0x00930E20 carries none. Natural marker
  `for` without the FITTED guard cannot be rescued by an inline flag: IS_THING_ALIVE stays expanded down to /d2inlT60 (243 hunks)
  while site-1 LowerBound flips first -> the guard stays (inline-frequency fact, not a threshold).
## (wfE9-6) 0x00648820 NPlayerGui::CDrawMiniMap::Draw -- surveyed, not ported (DIFFER(3419v3464) unchanged; port plan below)
- The draft still uses ~25 local models AND a stand-in IS_THING_ALIVE (IsThingAliveImpl, auto_inline(off)): both rejected by the
  lander; the port is a header project before any residue work. Shared headers that already carry the needed members:
  CDisplayManager (GetRenderTargetDimensions), CDisplayEngine (IsLetterBoxMode), CFadeInFadeOutBase (GetFadeInOutAlpha), CPlayerGui
  (PeekGuiDef, Draw2DText, ScaleAlpha), CPlayerGuiDef (MiniMapScreenOrigin .. RegionChangeBorderLeftGraphic), CIEngine /
  CEnginePrimitive (SetPersistentPrimitive), NPlayerGui_Graphics.h (GFAnchorToTopRight), CFontBank (GetStringWidth, GFAXToVX),
  NGameText_CDataBank (GetTextBySymbol), CMainGameComponent (PeekCutSceneMainFont), CPlayer (PeekPControlledCreature),
  engine/ThingAlive.h (the real IS_THING_ALIVE), NPlayerGui_CDrawBase.h.
- Missing (new additive opt-ins / headers needed): CDisplayEngine::GetMainWindow 0x00434B50; CMiniMapDisplay
  DrawPreparePrimitivesForEngine 0x0082C060 / GetDrawPosition 0x0082A020 / IsThingAsPosBeingDrawn 0x00829D20 (only in
  CTCInventoryMap.h today); NPlayerGui::GFToScreenScaled 0x00643BFC; CTCRegionDisplay (no header; IsDrawing 0x00639B70);
  CGraphicDataBank slots 23/24 (GetGraphicWidth/Height); a TYPED CDrawMiniMap layout opt-in (today's CDrawMiniMap.h is flat:
  ScreenBox_TLX.. floats, RegionExits[0xc] bytes; the landed siblings SetRegion 0x0064DFDE / ClearPrimitivesIfNecessary 0x00643DE5
  still use local models, so no typed twin exists). Estimate: 10-15 opt-ins before the first honest grade; then the known
  45-byte / side-border residue (draft line 3: FableWin operator+(C2DVector, C2DExtentsF) for the borders + the GFToScreenScaled copy).
## (wfE9-7 successor, 2026-11-15) 0x005A4CDD / 0x005AC10E -- new probes, still open
- Sell: a REAL guard around loop 3 only, with `long index = 0;` INSIDE it (`if (!buyableItems.empty()) { long index = 0; for (...) insert; }`,
  probe/005a4cdd_c_guard3_indexin.cpp) gives retail's threaded flow (loop-2 head `je 0x005A4F19`), residue = the Buy residue
  (`mov eax,[ebp+4] ; cmp eax,[ebp+8] ; mov esi,eax`, 3670v3668). With index OUTSIDE the guard, the guard folds (= plain, 3664).
  Same 3670 residue: begin()!=end() guard, it/it3 declared outside, shared it 2-3, pointer while, &front(), loop 2 as guarded
  do-while / while / extra guard. it3 initialised BEFORE the guard -> plain (no threading). size()!=0 guard 3676 / 5 hunks.
- Model (c2prio on the c-form): threading needs the block reached by the loop-2 empty edge to be a pure test (no defs: `index = 0`
  or `it3 = begin` in it block the threading); then the guard's begin load is CSE'd with loop 3's init (#38, lines 127/129) instead
  of the loop-2 begin local #9, so #9 stays short (spill 1, prio 4*K5 = 20 > it 13) -> eax. The END value (#98) IS carried from the
  loop-2 head into the guard (it is reloaded by the loop-2 bottom test, so available on both paths); begin is not (erase kills it).
  So plain (colouring right, no threading) and c-form (threading, colouring wrong) are mutually exclusive in every spelling tried.
- Landed sibling 0x005A8C66 (Special): its loop begin locals have prio 8 (K = 2: the loop init block follows a je alone) and lose
  to `it` -> `mov esi,[b] ; jmp test`. Buy/Sell need the same small init block; the Init call (`this`) + hoisted flag constants keep K = 5.
- Buy: `&buyableItems.front()` as the iterator start: identical (4 hunks). 25 grades this session.
- (wfE9-7) WHY the begin() local is never copy-forwarded (new tool tools/c2fwd.py = c2prio.py + hooks inside 0x107820b7, log
  tmp/fwck.log): the forwarding check first tests the candidate's defining tuple ([cand+0x3c], opcode at +4) for opcode 1 (a plain
  copy `t = u`). Buy #6's def is opcode 0x26a (an assignment from an expression: `t = [b+28h]`), like 45 of the 80 two-reference
  candidates in Buy; it fails at that first test. Only copies (#82, a PeekTC out-local, opcode 1) are forwarded. So no begin()
  spelling (begin, &front, const_iterator, alias) can be forwarded: the load must be a separate candidate, and only priority
  (K of the init block) decides. Landed sibling 0x005A8C66 shows K = 2 init blocks (after a je, no flag constants: named
  CDefPointer locals, no || temporaries). Buy would need K(init) <= 2 = no call, no `this`, no hoisted flag constants in the block.
- 0x005A4CDD / 0x005AC10E parked again (this session: Sell ~20 grades, Buy 2 grades + traces).
## (wfE9-7 successor, 2026-11-15) 0x00879900 -- structural 22 -> 18 with an EXISTING opt-in, still open (parked)
- probe/00879900_r2_addxy.cpp = r1 + `#define FABLE_RETAIL_C3DVECTOR_ADD_XY_GROUP` (existing shared opt-in, donor018C0B60 text,
  no header change): 18 structural hunks (sde2), 3370 bytes, frame 0x130. The &leftPos push hunk shrinks to a 4-instruction add
  displacement at 0x00879C40/9C60 (retail interleaves the targetPos+up*r adds after `lea ecx ; push ecx`). Single opt-in sweep on
  r1 (structural hunks): NAMED_RESULT_SUB 78, SUBTRACT_XY_GROUP 23, SQUAREDMAGNITUDE_XYZ / _ZYX 22 (identical), MAGNITUDE_XY_THEN_Z 23,
  ADD 22; with ADD_XY_GROUP: + SCALE_X_PAREN 18 (identical), + SCALE_NAMED_RESULT 21 (3417 bytes), + SUBTRACT_XY_GROUP 24,
  + MAGNITUDE_XY_THEN_Z 26.
- Open on r2: scheduling (0x00879C40 adds, `push ebp` 0x00879F43, `xor ebx,ebx` / `mov ecx,esi` around the nearby-vector zero
  stores 0x0087A500..0x0087A51E, `mov eax` 0x0087A477) and three x87 "last use" sites where retail does not consume a dying stack
  value (lookAt Y 0x0087A2B0 `fld [pos.Y] ; fadd st1 ; ... fstp st0`; zoom 0x0087A30B `fld [d] ; fsub st1 ; fstp [d] ; fstp st0 ;
  fld [d]`, i.e. `distance - zoomOffset` stored to distance and reloaded) while ours does, and one where ours does not (sway
  0x0087A4AA: retail Z = `fxch ; fmul [side.Z]` consumes the scale). Reading: these look like scheduling-region effects (a cut
  forces the value to its memory home), not spellings: every local spelling tried is byte-identical.
- No move (all on r1 or r2): std::max clamp (43 hunks: makes distance address-taken, lea), /Op (150), dead-arm assert keeping
  distance live (identical), GFLimit for HeightParam (27) / heightParam (identical), parens on phi / heightParam / Phi += /
  HeightParam += (byte-identical), lookAt `v*d + position`, `zoom = zoom + ...` (24), `distance -= / = (distance - zO)` arm forms,
  operator*= arm (22), `zoomOffset > distance`, sway as named float / named vector / side *= / 0.25*exp / position = position + ...
  (22-25), leftPos/rightPos term orders (18-73), observedPos spellings (18-28). ~50 grades this session.
- (wfE9-7) differ/00879900_r2.cpp (= probe r2, 18 structural hunks) and differ/005a4cdd_threaded.cpp (retail flow, 1 colouring hunk) added as alternative bases; differ/00879900.cpp and differ/005a4cdd.cpp unchanged.
## (wfE9-7 acctC successor, 2026-11-15) 0x00931390 -- 80 -> 68 sd hunks (differ/ updated; old one in probe/00931390x/differ_prev_wfE9-6.cpp)
- NEW SOURCE: Anniversary Xbox 360 PDB (`llvm-pdbutil dump --symbols ... | grep -A52 'S_GPROC32 [size = 84] `CAIStateGroup_MinionReactToCriminal::Update`'`,
  tmp/00931390_anniv_full.txt) lists the stack locals with types: doors, owner, door, ret_bool, ptc_talk | home_pos, ptc_nav, ptc_physics, ret_bool,
  ptc_talk | pmarker, available_markers, not_used, ret_bool | ptc_nav, flee_pos, ret_bool, ptc_talk | ptc_talk. No `away` vector, no `tracker`.
- Closers (each measured): (1) flee direction as a temporary, no `away` local: 80 -> 77 (brain/creature order at 0x00931DEB right);
  (2) block-3 tracker test: retail calls GetPThingInside FIRST and compares `cmp inside,home` (0x00931A53..0x00931A64); VC7.1 evaluates the right
  operand of a built-in != first, so `CThing* inside = tracker->GetPThingInside(); if (inside != PHome.Get() && ...)` in a nested if (FITTED
  spelling, same bytes as an item-first `operator!=(const CBaseObject*, const CIntelligentPointer<T>&)` force-inlined; that operator NOT inlined
  naturally here: sc 75 > 74, f 0.037) -> 76; (3) the marker temp: retail `lea ecx,[temp]` for the assign's GetPItem (0x00931C26), ours used the
  ctor's eax: a NAMED `CIntelligentPointer<CThing> chosen(...)` in the if's braces -> 68 (frame slots of the temp now = retail).
- No effect (byte-identical, md5): dead FABLE_RETAIL_ASSERTs (1/2/4 at top / end / in talk blocks, plain and IS_THING_ALIVE conditions: device d
  does nothing here, and retail has no extra entry store); Anniversary declaration order (home_pos/ptc_nav/ptc_physics, ret_bool at loop top);
  local renames; FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE (both PDBs: reference; kept, identical); !IS_THING_DEAD / spelled-out alive tests;
  STLP_MALLOC_ALLOC_DEALLOCATE / ALLOC_PROXY_REBIND; named index/count locals in the marker pick.
- Markers test: retail is empty() (cmp start,finish + the empty-LIST edge threaded straight past the markers test AND the vector dtor, 0x00931B53 je
  0x00931C55 -- only possible with empty()). With empty() c2 coalesces the size temp into the promoted markers._M_finish local #179 (block 121
  cost 8 = 4 refs, +96) -> prio 92 > result 89 -> #179 takes ebx, result goes to memory, +24 bytes (180 hunks). Retail: finish in memory, reloaded
  into eax, size moved to ebp. Still open: the source fact that keeps the size temp out of #179 (begin()!=end(), ==false, named count/index: same).
- Frame: retail doors (-316) below the 4 ret_bools (efm.py); c2 packs by density refs*1000/size: retail doors needs w >= 12 (ours 11): the extra
  reload of doors._M_start at 0x0093170D (retail `mov esi,[start] ; test ; mov eax,[start] ; push eax`) is that reference -> frame order follows
  the register residue, not a separate fact.
- (wfE9-7 cont.) markers empty() trade-off, measured with c2prio --blocks (tmp/00931390_prio_e6b.txt / _j2b.txt): #179 (markers._M_finish local)
  = +40 (block 110, K 10) - loop live-through penalties + 8 + 96 (block 121 lines 199-201, K 6, cost 8) = 92 v result 89 (for-form + guard,
  probe/00931390x/e6.cpp, 180 hunks). With the marker loop written `while (it != end) { ...; ++it; }` (j2.cpp; guard / for(;cond;) / for(;;) +
  break are byte-identical to it: the `++it` position decides) #179 = 68 and the markers test, threading and dtor are retail's, length 3863, BUT
  frequencies move: marker-loop IS_THING_ALIVE f .335 sc 42 (expanded; retail calls), pick operator= f .019 sc 128 and IS_THING_ALIVE(pmarker)
  sc 83 (called; retail expands) -- no single /d2inlT fixes opposite directions. List-empty guards (`!list.empty()`, begin()!=end()) keep the
  calls but cost 5..32 bytes. So differ/ keeps the FITTED size()!=0 + for-form.
- Ideas not yet tried: an opt-in operator= text without the named `item` local (block 121 K 6 -> 5 would give #179 = 76 < 89 in the for-form;
  needs evidence of the real body: FableWin ??4?$CIntelligentPointer@VCThing@@@@QAEAAV0@ABV0@@Z 0x01B54E40 is not in the signatures file);
  the &PHome / &PHero CSE pieces (block-3 temp #182 prio 10 coloured after the tracker `map` #112 prio 60, which takes esi; retail map = ebp).
- (wfE9-7 cont.) 68 -> 61 sd hunks: NEW overlay opt-in FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_DIRECT (operator= = `SetPItem(other.GetPItem())`,
  no named `item` local; same out-of-line bytes as 0x004A9760) lets the row use retail's `!available_markers.empty()`: block 121 K 6 -> 5, #179
  below result, markers test / threading / dtor all retail. The Get/SetPointer chain (`SetPointer(other.GetPointer())`) behaves like the named
  local (its parameter is a candidate: e6-identical). LESSON: named locals INSIDE inline bodies are c2 candidates that set block K (row-level
  scalar locals are forwarded and change nothing: `thing`, `first`, initialised `spoken`, index/count -> md5-identical).
  Best = differ/00931390.cpp (probe/00931390x/best8.cpp). ~120 grades this session.
- Remaining (61): &PHome pieces (block-1 cond 0x009313BF..E5 + doors start zero `xor esi,esi`; block-3 cond 0x009319C2..0x00931AC2 where the tracker
  map #112 prio 60 takes esi before the block-3 &PHome temp #182 prio 10), doors start reload 0x0093170D (frame: doors density, ret_bool slots
  0x00931675/1956/1C98/2023), TCI_TALK creature eax/esi tie (0x00931541/1868/1F35), flee &PHero piece (0x00931DA5..0x00931E32), loop padding.
## (wfE9-7) 0x00648820 NPlayerGui::CDrawMiniMap::Draw -- port resources found in sibling overlays (re-apply the blocks, do not copy files whole)
- wfA2/hdr/engine/CDrawMiniMap.h FABLE_RETAIL_NPLAYERGUI_CDRAWMINIMAP_CLASS: the real `NPlayerGui::CDrawMiniMap : public CDrawBase` with Draw(info, bool)
  declared (members from +0x28 still opaque: a TYPED nested opt-in is needed: CMiniMapDisplay MiniMap, C2DBoxF ScreenBox, 3 CFadeInFadeOutBase,
  vector<pair<C3DVector,CCharString> > RegionExits). wfA2 also: CFadeInFadeOutBase _FADE (+ _ISOPEN_OUTOFLINE), NPlayerGui_CDrawBase _VSLOTS,
  CDisplayEngine _DORENDER_BODY (typed MainWindow C2DBoxF, LetterBox CLetterBoxModeInfo), new CLetterBoxModeInfo.h.
- wfE11/hdr: NEW engine/CTCRegionDisplay.h, CDisplayEngine _REGIONLOADFADE (IsScreenFadeOutClosing ...), CFadeInFadeOutBase _ISCLOSING.
- agentC38 / agentC39 hdr: CFadeInFadeOutBase with GetFadeInOutAlpha. Repo: CTCInventoryMap.h has the CMiniMapDisplay methods
  (IsThingAsPosBeingDrawn ...), C2DBoxF CORNERS / EXTENTS ctors, CDisplayEngine _ISLETTERBOXMODE, ThingAlive.h _NOINLINE (retail calls
  IS_THING_ALIVE 0x0043DBF0 here), twin idioms: rebuild/src/compiled/00/64/NPlayerGuiCDrawAbilityButtons_DrawButton_00645099.cpp
  (pragma optimize y off / s on, CEnginePrimitive2DSprite + SetPersistentPrimitive through the shared headers).
- Still missing anywhere: CDisplayEngine::GetMainWindow 0x00434B50, NPlayerGui::GFToScreenScaled 0x00643BFC, CGraphicDataBank slots 23/24,
  CMiniMapDisplay::GetDrawPosition 0x0082A020 / DrawPreparePrimitivesForEngine 0x0082C060.
## (wfE9-8, 2026-11-15) 0x00648820 NPlayerGui::CDrawMiniMap::Draw -- PORTED to shared headers; 209 -> 11 sd hunks (differ/00648820.cpp)
- No local class model left; real IS_THING_ALIVE (ThingAlive.h NOINLINE, retail calls 0x0043DBF0). New overlay opt-ins (hdr/README.md):
  NPLAYERGUI_CDRAWMINIMAP_TYPED (typed class), CMINIMAPDISPLAY_DRAWAPI (DrawPreparePrimitivesForEngine / IsThingAsPosBeingDrawn /
  GetDrawPosition), CDISPLAYENGINE_GETMAINWINDOW, NPLAYERGUI_GFTOSCREENSCALED, CTCREGIONDISPLAY_ISDRAWING, CFADEINFADEOUTBASE_ISOPENING,
  CENGINEPRIMITIVE2DSPRITE_BOX_CTOR_BODY; copied verbatim from wfE11: CDisplayEngine REGIONLOADFADE, CTCRegionDisplay.h, CFadeInFadeOutBase ISCLOSING.
- Param names from the Anniversary PDB (S_REGREL32 of NPlayerGui::CDrawMiniMap::Draw: interpolation, drawing_hud, phero, ptc_region_display,
  map_box, text, text_pos); `forceDraw` of the draft was descriptive.
- What closed hunks (measured): (1) the clip-box sprite ctor 0x0041BEB0 with a VISIBLE inline body (new opt-in BOX_CTOR_BODY, called out of
  line all the same under /Os): the six tiles' x87 / push interleave (58 -> 11; the draft's auto_inline(off) stand-in did the same);
  (2) borders as `C2DExtentsF size(w, h)` + box from pos + `left = GFToScreenScaled(left)` (retail movsd copy from the result temp);
  (3) border height `(float)Bank->GetGraphicMaxHeight(L) * (dims.Y * (1/480))` (retail calls GetRenderTargetDimensions first).
- Open: frame 0x324 v 0x334: retail's two GFToScreenScaled result temporaries have their own slots (-0x14 left, -0x24 right), ours share
  one; border box x87 (retail `fst [ebp+40h]` spills pos.Y and reloads it for BRY, 0x00649163 / 0x006492BB); SIB base/index order of the
  RegionExits element address (0x006493B5 / 0x0064941B, `lea esi,[ecx+eax+0Ch]`). Tried: C2DBoxF(C2DVector, C2DExtentsF) inline or
  out of line (called, not expanded: worse), float locals (p5 120 / p10 13 / p12 14 hunks), C2DVector copy (= p9), named scaled temp (13, frame 0x33c).
## (wfE9-8, 2026-11-15) twins 0x005A4CDD / 0x005AC10E -- ROOT CAUSE FOUND: escape analysis of the local `buyableItems`
- DIAG (tmp/005ac10e_d5.cpp, tmp/005a4cdd_d5.cpp): with vector::erase's body VISIBLE but called (out-of-class definition under
  auto_inline(off)), InitialiseBuyableItems' body visible above the row (stub: one items.push_back) and push_back visible+noinline
  (FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE instead of _OUTOFLINE), c2 knows no callee captures &buyableItems, so calls after the
  loop do not kill its words: the begin() value at the loop head is reused on the empty path (spill <= 0 -> `it` coloured first, head
  copy cross-jumped). Sell plain differ -> RELOCATION_MATCH; Buy -> 3532v3532, 1 hunk left (0x005ACB58 `mov esi,[74h]` / `mov edi,[4Ch]`
  reload order at a join). Any one of the three bodies declaration-only (or a capturing erase) = old 4 hunks.
- Legit version needs: (1) new overlay opt-in FABLE_RETAIL_STLP_VECTOR_ERASE_NOINLINE (default erase text, __declspec(noinline), mirror
  of PUSHBACK_NOINLINE); (2) the REAL InitialiseBuyableItems body (landed 0x005A30A1, local models) ported onto shared headers above the row.
- (wfE9-8 cont.) BOX_CTOR_BODY text PROVEN: the inline body compiles to retail 0x0041BEB0 = MATCH when C2DBoxF has its user copy ctor
  (FABLE_RETAIL_C2DBOXF_COPY; probe/00648820/ctorchk2.cpp); without it DIFFER(175v171). The row defines both (no byte change in the row).
- Frame (the real blocker; sd.py masks it): tools probe/00648820/fmap.py (retail->ours slot map over aligned instructions) and tools/pk.py
  (packer nodes, tmp/00648820_pk15.txt). Retail: sprites -0x2c4..-0x24 in slot order R, TR, L, BL, BR, TL, BM, TM (ours R, TL, L, BL, BR, TM,
  BM, TR: the top row rotated; sprite NAMES do not move them, sprites as temporaries change the code: 3533 bytes); GFToScreenScaled temps
  -0x24 / -0x14 alone (ours: one 16-byte slot shared with the C2DExtentsI temps); main window -4, ScreenBox temp + right +0x10, box +0x20,
  def spill +0x38, C2DExtentsI temps +0x3c, this +0x44, tile AND left share +0x48, alpha +0x64; loop offset in the interpolation home +0x78.
  Tried without effect on the frame: tiles in their own { } block, const-ref bound GFToScreenScaled results (temps separate but frame 0x33c
  and `lea esi` copies), left built in `tile` (63 hunks), named scaled temps (frame 0x33c). k = 1..7 unused locals (top / loop body) and
  index type long / int / unsigned int / begin()+i: SIB unchanged. Border spellings b_v1..v11: best is the current one (p15).
- (wfE9-8 frame model, measured) Address-escaping named locals interfere by SCOPE in the packer: borders alone in a `{ }` block (probe
  p24.cpp) makes `tile` and `left` share one slot as retail +0x48 (frame 0x314, 12 sd hunks); tiles AND borders each in a block (p23) lets the
  tile and border sprites share too (frame 0x26c; retail keeps all 8 sprites apart, so the sprites are in one scope). The two GFToScreenScaled
  result temporaries stay statement-scoped (shared with each other and the C2DExtentsI temps) in every natural spelling; binding them to
  const references separates them (p25: frame 0x32c, but `lea esi` copies instead of retail's `mov esi,eax`). Next idea: find the construct
  that gives the GFToScreenScaled temps block lifetime while the copy still reads eax (retail 0x00649189 / 0x006492DB), on top of p24.
- Grades this session on 0x00648820: ~75 (port + residue). differ/00648820.cpp = p15 (frame 0x324); p24 is the packer-closer variant.
- (wfE9-8) Border operand order (x + w v w + x, both coordinates) is byte-identical (c2 normalises it); C2DBoxF has no operator= in either PDB (implicit only), so a user operator= cannot be what gives the GFToScreenScaled temps block lifetime. 0x00931390 not reopened this session.
## (wfE9-8) 0x005A4CDD CTCInventoryTrade::ConstructActualSellInventoryPC -- RELOCATION_MATCH, test PASS (mutants fail), strict vland clean
- What made it match: the escape facts above, done for real: (1) the same-TU InitialiseBuyableItems body above the row = the landed
  0x005A30A1 logic written over the shared headers (BuyableItems -> SpecialItemsToDelete, TotalBuyable -> ItemBuyCount, Mode ->
  CategoryIdent, GetShopThing -> GetBuildingThing, CTCInventoryItem::IsBuyable); (2) FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE instead of
  _OUTOFLINE; (3) NEW overlay opt-in FABLE_RETAIL_STLP_VECTOR_ERASE_NOINLINE instead of ERASE_OUTOFLINE. Plain source otherwise unchanged
  (the earlier differ/ draft). No flags, no devices, nothing FITTED.
- Also fixed while landing-proofing: local `class CThingPlayerCreature : public CThing {}` -> engine/CThingPlayerCreature.h (+ CCLASS,
  CINTELLIGENTPOINTER, CTHINGPHYSICAL, CTHINGGAMEOBJECT, CTHINGCREATUREBASE, as row 0x005BE284); subtitle globals g_CCharString_013b8b70/74/78/94 ->
  the bound `namespace { extern CCharString WEAPONS_SUBTITLE; }` .. CATEGORY_CHILD_SELL_LIST_STRING (BuyItem 0x005AF64E spelling);
  FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (agentNF name fix, vland RELOC_NAME otherwise). Init needs the extra defines CTCSHOP_GETSTOCKLIST,
  CTCSHOP_STOCKQUERIES, DEF_CLASS_BASE, CDEFINITIONMANAGER, CGAMEDEFINITIONMANAGER, CHEROTITLEDEF, CTCINVENTORYITEM_KINDS, CDELETION,
  CRESOURCELIST and the new CTCINVENTORYTRADE_ADDITEMOFTYPE (hdr/README.md).
- Name evidence found: CInventoryItemDef +0x5C IsSellable / +0x5D IsBuyable (struct_layouts_egor) and Ego_r donor order IsSellable 0x0055AB00 <
  IsBuyable 0x0055AB20: retail 0x005D9B30 (reads +0x5C) = CTCInventoryItem::IsSellable, 0x005D9B50 (+0x5D) = CTCInventoryItem::IsBuyable.
  The shared header (KINDS) and BuyItem 0x005AF64E bind IsBuyable to 0x005D9B30; the landed Init binds ?IsBuyable@@YI_NJ@Z to 0x005D9B50 (NAME_FIXES).
## (wfE9-8) 0x005AC10E ConstructActualBuyInventoryPC -- DIFFER(3532v3532), 1 hunk (differ/005ac10e.cpp; previous draft kept in tmp/005ac10e_differ_prev_wfE9-7.cpp)
- Escape facts (same as Sell) + shared CThingPlayerCreature.h + bound subtitle names + RELEASEDATA: hunks 1 (loop head) and 2 (empty() operand
  order) close; left: reload order at the subtitle join 0x005ACB58 (retail `mov esi,[ebp+74h] ; mov edi,[ebp+4Ch]` = entries then pFrame; ours
  pFrame first). Retail Sell orders its twin join 0x005A57A0 pFrame-first (= ours), so the fact is Buy-specific.
- c2prio: pFrame #30 prio 197 (block 28 K15 incl. two buyableItems temps #6 / #84 that the escape facts keep live; Sell's twin block K13),
  entries #51 prio 177; pieces: pFrame edi #63 (split pass 2, id from the LIFO free list), entries esi #125 (pass 1). Reload order rule unknown
  (colouring order and piece-id order both predict ours).
- Tried, no move: k = 1..7 initialised unused locals at the top / before xString / in the entries loop; 1..4 dead FABLE_RETAIL_ASSERT(pFrame);
  forceinline RemoveCards helper (it closed this hunk on the OLD base, not now); subtitle decl order; pSubtitle/pMenu order. Worse: pBranch local
  (Sell idiom, 3535), one shared EItem_Menu_Type local (95 hunks: frame), GetMenuBranchFromDefIndexPC body visible (6 hunks; also breaks Sell,
  so 0x0059C0C6 is NOT visible in the retail TU: the CTCInventoryTrade object boundary lies between 0x0059C0C6 and 0x005A30A1).
- Untested lead: AddBuyItemToListPC 0x005A6CA1 lies between Sell and Buy (visible to Buy only, if same obj): its body (not landed, ~1.7 KB in
  Ego_r) could change c2's mod/ref facts in Buy only. Consistency check done: the landed Special 0x005A8C66 still RELOCATION_MATCHes with the
  escape facts (tmp/sp1.cpp).
- (wfE9-8 Buy, more no-move probes on the 0x005ACB58 reload order) AddBuyItemToListPC body visible (non-pure DIAG stub; it lies between Sell
  and Buy), map body visibility opt-ins for branchCounts (MAP_ORIGINAL_INSERT_BODY, TREE_INSERT_UNIQUE_HINT_BODY, TREE_MERASE_NOINLINE):
  all byte-identical (1 hunk). Regraded 009a25d0 after the overlay additions: still RELOCATION_MATCH.
## (wfE9-8) 0x00879900 NCameraModes::CProjectileWeapon::Update -- escape lever measured, no gain (parked; differ/ unchanged)
- The callees GetPosFromParams 0x00878BE0 / GetLookDirectionFromParams 0x00878DB0 / GetHeightParamForPostionWithPhi 0x00879070 /
  GetHeightParamForTargetedThingWithPhi 0x00879560 / GetPhiForPosition 0x00879680 / GetPhiForTargetedThing 0x008798C0 are CProjectileWeapon
  methods laid out just before Update (same TU; four landed: 00878be0, 00878db0, 00879560, 008798c0), so retail's c2 saw their bodies.
  DIAG (tmp/cam_d2..d4.cpp, auto_inline(off) stubs that call an unknown external, i.e. non-capturing but not pure): structural hunks
  (tools/sde2.py) 18 -> 20 (all six) / 20 (the two position-taking ones) / 18 (the two param ones); full hunks 155 -> 151..154. Pure stubs
  (cam_d1): 85 struct hunks (c2 then drops reloads), so real bodies would be needed for a fair test -- not worth it on this evidence.
- C3DVector::operator*(float) is by value in Ego_r (??DC3DVector@@QBE?BV0@M@Z), so the memory-resident `distance` in the zoom block is not a
  const-reference parameter effect.
## (wfE9-8) hand-back summary for the coordinator
- 0x005A4CDD matched (RELOCATION_MATCH, test PASS, addmatch + strict vland clean). New overlay opt-ins: FABLE_RETAIL_STLP_VECTOR_ERASE_NOINLINE
  (stlport/fable_containers.h) and FABLE_RETAIL_CTCINVENTORYTRADE_ADDITEMOFTYPE (engine/CTCInventoryTrade.h, newly copied into hdr/). The row
  carries the same-TU body of InitialiseBuyableItems (landed 0x005A30A1 logic over shared headers): the lander must accept a second function
  definition in the row object (precedent: BuyItem 0x005AF64E carries GetDef / GetSubDef bodies).
- Name fixes (NAME_FIXES.tsv): 0x005D9B30 = CTCInventoryItem::IsSellable (header KINDS comment + BuyItem bind it as IsBuyable); 0x005D9B50 =
  CTCInventoryItem::IsBuyable (landed Init binds ?IsBuyable@@YI_NJ@Z). The landed Init's local names BuyableItems / TotalBuyable / Mode are
  Ego_r SpecialItemsToDelete / ItemBuyCount / CategoryIdent.
- Landed twins Wanted 0x005A5B31 / Special 0x005A8C66 / BuyItem 0x005AF64E still model CThingPlayerCreature locally; engine/CThingPlayerCreature.h
  works in this TU with CCLASS + CINTELLIGENTPOINTER + CTHINGPHYSICAL + CTHINGGAMEOBJECT + CTHINGCREATUREBASE (Sell matched that way).
## (wfE9-9 successor, 2026-11-15) 0x005AC10E -- 0x005ACB58 reload-order hunk, new measurements (still 1 hunk; parked)
- Model (c2prio --trace, tmp/005ac10e_prio_s1.txt): at the join both reloads are split pieces: pFrame #30 -> pass-1 piece #90 -> re-split in
  pass 2 -> edi piece #63 (the freed id of the `||` conditional-temporary flags local #63); entries #51 -> pass-1 piece #125 (a NEW id: the
  pass-1 free list holds exactly 17 ids and 18 pieces are cut before #51, so #50 pMenu gets 124 and entries 125). Ours emits ascending
  piece id (63 before 125); Sell matched with pFrame #31 < entries #141 (same rule). Retail Buy therefore needs either entries recycled in
  pass 1 (>= 2 more freed records before pass 1, or 2 fewer pass-1 pieces before #51) or pFrame not re-split in pass 2 (if the rule is
  "later split first"). The two rules are not yet discriminated.
- Record ids: named locals from #38 up are numbered in source order; #1..#37 are a recycled, scrambled pool. Functions compiled BEFORE the
  row in the TU change that pool (DIAG tmp/005ac10e_d1.cpp: two tiny functions above the row move pFrame #30 -> #31, piece #63 -> #53;
  tmp/005ac10e_dg{10..200}.cpp: a preceding function with N locals moves pFrame 27..31) but never entries (#51 / piece #125): no flip.
  Retail TU order before the row (Ego_r donor order + retail layout): ConstructTradeDescriptionClothing 0x005AAF28 (landed), then
  OpenActualSellInventoryPC 0x005AC0EE (DeleteComponents 0x0059C8AE + tail jump to Sell) and OpenActualWantedInventoryPC 0x005AC0FE
  (same, tail jump to Wanted). Not ported (the tiny-function DIAG says they would not flip it).
- Tried this session, no move (all 1 hunk unless noted): 2-ref named locals pBuyEntry / pBuilding (not forwarded: copy forwarding needs an
  opcode-1 copy def), unused named FindChild result, entries renamed (ids are not name-hashed), const entries, int i, entryName before
  subtitleText (8 hunks), entries declared before pMenu (212 hunks), pSubtitle after entries (5), card filter as && form (6) / bool local /
  defIndex local (91), TotNum zeroing as three statements (1 / 4 / 3), non-capturing visible stubs for ConstructSubtitlesPC 0x0059E4D7 /
  RefreshEntriesAmountsAndAvailabilitiesPC 0x0059FAFE / ResetTradeLists 0x0059D9AD (DIAG; possible same-TU callees between 0x0059C0C6 and
  0x005A30A1), k = 2 unused locals (not candidates at all: they only shift ids). ~30 grades + 14 c2prio traces.
- Name note: rebuild/oracles/reloc_bindings.tsv binds 0x005AC10E as ?ConstructActualBuyInventoryPC@CTCInventoryTrade@@QAEXXZ (public);
  Ego_r + FableWin both spell it AAEXXZ (private), as the row's Leaf does.
- (wfE9-9 Buy, cont.) NotInStock loop `long defIndex = *n` (60 hunks), named `CList* pList = BuyListsByCategory.find(branch)->second` in the
  NotInStock AddBuyItemToListPC call (1 hunk; ids unchanged: the pass-1 free list is still the same 17 ids, entries #125) / in the buyable
  call (4 hunks). The pass-1 free list [108, 79, 117, 111, 110, 109, 107, 106, 105, 98, 95, 93, 92, 91, 90, 72, x] survived every variant
  traced this session (14 traces): whatever retail differs in is not a local spelling of the statements around the loops. Parked.
## (wfE9-9 successor) 0x00879900 -- zoom-arm measurements (parked; differ/ unchanged)
- Retail zoom arm 0x0087A30B: `fld [d] ; fsub st1 ; fstp [d] ; fstp st0 ; fld [d] ; fmul [look.X] ; fld [look.Y] ; fmul [d] ; ...` = the
  difference stored into distance's slot [esp+18h] (the slot is re-used for zoomParameter = 1.0f at 0x0087A36B, so the store is dead in
  retail too: either the inlined operator*(float) parameter homed in the shared slot or a dead store c2 kept); no lea of the slot anywhere.
  Ours: `fsubr [d]` (zoomOffset consumed) and the product kept on the x87 stack.
- On differ/00879900_r2.cpp (tools: tmp/00879900_sd.sh = q.py + sde2 header line): `distance -= zoomOffset; zoom += lookDirection * distance;`,
  `distance = distance - zoomOffset; zoom += ...`, named float / named C3DVector temp, C3DVector(lookDirection) copy: all byte-identical to
  r2 (3370, 18 struct hunks). `zoom = zoom + lookDirection * ...` (either distance form): 3403 bytes, 868 v 867 instructions, but 21 struct
  hunks (the + result is copied into zoom with integer moves; retail adds in place with fadd/fstp). `distance * lookDirection`: no such
  operator (SRC_FAIL). ~12 grades. Still open: frame-slot permutation (the 15..45-byte length gap is disp8/disp32 encodings) + 18 struct hunks.
## (wfE9-9, 2026-11-15) 0x00648820 NPlayerGui::CDrawMiniMap::Draw -- frame SOLVED; DIFFER(3464v3464), only 2 non-reloc lines left (differ/00648820.cpp = probe/00648820/e/p4.cpp)
- Tools (new): tools/pk2.py (phook packer dump incl. presort + interference), tools/pkjoin.py (slot-joining model: params first-fit with
  size <= param size, then latest slot backwards with size <= 2*slot and no interference; reproduces c2 exactly on 4 sources),
  tools/pkfull.py (join + K&R qsort by density + downward offsets from ebp+0x70; reproduces offsets exactly), tools/pkv.sh (slot view),
  tools/g8.sh / tools/nr8.sh (grade + frame + non-reloc diff count). The sprite permutation was a qsort tie: one extra 16-byte slot fixes it.
- What closed the frame (each measured, all source facts): (1) same-TU GFToScreenScaled body above the row (landed 0x00643BFC logic on
  shared headers; retail TU also holds GetTRPos 0x00643970 + SetTLPos/SetWidth/SetHeight COMDATs 0x00643930..60); (2) tiles written with
  C2DBoxF::SetTLPos / SetWidth / SetHeight (FableWin.pdb emits exactly these three right after CDrawMiniMap::Draw = first used there; new
  overlay opt-in FABLE_RETAIL_C2DBOXF_SETTERS): the next tile is SetTLPos(C2DVector(tile.TLX + tile.GetWidth(), tile.TLY)), row 2
  SetTLPos(C2DVector(def.RegionChangeTLOffset.X, tile.BRY)) -- replaces the FITTED xTM..xBR floats; frame 0x324 -> 0x334, all 8 sprite
  slots + both GFToScreenScaled temps = retail; (3) borders `C2DVector left_pos = def.RegionChangeBorderLeftOffset; C2DBoxF left(left_pos,
  left_size);` with the plain-inline FABLE_RETAIL_C2DBOXF_EXTENTS ctor (expanded at the borders, still called at ScreenBox 0x0041A7E0, no
  device): closes the border x87 pos.Y spill hunks (0x00649163 / 0x006492BB) -- retail [ebp+40h] is left_pos.Y; (4) `C2DBoxF centred(
  centre_tl, C2DExtentsF(size, size)); C2DBoxF box = centred;` with the fade reading centred.* (replaces centre_br): every frame slot = retail.
- Open: SIB base/index of &RegionExits[i].second (0x006493B5 / 0x0064941B: retail [ecx+eax+0Ch] = offset IV base, ours begin base).
  Tried (no move): k = 1..7 unused int locals (top / loop body), dead FABLE_RETAIL_ASSERT x1..5 (top / loop / end; >=6 flips inlining:
  3488 bytes), unsigned int / split declaration / i++ / size() > i, begin()[i], (begin()+i)->, VECTOR_INDEX_START (80 diffs),
  no named symbol ref (worse).
- (wfE9-9 cont.) SIB residue, more tries (all byte-identical, still 2 lines): PeekTC one-arg; C2DBoxF box(centred); GetTLX()/GetTLY()/GetBRY()
  accessors in the tile SetTLPos args; named pair refs (exit / exit2) or (&RegionExits[i])->second or a CCharString* for symbol/name;
  dead asserts with class-temporary conditions (text != L"", symbol == "X" || ..., GetLength() > 0, PlayerGui) x1/x3 at loop top / after
  name / before the IsThingAsPosBeingDrawn test / after font (x3 with temps flips inlining: 3488 bytes). c2prio (tmp/00648820_prio.txt):
  the offset IV is candidate #138 (memory home [ebp+78h]); the per-use begin loads are forwarded tuples, not candidates. Retail has
  (IV, begin) for the two `.second` leas but (begin, IV) for the two `.first` adds; ours (begin, IV) for all four. Not a symbol-count tie
  that k locals / dead asserts move. differ/ updated (header comment rewritten: EVIDENCED list; box/centred names). ~45 grades this session.
