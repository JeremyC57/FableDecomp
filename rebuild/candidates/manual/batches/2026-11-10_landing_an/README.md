# 2026-11-10 landing an -- sound-manager names (agentSND): CASoundManager.h named, 40 rows respelled, 2 held rows landed

Source: `pending/2026-11-10_linux_handSND/` (SLOTS.tsv, NOTES.md, RENAMES.tsv). Owner rule of 2026-11-10 on made-up names applied.
- engine/CASoundManager.h: all 54 virtual slots named. 52 real names from the Fable Anniversary Xbox 360 PDB publics, aligned through
  the Anniversary PC RTTI vtables (slot map in SLOTS.tsv), each with a TLC second source (a PDB-named caller, an Ego_r / FableWin
  ConsoleAudio_* stub, or the body). Two descriptive (no original anywhere): slot 9 Get2DEmitter, slot 51 ExtractMetaData.
  Opt-in ENABLE_NAMES adds EnableMusic / EnableSound / EnableAtmos. New headers: CASoundEmitter.h (the CSoundManagerLH+0x44 object's
  base, vtable 0x0129E684, 23 real + 2 descriptive slot names), CSoundInit.h, CInventoryDef.h (partial). Opt-ins: CASoundBank
  SOUNDINDEX, CTCInventoryAbilities PRIORITISED, CTCInventoryTrade OPENINVENTORY / ACTUALINVENTORYPC, CTCInventoryBase PDEF, C2DBoxF
  CORNERS_FORCEINLINE. hdrcheck2 default=same for every header except CASoundManager.h (renames only; virtual calls do not bind
  symbols, and the two landed includers 0x00417A58 / 0x00991840 recheck RELOCATION_MATCH).
- Respelled (vland --recheck-landed, all MATCH / RELOCATION_MATCH + PASS, logs/vland_recheck_respells.log): the 10 rows that used
  the placeholder slot names, and 30 self rows renamed (rerow: files, ps1, catalog, manifest), e.g. CSoundManagerLH vfuncNN rows,
  AddBank -> NewAtmos, AddSound -> NewChannel, FlushLoadingQueue_00991670 -> CASoundManager::Update, the two CWorld_SetMiniMapEnabled
  rows -> EnableMusic / EnableSound, 0x00991620 -> EnableAtmos. Coordinator fixes at landing: ten old tests linked the row object under
  the old names or printed a fixed string -- replaced with self-contained host-model tests; 0x00A3EB20 declares Update virtual
  (UAE, as retail's slot 3); 0x00A3ED90 / 0x00A3EED0 return the CSoundBankLH the established FindByName / FindByNamePair spellings use.
  reloc_bindings: five stale entries removed, three added (EnableMusic / EnableAtmos from 0x006286F0, CASoundManager::Update from
  0x00A3EB20).
- Landed (vland --land RELOCATION_MATCH): 0x0056F30B CTCInventoryExperience::CloseInventory, 0x0058F649 CTCInventory::OpenPCInventory.
- Parked (differ/): CycleAssignedAbilities 0x005F64DD DIFFER(512v508), CycleAssignedExpressions 0x005F6793 (445v442),
  CTCInventoryTrade::OpenInventory 0x005A94BD (1488v1482), ConstructActualInventoryPC 0x005ACEDA (1113v1067).
Queued land1011al: agentSND's 27 name fixes + four caller respells found at landing.
Debt: about 90 landed rows still model PSoundManager locally; the CSoundEmitterLH vfuncN self rows are not yet respelled.
