# Resume here (one page) — updated 2026-10-26

**Fresh container:** follow `docs/pipeline/FRESH_SESSION_HANDOFF.md` (Linux Wine lane setup with compiler downloads and
hashes, standing rules, landing/pass workflow, open work). Previous one-page handoffs are archived at the end of
`docs/journal/HANDOFF_ARCHIVE.md`.

## State
- Branch `FullDecomp` (`JeremyC57/FableDecomp`), clean and pushed. Catalog **49,920 rows**: **49,888 of 57,267 known
  functions (87.1%)**, or 43,317 of the original 49,568 (87.4%); the old "rows / 49,568" ratio mixed the 7,699 gap-scan
  functions into the count but not the denominator (FINDINGS_LOG 2026-10-27) (`rebuild/oracles/auto-re-candidates.tsv`). Agents (owner: at most two authoring at a time): cleanup pass 1 running (respell wrong identities in landed rows; settle 0x0062CF90) (513-518, re-queue under 128 bytes: 193 of 240; 8 rows of 517 held in pending/2026-10-27_linux_hand517/held on invented *_Sink stand-in callee bodies) (509-512, 256-512 bytes: 62 of 80; 0x0062CF90 held in pending/2026-10-27_linux_hand509/held: IS_THING_ALIVE twin class not provable; 0x006D4850, 0x00A70CB0, 0x00A74AC0 proven and landed 2026-10-28) (485-502, the singles under
  128 bytes, landed 671 of 713; 503-508 151 of 180); chunks 519-534 queued but paused by the owner during the cleanup (557 re-queued orphans: chunk_history rows never landed, parked or skipped; 513-520 under 128 bytes, 521-531 128-512, 532-534 512-1024) (chunk_history); family pass 8 (chunk 957, 979 rows) done by an external ChatGPT session and landed 2026-10-28 (914 of 979; 3 held on the 0x004C73D0 / 0x006A4D00 GetPosition twin; 62 pending).
- Retail compiler: cl 13.10.3077 (Rich header: 1,369 objects; 11 by QFE 4035; linker 3052). Linux lane runs 3077 by
  default, 4035 via `--qfe`.

## What changed 2026-10-25/26 (all regraded MATCH/RELOCATION_MATCH + test PASS; notes in `docs/journal/2026-10/`)
- **STLport namespace:** `_STL` -> `std` everywhere (retail mangles `@std@@`).
- **Landed-errors sweeps 1-3:** ~2,100 renames + ~510 source fixes (`*_sweep{1,2,3}_landed_errors.md`).
- **Family passes 1-7:** 613 + 576 + 587 + 577 + 560 + 591 + 584 rows (batches `2026-10-24_linux_hand950..953`, `2026-10-27_linux_hand954..956`).
- **Class-key pass:** every type uses retail's `class`/`struct` key from the PDB (`ghidra_out/pdb_class_keys.tsv`;
  951 headers, ~107k declarations; `2026-10-26_class_key_pass.md`).
- **STL layout pass:** 714 engine headers re-laid out with STLport 4.6 container sizes, bases from Fable.exe RTTI
  (`ghidra_out/rtti_bases.tsv`); 322 class sizes confirmed by retail code (`2026-10-26_stl_layout_pass.md`).
- **CRetailTree pass:** 388 rows from invented tree stand-ins to real `std::_Rb_tree`/map/set
  (`2026-10-26_retail_tree_pass.md`, `treepass/`).
- **One-address-one-name pass:** 6,644 rows call PDB-proven names through inheritance / base-dtor chaining
  (e.g. 0x993B10 = `CDataStream::~CDataStream`); bindings table has no symbol bound to two targets
  (`2026-10-26_one_address_one_name_pass.md`, `namepass/`).

## Open (owner picks the order)
1. Family pass 8 (979 rows) out to ChatGPT, re-verify on our lane before landing; 7,379 functions left in all.
2. One-address-one-name remainder: GetPItem return type (~340 rows), free/static stand-in callers (~3,490), CRT
   free-mock tests (59), LowerBound const overloads, CDataInputStream (227); still ~5,085 targets with >1 spelling.
3. CRetailTree remainder: 882 kept rows + ~180 misnamed /Os tree functions (`treepass/KEPT.tsv`, `OS_FAMILY.tsv`).
4. Recheck sweep: 35 rows failing since before the class-key pass, 93 Leafs naming no symbol, ~484 stale bindings,
   318 unknown-key headers, 389 headers without size evidence, CEngineResourceManager size drift.
5. Parked rows (`rebuild/candidates/manual/pending/*/differ/`), incl. family pass 3/4 parks; retail_asm candidates (no instruction cap since 2026-10-27; rasm2 landed 44 and parked the rest with verdicts, batch 2026-10-27_linux_retail_asm_rasm2)
   (0x774610, 0x779EF0, 0x81EA50, 0xB17BD0, 0xB1BC80, 0xA145C0, J032 x87 power-of-ten helpers 0xC40E60 ...);
   volatile candidates 0x9919C0, 0x991C10.
6. Review-at-landing items in each batch `LESSONS.md` (e.g. 0x81B510/0x81A4F0 are CParticleMorphs code; 0x62FC20 uses
   an invented `SElement12`; `reloc_bindings` binds the CRegionConnectionNode `__unguarded_linear_insert` name to
   0x448520, the real one is 0x8280A0).

## Where things are
- Rules: `CLAUDE.md`, `CONTRIBUTING.md`, `docs/pipeline/GOTCHAS.md` (one-liners), `docs/journal/FINDINGS_LOG.md`.
- Lane tools: `tools/decomp_pipeline/linux_lane/lane_tools/` (`recheck.py`, `vland.py`, `chunk_pool/` incl.
  `EXTRA_RULES.md`, `landone.sh`, `land5.sh`; `naming/` converters and `leafscan.py`).
- Tasks: `docs/ROADMAP.md`. Numbers: `rebuild/COVERAGE.md` (regenerate, never hand-edit).
