# Current Pass21 extent

The supplied Pass21 ROWS.tsv assigns 6176 bytes. The complete selected function extent is 6164 bytes, ending exclusively at 0x00cf66d4. Complete selected native function extent; remaining assigned suffix is alignment only.

## Detailed boundary evidence

Native `?Main@CFanMarker@CQ_ArenaHoldingScriptScript@NScript@@EAEXXZ` begins at `0x00cf4ec0`. Final instruction 00CF667C ret ; all21 original jump-table entries retained through00CF66D3; 12CC alignment bytes excluded. Correct extent is 6164 bytes ending exclusively at `00CF66D4`.

The selected compiled leaf is exactly the name in LABELS.tsv. Native Alloc `00CF4DF0` installs table `012C657C`; slot1 points to this address. Both original PDB names and actual allocator store/table bytes are retained in NATIVE_IDENTITY_BOUNDS.json. The current candidate remains DIFFER; this document proves identity/bounds only.
