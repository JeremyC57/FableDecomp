# Current Pass21 extent

The supplied Pass21 ROWS.tsv assigns 4896 bytes. The complete selected function extent is 4894 bytes, ending exclusively at 0x00de968e. Complete selected native function extent; remaining assigned suffix is alignment only.

## Detailed boundary evidence

Native `?Main@CRansommer@CQ_RansomVictimScript@NScript@@EAEXXZ` begins at `00DE8370`. The final instruction is `RET` at `00DE968D`; the exclusive end is `00DE968E`, so the correct extent is **4894 bytes**. The assigned 4896-byte extent includes two following `CC` alignment bytes. There is no jump table. Direct native disassembly and the exact excluded bytes are retained in NATIVE_IDENTITY_BOUNDS.json.

Native allocator `00DE1BA0` directly installs vtable `012DD1C8`; slot 1 contains `00DE8370`. Both original PDB name sets contain the exact compiled leaf, which is the sole label in LABELS.tsv. This proves identity and bounds; the selected leaf remains DIFFER at two task-query latches.
