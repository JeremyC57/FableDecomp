# agentC41 NOTES (row 1 0x00652FA4; start = prev_C39/differ = differ/v25.cpp, 4654v4653, 23 nd hunks)
- Tools added: tools/gg.sh <src> (grade + one-line order signature via tools/sig.py), tools/vg.sh <base> <name> <old> <new>.. (textual variant + grade),
  tools/sw.sh <base> <type> "<k..>" <marker> (DIAG ruler sweep; type b = `inline long DiagR(long v){long t=v;return t;}` = ONE symbol per unit). A grade takes ~2 s.
- C41 g1-60 MEASURED (X order model): X order of each inlined operator+ sum depends on the number of NAMED SYMBOLS created before the site, PERIOD 8
  (empty inline / pass-through inline = 0, each named local in an expanded inline = 1). The count is cumulative in source order: a ruler before loop 1 does not
  move the tip, a ruler before loop 2 does not move loop 1; seal / flash / d-pad react equally to rulers at the top, before loop 1, before loop 2.
  v25 windows (top ruler k mod 8 that makes the site right): tipX {3,4}; l1X {4,5,6,7}; sealX {6}; flashX {6}; dpadX {2,3,4,5}; loop-2 X never (not count keyed).
- Y at the main seal is ROLE keyed when ringsPos is spelt `def.DPadRingsPos`: `def.GuildSealMainLevelUpOffset + def.DPadRingsPos` (tmp/s8) gives retail's Y
  (rings first), same bytes otherwise; with the named reference `ringsPos` the roles do not matter (C39). On s8 the windows are tipX {3,4}; l1X {4..7};
  sealX {1..5}; flashX {5}; dpadX {1..4} -> "s8 + 4 symbols before the tip" leaves only l2X, flashX, flash fst, dpadY, colour.
- C41 h6 (tmp->differ/h6.cpp): s8 + DIAG forced-inline helper 'C2DVector DiagAnchorOffset(const C2DVector& pos, const C2DVector& offset) { C2DVector result = GFAnchorToBottomLeft(pos) + offset; return result; }' used at flash + d-pad (replaces the double cast): 4654v4653, WRONG only: col, l2X, flash fst position, dpadX. Y orders all right, add edi before call kept, no priority swap.
- C41 g60-200 (cheap 2 s grades). FACTS:
  (F1) What counts as a "unit" for the X-order phase: a NAMED LOCAL INSIDE AN EXPANDED INLINE (DiagR `long t = v; return t;`). An empty inline, a pass-through
       inline (`return v;`) and a dead named local written directly in Draw do NOT move anything. Period 8 exactly (k=8..256 identical; k=512 flips l1Y).
  (F2) priority lead closed: a named `const C2DVector& middleOffset = def.DPadMiddleOffset;` (any place) is FORWARDED into both field reads, so def is referenced
       twice in the last block (trace: `referenced, cost 6: +72` v `cost 3: +39`) = the +33; retail has ONE def reference there (`add edi,1C8h` in place = a
       single-use temp). So retail's &def.DPadMiddleOffset is an inline-expansion PARAMETER temp, not a named reference. Do not retry named references.
  (F3) d-pad orders (X def first, Y anchor first, add edi before the call, ringsPos home kept) come out right only when def.DPadMiddleOffset is bound to an
       inline parameter created BEFORE the anchor temporary: tested shapes (DIAG helpers, made-up names):
       A = `FABLE_FORCEINLINE C2DVector H(const C2DVector& pos, const C2DVector& offset) { return C2DVector(GFAnchorToBottomLeft(pos) + offset); }` used as
           `H(ringsPos, def.DPadMiddleOffset)` (differ/h5.cpp; plain inline is called out of line): 4654, wrong only col, l2X, flash-fst, dpadX; + 4..6 units
           between the flash block and the d-pad `if` -> only col, l2X, flash-fst wrong.
       B = plain `inline C2DVector H(const C2DVector& a, const C2DVector& b) { return C2DVector(a + b); }` used as `H(GFAnchorToBottomLeft(ringsPos),
           def.DPadMiddleOffset)` (differ/q3.cpp; expanded without force): 4654, d-pad X+Y right; + 6 units at the top -> every X right, flash Y wrong;
           one unit inside the flash block before the sprite fixes flash Y.
       Both helpers replace the FITTED double cast (the helper's by-value return is the copy). No such helper name in FableWin / Ego_r PDB name sets.
  (F4) New opt-in FABLE_RETAIL_C2DVECTOR_TOC2DVECTOR (PDB ?ToC2DVector@C2DVector@@QBE?AV1@XZ, `return *this;`): `(a + b).ToC2DVector()` compiles like the double
       cast (same bytes shape, 4654) = the natural spelling of the copy if no helper is used; orders as v25 though.
  (F5) fwsize: FableWin operator+ is 26i/68b = the NAMED RESULT text (ours unnamed 21i/52b; named 25i/66b, the systematic FableWin +1i/+2b). With it retail
       would copy at every site (n1: 4702, 1547 insns) -> retail's build has the unnamed text; FableWin differs. ToC2DVector FableWin 12i/28b v `return *this` 10i/25b.
  (F6) flash `fst [ebp-4]` position (0x006540C3: retail after the 3rd push, ours after the 2nd) never moved with any ruler or helper: structural / scheduling, open.
  REJECTED this session: `pos = sum` inside the argument (stores stay), `C2DVector(anchor) += offset`, sealPos named local / copy-then-+=, component-wise
  ctor at the seal, named middleOffset at top or at the site (priority swap), `def.M + GFAnchor(ringsPos)` in any form (add after call, ringsPos rematerialised),
  NAMED_RESULT_ADD, natural `float fade` / `long fade` locals in the flash block (do not flip flash Y).
- C41: col (0x006534AE) SOLVED = IL order ScaleAlpha -> font load -> colour stores: 'unsigned char alpha = ScaleAlpha(0xff); const CFontBank& font = Component->PeekCutSceneMainFont(); PlayerGui->Draw2DText(text, name, font, CRGBColour(0xff,0xff,0xff,alpha), ...)' (tmp/c7). The indirect font load may alias the address-taken colour temp, so c2 keeps IL order; retail has load then stores. font-before-ScaleAlpha (c3) = 4660.
- C41: loop-2 1 byte SOLVED in size = 'const CQuickAccessItemButtonGuiDef& button = def.QuickAccessItemButtons[i];' with button.Offset used twice (as loop 1) -> retail's 'add eax,[i*50h] ; add eax,48h' (4653 bytes), l2X then phase keyed (right at top k=6,7) BUT priorities this 655 v def 656 -> esi/edi swap (need this >= def).
- C41 K5 (differ/K5.cpp): loop 2 in loop 1's exact idiom ('const CQuickAccessItemButtonGuiDef& button = def.QuickAccessItemButtons[i]; const C2DVector& offset = button.Offset; pos = GFAnchorToBottomLeft(ringsPos + offset); ... DrawChargeGraphic(info, button.Offset, slot)') + c7 + s8 + ToC2DVector: 4653v4653 (retail size), tipX l1X l2X sealX sealY dpX col all right; WRONG flX, flash fst, dpY + this/def register swap (prio this 655 v def 656).
## C41 FINAL STATE (row 1 0x00652FA4 not matched; row 2 0x0058807D not opened)
- Hand-back: differ/00652fa4.cpp = "K5": DIFFER(4653v4653), retail size, 1524 v 1524 insns. Raw nd 125 hunks because this/def swap registers; swap-masked (tools/nd.py <g> n 0 1)
  12 hunks = 5 prologue/epilogue push/pop order + flash X (0x0065409E) + flash fst position (0x006540C3) + d-pad Y (0x00654151) + 2 reloc-masked.
  differ/00652fa4_noswap.cpp = "w4": DIFFER(4654v4653), registers as retail, 16 masked hunks (loop 2 still with C39's offset reference).
- Changes C39 v25 -> K5, each EVIDENCED by bytes unless said:
  1. tip text: `unsigned char alpha = ScaleAlpha(0xff); const CFontBank& font = Component->PeekCutSceneMainFont(); Draw2DText(text, name, font, CRGBColour(0xff,0xff,0xff,alpha), ...)`
     -> 0x006534AE..C2 store scheduling exact (IL order call -> font load -> colour stores).
  2. loop 2 in loop 1's idiom (button reference + offset reference, `button.Offset` again in DrawChargeGraphic) -> the extra byte gone (0x0065396F add/add form), loop-2 X right.
  3. main seal `def.GuildSealMainLevelUpOffset + def.DPadRingsPos` -> Y order 0x00653F18 right. FITTED-ish: spelling `def.DPadRingsPos` beside the `ringsPos` reference is odd
     but it is the only form found that gives retail's order (same bytes otherwise).
  4. flash / d-pad position `(GFAnchorToBottomLeft(ringsPos) + def.DPadMiddleOffset).ToC2DVector()` (new opt-in FABLE_RETAIL_C2DVECTOR_TOC2DVECTOR, PDB-named member) replaces the
     FITTED double cast with identical bytes.
  With 1-4 the top-ruler phase scan has tipX, l1X, l2X, sealX, dpadX right at k=0 ONLY (tools/best.sh) -> the symbol counts up to the seal are now natural; do not add rulers.
- OPEN (all in the flash / d-pad tail + one priority unit):
  (O1) priority: this 655 v def 656 (trace tools/tr1.sh; K5 ids this 12, def 27; tie goes to the later id = def, so `this` needs +2 or def -1). The loop-2 change raised the K
       of one loop-2 block 14 -> 15 (def cost 9, this cost 3 there). Per-block contribution = cost x K, K = block size: any text change that adds a tuple to a block that
       references `this` and not def (flash block: FlashFade, ScaleAlpha, PrimitiveHandles; Effects loop) fixes it. Likely the SAME missing text as (O2).
  (O2) flash sprite: retail X `fld def.X ; fadd anchor.X` (ours anchor first; right only at top phase k=4), and `fst [ebp-4]` after the 3rd `push ebx` (ours after the 2nd; never
       moved by any ruler, helper, or flashAlpha spelling: long / const / float / in-argument / declared earlier).
  (O3) d-pad sprite Y: retail `fld anchor.Y ; fadd def.Y` (ours def/t first). Comes right only when &def.DPadMiddleOffset is an inline PARAMETER created before the anchor temporary
       (probe/00652fa4_helperA_DIAG.cpp, _helperB_DIAG.cpp: made-up DIAG helpers, NOT hand-back candidates; they also shift the phases so tip / loop 1 / seal go wrong again,
       which argues AGAINST a helper and for the plain `.ToC2DVector()` text plus a missing fact in the flash block).
  Ideas not tried: the flash block's statements re-derived from retail 0x00653F7x..0x0065407D block by block (FlashFade.Open/Update/IsOpen/Close order, `if (!FlashFade.Active)`
  v an inline accessor IsActive (PDB ?IsActive@CFadeInFadeOutBase@@QBE_NXZ exists; IsOpen/IsClosed/IsOpening too), the switch as if-chain or array index
  (`def.QuickAccessItemButtons[FlashButton - 13].ButtonGraphic` with a range test), `CPlayer::IsAggressiveMode()` / `CDrawBase::GetPlayer()` accessors (PDB names exist) for K.
- Same-TU callee bodies graded from differ/00652fa4_noswap.cpp's TU at their own addresses: GFAnchorToBottomLeft 0x00643CDA, DrawIcon 0x00645292, DrawEffectAtPos 0x0064546F
  = RELOCATION_MATCH each (q.py with the mangled leaf, /GS).
- Siblings on the final hdr/ (C2DVector.h opt-in added): 0x006096E0, 0x0061E6D0, 0x00601310, 0x00651B04 (repo source) and probe/00651b04_long_HUDRingAlpha.cpp = RELOCATION_MATCH.
- No behaviour test / addmatch / payload.json: no row matched.
