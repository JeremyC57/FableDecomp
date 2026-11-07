# 0x00AE2E20 CParticleProgram::OptimiseSpringOrder: RELOCATION_MATCH + PASS, held for one naming decision

The row matches (chunk 849; it needs the `FABLE_RETAIL_CARRAY_PUSHBACK_FASTPATH` opt-in in `../hdr/engine/CArray.h`), but it binds
`??$__destroy_aux@PAV?$set@KU?$less@K@std@@V?$allocator@K@2@@std@@@std@@YIX...` at 0x00AE76E0. The landed 0x0051C470
(vector<set<unsigned long> >::_M_insert_overflow) binds the same symbol at the byte-identical 0x0051AC70. One symbol, one address.
Open queue line cx12 (PRIORITY) says 0x0051AC70 is `__destroy_aux<CMapDesc*>` (Ego_r CMapDesc = { set<long> VisibleMaps }), which
would free the symbol for 0x00AE76E0. Settle that, respell 0x0051C470's callee if so, copy the CArray.h opt-in, and land this row.
