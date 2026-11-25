# NF417 hand-back (2026-11-25): the remaining GetClassTypeName stand-ins

195 rows, all ACCEPT, landed as `2026-11-24_landing_nf417_{s1,c1,b1,b2,b3,b4,p1}` (RELOCATION_MATCH PASS, 0 LITERAL_DIFF).
- s1 (5): stand-in statics respelt to `?StatGetClassTypeName@<C>@@SI?AVCCharString@@XZ` (0x004D3EF9 CTCDNone, 0x004D41DF
  CTCDNavigationSeed, 0x0049D450 CTCTargetingPlayer, 0x004D3DFC CTCDParticleEmitter, 0x004D4DF4 CTCAssassinRush); the constructor
  name `??0CThingComponentDefNameTag@@QAE@XZ` leaves all three addresses it sat on (2 LNK4006 duplicates -> 0).
- c1 (3): their forwarders 0x004D3F30 / 0x004D4212 / 0x004E230A (requires s1).
- b1..b4 (178): the expanded 25-byte bodies re-formed as `?GetClassTypeName@<C>@@UBE?AVCCharString@@XZ`, a virtual calling the
  in-class inline static (VC7.1 inlines it to retail's bytes); p1 (9) the NParticleEngine::CPSC* direct-return form.
Gates: r10cmp EQUAL v RegisterTCs and every landed static; pk_all new side 0 NEQ (HEAD 1); /FORCE link of all 195 duplicates 2 -> 0.
Table: 1 add, 3 deletes (`?kActionStr@@3PADA`, `?kActionNameLiteral@@3QBDB`, `_g_actionName`: three baked-string rows fixed on the way).
195 self lines applied (each old line checked equal to its `current_line`).
Coordinator note: NF417's orig/ copies are named by repo file name, which the lander's address-keyed orig check skipped; the
coordinator compared all 390 orig files with the repo by name (0 differences) and fixed the lander to resolve repo-named copies.
