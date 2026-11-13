# Chunk 2020 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 2020, 0x00949AB0) A large same-TU member function with NULL default arguments can be EXPANDED at a constant-argument call
  site and called at another: CAIStateGroup_GoToPub::IsRunnable's random-table half (0x00949D7C..0x0094A0B7) is GetSeat(0, 0)
  (0x009482A0, 1072 bytes) with the table / seat tests folded, while `GetSeat(PTable.Get(), &seat_name)` stays a call. Before
  reconstructing a long "inline" region, compare it with the bodies of the same-TU callees.
- (chunk 2020, 0x00949ABF / 0x00949B06) `T* tc; thing.GetTC(&tc); tc->...` (no initialiser, result ignored) is retail's
  `mov [esp+N],0` at entry plus `mov esi,[esp+N]` on the miss path with the thing in esi (`add esi,44h`); `T* tc = 0;` folds the zero
  (`xor esi,esi`) and moves the thing to eax. Where the result is tested (`if (owned)`), `= 0` is right (`xor edi,edi`, 0x00949BF0).
- (chunk 2020, 0x006416C9) A 4-way result chosen from a loop index (`segment` 0 / 1 / 2 / other) is an if / else-if chain
  (`test ecx,ecx ; je`, `cmp ecx,1 ; jne`, `cmp ecx,2 ; jne`); a switch gives `sub ecx,0 ; je ; dec ecx ; je ; dec ecx`.
- (chunk 2020, 0x0064186E) A dot product whose last term consumes an x87 register (`fxch st(1) ; fmul [Z]` and one pop at the end) comes
  from a vector built with a literal zero Z (`C3DVector v(-s.X, -s.Y, 0.0f); v.Normalise(); v.Dot(w)`); `v = -s; v.Z = 0.0f;` keeps Z*r
  as a variable (`fld [Z] ; fmul st(2)` and two pops). The Dot operand order (v.Dot(w) / w.Dot(v)) makes no difference.
- (chunk 2020, 0x00641842) One integer instruction (`mov ecx,esi` for the next call) one slot late inside an x87 GFFastOneOverSqrt
  sequence closes with FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_PAREN (region cut), as in chunk 892.
- (chunk 2020, 0x006D1400) Two candidates tied in c2 priority (loop-2 &set / &*it, 30 each) are decided by key; one more candidate
  referenced in the loop-head block (`long def_index = *it;` used only for the GetDef) lifts &*it (+w*K) and gives retail's ebx / ebp
  and the eax / ecx order of the head. c2prio `--blocks` shows the per-block w*K*cost lines that make such a tie.
- (chunk 2020, 0x006D1250) The this / hero allocation of the whole post-block region was decided by ONE set-find site (the tattoo loop):
  with `if (s.find(k) == s.end())` there (end() evaluated first) the hero piece loses ebp to this; with a named iterator (find first,
  retail's order) the hero piece (prio -37) is coloured before this's (prio -67). Bisect such global swaps site by site with c2prio
  (free) before grading.
- (chunk 2020) c2prio-based feature checks (register of a named candidate, presence of a retail instruction shape in our own object)
  are a cheap fitness that needs no retail diff; a full-row diff counts as a grade.

Review at landing:
- 0x00949AB0 defines GetSeat 0x009482A0 in its TU (needed for the expansion); that body is byte-identical to retail 0x009482A0, so the
  parked GetSeat (pending/2026-10-30_parked_retry2_sec4) can land from the same source with leaf
  ?GetSeat@CAIStateGroup_GoToPub@@IAE_NPAVCThing@@PBVCCharString@@@Z (pair landing; I did not grade it through q.py -- no oracle row in
  this lane -- only with a relocation-masked diff against Fable.exe bytes and a call-target check).
- Both matched rows need this chunk's hdr/ (new CAIStateGroup_GoToPub.h and NGraphicAppearance_CGraphicAppearanceDrawInfo.h; opt-ins in
  CTCTavernTable, CTCAIScratchpad, CTCVillage, CTCLight, CEnginePrimitiveLight, CGameTimeManager, CEnvironment, CRGBFloatColour; the
  CTCGraphicAppearance LIGHTINGCHANNEL-with-MOVEMENT layout fix). The overlay also carries chunk 2012's / 2007's headers unchanged.
- 0x006414A0 uses the descriptive global `g_LightsDrawnThisFrame` (0x013BA85C); the display-engine row that zeroes it (0x004355BB,
  pending hand829 draft spells it g_013BA85C) should use the same name.
- NAME_FIXES.tsv: 0x00947EB0, 0x009486D0, 0x00658E38, 0x0065F8CA, 0x004C4160 (catalog label).
- 0x006D1250 parked after pass 1 only (3 of 30 grades) for working budget; the measured levers and the next idea are in NOTES.md.
