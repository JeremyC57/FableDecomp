# Chunk 905 (skipped-row ledger: the two functions of catalogue row 00837040)

- 00837040 -- CThingTrackNode::DrawIsDrawable(): vtable 0x01272CE4 slot 27 (the CThing DrawIsDrawable slot, as
  CThingHolySite/CThingMarker/CThingSwitch/CThingVillage at the same slot). `return GetDisplayEngine()->IsDrawingEnabled();`
  with the virtual call as a tail jump. Manifest label "vfunc_27" is the slot number.
- 00837050 -- CThingTrackNode::GetDrawRecordA(): no callers, no vtable slot. Reads the first dword of the packed draw
  record at +0xe6 of the global mesh bank, the same record DrawGetDrawSizeX @ 0x00837060 reads at +0xee. Modelled with
  the same `#pragma pack(1)` record as 0x00837060.
- Both RELOCATION_MATCH on the first grade; tests pass under g++ and VC7.1.
