# hdr overlay for row 0x00607330 CTCInventoryMap::ConstructLogbookList (full copies of the current repo headers + opt-ins)
- engine/CTCInventoryMap.h: FABLE_RETAIL_CTCINVENTORYMAP_LOGBOOK -- CategoriesList retyped std::map<ECategory, CComponent*> (same 12 bytes at +0x144) + ConstructLogbookList / GetMenuEntryFromCategory declarations.
- engine/CTCInventoryItem.h: FABLE_RETAIL_CTCINVENTORYITEM_STARTINGRHSET -- static __fastcall GetStartingRightHandedSet(long) declaration (0x005D9730).
- engine/CHeroLogBook.h: FABLE_RETAIL_CHEROLOGBOOKENTRY_ACCESSORS -- inline GetAbbreviatedName / GetContent and CHeroLogBook::GetLogEntries (FableWin.pdb names).
- stlport/fable_containers.h: FABLE_RETAIL_STLP_LIST_INSERT_DISPATCH_NOINLINE -- list::_M_insert_dispatch body visible, __declspec(noinline) (retail calls 0x0042B4C3 at both CDeletion copies).
(Session-1/2 probe opt-ins were removed; a backup of the probe overlay is in ../hdr_probe_backup/.)
