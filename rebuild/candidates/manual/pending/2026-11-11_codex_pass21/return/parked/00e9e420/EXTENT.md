# Current Pass21 extent

The supplied Pass21 ROWS.tsv assigns 5920 bytes. The complete selected function extent is 5915 bytes, ending exclusively at 0x00e9fb3b. Complete selected native function extent; remaining assigned suffix is alignment only.

## Detailed boundary evidence

Native `?Main@CBriarRose@CV_IntroductionToTrophiesScript@NScript@@EAEXXZ` begins at `0x00e9e420`. Final instruction 00E9FB3A ret ; 5CC alignment bytes excluded. Correct extent is 5915 bytes ending exclusively at `00E9FB3B`.

The selected compiled leaf is exactly the name in LABELS.tsv. Native Alloc `00E9D9D0` installs table `012ED67C`; slot1 points to this address. Both original PDB names and actual allocator store/table bytes are retained in NATIVE_IDENTITY_BOUNDS.json. The current candidate remains DIFFER; this document proves identity/bounds only.
