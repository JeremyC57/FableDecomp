# Current Pass21 extent

The supplied Pass21 ROWS.tsv assigns 5376 bytes. The complete selected function extent is 5373 bytes, ending exclusively at 0x00d0d90d. Complete selected native function extent; remaining assigned suffix is alignment only.

## Detailed boundary evidence

Native `?Main@CBanditForger@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ` begins at `0x00d0c410`. Final instruction 00D0D90B jmp 0xd0d8c8; 3CC alignment bytes excluded. Correct extent is 5373 bytes ending exclusively at `00D0D90D`.

The selected compiled leaf is exactly the name in LABELS.tsv. Native Alloc `00D0C2C0` installs table `012C9700`; slot1 points to this address. Both original PDB names and actual allocator store/table bytes are retained in NATIVE_IDENTITY_BOUNDS.json. The current candidate remains DIFFER; this document proves identity/bounds only.
