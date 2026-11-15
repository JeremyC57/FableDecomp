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
