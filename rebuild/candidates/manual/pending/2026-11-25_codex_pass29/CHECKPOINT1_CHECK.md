# Codex pass 29, checkpoint 1: coordinator check (2026-11-26)

The package is `FableDecomp_pass29_checkpoint_1.zip` (5.6 MB, 4,055 entries). It was extracted in scratch, outside the repo, and nothing in it was run. Codex reports 200 rows: 184 verified (180 RELOCATION_MATCH + 4 retail_asm) and 16 held (4 DIFFER, 4 HELD_PICKANY, 6 HELD_VFTABLE, 2 HELD_SOURCE_REVIEW).

## Results
- **Integrity:** all 4,054 `SHA256SUMS` lines verify.
- **Header patches** (`hdr/PATCHES.json`, 572 records, 342 files, 64 new): replayed on the package base 6834d3bfd2, all 572 apply. All 342 files equal Codex's `PATCH_REPLAY.tsv` SHA-256 values.
- **Rebase onto HEAD** (3-way, base 6834d3bfd2): 251 files take Codex's text (unchanged at HEAD since the base), 64 are new, 20 merge cleanly and 7 conflict.
  - The 7 conflicts were resolved by hand: CAIStateGroup_MinionListenToStory, CAIStateGroup_MinionReactToPlayerInteraction, CEnginePrimitiveHandle, CQuestManager, CTCCreatureGenerator, CTCHeroOpinionDeedLog and CWorld.
  - They conflict because NF429-NF478 and Account B opt-ins landed at the same spots after the base.
  - Two needed more than keeping both blocks:
    - CEnginePrimitiveHandle: one `#if` head for both declaration-only `Clear` opt-ins.
    - CTCCreatureGenerator: Codex's `AddCreatureTriggerer` is guarded against NF435's.
  - MinionListenToStory: Codex's minimal view becomes an `#elif` of the retail view chain.
  - All 34 files changed on both sides were checked with a conditional evaluator under three macro sets (none, ours only, Codex's only). The merged text equals the HEAD text for ours and the Codex text for Codex's.
- **Strict-header dry run** (`LANDVERIFY_STRICT_HEADERS=1`, Codex tree as overlay, retail bytes from Fable.exe at each STATUS extent): **178 of 184 RELOCATION_MATCH, tests PASS.**
  - 10 of these needed the payload `leaf` set from `LABELS.tsv`. These parked rows have no catalog name yet, so the grader otherwise picked the TU's largest body.
- **Purity (6 rows graded NOT_GENUINE, `crawl/purity.py`):** each calls a shared retail-asm helper header without passing the evidence rule (ASM_RULES).
  - 0x005430F0, 0x005502E0, 0x00554740 and 0x00BFAD90 call the existing approved GFFloatToLongNear / Ceil and CMatrix4x4AlignedSSE helpers, but carry no `// Retail-asm:` line of their own. Codex lists them as RELOCATION_MATCH; they are retail_asm rows.
  - 0x00C5CA60 (Pow10 / FtoL) has a `// Retail-asm:` line that does not name an instruction at a retail address.
  - 0x00C6B260: the new `SalFrame.h` helper header itself fails the evidence rule. Its ROW_EXCEPTION cites an owner authorisation and failed C++ / intrinsic attempts, not a retail address and instruction.
  - 0x00A5C360 (NVectorMathSSE, FableWin procedure record attributing inline asm) and 0x00E7FD50 / 0x00542A30 pass.
- **Whole-tree equivalence** (HEAD include v rebased): 57,166 rows checked, and 25 landed rows' inputs differ (23 sources, 13 tests). Regraded all 25 on both trees with VC7.1: identical grades (14 MATCH, 8 RELOCATION_MATCH, 3 HEADER_POLICY on both sides).
- **Names:**
  - 174 of the 184 emitted leaves are in a PDB name set (Ego_r / FableWin / Ego_d). The other 10 are 8 GameCODA SDK functions, one STLport `_M_fill_insert` instance and `_salFrameFloatToInt`.
  - `NAME_FIXES.tsv` has 159 lines for landed spellings. The owner asked for them immediately after this check: agent NF479 applies them under the usual gates.
- **Reservation:** no verified row has been landed since the reservation. 183 still hold the `CODEX29` lock; 0x00C0B570 holds `COORD_HOLD_NF453` and needs checking before it lands.

## Before landing the rows (not done; the owner has not asked to land)
1. Add the `// Retail-asm:` evidence lines to the 5 helper-calling rows (address plus instruction of the helper block in each function).
2. The owner confirms the 0x00C6B260 / 0x00A5C360 asm approvals; 0x00C6B260 also needs a retail address and instruction for `SalFrame.h`.
3. Give catalog names from the demangled leaves.
4. Apply the rebased header tree (the scratch resolution and checker are reproducible from this note).
5. Land with `vland.py --land`, filtering new bindings that would split an address (as p27 / p28).
6. Settle 0x00C0B570's NF453 hold.

## Outcome (2026-11-27)
The owner asked to land the landable rows and fix the ones marked match that don't. 176 rows landed, and the rebased header tree was applied. Five rows got `// Retail-asm:` evidence. Seven rows were backed out and 0x00C6B260 is held. Details: `batches/2026-11-27_landing_p29/README.md`.
