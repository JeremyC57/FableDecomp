# Landing 2026-11-08ac: single-row worker 884 (CTCBase::RegisterTCs, Opus xhigh), 1 of 1

0x004EE23F CTCBase::RegisterTCs (44,806 bytes, one `ret`): RELOCATION_MATCH + PASS (VC7.1 and g++), 4 grades, no flags.
Hand-back: `pending/2026-11-08_linux_hand884/` (tools/regs.py + gen.py generate the source from the retail statement list).

- Real name from Ego_r.pdb (0x0050C7CD) + FableWin.pdb (0x01B846AD), sole caller CMainGameComponent::Init 0x00418585; the catalog's
  `Init_RegisterGameComponents` was descriptive (also on 0x0073B130, which is CTCCreatureModeManager::RegisterModes: both renamed).
- What it does: empties CTCBase::RegisteredTCs, eight StatRegisterClass setups, 340 component registrations (name, allocator, driver
  type) with 111 definition-class registrations interleaved, RegisterModes, then RegisteredTCs.Update().
- Devices: file-level `#pragma optimize("s", on)` (a size-optimised TU in retail); the expanded-then-called split (41 / 41 / 8) is
  VC7.1's own inline budget from plain source. CVectorMap::Insert builds `value_type(key, value)` (make_pair: DIFFER 46123v44806).
- Headers (additive opt-ins, hdrcheck default path unchanged): CTCBase.h `_REGISTRY`, CVectorMap.h `_INSERT_VALUE_TYPE` + `_SORT`,
  CDefinitionManager.h `_ADDDEFCLASS`, NHitLocations_CHitLocation.h `_STATREGISTERCLASS`; new CDefClassInit.h.
- Header debt: the 451 registered classes are declared in the row with only their registration statics (no layout); 136 of them have
  generated engine headers that declare none of these statics and do not compile together (58 SHARED_HEADER_AVAILABLE warnings).
  CVectorMap::Insert now has two bodies (0x005DD670 matched on make_pair; redo it on the value_type form).
- Names: labels `ghidra_out/labels_landing_2026-11-08ac.tsv`; 21 respellings queued as land0811ac (StatRegisterClass x8,
  CVectorMap::Sort 0x004EBACE landed as CGuiVarTransferBool::TransferToGui, the __fastcall allocator convention, the registry symbol).
