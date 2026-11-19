# ESCALATE 0x005AC10E CTCInventoryTrade::ConstructActualBuyInventoryPC -- DIFFER(3532v3532), 2 instructions (181 hand2b grades)

Residue (unchanged since hand1/fable8): at 0x005ACB58 (merged SetName tail of the trading-goods if/else-if chain) retail reloads
`mov esi,[ebp+74h]` (ui_trading_goods_list_children_buy reference) then `mov edi,[ebp+4Ch]` (pframe); ours the reverse. Everything
else byte-identical (relocations masked). Best source: differ/005ac10e.cpp (= Codex pass 24 draft; the Ego_d-scoped variants of
hand1 / fable8 give the same 2 instructions).

Cause (fable8, c2.dll measured): reloads at a region entry come out in piece-id order; ui_trading_goods_list and its children
reference are the 18th/19th root takers and get fresh ids (#124/#125) above pframe's piece. Retail needs two more freed candidate
records before them (or two fewer takers). fable8's proof: named end iterators in the two NotInStock loops give the 2 records and
retail's pair order, but then those loops store/compare a cached end (code differs). Ordinal (include-order) shifts do nothing here.

Levels tried by hand2b (all: same 2 instructions, or code changes):
- Every existing ORIGINAL_* opt-in of every header this TU includes, singly (49; C3DVector, CArray, CCountedPointer,
  CIntelligentPointer, CIVCountedPointer, CThing GetTC/PeekTC chains, CVectorMap find, STLport copy/map/vector originals, ...).
- Every other existing opt-in of the inlined callee models, singly (131: CCountedPointer decl-only / noinline / reset forms,
  CList, CTCInventoryTrade / CTCShop member opt-ins, STLport list (size, clear, erase, insert, ne, ctor/dtor visibility), map
  (empty, index, ctor/dtor, begin), tree (find / bounds / erase / insert / inc / dec / postinc / iterator traits)).
- (hand1 / fable8, not redone) Ego_d scopes and every Ego_d local (plist, pparent, pgold, pupper_menu_entry, found, findit, endit),
  iterator forms, declaration orders, TotNum stores, empty() spellings, push_back / erase noinline.

Lesson from this round that may apply: 0x00AE3C30 (also a pure c2 allocation residue with a line-table-pinned body) closed with an
existing ORIGINAL-scope opt-in of an inline helper (CDataInputStream::Read's FableWin locals). Here no existing opt-in moves the
count, so the missing records probably come from an inline helper body whose retail form is not modelled yet: candidates are the
helpers inlined in the Buy-only code (the NotInStock list loops: STLport list iterator / end(); same_categories map find /
operator[]; GetMenuBranchFromDefIndexPC call setup) -- a helper whose original body has one more local or temp than ours
(load-CSE temp or forwarded copy) in each NotInStock loop. FableWin.pdb's line table for this TU (llvm-pdbutil dump -l -modi=<N>
FableWin.4k.pdb) can pin those loop lines' sizes.

## Update 2026-11-19 (hand2b): FableWin instruction reading (coordinator's method)

Source: FableWin.exe 0x02330B1D (9,246 bytes, tc_inventory_trade.obj = module 268), lines from fw_trade_fn.txt, call targets
resolved through the incremental-link thunks to ghidra_out/fablewin_pdb_names.tsv. Full annotated listing:
/tmp/claude-0/-home-user-FableDecomp/e0d0f58f-e880-4604-914d-32424a97cf0b/scratchpad/r4/fw_trade_annot.txt (script r4/fwlines.py).
Ego_d's frame offsets equal FableWin's (found -0x12C / -0x198), so the two donors are the same revision.

Per line (VC10 Dinkumware STL in the donor, so iterator calls are out of line):
- 5219 (buyable counting loop): `if (0) {} else for (vector<pair<long,long> >::const_iterator it = buyable.begin(); it != buyable.end(); ++it)`
  -- the iterator is a CONST_iterator (??0_Vector_const_iterator from begin()); end() called EVERY iteration (no cached end).
- 5222 GetMenuBranchFromDefIndexPC(it->first, PCurrentOpenTradeMenu [this+0x2BC], type) -> pentry [ebp-0xDC].
- 5224 `found = same_categories.find(pentry)` -> [ebp-0xD4]; 5226 `if (found == same_categories.end())` (end() per test);
  5228 `same_categories[pentry] = 1;` 5230 else; 5232 `++found->second` (operator-> once, +4, inc).
- 5235..5250 (first NotInStock loop): identical shape with `list<long>::iterator it` (non-const), `*it` as defindex,
  end() per iteration, find / == end() / [] = 1 / ++second. No find call outside find(); no cached end.
- 5252 same_categories loop: `_Tree_iterator it`, end() per iteration.
- 5264 buyable AddBuyItem loop: const_iterator again; 5270 `found = BuyListsByCategory.find(pentry)` NAMED ([ebp-0x12C]);
  5272 AddBuyItemToListPC(pframe [ebp-0x90], it->first, it->second, found->second, pviewport [ebp-0x10], true);
  5275 (inside the if) `findit = NumBuyableThings.find(pentry)` NAMED ([ebp-0x140]); 5276 `if (findit == NumBuyableThings.end())`;
  5278 `NumBuyableThings[pentry].second = it->second;` 5282 `NumBuyableThings[pentry].second += it->second;` 5284 switch(type).
- 5301 NumBuyableThings loop: `for (it = NumBuyableThings.begin(), endit = NumBuyableThings.end(); it != endit; ++it)` -- endit is the
  second for-init variable (begin first, then end), the only cached end in the function.
- 5343..5353 (second NotInStock loop): `list<long>::iterator it`, end() per iteration; 5347 GetMenuBranch(*it, ...);
  5349 `found = BuyListsByCategory.find(pentry)` NAMED ([ebp-0x198]); 5351 AddBuyItemToListPC(pframe, *it, 0, found->second, pviewport, true).
- No floating point anywhere (no x87 / qword operands in FableWin or retail), so literal type is not a factor.

So: end() is called per iteration in both NotInStock loops (fable8's named-end form is NOT the original); the named iterators are
`found` (BuyListsByCategory) in the buyable loop and the last NotInStock loop, and `findit` (NumBuyableThings) inside the if.
Graded: the draft + those three named locals (probe/r4_found.cpp) = the same 2 instructions at 0x005ACB58.
Include order (owner rule 2026-11-19): 46 variants (each of the 24 includes moved to the front / to the end) -- results below.
- Include-order results: 44 of 46 variants compile (F07 CTCInventoryItem.h-first and F09 CGameDefinitionManager.h-first fail on
  dependencies); all 44 give the same 2 instructions at 0x005ACB58. Ordinal shifts across every header position do not move
  the piece-id order here (agrees with fable8).
- FableWin-faithful iterator forms (probe/r4_fw.cpp = differ/005ac10e_fw.cpp: const_iterator in both buyable loops, endit as the
  second for-init variable, named found / findit): same 2 instructions.
- So none of the source facts FableWin shows (named find iterators, per-iteration end(), const_iterator, for-init endit) supplies
  the 2 missing records; they must come from a retail-only difference (2005 revision vs the Dec 2012 donors) or a callee model.
  Best reference sources: differ/005ac10e_fw.cpp (FableWin-faithful) and differ/005ac10e.cpp (Codex draft).
- Grades: 229 hand2b grades on this row in total.
- Second pass (advisor: scope-by-scope Ego_d/FableWin diff of the whole function), each graded, all the same 2 instructions:
  * every cast through GFDebugPDynamicCast (10 FableWin sites) -- with and without FABLE_RETAIL_GFDEBUGPDYNAMICCAST_ASSERT + dead arm;
  * one function-if-scope `CText* pupper_menu_entry` reused in the three PC_TEXT_* blocks (FableWin [ebp-0xA8], lines 5320/5328/5336);
  * `CList* plist = GFDebugPDynamicCast(plist_component, ..)`, unused `CComponent* pparent = plist_component->FindChild(..)` (5187/5189),
    AttachToFrame(plist->FindChild(..)) (5193);
  * ui_clothes/weapons/items_goods_list + their children references at function scope (no braces; 5402..5423);
  * one-argument GetTC<CTCShop>(&ptc_shop) (5163) and PeekTC<const CTCHeroStats>(&pTCHeroStats) (5441) (+ FABLE_RETAIL_CTCSHOP_INTERFACETYPE);
  * 5163 / 5441 are evaluating asserts-with-message (debug: if (!x) DoErrorMessage(..)); modelled as if (!x) { dead-arm assert }.
  Best reference now differ/005ac10e_fw.cpp (all FableWin facts above except the verify model).
  Not yet reconciled: line 5157 calls ?ResetInventoryTradeItems@CTCInventoryTrade@@QAEXXZ where the draft says ResetTradeLists()
  (relocation-masked; check the retail callee's landed name). Ego_d pgold (5435: pgold = plist_of_goods->FindChild(..); if (pgold)
  { pgold_real = GFDebugPDynamicCast(pgold, ..) ... }) is after the residue and not applied.
- hand2b grades on this row: 241.
- NAME: 0x0059D9AD is ResetInventoryTradeItems (FableWin call site + Ego_r 21-byte body); ResetTradeLists is in no PDB (NAME_FIXES.tsv).
