# Current Pass21 extent

The supplied Pass21 ROWS.tsv assigns 5872 bytes. The complete selected function extent is 5864 bytes, ending exclusively at 0x00d0a6f8. Complete selected native function extent; remaining assigned suffix is alignment only.

## Detailed boundary evidence

Native `?Main@CCampHostageGuard@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ` begins at `0x00d09010`. Final instruction 00D0A6C1 ret ; all13 original jump-table entries retained through00D0A6F7; 8CC alignment bytes excluded. Correct extent is 5864 bytes ending exclusively at `00D0A6F8`.

The selected compiled leaf is exactly the name in LABELS.tsv. Native Alloc `00D08E50` installs table `012C9130`; slot1 points to this address. Both original PDB names and actual allocator store/table bytes are retained in NATIVE_IDENTITY_BOUNDS.json. The current candidate remains DIFFER; this document proves identity/bounds only.
