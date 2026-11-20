# 2026-11-20 landing nf119: PRIORITY slice 5 remainder, first hand-back (5 rows)

Records: `naming/2026-11-20_agentNF119/` (selfcheck 0 problems). 19 entries (NF115's leftovers): 1 done (q4497), 8 blocked (port; STATUS names
the classes: q3885, q4491, q4495, q4742 carried over; q6158 0x007CC4D0 left; q6371 / q6406 0x0066E630: 0x00858130, 0x00858400, 0x00858590,
0x00951B20 use a CVector stand-in in large local models; q6407 0x00A887B0 needs a CSBCCorridor chain), 10 open (q3864, the CTCCarrying family
q4469..q4473 + q4984 now NF121's, q6231 / q6232, q6393).

## Rows (5): 5 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- 0x0065F6A6 count_if: explicit instantiation hits C2785 four ways; the instance's address taken in a scaffolding helper (nf86 / nf88 pattern)
  works. It binds the PDB spellings of 0x0065F152 and the 0x0050F410 copy ctor; its own leaf moves YGJ -> YIH (what its only referencer
  0x00659800 binds). `/Gr` carried into CompilerFlags.
- 0x008B7B10 SheatheWeapons::PerformAction: MakeNoise / SheatheAllWeapons on CThingCreatureBase (PDB), not CThing.
- 0x0078EBF0: self rename to `?PlaySoundW@CTCDivineWrath@@AAEKABVCCharString@@_N@Z` (PDB); 0x0066BD00 bound as CTCSoundPlayer::PlayAttachedCriteria.
- 0x0075F980 AppendControlPoint onto the shared CCardinalSpline.h body; 0x0066E370's binding off the CArray `__true_type` stand-in.
- 0x0066E370: truncated Leaf line completed.

## Checks
bindings_multi_check 1; funclet_includes_check 1355 / 0; no header changed. Referencing rows: 98 good / 0 bad. No-new-split gate 0; table
spelling sets: none grew; 0x0065F152, 0x0050F410, 0x00662110, 0x0066BD00 2 -> 1; 0x006647F0, 0x0066E370 3 -> 2. NAME_FIXES: 0x007940F0 is
`?PlaySoundW@CTCUnholyPower@@AAEKABVCCharString@@_N@Z` (the CTCUnholyPower ctor 0x007941D0 binds it; byte twin of 0x0078EBF0).
