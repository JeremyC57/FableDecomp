# NF96 header overlays (hdr/engine/)

Every overlay is the current repo header (base copy in `orig/engine/`, identical to `rebuild/include/engine/` at hand-back, checked with
`cmp`) plus `#ifdef` opt-ins only. With none of the new macros defined, each file preprocesses to the same tokens as its base.
`pin_asserts.py` was run on each after editing. No landed row or test defines any of the new macros (grep over rebuild/src and
rebuild/tests: 0 hits), so the opt-ins change none of the landed includers.

| header | opt-in (new macro) | what it adds | pin_asserts | landed includers |
|---|---|---|---|---|
| engine/CTCTavernGameCardBase.h | FABLE_RETAIL_CTCTAVERNGAMECARDBASE_ECARDGAMETYPE_VALUES | `enum ECardGameType { ECGT_TIME_LIMIT=0, ECGT_ROUND_LIMIT=1 };` after the forward declaration | pinned 0 | 11 (work/users_CTCTavernGameCardBase.h.txt) |
| | FABLE_RETAIL_CTCTAVERNGAMECARDBASE_ENTERGAME | `virtual void EnterGame();` (slot 29, 0x008F1E30), after ProcessInput | | |
| | FABLE_RETAIL_CTCTAVERNGAMECARDBASE_ENDROUND | `virtual void EndRound();` (slot 33, 0x008F1720), after the NEWROUND block | | |
| engine/CTCTavernGameCardPairs.h | FABLE_RETAIL_CTCTAVERNGAMECARDPAIRS_ROUND_OVERRIDES | the EnterGame / NewRound / EndRound override declarations | pinned 3 (FABLE_STATIC_ASSERT_AT 21/22/23) | 1 (work/users_CTCTavernGameCardPairs.h.txt) |
| engine/CTavernGameCardBaseDef.h | FABLE_RETAIL_CTAVERNGAMECARDBASEDEF_HIGHLIGHT_COLOUR_BAD | types `CRGBColour HighlightColourBad; // +0x58` and includes engine/CRGBColour.h (unless PASS24) | pinned 0 | 16 (work/users_CTavernGameCardBaseDef.h.txt) |
| engine/CCategory.h | FABLE_RETAIL_CCATEGORY_GETPREVIOUSOCCUPIEDSLOT | `private: long GetPreviousOccupiedSlot(long index) const; public:` after the GETNEXTOCCUPIEDSLOT block | pinned 0 | 51 (work/users_CCategory.txt) |
| engine/CTCBase.h | FABLE_RETAIL_CTCBASE_ADDTOGLOBALTCS | the existing `void AddToGlobalTCs();` declaration, now under `#if defined(FABLE_RETAIL_CTCWIFE_MARRY) \|\| defined(FABLE_RETAIL_CTCBASE_ADDTOGLOBALTCS)` (the default and WIFE_MARRY paths unchanged) | pinned 0 | 1200 (work/users_CTCBase.txt) |
| engine/CGamePlayerInterface.h | FABLE_RETAIL_CGAMEPLAYERINTERFACE_CONTEXTSENSITIVEITEMBUTTONS_TYPED | types `std::vector<CCountedPointer<const CInputTypeBase> > ContextSensitiveItemButtons; // +0x7d0` (else the old 12-byte array), and joins the NF94 QUICKACCESSITEMBUTTONS_TYPED include block (`#if defined(A) \|\| defined(B)`) | pinned 0 | 52 (work/users_CGamePlayerInterface.txt) |

Rows that use them: 007D4E30..008F0B80 (q11339, the tavern-card headers), 0x005BCFE2 / 0x005BD0F7 (CCategory.h), 0x00686ED0 (CTCBase.h),
0x00445960 (CGamePlayerInterface.h). NF93 / NF95 overlays: NF95's CTCBase.h overlay is identical to the repo header (already landed), and
no other agent overlay of CCategory.h declares GetPreviousOccupiedSlot, so there is no clash.
