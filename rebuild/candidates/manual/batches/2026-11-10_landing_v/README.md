# 2026-11-10 landing v -- chunk 1005, the CTCInventoryExperience TU (6 of 11 rows)

Source: `pending/2026-11-10_linux_hand1005/`. vland --land: 6 RELOCATION_MATCH, tests PASS (VC7.1 and g++).

- 0x005726A0 RecheckAvailabilities, 0x00574885 ConstructCategoryStrength, 0x005759BF ConstructCategoryStrengthPC, 0x00576C4F
  UndoBuyAbility, 0x00576ED1 UndoBuyStat, 0x00577216 OpenInventoryInternal (vtable slot 86; manifest / catalog names corrected from
  "vfunc_86", and 0x0056EF2D from "vfunc_29" to UpdateMenuScrollButtonsItem). Levers in the pending NOTES / LESSONS (e.g. VC7.1 calls
  the later-declared function in `f() < g()` first: FABLE_RETAIL_CTCINVENTORYABILITIES_MAXLEVEL_FIRST).
- Headers (opt-in, default paths unchanged): CTCInventoryExperience.h MENULIST / VIRTUALS / UNDO / PRIVATE; CTCInventoryAbilities.h
  MAXLEVEL_FIRST / SETLEVEL; CTCHeroExperience.h MODIFYLEVEL; NUISystem_CYesNo.h CYESNO_CALLBACKS; NUISystem_CManager.h MENUCOUNT.
- Held, not landed: 0x0056F30B CloseInventory (RELOCATION_MATCH, `held/`): it calls CASoundManager members through the descriptive
  slot names (SetGroupVolume, Virtual09_00a3e3a0) that engine/CASoundManager.h says are not landable until the owner accepts them.
- Parked: 0x0056EF2D (one register choice), 0x00570332 / 0x00571280 AddAbilityToList(PC) (frame 0xDC v 0xD8: one shared slot),
  0x00577AC0 OpenInventoryInternalPC (operand order of a four-field sum).
- Flag fix at landing: the payload carried `flags: ['none']` from `// Flags: none`, recorded as CompilerFlags '/O2 /Oy /W3 none';
  nine entries had it (these six and 0x00E0F8A0, 0x0073ACC0, 0x007534B0 from older chunks). All nine corrected and rechecked
  RELOCATION_MATCH on the base flags. Queued `land1011w`: 13 name fixes.
