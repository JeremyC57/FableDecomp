# Landing nf341 b2x (Account A, 2026-11-24): 0x006EA430 is CCombatManager::SetAttackerAsBeenStruck

Agent NF341's item 2, from B's NAME_FIX for 0x00839940. Hand-back: `pending/2026-11-24_nf341_handback/` (NOTES.md first).
- **Identity:** landed row 0x006EA430 defined `?DeregisterCombatant@CCombatManager@@QAEXAAVCThingCreatureBase@@@Z`, but the real
  DeregisterCombatant is 0x006ED9C0 (table witness 0x007355F0). 0x006EA430 is `?SetAttackerAsBeenStruck@CCombatManager@@QAEXAAVCThingCreatureBase@@@Z`.
  - Ego_r.pdb, combat_manager.obj record 81 (32 B), comes right after PerformSequenceReset, in the same order as retail (0x006EA2C0, then 0x006EA430).
  - FableWin.pdb record 214: its body calls IsInMeleeAsAttacker and then PerformSequenceReset, as retail does.
- **Rows:** the self 0x006EA430 and its only landed binder 0x00734050 moved as a unit (NF304 Q6), through the add-only opt-in
  `FABLE_RETAIL_CCOMBATMANAGER_SETATTACKERASBEENSTRUCK` in engine/CCombatManager.h, which only declares the function.
  - objsec is ALL_EQ under the one rename.
  - Both rows RELOCATION_MATCH before and after. The link no longer carries two strong DeregisterCombatant definitions.
  - equiv_check: 0 of 210 CCombatManager includers differ (planted control 38).
  - Coordinator includer recheck on the landed tree: 33 / 33 PASS.
- **Table:** one add line, with witness 0x00734050.
- **Catalog:** renamed to `CCombatManager::SetAttackerAsBeenStruck` in the catalog, the manifest and the file names. The recheck gives
  RELOCATION_MATCH PASS, and the RELOC_NAME / STANDIN_LEAF warnings are gone.
- **Queue:** NAME_FIXES_QUEUE line land1011cr, which gave the wrong name for this address, is removed and kept in `NAME_FIXES_REMOVED.tsv`. A
  line with a wrong target never reads as closed, and an agent could respell the row back from it.
- **Landing note:** the first run was held by A's own claim line, whose item text named the address. That line is now released.
- **Item 1** (the `GetDef<CMaterialDef>` copy 0x0066C340) was not landed as built. It changed the default header path for four landed
  objects and for 13 definers that were never compiled. NF341 is reworking it: first the existing `FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD`
  macro in the emitting rows, and if that is not enough, a new opt-in.
