# Current Pass21 extent

The supplied Pass21 ROWS.tsv assigns 5184 bytes. The complete selected function extent is 5178 bytes, ending exclusively at 0x00d02a6a. Complete selected native function extent; remaining assigned suffix is alignment only.

## Detailed boundary evidence

Native `?Main@CGate1GuardOuter@CQ_BanditCampScript@NScript@@EAEXXZ` begins at `0x00d01630`. Final instruction 00D02A69 ret ; 6CC alignment bytes excluded. Correct extent is 5178 bytes ending exclusively at `00D02A6A`.

The selected compiled leaf is exactly the name in LABELS.tsv. Native Alloc `00D014C0` installs table `012C8604`; slot1 points to this address. Both original PDB names and actual allocator store/table bytes are retained in NATIVE_IDENTITY_BOUNDS.json. The current candidate remains DIFFER; this document proves identity/bounds only.
