# Landing hand4d b4 (2026-11-19): CThingSearchTools::GetAllThingsInRoughArea<CThingGameObject, list, CFilter_Bind4<..>> (0x0085B2A0)

**Result:** strict vland --land gives RELOCATION_MATCH + PASS; no flags.

**IDENTIFY:** the catalog label (GetPBestThingInRoughArea<CFilter_Bind3<..>>) was a byte-twin propagation. The real instance comes from Ego_r 0x0083B010 / FableWin
0x0282DAA0 plus a callee match: Bind4<IsNot, CombatIsStrikeToStrike, CombatOverlappingSphere, CombatCanCollideOrInteractWithThing>. It is the twin of the landed 0x0085B5B0.

**Source:** the shared ROUGHAREA_EGOD body (the 0x00878430 recipe), the PDB filter classes and the sphere test in FableWin's statement order.

**Forced inlines:** new opt-in FABLE_RETAIL_CFILTER_BIND4_FORCEINLINE in CFilter_Bind4.h. Both forced inlines carry `// Inline-budget:` and `// Inferred:` lines.
- Plain inline gives 622v775 bytes; forcing only the sphere test gives 581v775.
- Retail also keeps an uncalled out-of-line Bind4 copy at 0x0085B160, so the force is a budget lever, not the source's spelling.

**Header-user recheck:** CFilter_Bind4.h, 5 of 5 good.

**NAME_FIXES.tsv:**
- the 0x0085B2A0 instance spelling;
- 0x0049C6F0 ?GetAllThingMapsInArea@CThingSearchTools@@... (Ego_r 0x004A6580 + FableWin 0x01CEDC40), replacing the descriptive catalog label.
