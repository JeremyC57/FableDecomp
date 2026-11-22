# Landing nf231 (Account A, 2026-11-22): the nf230 follow-ups (SetQuit spelling, 0x0042EBB6 full header port, two header tests)

Agent NF231. Hand-back: `pending/2026-11-22_nf231_handback/`. Entries q12625, q12627, q12628.

- **Recheck** (`logs/recheck.log`): 3 rows, 1 MATCH (0x00430340) + 2 RELOCATION_MATCH (0x0042EBB6, 0x0041D0A8 test only), all PASS, no
  RELOC_CONFLICT; HEAD control the same. Link: no duplicates (`logs/link_check.txt`).
- **0x00430340 SetQuit:** the self row now spells `?SetQuit@CNewFrontendGameComponent@@UAEXXZ` (public virtual: FableWin, Ego_d, retail
  RTTI vtable 0x01230CA0 slot 4), on the header's landed SETQUIT opt-in; body `Quit = true`. Only the leaf changes from HEAD. No code binder;
  the only reference is the vtable slot, which no landed object binds yet (both vtable emitters model slot 0 only). One spelling.
- **0x0042EBB6 Shutdown, full header port:** every local model is gone (CNewFrontendGameComponent, CDefPointer, vector, CWideString,
  CDateAndTime, CRGBColour, CDisplayManager, CFrontEndDef); `m_pFrontEndDef.Reset(0)` (CDEFPOINTER_RESET); the two header-less stand-ins named:
  `PFontManager->SetLoadFontGlyphsAsynchronously(true)` (Ego_r, CFONTMANAGER_ASYNC) and `CStartupDefinitionManager::Set(0)`
  (`?Set@CStartupDefinitionManager@@SIXPAV1@@Z`, Ego_r + FableWin). Same grade as the landed in-place version. Callee spellings move onto the
  ones other landed rows already emit: 0x004336B0 3 -> 2, 0x00412450 4 -> 3, 0x00411B00 3 -> 2.
- **Header opt-ins** (add-only): CNewFrontendGameComponent.h `FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_FILENAMES_FILETIMES` (FileNames /
  FileTimes as `std::vector<CWideString>` / `std::vector<CDateAndTime>`, Ego_r + Ego_d, RUN branch); CStartupDefinitionManager.h
  `FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER_SET` (declares `Set`). Proof: pin_asserts 0 / 3 pinned, assert_names_check 0 mismatches; equivalence over 57,022 landed rows: only the user 0x0042EBB6 differs (rechecked above).
- **Tests:** new header tests for 0x00430340, 0x0042EBB6 and 0x0041D0A8 (the last two define the private static in the test; no `?g_` model).
- **reloc_bindings:** 3 stand-in lines deleted whose only witness was 0x0042EBB6 (`?SetActiveFrontend_004336b0`, the two pre-STLport `erase`).
- **Pick-any** (`logs/pickany_vs_retail.tsv`): every new copy has no retail owner.
- **Coordinator rulings on NF231's questions:**
  1. `#define private public` in header tests: accepted (tests only; 190 landed tests use it). The narrower form (scoped to the one class
     header, as 0x0041D0A8) is preferred.
  2. The two erase self rows take the STLport spelling their landed binders emit, not the PDBs' debug-STL `_Vector_iterator` one: retail is a
     VC7.1 STLport build; the PDBs are VC10. Queued (NAME_FIXES).
  3. 0x0054F410 / 0x0054F370: queued as row ports first, then tests.
  4. The CNewFrontendGameComponent vtable-name split between 0x0042EA8F and 0x0042F50B: queued as a census + port (must define SETQUIT).
- **Queued** (tag land1122nf231): 0x004336B0 self row `@OperatorNew@4` -> `?Set@CStartupDefinitionManager@@SIXPAV1@@Z`; the 0x00412450 /
  0x00411B00 erase self rows; the two row ports; the vtable split; 5 rows still on HEAD host-model tests.
