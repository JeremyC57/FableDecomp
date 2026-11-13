# Header overlay, chunk 2025 (export FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2025/hdr)

All changes are opt-in or new files; landed includers of the repo headers keep their bytes (both rows of this chunk were regraded
after the last overlay change: 0x00600B60 and 0x00605570 RELOCATION_MATCH).

## engine/CTCInventoryMap.h (copy of rebuild/include/engine/CTCInventoryMap.h + one nested opt-in)
- `FABLE_RETAIL_CTCINVENTORYMAP_REGIONDESCRIPTION_ALL` (needs `FABLE_RETAIL_CTCINVENTORYMAP_REGIONDESCRIPTION`): declarations only, no
  layout change, all `protected` (`IAE`):
  - `ConstructRegionDescription(NUISystem::CComponent*, CRegion&, bool)` 0x00605570 (`ret 0Ch`)
  - `ConstructRegionDescriptionQuests(NUISystem::CComponent*, CRegion&, bool)` 0x005FD5E0 (`ret 0Ch`)
  - `ConstructRegionDescriptionBuyableBuildings(NUISystem::CComponent*, CRegion&, CUIRegionInfo*, bool, bool)` 0x00600B60 (`ret 14h`)
  - `ConstructRegionDescriptionMapKeys(NUISystem::CComponent*, CRegion&, bool)` 0x005FB7E0 (`ret 0Ch`)
- Evidence: (1) Ego_r.pdb and FableWin.pdb name sets both list these four exact mangled names (ghidra_out/egor_pdb_names.tsv,
  fablewin_pdb_names.tsv); (2) retail: 0x00605570 calls the other three (and Shops) with its own parent / region / pc
  (0x00605A99 Quests, 0x00605B55 Shops, 0x00605B6D BuyableBuildings, 0x00605C45 MapKeys), the `ret` sizes match the parameter
  lists, and the literals identify each body (UI_/PC_QUEST_NAME and *_ICON_AND_NAME in 0x005FD5E0; UI_/PC_MAP_KEYS_CONTAINER in
  0x005FB7E0; TEXT_GUI_MENU_HOUSE(S)_OWNED ... in 0x00600B60).

## engine/CTCBuyableHouse.h (copy of the repo header + one nested opt-in)
- `FABLE_RETAIL_CTCBUYABLEHOUSE_ISFORSALE` (inside `FABLE_RETAIL_CTCBUYABLEHOUSE`): declaration `bool IsForSale() const;` 0x006BF960.
- Evidence: Ego_r.pdb `?IsForSale@CTCBuyableHouse@@QBE_NXZ`; reloc_bindings.tsv binds that symbol at 0x006BF960 (landed row
  CTCBuyableHouse_IsForSale_006bf960.cpp, caller 0x0082ACD0); 0x00600B60 calls it on the TCI_BUYABLE_HOUSE (213) component.

## engine/CRegionDef.h (new; no repo header existed)
- Retail model of Ego_r.pdb CRegionDef: base CParentDefClassBase (retail 0x39 bytes -> first member +0x3C; Ego_r 0x3D -> +0x40), the
  PDB members at retail offsets (every member after the 0x20-byte retail CSoundMap sits 0x0C below its PDB offset). Opaque members keep
  their real type in a comment (CSoundMap, CDefString, the two enums). Needs FABLE_RETAIL_DEF_CLASS_BASE.
- Evidence: the landed CRegionDef::Transfer 0x004715AB (every member's retail offset, its local model) and CRegion::GetWorldMapOffset
  0x006BBF40 (WorldMapOffset +0xAC); 0x00605570 reads RegionDescription (Ego_r +0xAC, unsigned long) at +0xA0 (0x006058EB, 0x0060595E)
  and passes it as SetValue(unsigned long, const CWideString&, bool)'s text id.
- Used by CRegion::GetPRegionDef() (FABLE_RETAIL_CREGION_PREGIONDEF, existing opt-in) in 0x00605570.
