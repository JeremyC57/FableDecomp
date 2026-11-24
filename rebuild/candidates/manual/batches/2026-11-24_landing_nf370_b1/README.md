# Landing nf370 (b1, b2, b3; 2026-11-24): ten selves onto their PDB access / const / virtual spellings, and one catalog name

Hand-back: `pending/2026-11-24_nf370_handback/` (NOTES.md, STATUS.tsv: tracker T0 26 + T2 43 addresses). Every accepted symbol is in
both PDB name sets and the table binders already bind it. Per row: HEAD and new MATCH / RELOCATION_MATCH with the test passing; objsec
ALL_EQ (only the self symbol changes); both pick-any checkers HEAD = new, 0 reach flips; no landed object references the old spelling;
the link resolves the new symbol with 0 duplicates. land_scripted rechecked all ten on the landed tree (`logs/recheck.log` per batch).
- b1 (access letter): 0x0094CE60, 0x00B8FD40, 0x00B2C8E0, 0x0082A500, 0x004AEAA0, 0x009F7250
- b2 (const): 0x004348D0, 0x009FF8D0
- b3 (`virtual`): 0x008F1720, 0x006A5AF0 (each PDB class's RTTI vftable holds the row). Checked by the coordinator: the HEAD and new
  objects carry the same `??_7` set (none in 0x008F1720; `CIntelligentPointer<CThingPhysical>` in 0x006A5AF0, `.rdata` identical) and
  the same sections and sizes, so no vtable changes (NF320 Q3).
self_symbols.tsv updated for the ten (column 4 = the `// Leaf:` text).

T3 queue (all ten were named; advisor decision, owner delegated: no line reserved the self fix NF370 made): access1 0x008F1720 closed
(`../2026-11-24_landing_nf370_b3/T3_CLOSED.tsv`); the nf303 x3, nf311 and nf310 lines annotated (binders only now); nf297 0x009FF8D0
cut to its remaining binder 0x009DCA80; nf290, nf283, nf288 and nf297 0x009F1650 unchanged (they reserve other fixes).

T0: 0x009F98D0 is std::list<CProfile*>::remove in the catalog (FableWin name + its only caller's RemoveProfile shape, Ego_r ProfileList
at +76). Not applied: 0x009BDE90 / 0x009BDFA0 (one external name on two addresses) and 0x00A70130 (the caller evidence is its sibling
0x00A6F390's); both queued as identity.

Rulings (advisor decision, owner delegated): (1) the eleven unreferenced `??_GCCharString` rows and five `??_GCWideString` rows stay T3
identity; (2) the proven-but-held self fixes split: those with a binder or witness on the old spelling go to a unit_fix pass, those whose
PDB symbol is a strong copy in another row join owner question 2; (3) 0x0040EC50 is renamed only with a self port that drops the
`?VecDel@T` stand-in. 0x00C3F600 / 0x00C596A0 skipped (propagated libpk / CODA label).
