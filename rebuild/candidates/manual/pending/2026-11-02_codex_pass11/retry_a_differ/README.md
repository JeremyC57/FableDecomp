# Codex pass 11, retry a: parked rows

The retry agent tried each of 23 Codex pass 11 rows at least 10 times (probe counts are in STATUS.tsv).

- **Matched (2):** 0x00520670 CThingManager::LoadHeroStateBinary and 0x00521620 NThingManager::CEntityRuntimePersistence::SaveToGameStateBinary. Both are RELOCATION_MATCH. They land with the CThing-family consolidation, because their headers overlap that work.
- **Parked as DIFFER (20):** `<addr>.cpp` here is the closest source the agent found. STATUS.tsv gives the size against retail and the remaining difference.
- **Blocked (1):** 0x005EB9E0 CTCTavernGame::FrameUpdate. Retail reads CTCOwnedEntity::POwner, a `CIntelligentPointer<CThing>`, at +0x0D, and the next member, OwnerUID, at +0x18. So the intelligent-pointer classes have alignment 1 in retail. Ego_r has 60 such members at odd offsets.

Before you retry a parked source: line 4 of each one says it grades against the Codex row's own header context, not against rebuild/include. Rebase it onto the landed headers first. NOTES.md and LESSONS.md are the agent's working notes.

The most common blocker (8 rows) is the inline budget: within one function, retail inlines an inline member at some call sites and calls it out of line at others.
