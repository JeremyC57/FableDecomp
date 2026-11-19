# hand4d batch 12 (2026-11-19) -- 0x00774D20 CTCShop::BuyItem: RELOCATION_MATCH + test PASS (strict read-only vland)
- 0x00774D20 ?BuyItem@CTCShop@@QAEXJ@Z -- RELOCATION_MATCH 1149 / 1149, strict read-only vland `00774d20 RELOCATION_MATCH PASS BuyItem`
  (WINS 1; vland.log). Base flags, no per-row flag. Plain C++, no asm / pragma / helper / macro.
- Name: Ego_r.pdb 0x00782AB0 + FableWin.pdb 0x0261E960; only caller CTCInventoryTrade::SellHighlightedItem 0x005A3436 (landed, binds
  0x00774D20 as BuyItem). NAME_FIXES.tsv: the catalog label `CTCShop_SellHighlightedItem` (labels_forge_autoname2.tsv, descriptive
  autoname) is wrong -> ?BuyItem@CTCShop@@QAEXJ@Z (vland WARN STANDIN_LEAF is this).
- Lever (source shape, Ego_d.pdb C13 line table evidence/egod_buyitem_lines.tsv v our VC7.1 /Od per-line sizes):
  1. Ego_d 842..844 is ONE `if` (4-term && chain: both GetSubDef tests, DefaultPrice <= MaxPriceToDisplay, CanBeDisplayedInShop),
     not two nested ifs (120 bytes = 55 + 57). Nested ifs gave c2 the K25 tie / swap (places._M_start piece priority 22 >= pshop_def piece
     22, ties to the larger key) -> esi/edi/ebp permuted from 0x00774D7B. One `if` changes the block structure and the priorities.
  2. Ego_d 883..890: braced clamp, then `value -= MinGoldForDonation - 1;` FIRST (885, FableWin statement), max_value as Max + 1 - Min,
     then GFPower(value, ...) twice (888 / 890 the same size). The old GFPower(value - (Min - 1)) form let c2 CSE MinGoldForDonation from
     the compare (retail reloads it at 0x0077508A).
- FITTED (marked in the source, `// Inferred:` on the line): the grouping parentheses of `morality = (morality_adjustment * log_value)`.
  VC7.1 keeps a no-code PAREN tuple that moves the scheduler's 81-tuple region cut and places `mov ecx,esi` after `fld [1.0]` at
  0x007750E5. Equivalent alternative that also matches: `1.0f - (ptc_stats->GetMoralityPercentage())`. Ego_d has two more code lines in
  this tail that the text does not reproduce: 894 (10 bytes, no call) and 898 (17 bytes, after the SetMorality-sized 897).
  Coordinator decision (2026-11-19): keep q1; the `// Inferred:` line names p6 as the equivalent alternative.
  Best guesses: 894 = a plain float statement on the morality value (10 bytes at /Od is one load / one op / one store, e.g. a
  complement or clamp of `morality`); 898 = debug-only code after the SetMorality call (a trace / console print or an ASSERT arm on
  the new morality -- 17 bytes is one call with one or two pushes), which the release compiler drops, so it is byte-neutral and
  cannot be graded. One test (one grade, ATTEMPTS.log 2026-11-19, c2work/var/v_l894.cpp): 894 as `morality = 1.0f - morality;` with
  `new_morality = 1.0f - immorality * morality` -> DIFFER, ndiff 4 (retail keeps `fsub` then `fmulp` at 0x007750E7; the split form
  folds to one `fmul`). Not adopted; no further probing per the coordinator's cap.
- Opt-ins: FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY + FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY (both already in today's
  rebuild/include/stlport/fable_map.h; no header change). The row grades RELOCATION_MATCH with and without them; the behaviour test needs
  the out-of-line map::operator[] bodies they provide. No hdr overlay needed (this row includes no overlay header).
- Behaviour test test/00774d20.cpp (self-contained; doubles for GetSubDef / GetDef / AddSaleItem / hero stats / experience, a fake
  CMainGameComponent with sorted Components): 9 scenarios on exact power-of-two morality values -> `00774d20_TEST PASS`.
  One-decision mutants test/mutants.tsv: 16/16 killed (test/mut.log).
- c2 measurement history, coordinator checks 1-4 and every probe: evidence/ESCALATE_00774d20.md (section RESOLVED); ATTEMPTS.log.
