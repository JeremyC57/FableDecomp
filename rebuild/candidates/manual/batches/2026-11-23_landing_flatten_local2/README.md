# Landing flatten local 2 (Account A, 2026-11-23): two held pilot-1 rows, released by the identity review

Run `flat3` of `tools/decomp_pipeline/linux_lane/lane_tools/flatten_pilot.py` (local-model route) over the complete measurement pass
(`rebuild/oracles/self_symbols.tsv`).

- **Candidates.** 718 in all (732 before the pass finished), so the local route is exhausted. 12 generated: 3 accepted, 9 rejected (the
  same rejects as before).
- **The 3 accepted rows are pilot-1 held rows** that the source-level identity guard clears. Each has a second source that agrees, and none
  has a conflicting claim:
  - **0x00662090** `?GetDazedHealthThreshold@CThingCreatureBase@@QBEMXZ`. `reloc_bindings.tsv` already binds this symbol to this address,
    witness 0x00865E40, and that witness's object references exactly this symbol (checked at landing). **Landed.**
  - **0x00B25D00** `?OnPostDeviceReset@CEngine@@EAE_NXZ`. Its callers spell the PDB name, and the RTTI vftable placement names the same
    symbol at this address. **Landed.**
  - **0x006D6570** `?GetExperiencePickUpMultiplier@CTCHeroExperience@@QBEJXZ`: **held.** Its binder 0x00899C90 still spells
    `@Sub_GetExperiencePickUpMultiplier@4`, so the two move as one unit.
- **Queue.** This resolves the `flatten1 ... identity research` T3_PORT_QUEUE lines of the two landed rows.
- **Table.** No change.

## Recheck
`logs/recheck.log`: 0x00662090 MATCH PASS, 0x00B25D00 RELOCATION_MATCH PASS, witness 0x00865E40 RELOCATION_MATCH PASS.
