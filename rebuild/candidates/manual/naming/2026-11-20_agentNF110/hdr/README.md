# NF110 overlay (round 10, 2026-11-20)

Base copies: `orig/engine/` (repo headers at 14ae8c3b0f, after the nf107 landing; copied at the start of this session). Each overlay is the
repo header plus one additive `#ifdef` opt-in. Without its macro every overlay is token-identical to the repo copy (the added text sits
entirely inside the new `#if` / `#ifdef` block). No landed row defines either new macro (`grep -r` of rebuild/: 0 hits).

## engine/CThingSearchTools.h
- Opt-in `FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH_EGOD` (new block appended after the last existing block, just before the include guard's
  `#endif`): the out-of-class `GetPFirstThingInRoughArea<Filter,T>` template body that the landed row 0x009871C0 defines in its own source,
  verbatim (Ego_d.pdb's record of the GetPFirstThingInRoughArea instances: area / filter_func / thing, `flags`, it, thing_map, local_area, c1,
  clipped_area, cy, pcell, cx, ploop_node, pmapwho, loop_thing, tthing; the ROUGHAREA_EGOD text with the first hit returned).
  Guard: `FABLE_RETAIL_CTHINGSEARCHTOOLS && FIRSTROUGH_EGOD && FIRSTROUGH && !FIRSTROUGH_BODY && ROUGHAREA_EGOD && ORIGINAL_ROUGHAREA`, so it
  never fires for 0x009871C0 (which does not define FIRSTROUGH_EGOD) nor beside the older FIRSTROUGH_BODY.
  Used by the 8 GetPFirstThingInRoughArea rows (0x007FCEF0, 0x007512E0, 0x007FD110, 0x007313B0, 0x0079A280, 0x007FE730, 0x0096F260, 0x00987430).
- pin_asserts.py: `pinned 0` (the block is appended after every FABLE_STATIC_ASSERT_AT line; no assert moved).
- Landed includers: 333 files in rebuild/src/compiled include engine/CThingSearchTools.h; none defines FIRSTROUGH_EGOD, so the opt-in
  changes none of them. Control recheck (work/ctl.log): 0x009871C0 and Account B's 0x0063F4B0 compile with this overlay unchanged.

## engine/CThingTrackNode.h (outside the four headers the brief assigns; one additive opt-in, as NF107 did for CThingMarker.h etc.)
- Opt-in `FABLE_RETAIL_CTHINGTRACKNODE_STATTYPEFLAGS` (right after the class head's `public: static CClass* __fastcall Alloc();`, before
  the FLIP opt-in; it opens with `public:` and the next line was already public): `static unsigned long FABLE_FASTCALL StatGetTypeFlags()
  { return 0x10007; }` = FableWin.pdb ?StatGetTypeFlags@CThingTrackNode@@SIKXZ (0x01D3F970), inline in retail (0x0093CF07 `and edx,10007h ;
  cmp edx,10007h`). Used by 0x0093CDB0.
- pin_asserts.py: `pinned 0` (the header's one assert is already the `FABLE_STATIC_ASSERT_AT(64, ...)` form; it moves 7 lines down,
  which the _AT form allows).
- Landed includers: 17 files include engine/CThingTrackNode.h; none defines the macro, so the opt-in changes none of them.
  NF108 / NF109 overlays (`$S/w22/agentNF108/hdr`, `agentNF109/hdr`) do not touch CThingTrackNode.h or CThingSearchTools.h.

NThingMap_CThingMap.h, CThingDeadCreature.h, CThingMarker.h: no overlay (no change was needed: the
existing NF107 CThingDeadCreature STATTYPEFLAGS opt-in served 0x0096F260).
