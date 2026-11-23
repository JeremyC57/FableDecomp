# Landing nf302 (Account A, 2026-11-23): T1 "other" class / namespace, NF301's continuation (14 rows, 6 header opt-ins, 21 table lines)

Hand-back: `pending/2026-11-23_nf302_handback/`; helpers under `naming_batch/agent_tools/nf302/`. 74 entries reached (lines 1-65, plus 72
and 119-126 pulled ahead): 12 done, 2 done-already, 5 done-partial, 35 skipped-port, 20 skipped-held, no rejects; no vtable change.

- Header batch 1 (NF301's entries 14, 15, 44, 46, 75): add-only, declaration-only opt-ins just before each class's closing `};`:
  `CPixelFormat.h` `FABLE_RETAIL_CPIXELFORMAT_FLAT_GETINIT` (binder 0x00993F60), `CPlayer.h` `FABLE_RETAIL_CPLAYER_PREPAREFORLOAD`
  (0x00449C30), `CResource.h` `FABLE_RETAIL_CRESOURCE_FLAT_SETRESOURCESIZE` (0x009C8470), `CResourceList.h`
  `FABLE_RETAIL_CRESOURCELIST_FLAT_UNLOADNEXTRESOURCE` (0x009FC210), `CTCBuyableHouse.h` `FABLE_RETAIL_CTCBUYABLEHOUSE_FLAT_GETPRICE`
  (0x0072E940, which also drops its stand-in struct and two casts). The unpinned asserts in CResource.h / CResourceList.h are pinned to
  their HEAD lines (`FABLE_STATIC_ASSERT_AT`). equiv_check: 0 of 8,772 includers differ; the planted control flags 6,218.
- Header batch 2: `CTCInventoryItem.h` flat branch gains three static `__fastcall` declarations (GetEngineGraphic for 0x00581CFD /
  0x00581FA4, GetStartingRightHandedSet for 0x005BCD2D, GetContextSensitiveType for 0x00581D8E); 0 of 1,521 includers differ, control flags 148.
- In place: 0x006412B0 local CTCGraphicAppearance into `NGraphicAppearance::`; 0x00ED99D0 parent struct into `NScript::` (six ProcessSongs*
  calls respelled, 0x00EDAAD0 among them); 0x005BD4EA GetHighlightedItem protected const; 0x005BDA1D GetInventoryText const, landing together
  with the owner 0x005BD273's const self fix (full `// Leaf:`), which resolves seven landed const callers HEAD left unresolved.
- Proof: objsec ALL_EQ, one rename per row; links resolve the stand-ins HEAD left unresolved, no duplicates. Staleness: all 14 orig/ copies
  and the 6 headers equal HEAD at landing (after nf299 / nf300 / nf301); no nf299 / nf300 row includes the six headers.
- Table: 10 replace, 10 delete (3 stale witnesses: entries 53, 119, 120), 1 rewitness: `?GetPlayer@CPlayerManager@@QAEPAVCPlayer@@J@Z`
  from the stale 0x0066FB80 to 0x00449BF0, whose object binds it (`logs/witness_getplayer_head.log`).
- Queued: 35 T3 ports, entry 4 among them (GetUnusedFrames: a CResource base on the flat CManagedTextureData is a layout change, so a port),
  plus the local-model debt of 0x00ED99D0 / 0x006412B0 (namespaced shared headers exist); NAME_FIXES_QUEUE: owners 0x007C21A0
  (`NThingShot::CTCShotDefinitionBase::Update`, which also unblocks the withdrawn entry 72, 0x00704150) and 0x005BCAE3
  (`NInventory::CTCInventoryBase::GetHighlightedItem` protected const).
- Rulings (standing brief): Q2 a binder bound to the PDB name whose owner still defines a stand-in lands (unresolved before and after; it
  resolves when the queued owner fix lands); Q5 an owner's own const / access self fix is in place when objsec is ALL_EQ and every landed
  binder of the old spelling moves in the same landing (not NF282 D2, which is about re-scoping the self); Q3 reading the owner's HEAD
  object symbol when `self_leaf` is empty goes on the ROADMAP.

## Recheck
`logs/recheck.log` (landed tree, 14 rows + witness 0x00449BF0): 15 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT.
