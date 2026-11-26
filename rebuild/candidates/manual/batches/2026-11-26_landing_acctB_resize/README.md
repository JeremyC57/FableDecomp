# 0x00AF2200 std::vector<NParticleEngine::CParticleEmitterInfo>::resize

Landed RELOCATION_MATCH + `00af2200_TEST PASS` on cl 13.10.3077.

**Name:** `std::vector<NParticleEngine::CParticleEmitterInfo>::resize(unsigned int)`

Mangled: `?resize@?$vector@VCParticleEmitterInfo@NParticleEngine@@V?$allocator@VCParticleEmitterInfo@NParticleEngine@@@std@@@std@@QAEXI@Z`

Evidence, two PDB name sets plus the retail call:

- Ego_r.pdb `egor_pdb_names.tsv` 0x00AB1C40
- FableWin.pdb `fablewin_pdb_names.tsv` 0x02F03D80 and `fablewin_obj_functions.tsv` `particle_data_bank.obj`
- both are the one-argument `QAEXI` overload
- retail loads `n`, xor-zeroes a 12-byte `CParticleEmitterInfo` (`Data`, `Bank`, `Index`) and calls the two-argument `resize` at 0x00AF2170, which is already landed for this element type

**Was:** `vector<EWeaponClass>::_Umove<EWeaponClass*>`, length estimate 80. Retail extent is 36 bytes (`ret 4` at 0x00AF2221; `rebuild/candidates/manual/BOUNDS_FIX.tsv`). The old label is still in `rebuild/manifest/functions.tsv` and the ghidra label tables. Account A applies that respell; this batch only records it in `NAME_FIXES.tsv`.

The earlier draft modelled a counted pointer and emitted a destructor the retail body does not call.
