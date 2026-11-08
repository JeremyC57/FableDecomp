# Landing 2026-11-08aa: chunk 880 (CTCInventory::OpenPCOptionsMenu, Opus high), 1 of 1

0x0058AF10 CTCInventory::OpenPCOptionsMenu (13,048 bytes incl. its 17-entry jump table): RELOCATION_MATCH + PASS (VC7.1 and g++),
no flags, ~20 grades. Hand-back: `pending/2026-11-08_linux_hand880/`.

- Inline budget: retail expands `CCharString::operator==(const char*)` for the `GetName() == "PC_OPTIONS_LIST"` test; `/d2inlS`
  overshoots elsewhere, so the row uses the shared opt-in `FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ` (line 2 `// Inline-budget:`).
- Shape: STLport's own inline `push_back` (not the out-of-line opt-in) keeps `list` unaliased in a register and gives the 0xE4
  frame; a separate `case SURFACE_MULTISAMPLE_NONE` body starts the jump table at 0; named locals for colour / redefiner list /
  quit pictures.
- Headers (all additive opt-ins, default path unchanged by hdrcheck): stlport/fable_containers.h `FABLE_RETAIL_STLP_MAKE_PAIR_ARRAYS`
  (STLport 4.6 string-literal make_pair), CTCInventory.h `_OPTIONSMENU` (CURRENT_OPEN_MENU, CurrentOpenMenu +0x21C), CTCInGameMenu.h,
  NUISystem_CRedefinerList.h, CUserProfileManager.h, CDisplayManager.h; new CDisplayModeDesc.h, NUISystem_CActionParamEBOOL.h.
- Names: labels `ghidra_out/labels_landing_2026-11-08aa.tsv` (SetAcceptButton 0x00556570, GetDisplayModes 0x00406880 -- an ICF
  fold, check label_trust.py); 0x00592495 respelling queued as land0811aa (pair<CCharString,CCharString> from const char*, a list
  insert, not a map).
