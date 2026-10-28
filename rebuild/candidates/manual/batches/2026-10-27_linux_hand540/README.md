# Retry chunk 540 (agent540, 2026-10-28): 1 of 8 parked rows lands (RELOCATION_MATCH), 7 re-parked

0x005C0E81 `CPersistTraits<NInventory::CItem>::TransferOut` (587 bytes): STLport `operator[]` written as `*(begin()+n)`
(gives retail's byte-offset CSE; `_M_start[n]` was the 2-byte gap), reference leaf params, PDB callee spellings
(CPersistTraitsSLONG::TransferOut, CDefinitionManager::GetDefNameFromGlobalIndex), shared CItem / CDefinitionManager headers.

Re-parked (pending/2026-10-27_linux_hand540/differ), all frame-slot / scheduling / register-allocation residues at equal or
near-equal length: 0x0058F649, 0x0059FC17 (CTCInventory... AlreadyHaveSpecial: really the hero-title check), 0x005C71AA,
0x005CDD86, 0x005D8978 (really `TransferMap<long, unsigned long>`; catalog `<long,long>` wrong), 0x005F0DC5, 0x005FB140.

NAME_FIXES.tsv: 29 rows, deferred to the naming pass (0x00415DD0 `??BCDefString@@QBE?AVCCharString@@XZ`; 0x009E2590
CPersistTraitsSLONG::TransferIn; 0x004A93C0 CBasicString<char>::operator==; 0x0071DD50 CTCHeroMorph::GetTattoos).
