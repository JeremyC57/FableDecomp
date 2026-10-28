# Chunk 514 (agent514, 2026-10-28): 33 of 40 re-queued single functions landed (under 128 bytes)

Second re-queue chunk: 40 chunk_history orphans under 128 bytes (address order, 0x00625160..0x00712B60), mostly
NPlayerGui draw classes, defs, combat-ring and music code. One Opus 5.5 agent at high effort.

- **Landed:** 32 rows with `vland.py --land` (4 MATCH: 0x00643E09, 0x006B74E0, 0x006BD780, 0x006BD840; 28
  RELOCATION_MATCH), plus 1 retail_asm row (payload_retail_asm.json): 0x006472B2 NPlayerGui::CDrawDamage::AddDamageChange,
  RELOCATION_MATCH, using the engine's inline-asm ROUND helper (bare `fistp dword`) with a `// Retail-asm:` evidence line.
  Every test PASS under g++ and VC7.1; no RELOC_IDENTITY, RELOC_CONFLICT or RELOC_DUPLICATE.
- **Renamed before landing:** landed 0x00644526 carried `?OnEventAttack@CDrawTargetInfo@NPlayerGui@@QAEXXZ` but is
  OnEventUnsheatheWeapon: Ego_r.pdb has OnEventUnsheatheWeapon at 0x006875D6 (97 bytes) then OnEventAttack at 0x00687637,
  FableWin.pdb has the same order (0x020FD5A7, 0x020FD627), and retail 0x00644526 is 97 bytes with 0x00644587 (this
  batch's 51-byte OnEventAttack) right after it. Renamed source and test recheck RELOCATION_MATCH PASS
  (verify_rename_00644526.log); label in ghidra_out/labels_onevent_unsheathe_2026-10-28.tsv.
- **Parked (7, pending/2026-10-27_linux_hand514/differ/):** 0x00625160 / 0x0067C520 scalar `??_G` of CUIStateDef /
  CBlurredAttitudesDef (implicit destructors called rather than inlined), 0x0064A2DB CDrawPerceivers::AddPerceivingThing
  (one store scheduled early), 0x006D63A0 CTCHeroExperience::GetRunningNumHits (`fld 1.0 ; fdiv` vs retail `fdivr`),
  0x006AF4E0 GetPOriginator<CThing> (dead spill, constant-false test), 0x006BB5B0 CGameTimeManager::IsTimeBetween (shared
  compare), 0x0070C4E0 CAnimateThing::GetFirstCombinationAnimFromType (find_if re-reads end).
- **Identities:** 28 proven labels written (ghidra_out/labels_chunk514_2026-10-27.tsv); probable: 0x0064A37B,
  0x006AD220, 0x006AF520, 0x006B1900, 0x006BD780.

Label corrections (NOTES.md): 0x00644334 CDrawStatUpdates::Draw, 0x0064DD4D CDrawSpiritTargetingCursor ctor, 0x0065FFCA
`find_if<..., CIsHornTaskWithThisHorn>`, 0x00663340 CThingCreatureBase::GetPHeroCombatDef, 0x00686F10 CTCBase::AllocateTC,
0x006F4850 / 0x006F4A00 CDefendingCombatantInfo / CCombatantInfo ctors, 0x006EAB00 CCombatRing::GetAllProngs, 0x006BBFA0
CRegion::IsMapIncluded, 0x006B1900 CAtmosProcess::MixAtmos, 0x006AF520 CMessageEvent::GetPTarget<CThingCreatureBase>,
0x00710700 vector<short> copy ctor, 0x006D63A0 CTCHeroExperience::GetRunningNumHits, 0x006BD780
CMusicManager::CState::GetDanger.

Review items (NOTES.md "Review at landing"): invented landed spellings reused by this batch for callees 0x00655EB4,
0x013BA89C / 0x006870C0, 0x0042B467, 0x00656D20, 0x004433F0, 0x004A9730, 0x004C7990, 0x00438570, 0x005937E1, 0x006BCC10,
0x006B2660, 0x013B8394; const mismatches at 0x00643E09 and 0x006F9130; caller bindings at 0x00710B80, 0x00744D00,
0x007363A0. 0x006D63A0 was graded five times against the four-grade cap.
