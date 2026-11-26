# NF467 hand-back (2026-11-26): r459a short-vftable ports, pilot + AIStateGroup ctor wave 1

No repo writes, no git. HEAD read at the start (FullDecomp working tree). PILOT.md has the method, the per-step times and why the top of the
ranking was skipped. RULINGS_NEEDED.tsv, NAME_FIXES.tsv (61 lines) are beside it.

## Batches (NF464 layout). ALL HOLD -- see PILOT.md "Result"
| batch | rows | verdict |
|---|---:|---|
| b0_hdr | 3 headers (StandStill, MinionKidCalledToMother, MinionWander) | HOLD with their rows (HEADERS_NEEDED_BY.tsv); add-only opt-ins, g++ -E -P identical, moved asserts pinned |
| b1_standstill_pilot | 1 (0x008FDE1B) | HOLD r467a (+ r467b-wide: Update slot self misspelt) |
| b2_aisg_ctor_ready | 19 | HOLD r467a (some + r467b); ready the moment r467a is ruled |
| b3_aisg_ctor_rework | 22 | HOLD r467a + r467f (wrong model; headers withdrawn to hdr_rejected/) (some + r467b) |
TABLE_CHANGES: none. fullcensus 42/42 OK, 0 BAD (no respelt symbol; each table's only retail referencer is its ctor row).
VFTABLES_FIXED.tsv: the 42 names, 4 -> 56 (retail 56), all built and gated, none ACCEPT.

## Gates (every row)
- vland HEAD v new: RELOCATION_MATCH PASS 42/42; WARN 2 -> 0. Tests unchanged, PASS.
- objproof: ctor data + relocations equal (alignment 16 -> 1, r467c); table intended; added ??_G + ??1CAIStateGroupBase EQ to retail; .drectve unchanged.
- link: without 0x008FD288 in the set dup 2 -> 2, unresolved 193 -> 183 (accept-shaped rows); WITH it dup 2 -> 3 (r467a). Rows with 2+ row-less /
  misspelt override slots grow unresolved (r467b).
- sandbox: apply_table --dry 0 misses, multi 1 -> 1, bindings_multi_check 1, descriptive_check 2,030 / 0 FAIL, tracker dry run no moves.
- Base: FullDecomp d1e70f2a4e (.git ref read with cat); no file under rebuild/include, rebuild/oracles or the catalog newer than the first
  vland log; every orig/ file equals the working tree.

## Not done / resume
Wave 2 AIStateGroup (179 names: intermediate bases, more slots, other shapes), CInputProcess (r467d), CACompressor (r467e), the 37
shared-table names (evidence/shared_names_37.tsv; untouched, none ported here), and everything below them in evidence/rank_by_name.tsv.
