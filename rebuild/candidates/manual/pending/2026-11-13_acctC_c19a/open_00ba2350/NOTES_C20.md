# agentC20 NOTES (continues prev_C19/NOTES.md). Tools: tools/ (C19's repointed) + tools/win.py (aligned retail|ours window), tools/od.py (obj -> tmp/o_ for win.py),
# tools/rep.py (exact string replace -> variant file), tools/g2.sh <abs srcs> (grade row 1: size, diff count, first prologue mov).
## 0x00BA2350 findings (agentC20)
- FableWin.exe / Ego_d are NOT on this machine: only ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv (size + instruction count per name). Row at /Od(/EHsc) = 1166i v FW 1488i
  (+322i ~ 3-4 ASSERTs at ~85i each + text). One FW assert ~ 85-90 instructions (GFDebugPDynamicCast 9 -> 99, AddStateToUpdateList 23 -> 108).
- FW helper texts (by /Od size): C4DVector(const CRGBFloatColour&) 12i = { InitialiseFromColour(colour); } (23i member copy); CRGBFloatColour(MMMM) 22i/52b = { Initialise(r,g,b,a); }.
- RETAIL block 2 (0x00BA23D9..E8): `mov eax,[sub+4]; test al,8; mov al,[sub+8]; je` = the flags temp takes eax, so alpha is NOT live at the flags test: the alpha load
  is in BOTH arms of the `if (flags & 8)` and c2 head-merges identical loads above the branch (measured, w/v6.cpp: exact retail shape of block 2). A single
  `alpha = PeekAlpha()` before the if gives the flags temp ebx/edx (ours) -- the source has the PeekAlpha() read inside the arms.
- Release-form asserts (fable_assert.h dead arms) in the prologue: no effect on priorities (b1..b5), only small ordering changes; keep for the final tie-breaks.
- Retail register picture: esi primitive, edi mesh_primitive (dead in the combined path: edi reused for material / alpha_value temps), ebx sub_primitive then `lights` piece,
  ebp = a piece of `this` from entry (mov ebp,ecx; mov [esp+14],ebp) reaching the non-combined passes (lea eax,[ebp+2E8]); G tint byte = split (home [esp+10], piece dl).
- c2 tuple count of the row = 4191 (tools/c2/cnt_host.py): regime 0 (global optimizer on; head merging / CSE active).
- Trace (tmp/tr.prio): `dp3_bump` (#126, byte candidate live across calls, allowed only ebx, prio 59) is coloured just before sub; sub takes edi (ebx cost 2300), mesh is left with ebp.
- FORCED mesh=70 on w/f1.cpp (alpha read in both arms): idx-equal 799/2419, masked lcs 2302, frame-masked diff only 332 lines / 81 hunks (tmp/dd_x.txt). So
  head-merged alpha + mesh coloured before sub is retail's shape; what remains is the natural reason for prio(mesh) > prio(sub) (ours 49 v 59/60) and the frame slots.
- Static shader getters (FableWin GetPixelShaderXxx, new opt-in FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_STATICMESHSHADERS, tools/mkget.py): code-neutral so far (d 1374 -> 1360).
- PRIORITY ARITHMETIC (c2prio --blocks, tools/blk.py): K(b1)=16 = {sub, primitive, graphic_bank, #142 bank temp, const 1, tint byte temps #145 #146 #147 #332, tinted,
  const 0xffffff, mesh, flags, pass, const 2, this}. sub = 4*K1 + S, mesh = 2*K1 + 17, S = -5 (a0) / -4 (f1). With Kp = K of the block that holds
  `primitive = sub_primitive.GetParent()` and the rest unchanged: mesh - sub = 21 - 2*Kp, so retail's order (mesh first) needs Kp <= 10 (ours 16).
  MEASURED: any split of block 1 after the GetParent statement flips the order (w/t2.cpp branchy `tinted`, w/u1.cpp branchy block_on_load: mesh edi, but real branches in
  the code = wrong bytes). So retail's IL has block 1 split (or 6 fewer candidates next to GetParent) at priority time, with no branch left in the final code.
- Things that do NOT split / add references (byte-identical objects): dead-arm asserts, `__assume(x)`, `if (!x) {}`, `if (!x) __assume(0)`, `if (!x) EmptyInline()`,
  `(void)(x != 0)`, ternary spellings of tinted / block_on_load, a local copy of &sub_primitive (copy-propagated; w/k1.cpp).
- The alpha head-merge happens AFTER allocation (at priority time the two PeekAlpha reads are still in the arms: sub cost 2 in b3 and b7), i.e. a machine-level
  prefix merge of both successors, placed between the compare and the jcc -- exactly where retail's `mov edi,[ebx+18]` and `mov [esp+34],109h` sit in block 1.
- Constraint check: retail needs colouring order mesh (edi) before sub (ebx); bytes B/G in ecx/edx either side. No block split is possible (c2 cleans every dead
  branch / once-through loop before the priority pass; w/L1, m1..m6), named references are forwarded (w/q1), cloth-style CRGBColour(R,G,B,alpha) is byte-identical (w/r1).
- Second, independent inversion INSIDE the SetPixelShaderConstant expansion (0x00BA2475..B7): retail colours T (PSConstTable data ptr) eax, the inline `this` ecx
  (`mov ecx,eax`), the member-copy temps edx; ours: `this` temp #160 prio 88 eax, copy temp #155 prio 87 (ONE candidate, 11 refs) ecx, T #340 prio 40 edx.
  The cloth row (same helper text) has ours' pattern in retail, so here the helper IL differs: lead = FableWin texts C4DVector(const CRGBFloatColour&) = InitialiseFromColour (12i).
- FableWin helper texts tried as new opt-ins (C4DVector via InitialiseFromColour, CRGBFloatColour(MMMM) via Initialise): byte-identical objects (IL-neutral after the optimizer).
- Overlay regrade after agentC20's header additions: 0x00B9E5B0 / 0x00BB30A0 / 0x00BB3E80 RELOCATION_MATCH; TU rows (w/tu.cpp = C19's TU; w/tu2.cpp = TU with the new
  0x00BA2350 body) 0x00BA5260 / 0x00BA50A0 / 0x00B9D670 / 0x00B9E5B0 / 0x00B9E750 RELOCATION_MATCH with either body.
- differ/00ba2350.cpp = w/d1.cpp (v6 + getters): unforced DIFFER(9376v9632); forced mesh=70: 9412 bytes, masked lcs 2310/2419, frame-masked diff 309 lines
  (remaining classes: SetRenderState bookmark operand swaps -> expect release asserts as in the cloth row; PS-constant T/this registers; the two outline tails
  that retail does not cross-jump (116 bytes; temps differ: dp3 temp edx v eax); frame slots).
- Named locals in the non-combined passes DO move mesh's priority (w/e2.cpp: the three `effect` locals written inline -> mesh 45, 9560 bytes): a surviving named local adds
  +2 to mesh per block it shares with a mesh reference and -1 per block mesh only lives through. Not enough by itself (best estimate +3..5), but it is the only
  code-neutral lever found that moves mesh at all. Explicit CRGBColour(R,G,B,A) tint copy (w/e1), lights as a pointer (w/e3), `(flags & 8) != 0` (w/z1): no change (K1 16, K2 7).
- Last outline pass (retail 0x00BA45CF..E2): retail hoists `mov eax,[esi+58]; push eax; push ecx; mov ecx,ebp` above the `jne` (both arms identical), ours (forced) keeps
  two copies because the else arm's Scale temp rotates to edx (then arm eax). `effect` is a named local there in retail too (loaded into ecx while pass is in eax).
  => retail's expression-temporary rotation pointer is in the same state at both arms; ours has advanced by one. Same class as the two outline tails retail does
  NOT cross-jump (0x00BA42D0 dp3 temp edx / 0x00BA464F eax): the temp rotation state differs by one between ours and retail in the passes (c2prio --rotation can trace it).
- NOT YET TRIED (next agent): (1) c2prio --rotation on w/d1.cpp forced, to find the single extra / missing expression temporary before the last pass (it may be the
  same invisible IL difference that moves the priorities); (2) named-local sets per pass (effect / scale / dp3) with priority readouts, combined with sub-lowering
  locals in the stipple arm; (3) the four ~FableWin asserts as dead arms under the forced build (SetRenderState bookmark operand swaps at 0x00BA253D, 0x00BA413C);
  (4) GetRGBColour's FableWin text (54i/165b; none of 70 generated spellings in probe/od/p2.cpp hits it: 4x+2 with x = 11 instructions per channel) and the
  CRGBFloatColour(const CRGBColour&) 46i/141b text (closest: `*this = CRGBFloatColour(...)` 47i/142b); (5) a faster priority oracle (break only at c2 SORTED).
- FIRST frame-masked divergence of the forced build = the PS-constant branch 0x00BA248D. Cause (c2prio trace tmp/trd1.prio): the GPShaderRenderManager global-candidate
  web that starts at SetTextureFactor's test (block 8/9) is JOINED in ours with the use in `case 1: ApplyStaticVertexShader(0)` (the only case with no
  texture_stages store before its first shader-manager use), so it is live through the __ftol2 calls (allowed ...sibp), prio 16, split at the end, and the inline
  `this` copy U (prio 18) takes eax and is coalesced with it. Retail's code (`mov ecx,eax; mov eax,[eax+3CCh] ... or [ecx+3D8h],2`) is what a LOCAL web gives
  (block 8/9 only: prio 26 > 18 -> eax first, U -> ecx with a copy). So in retail something kills / separates that global between block 9 and case 1's first use.
  Taking the global's address or storing it in another function of the TU does not do it (w/p1..p4: identical objects).
- Diagnostic w/k2.cpp (a texture_stages store before case 1's shader call): the shader-manager web becomes local (prio 26, acdsibp, eax) as predicted, but the forced
  build's PS branch still has ours' registers (T edx, no `mov ecx,eax`), so the web join is not the whole story there. NOTE: candidate ids shift when the candidate set
  changes (in k2 mesh_primitive is id 5, id 19 is `tinted`): read the id from an unforced c2prio run before every C2_FORCEID run.
- Grades used by agentC20 on 0x00BA2350: see ATTEMPTS.log (about 75 full grades in ~25 idea groups) + 16 c2prio / forced traces + 2 /Od probe TUs (probe/od).
