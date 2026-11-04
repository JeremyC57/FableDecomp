# Landing 2026-11-04zb: chunk 764 (retry of 25 parked rows 1-16 bytes off retail, 297-342 bytes, C++ only), 8 landed

## Result
- **Landed 8 of 25** (`logs/land.log`), each graded read-only on the merged tree first (`logs/new2.log`): RELOCATION_MATCH + PASS.
  - **CTCChest::DisplayLockedMessage (0x00750110):** follows the landed sibling 0x0074EE90.
  - **CTCHeroExperience::GetAbilityPossibleLevelUpAmount (0x006D7B80):** a while loop with two counters, which VC7.1 folds.
  - **CCombatActionBase::ResetMeleeCombatAbilityEffects (0x00859660)** and
    **CCreatureAction_PickUpThingAtTable::PerformAction (0x00849F70):** same-TU `noinline` GetDef / GetSubDef bodies, plus
    `!IsDead()` in place of `IsAlive()`.
  - **NPlayerGui::CDrawQuickAccessIcons::ToggleCharge (0x0064D92A).**
  - **CEngineLightingManager::CalculateFogTransformMatrix (0x00B451D0):** the PDB signature (C4DVector by value).
  - **NInventory::CTCInventoryBase::SetNumberOfSelectedItemOwned (0x005BE284):** a retail-header port with PeekTC.
  - **CTCHeroOpinionDeedLog::OnSerialise (0x006C7370),** which the manifest called "CDeedManager::TransferState", an autoname:
    - its identity rests on retail vtable 0x0125EA48 slot 1 under the class's RTTI, plus the Ego_r members;
    - inline `context.IsSaving()`;
    - an inline `std::copy` over STLport `__copy`;
    - `records.reserve(map.size())`.
- **Asm rules (owner, 2026-11-04):** no asm, helper, pragma or `volatile`.
- **Parked 17** (`pending/2026-11-04_linux_hand764/differ/`). **Only 5 of them got the full 10+ grades** (`STATUS.tsv`); the rest
  got 2-9.
  - 0x005EBFA0 is not land-ready: its FindThingsWithScriptName stand-in is fabricated. It needs a real model.
- **Header debt:** the shared CCategory.h is global, but the PDB class is NInventory::CCategory, so 0x005BE284 keeps a one-method
  local class.

## Headers (new FABLE_RETAIL_* sub-macros only; `default=same`)
- Among them: CTCBASE_PEEKPLAYERMANAGER, CTCINVENTORYBASE_SELECTEDOWNED, CMATRIX3X4_PREMULTIPLY, CENGINELIGHTINGMANAGER_FOGTRANSFORM,
  CCOMBATACTIONBASE_INITIALISE / _MELEE, CTCHEROEXPERIENCE_STATEXPERIENCE and CWORLD_ENVIRONMENT / _MESSAGEEVENTMANAGER. Full list
  in `agent_NOTES.md`.
- **Merges with chunks 761/765:** CTCBase.h merged 3-way cleanly. CWorldMap.h was resolved by hand: 764's `_HEIGHTZ` block sits next
  to 761's `_THINGMAPMANAGER`, and both are kept.
- `ppcmp.py`: all 5,692 includers same.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04zb.tsv` (10): g_pEngineCamera 0x01436EA0, the CTCHeroOpinionDeedLog vftable
  0x0125EA48, OnSerialise 0x006C7370, and the class-qualified names of the 7 rows vland had landed bare.
  - The agent's LABELS.tsv uses a `label` column; `finish_landing.sh` now accepts it.
  - The first finish run had also queued the NAME_FIXES twice, and the duplicate lines were removed.
- **Queued PRIORITY** (`land0411zb`, 10), among others:
  - 0x005BDF08 GetNumberOfItemsOfTypeInInventory is const `QBEJJ`, where landed spellings have `QAEJJ`;
  - 0x005BD993 NInventory::CCategory::GetHighlightedItemMaxInCategory;
  - 0x0070D1A0 `CAnimSet::Init(CAnimateThing&, const CAnimInit&, float)`.
- `bindings_multi_check.py`: 0. `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0.

## Coverage
46,796 genuine of 49,568; 5,834,748 function bytes (57.09%).
