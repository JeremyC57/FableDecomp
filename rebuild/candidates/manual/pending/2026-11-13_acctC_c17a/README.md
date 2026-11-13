# acctC hand-back c17a (agentC17, 2026-11-13)

Bonus support row (not in acctC RESERVED.tsv; an unmatched draft of lane hand600 before):

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x00B9E5B0 | CEnginePrimitiveManagerStaticMeshesBase::RenderMeshPrimitiveBlocksIndexBuffer | 402 | RELOCATION_MATCH | PASS |

Verified by the coordinator with `lane/coordverify.sh` (read-only vland, strict headers): `coordverify.log`.
Header opt-ins it needs are in `hdrdiff/` (additive `FABLE_RETAIL_*` only; list and evidence in `HDR_README.md`).

Not landable yet, kept for the successor (agentC19):
- `open_00ba50a0/` SetupStaticBlocks: RELOCATION_MATCH only inside the 0x00BA5260 TU, and only with GetDiffuseTexture
  0x00BA9360 / GetBumpTexture 0x00BA95D0 bodies visible whose carried texts do not match at their own addresses.
- `cloth_replacement_texts/`: the two matched cloth rows (c8a) respelt for the repo's default `operator-`, the
  `SHADOWGUARD_DIRECT` opt-in and PDB global names. Coordinator decision: this replaces the fitted
  `FABLE_RETAIL_C3DVECTOR_NAMED_RESULT_SUB`; the c8a folder is restaged once the static-mesh row settles the shared texts.
- Drafts of 0x00BA5260 (DIFFER 4056v4056, one swapped x87 pair) and 0x00BA2350 (DIFFER 9516v9632) are in
  `2026-11-13_acctC_c8a/open_*`.

Review items for the owner: name of 0x01436E78 (g_pPrimitiveRenderer in bindings, g_pEnginePrimitiveRenderer in 42 landed
files, GPPrimitiveRenderer in the Anniversary PDB); other global respellings in `NAME_FIXES.tsv`.
