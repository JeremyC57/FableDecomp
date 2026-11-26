# 0x00AF2200 vector<CParticleEmitterInfo>::resize

Landed RELOCATION_MATCH + `00af2200_TEST PASS` on cl 13.10.3077.

Retail is 36 bytes: load `n`, xor-zero a 12-byte `CParticleEmitterInfo` (`Data`, `Bank`, `Index`), call the two-argument `resize` at 0x00AF2170. The catalog label `vector<EWeaponClass>::_Umove` is propagated. The earlier draft modelled a counted pointer and emitted a destructor the retail body does not call.
