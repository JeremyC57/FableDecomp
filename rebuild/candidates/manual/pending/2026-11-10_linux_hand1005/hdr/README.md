# Chunk 1005 header overlay (`FABLE_HDR_OVERLAY=$A/hdr`)

Every file here is a copy of `rebuild/include/engine/<same name>` at HEAD (checked after Codex pass 16, b244ef6c1: no HEAD
line is missing from any overlay copy) plus additions only, each behind a new `#ifdef FABLE_RETAIL_*` opt-in. The default
path (no new macro defined) is byte-identical to HEAD. Every row was graded with `FABLE_HDR_OVERLAY` exported.

| Header | Opt-in | Adds | Evidence / rows |
|---|---|---|---|
| CTCInventoryExperience.h | `FABLE_RETAIL_CTCINVENTORYEXPERIENCE_MENULIST` | `std::vector<long> MenuDisplayListExperienceItem; // +0x208` | Ego_r.pdb member; 0x0056EF2D (differ) |
| CTCInventoryExperience.h | `FABLE_RETAIL_CTCINVENTORYEXPERIENCE_VIRTUALS` | virtual overrides `UpdateMenuScrollButtonsItem()` (slot 29) and `CloseInventory()` (31), and the class's five new virtuals in retail slot order 86..90: `OpenInventoryInternal(bool)`, `OpenInventoryInternalPC(bool)`, `OpenInventoryOld()`, `CloseInventoryInternal(bool)`, `CloseInventoryInternalPC(bool)`; under this opt-in the default-path `OpenInventoryInternalPC` declaration (which would otherwise take slot 86) is skipped via `#ifndef` | vtable 0x0124F11C read from the exe (29 0x0056EF2D, 31 0x0056F30B, 86 0x00577216, 87 0x00577AC0, 88 0x00574576, 89 0x0057770A, 90 0x005778CA); Ego_r.pdb names; rows 0x0056F30B (calls slot 89), 0x00577216 |
| CTCInventoryExperience.h | `FABLE_RETAIL_CTCINVENTORYEXPERIENCE_UNDO` | public `UndoBuyAbility(EHeroAbility, bool)`, `UndoBuyStat(EHeroTrainableStatType, bool)` | Ego_r.pdb / FableWin.pdb QAE; rows 0x00576C4F / 0x00576ED1 |
| CTCInventoryExperience.h | `FABLE_RETAIL_CTCINVENTORYEXPERIENCE_PRIVATE` | private `RecheckAvailabilities()`, `ConstructCategoryStrength(CList*)`, `GetXString(long)` 0x0056EA4E, `AddMagicCategoryToList(...)` 0x0056F69F, `AddMagicCategoryToListPC(...)` 0x0056F8A4 | Ego_r.pdb AAE spellings; rows 0x005726A0, 0x00574885, 0x005759BF, 0x00577216 |
| CTCInventoryAbilities.h | `FABLE_RETAIL_CTCINVENTORYABILITIES_MAXLEVEL_FIRST` | declares `GetAbilityMaximumLevel` before `GetAbilityLevel` (else the original order) | VC7.1 evaluates the later-declared callee of `f() < g()` first; rows 0x005726A0 (match), 0x00570332 / 0x00571280 (compare fixed) |
| CTCInventoryAbilities.h | `FABLE_RETAIL_CTCINVENTORYABILITIES_SETLEVEL` | `void SetAbilityLevelTo(EHeroAbility, long); // 0x005EE01A` | Ego_r.pdb / FableWin.pdb; row 0x00576C4F |
| CTCHeroExperience.h | `FABLE_RETAIL_CTCHEROEXPERIENCE_MODIFYLEVEL` | `void ModifyTrainableStatLevel(EHeroTrainableStatType, long); // 0x006D6810` | Ego_r.pdb; row 0x00576ED1 |
| NUISystem_CYesNo.h | `FABLE_RETAIL_CYESNO_CALLBACKS` | public `virtual void SetYesCallBack(void (__fastcall*)())` (slot 142) / `SetNoCallBack` (143) | Ego_r.pdb; rows 0x00576C4F / 0x00576ED1 |
| NUISystem_CManager.h | `FABLE_RETAIL_CMANAGER_MENUCOUNT` | `static void __fastcall OnOpenMenus(); // 0x0041D122`, `static void __fastcall OnCloseMenus(); // 0x0041D165` | FableWin.pdb 0x01B10164 / 0x01B101F9, Ego_r.pdb 0x0042D956 (67 B) / 0x0042D999 (51 B) = retail sizes; row 0x0056F30B |

All additions are declarations or a reordering of declarations that exist at HEAD (no layout change; the VIRTUALS opt-in
changes vtable slot assignment only for TUs that define it, which then match retail).
