# Landing 2026-11-04zf: chunk 770 (23 groups-ledger rows: 11 tiny, 12 Fable bodies), 10 landed

## Result
- **Agent result:** 12 of 23 matched (1 MATCH, 11 RELOCATION_MATCH, every behaviour test PASS). Each was graded read-only on the
  merged tree (`logs/new2.log`).
- **Landed 10** (`logs/land.log`):
  - CThingManager::RuntimeSaveEntities 0x005239C0
  - the NUISystem::CIconText / CTable `vector deleting destructor` adjustor{4} thunks 0x00543B30 / 0x00553FD0
  - NInventory::CTCInventoryBase::RemoveAllConfiscatedItems 0x005BF969
  - CTCCreatureNavigation::IsPosNavLineOfSightToPos 0x006CB610
  - CTCWillLightning::SetAICasting 0x0079F960
  - NThingShot::CTCShotDefinitionBase::GetPWeapon 0x007BEFF0
  - ~CJoystickDX 0x00AB6A90
  - ~CEnginePrimitiveAttachmentManager 0x00B39F70 (the catalog had ~CTCInventoryStats)
  - NScript::CPersonalScript_EmptyScript::CWhispie::Main 0x00EF1690
  - All are plain C++, with no asm, helper, pragma or `volatile`.
- **Held, not landed (2):** 0x00C44E80 and 0x00C45980 (libpk). They byte-match, but the only names are one-source descriptive
  ones (CAmbientEmitter::Update, CAmbientCube::UpdateLevel). Landed callers bind them as `?Method_00c44e80@CObject_00c44e80` and
  `?SetScale@CParticleMorphs`.
  - Landing them would add a third spelling while the vendor naming pass is working on libpk. Their sources and tests are in
    `pending/2026-11-04_linux_hand770/held_vendor_naming/`.
  - They go to vendor naming wave V2.
- **Parked 3** (`differ/`):
  - 0x00E0CA50 CUndeadRisingFirefly::Main (11 grades). Retail calls ~CScriptThing out of line for the loop temporary.
  - 0x00E16E00 CWB_Guard1::Main (10 grades). Retail expands the predicate destructor but calls the member ~CScriptThing out of line.
  - 0x00E17320 CWB_Guard2::Main (1 grade). It is the byte twin of Guard1.
- **Not attempted (8, identities in LABELS/STATUS):** 0x00E11670, 0x00E137B0, 0x00E1F7E0, 0x00E62D60, 0x00EE8390, 0x0078C900,
  0x00828710 and 0x00785160. 0x00785160 is unidentified; it rounds with a bare `fistp`, which makes it a retail_asm candidate.

## Headers (7 changed, new sub-macros, `default=same`; new `NUISystem_CTable.h`, `NThingShot_CTCShotDefinitionBase.h`)
- New sub-macros:
  - FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
  - _CTCCREATURENAVIGATION_LINEOFSIGHT
  - _CWILLBUTTONHELPERTIMER_SETAICASTING
  - _CTCWILLLIGHTNING_AICASTING
  - _CJOYSTICK
  - _CJOYSTICKDX
  - _CENGINEPRIMITIVEATTACHMENTMANAGER_LIST
- `ppcmp.py`: all 49 includers read the same.
- **Header debt:** 0x005239C0 copies its landed sibling 0x00523B40's local model and carries the same four SHARED_HEADER_AVAILABLE
  warnings. Move the two to shared headers together.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04zf.tsv` (20 labels). They cover the landed rows, the parked and unattempted
  rows' two-source identities, and the callees 0x0079F810 (SetLevel, private) and 0x00B3A760 (`std::_List_base<...>::clear`).
- **Kept out of the labels** (`finish_landing.sh` now filters "descriptive", "not proven", "unidentified" and "unverified"):
  - the libpk descriptive names;
  - g_bRuntimeSaveBinary 0x0137548C (no PDB data symbol);
  - CTraderToEscort::Init (outer class not proven);
  - CCS_OakValeRevisitedScript::OakVale (PROPOSAL);
  - 0x00785160.
- **Coordinator fixes:** two `??_E` mangled names and one garbled `_List_base` name were rewritten to catalog form.
  `finish_landing.sh` now warns on any mangled or garbled label.
- **Checks:**
  - NAME_FIXES (16) are queued under `land0411zf`.
  - `bindings_multi_check.py` prints 0.
  - `sync_manifest_from_catalog.py --dry-run` prints 0 / 0 / 0.

## Coverage
46,853 genuine of 49,568; 5,842,574 function bytes (57.17%).
