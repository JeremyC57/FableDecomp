# 0x0085ED90 std::vector<std::vector<NThingMap::CThingMapCell>>::resize

Landed RELOCATION_MATCH + `0085ed90_TEST PASS` on cl 13.10.3077.

**Name:** `std::vector<std::vector<NThingMap::CThingMapCell>>::resize(unsigned int)`

Mangled: `?resize@?$vector@V?$vector@VCThingMapCell@NThingMap@@V?$allocator@VCThingMapCell@NThingMap@@@std@@@std@@V?$allocator@V?$vector@VCThingMapCell@NThingMap@@V?$allocator@VCThingMapCell@NThingMap@@@std@@@std@@@2@@std@@QAEXI@Z`

Evidence: Ego_r.pdb `egor_pdb_names.tsv` 0x0055E410 and FableWin.pdb `fablewin_pdb_names.tsv` 0x029DA890, both that one-argument spelling. Retail is 36 bytes and calls the two-argument resize at 0x0085ED20.

**Was:** `vector<EWeaponClass>::_Umove<EWeaponClass*>`, length estimate 96. That label stays in the manifest and ghidra tables for Account A (`NAME_FIXES.tsv`).
