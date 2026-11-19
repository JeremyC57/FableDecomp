# Landing hand4e b1 (2026-11-19): NThingMap::InitLayerInfo (0x006379D0), CMessageEventFilter_IsOriginatorAlive::operator() (0x006AF2D0)

Both rows pass strict vland --land: RELOCATION_MATCH + PASS, no flags.

**0x006379D0** is a namespace function, NThingMap::InitLayerInfo, per Ego_r + FableWin.
- NAME_FIXES:
  - The catalog / reloc spelling CThingMapManager::InitLayerInfo is wrong.
  - 0x013BA84C = NThingMap::MaxLayerCellSize.
- New opt-in FABLE_RETAIL_CTHINGMAP_INITLAYERINFO: declarations only, appended at the end of engine/NThingMap_CThingMap.h.

**0x006AF2D0** uses existing opt-ins only (ORIGINATOR_ORIGINAL + IS_THING_ALIVE).

**Header-user recheck:** NThingMap_CThingMap.h, 141 / 141 good.
