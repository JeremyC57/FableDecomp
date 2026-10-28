# Chunk 523 (agent523, 2026-10-28): 15 of 20 re-queued single functions landed (166-476 bytes)

Re-queue chunk of 20 rows of 166-476 bytes (0x00679560..0x007C0E80): combat manager and wheel, hero experience, special
abilities, enemy TC, object augmentations, creature actions, persistence traits and STLport vector / merge helpers. One
Opus 5.5 agent at high effort.

- **Landed:** 15 rows with `vland.py --land`, all RELOCATION_MATCH, every test PASS under g++ and VC7.1; no RELOC_IDENTITY,
  CONFLICT or DUPLICATE.
- **Parked (5, pending/2026-10-27_linux_hand523/differ/):** 0x006C9350 (314v312, retail inlines CBasicString::operator==
  at two of three tests and calls it at the third), 0x006E89B0 (epilogue pops against x87), 0x00723110 (esi/edi swap),
  0x00723E50 (x87 scheduling), 0x0076D2F0 CTCEnemy::AreThingsEnemies (369v356, retail keeps zero in ebx).
- **Identities:** all 20 row names proven; probable 0x008B0750 IsNextActionInSequenceStrikeAction (by elimination) and
  0x006E8600 IncrementDefenderAttackPattern. The agent removed invented AddTC / Lock / Unlock / DeleteThis stand-ins from
  0x00766920 and 0x007C0E80 before handing over (regraded RELOCATION_MATCH).

Name fixes (NAME_FIXES.tsv, 62 addresses, deferred to the naming pass): AreThingsEnemies (5 stand-ins), the combatant-map
`_M_find` 0x006F0250 (9 stand-ins), GetCombatTypeDef returns a const reference, PerformSequenceReset is private and takes
a reference, GetDef<CThingShotDef>, GetPDefFromGlobalIndex, CFaction::IsAlliedWithFaction. About 30 catalog labels were
wrong (NOTES.md).
