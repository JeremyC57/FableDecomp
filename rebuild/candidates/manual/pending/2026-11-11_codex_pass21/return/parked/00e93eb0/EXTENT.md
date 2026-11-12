# Current Pass21 extent

The supplied Pass21 ROWS.tsv assigns 5328 bytes. The complete selected function extent is 5321 bytes, ending exclusively at 0x00e95379. Complete selected native function extent; remaining assigned suffix is alignment only.

## Detailed boundary evidence

Native `?Main@CV_HauntedHouseScript@NScript@@UAEXXZ` begins at `0x00e93eb0`. Final instruction 00E95378 ret ; 7CC alignment bytes excluded. Correct extent is 5321 bytes ending exclusively at `00E95379`.

The selected compiled leaf is exactly the name in LABELS.tsv. Native Alloc `00E96DD0` installs table `012ECB94`; slot2 points to this address. Both original PDB names and actual allocator store/table bytes are retained in NATIVE_IDENTITY_BOUNDS.json. The current candidate remains DIFFER; this document proves identity/bounds only.
