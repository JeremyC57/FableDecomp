# Landing 2026-11-08k: chunk 862's seven static initializers

All RELOCATION_MATCH + PASS (vland `--land`; no headers). The four `$E_InventoryOrientationRHSet<N>` rows initialize the RIGHT_ / LEFT_ /
BACKWARDS_RIGHT_ / BACKWARDS_LEFT_FORTY_FIVE_DEGREES_ORIENTATION statics (siblings of landing_j's 0x0121AD80); 0x0121F590 / 0x0121F5C0 are the
PDB-named dynamic initializers of `hit_pos` / `hit_dir`; 0x01224CE0 initializes NScript::CQ_SunnyvaleMasterData::RelationSuffixes[3] (descriptive).
vland warnings: `_$E1` / `_$E2` RELOC_CONFLICT / RELOC_NAME (compiler-generated names shared across TUs; 0x0122AEF0 is landed under a wrong name,
queued `land0811l`). Labels: `ghidra_out/labels_landing_2026-11-08k.tsv`. Catalog sync restricted to this landing's addresses. `bindings_multi_check.py`
prints 1, the pre-existing NullCoord.
