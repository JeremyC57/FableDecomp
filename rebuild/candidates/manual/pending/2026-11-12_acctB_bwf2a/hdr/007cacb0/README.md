# bwf2 / 0x007CACB0 header overlay (full copies of the CURRENT repo headers + `#ifdef FABLE_RETAIL_*` additions only)
- engine/CTCHeroReceiveItems.h: FABLE_RETAIL_CTCHERORECEIVEITEMS_GLOBALUPDATE (nested in _PICKEDUPITEMS: GlobalUpdate decl, inline IsReceivingItems/IsDisplayingItems, private AddItemToInventory, members +0x1C..+0xA4 + offset asserts) and _PICKEDUP (HasPickedUpItem/SetItemAsPickedUp decls), re-based from hdr_prior (agent2057 scout) onto the current file (keeps _VSLOTS).
- engine/CThingObject.h: FABLE_RETAIL_CTHINGOBJECT_HASPROPERTY (FableWin inline HasProperty(bit index), `((GetProperties() >> p) & 1) != 0`), from hdr_prior.
- engine/CRightHandedSet.h: FABLE_RETAIL_CRIGHTHANDEDSET_INIT (declaration-only Init 0x004BBE10; do not combine with _CQUATERNION_PARTICLE_UPDATE), from hdr_prior.
- engine/CPlayerGuiDef.h: FABLE_RETAIL_CPLAYERGUIDEF_CONTAINERITEM (ReceiveItemsSoundCriteria as CCharString +0x644; ContainerItemDisplayAreaTLPos/BRPos as C2DVector +0x8C4/+0x8CC), from hdr_prior.
- engine/CTCInventoryItem.h: FABLE_RETAIL_CTCINVENTORYITEM_DESCRIPTION (const GetDescription 0x005D9070 / GetStartingRightHandedSet 0x005D92F0 decls), from hdr_prior.
- engine/CTCHeroStats.h: FABLE_RETAIL_CTCHEROSTATS_RENTMONEY (`#elif` branch: AddMoney decl, inline AddRentMoney/GetRentMoney, RentMoney +0x54), from hdr_prior / agent2039.
- engine/CEnginePrimitiveHandle.h: FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_FORCEINLINE (NEW, bwf2): the _CENGINEPRIMITIVEHANDLE Clear text as FABLE_FORCEINLINE (c2 trace: plain inline scores 74..108 v threshold 74 at both retail-expanded sites).
- NEW headers (no repo copy): engine/CTCFish.h, CTCFishingSpot.h, CTCDiggingSpot.h, CTCGold.h, CTCHeroOnlineScoreboard.h (Ego_r.pdb layouts; from hdr_prior / agent2039).
