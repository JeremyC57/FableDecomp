# chunkQ26 header overlays

Each file is a full copy of the CURRENT repo header (rebuild/include/...) with additions only, each behind a new
`#ifdef FABLE_RETAIL_*` opt-in (`diff` against the repo shows only `>` lines), so the default path is unchanged.

## New in Q26

| File | Opt-in | What it adds | Used by |
|---|---|---|---|
| engine/CStreamingFontBank.h | FABLE_RETAIL_CSTREAMINGFONTBANK_CTOR (inside the FABLE_RETAIL_CSTREAMINGFONTBANK branch) | `CStreamingFontBank(CStreamingFontDataBank*, unsigned long)` 0x009A3C20 (Ego_r / FableWin ??0CStreamingFontBank@@QAE@PAVCStreamingFontDataBank@@K@Z) | 009a3c20 (DIFFER) |
| engine/CBankFile.h | FABLE_RETAIL_CBANKFILE_READENTRYDATA | `void ReadEntryData(unsigned long, unsigned long, void*, unsigned long)` 0x009CC2A0 (Ego_r ?ReadEntryData@CBankFile@@QAEXKKPAXK@Z) | 009a3c20 (DIFFER) |
| engine/CThingAICreature.h | FABLE_RETAIL_CTHINGAICREATURE_ISRESPONDINGTOFOLLOWANDWAIT | inline `bool IsRespondingToFollowAndWait() const { return RespondingToFollowAndWait; }` (Ego_r ?IsRespondingToFollowAndWait@CThingAICreature@@QBE_NXZ; retail tests bit 2 of +0x1C9 in place at 0x0084710D) | 00846dd0 (MATCH) |

## Copied unchanged from chunkQ25/hdr (Q25 is landing them; listed in chunkQ25/hdr/README.md)

engine/CAIStateGroup_RecoverStealableItem.h, CCreatureActionBase.h, CCreatureAction_DropGenericBox.h,
CCreatureAction_PerformExpressionExtended.h, CCreatureAction_PickUpGenericBox.h, CEnginePrimitiveRenderUtil.h, CPlayer.h,
CQuestManager.h, CTCEmoteIcon.h, CTCSoundPlayer.h, CThingPlayerCreature.h, CWorldMap.h.
00846dd0 needs Q25's CCreatureActionBase.h (_GETFRAMESRUNNING), CCreatureAction_PerformExpressionExtended.h (FABLE_RETAIL_EXPRESSION_FRAME
branch), CTCSoundPlayer.h (_RELEASELOOPINGSOUND), CTCEmoteIcon.h (_STATTYPE), CQuestManager.h and CPlayer.h, exactly as 00847520 did.

## Grading note

As in Q25, 00846dd0 was graded with `FABLE_HDR_OVERLAY=$S/w22/chunkQ26/hdr_r2` (no fable_string.h; `engine` is a symlink to
`hdr/engine`), because MSVC's quoted-include search finds a root-dir overlay of fable_string.h twice (C2011). This overlay has no
fable_string.h anyway, so `hdr/` and `hdr_r2/` grade the same. Once Q25's overlays are merged, only the three Q26 opt-ins remain.
