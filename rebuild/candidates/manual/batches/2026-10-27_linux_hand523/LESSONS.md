# Chunk 523 lessons

## GOTCHAS one-liners
- (chunk 523) VC7.1 keeps an x87 value on the FPU stack across a call only when the callee's body is in the same TU, so its FPU depth is known. A same-TU noinline body of the real callee (GetRingRadius 0x006E8850) turned a frame spill into retail's st(0)-across-call (0x006E89B0).
- (chunk 523) A same-TU noinline STLport `_M_insert_overflow` body lets a local vector's finish live in a register across the loop's calls. 0x006ED030 (449 bytes) matched on the first grade with it; the same fix as 0x00766920.
- (chunk 523) A value read inside an inlined `a + b` helper may be read after the other operand's call; a named local sequences the read before the call (0x006E89B0 defender radius).
- (chunk 523) CCombatSequenceBase vtable 0x01278FEC: 1 OnCreate, 2 GetName, 3 GetCombatSequenceType, 4 GetCombatSequenceInterruptionType, 5..11 the seven PDB bool defaults in order, 15 GetZone. CCreatureActionBase 0x01259D24: 15 IsStrikeAction, 27 IsActionReadyForInterruption.
- (chunk 523) CResource +0x04 is CIVCountedPointeeBase::IVRefCount, and slot 1 is `Delete() const`; the PDB inline names are AddRef/Release (not Lock/Unlock/DeleteThis).
- (chunk 523) Retail CThing PhysicsTC is +0x60 (engine/CThingCreatureBase.h calls it `q`); CTCPhysicsBase::Radius +0x34; CCombatantInfo DefenderInfo +0x34 (Attackers count +0x38, CombatWheel +0x40); CCombatManager::Combatants +0x10.
- (chunk 523) CBasicString<char>::operator== inlined at two of three call sites in one function (0x006C9350) is not reproducible: forceinline, plain inline and an inline Compare body all fail, and the inline Compare stops VC from inlining operator== anywhere (233v312).
- (chunk 523) The VC/Wine behaviour-test compile has no STL headers (`<map>`/`<string>` gives TCC_FAIL); write tests with plain arrays and `<stdio.h>`/`<string.h>`.
- (chunk 523) Reading a 0.0 double from a global instead of the literal made 0x00723E50 worse (314 vs 306); the cos(0.0) hoist above the virtual call persists either way.

## Review at landing
- 0x0076D2F0 CTCEnemy::AreThingsEnemies (DIFFER 369v356, grades exhausted). Retail hoists 0 into ebx, keeps the bool in bl and uses a 4-word frame. Lead: give CalculateEnemyTC 0x0076CFE0 (same retail TU, landed in this chunk) a same-TU noinline body, as for GetRingRadius.
- 0x006C9350 CPersistTraits<pair<long, COpinionDeed*> >::TransferIn (DIFFER 314v312): retail's mixed inlining of CBasicString::operator==. Possibly a lane gap.
- 0x006E89B0 GetAttackerPlacementVector (DIFFER 344v344, same length): only the epilogue pop/x87 interleave differs. Also check the argument order (attacker, defender) of the inlined anonymous-namespace GetPreferredCombatDistanceForAttacker, and the PDB spelling `GetPrefferedCombatDistance` (sic).
- 0x00723110 vector<CGenerationPos>::_M_insert_overflow (DIFFER 300v300): esi/edi swap only.
- 0x00723E50 SetRHSetAsFlushOnNormal (DIFFER 306v310): x87 scheduling. AnglesToDirection is a descriptive inline helper with no retail symbol, and the class model has 93 v-numbered placeholder virtual slots; name them before landing.
- 0x008B0750 IsNextActionInSequenceStrikeAction is by elimination (probable): its body calls action slot 16, which the labels call IsPartOfComboChain.
- 0x005DA7D0 CTCWeapon::GetWeaponType vs the label NInventory::CCategory::GetCategoryIdentifier: possibly an ICF fold; keep both if the PDB proves the other.
- 0x006FC7A0 / 0x006FC950 / 0x006ED030 / 0x00766920 / 0x007C0E80 / 0x006E89B0 carry same-TU noinline bodies of real retail callees (STLport helpers, GetDef<CThingShotDef>, GetRingRadius); at landing the retail COMDATs win.
- NAME_FIXES.tsv: 62 addresses. Key fixes: 0x0076D2F0 AreThingsEnemies (5 stand-ins), 0x006F0250 `_M_find` (9 map stand-ins), 0x00666D10 GetCombatTypeDef (returns a const reference), 0x00662410 IsInMeleeAsAttacker, 0x00662650 FinishCurrentAction, 0x0085FB30 NCreatureMode::IsAbleToBeAttacked, 0x006E89B0/0x006E87D0/0x006E7FD0/0x006E8850 (the nested CDefendingCombatantInfo classes), 0x006EA2C0 PerformSequenceReset (private, by reference), 0x0078B230 GetDef<CThingShotDef>, 0x009AD9E0 GetPDefFromGlobalIndex, 0x006B9460 CFaction::IsAlliedWithFaction.
- No RELOC_CONFLICT / RELOC_DUPLICATE in the vland dry run.
