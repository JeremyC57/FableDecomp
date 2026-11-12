# Chunk Q23 (six parked rows, one at a time, Opus 5.5)

2 matched and landed in landing dp:
- 0x006C1250 CTCBuyableHouse::FrameUpdate (CTCBase slot 9; the batch's "Update" and the catalog's "vfunc_9" were wrong). Q22's
  local-model match is now ported onto the shared headers. Levers: HasTC through the FABLE_FORCEINLINE opt-in
  CTHING_HASTC_FORCEINLINE (a plain inline leaves the bitset test in a stack byte), and count_if over the const list from
  PeekVillagePeople() const (const_iterator copies trivially; the non-const one is built in its stack slot).
- 0x00814260 CTCBoastingPodium::OnInitialActivate: CTCBase::GetParentThing() in place of the raw member fixed the whole register
  rotation. CTCBoastingPodium and CBuyableHouseDef have no shared header, so they are local classes on Ego_r members.
DIFFER: 0x0063F7C0 ForEachInRoughArea Bind3 (785v785): the post-loop reload order of cells.TLX / BRX / BRY / map, the same residue as
Q19's 0x008780D0 twin. Not ported: 0x00A1D290 (needs CAStarState / CPriorityQueue / CNavigatorQueueManager models), 0x00584B86,
0x00622A00 (chunk Q24 takes the last two).
