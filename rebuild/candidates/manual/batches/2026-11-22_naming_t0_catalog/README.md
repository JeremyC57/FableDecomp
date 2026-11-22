# Naming tracker batch T0 (Account A, 2026-11-22): 380 catalog / file names onto the PDB spelling the code already uses

Owner (2026-11-22): "do 1 now" (the T0 sweep from the naming tracker, `tools/decomp_pipeline/naming_status.py`). No source text, header, binding or
flag changed: only catalog names (`rebuild/oracles/auto-re-candidates.tsv`), manifest names (`rebuild/manifest/functions.tsv`), build_candidates
Module / Source / TestSource and the file names (git mv, 748 files).
- **Selection:** every T0 address in `naming/status/OPEN.tsv` at commit 24e474d11a: one spelling across the binding table and the row's own Leaf,
  that spelling a PDB name (Ego_r or FableWin name set), while the catalog name carried a stand-in marker (`_<addr>` suffixes, `_global::` /
  `_unattributed::` helper labels, `::void `, `FUN_` / `DAT_` ...). Two sources per name: the PDB name set and the graded code (the row's Leaf /
  its callers grade RELOCATION_MATCH or MATCH on that symbol). None was held, reserved, or in another agent's entries (checked before the run).
- **New names:** `llvm-undname` of the PDB spelling, class / struct / enum keywords dropped, the last `::` component as the name (ctors / dtors
  without template arguments): e.g. 0x00835620 `_global::Dtor_CountedPtrAt0x24_00835620` -> `CBrainOutput::CBrainOutput` (it is the ctor,
  `??0CBrainOutput@@QAE@XZ`), 0x00437940 `_global::Forward_AddDamageChange_via0x1c0_00437940` -> `CPlayerGui::AddDamageChange`, 161
  `CCountedPointer_<addr>::Reset` / `DeleteData` labels onto `CCountedPointer<T>::...`. Full list: `RENAMES.tsv`.
- **Checks:** funclet_includes_check 1,360 checked, 0 broken; no source, test, header or tool references an old file name (grep of all 748).
- **Recheck** (`logs/recheck_*.log`, landed tree, 9 batches of up to 45): 380 PASS (317 RELOCATION_MATCH, 63 MATCH), 0 DIFFER, 0 RELOC_CONFLICT.
