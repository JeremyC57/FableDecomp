# hdr/ -- header overlay for chunk 864 (FABLE_HDR_OVERLAY)

Every file is the repo's `rebuild/include/engine/<Class>.h` plus additive opt-ins (`#ifdef FABLE_RETAIL_...` blocks) that
the rows define; nothing outside an opt-in block changes (`diff` against the repo header shows only `>` lines). The lines
inherited from the parked chunks' overlays are listed in those chunks' hdr/README.md files
(`rebuild/candidates/manual/pending/2026-11-08_linux_hand856/hdr`, `.../hand860/hdr`).

| header | from | lines | what |
|---|---|---|---|
| CQ_EndGameBossBattleScript.h | hand856 | +22 | CTheRealGuildmaster layout opt-ins for 0x00D33730 |
| CQ_FireHeartScript.h | hand856 | +8 | CFH_Prisoner / parent layout for 0x00D3A4A0 |
| CThingAICreature.h, CThingVillage.h | hand856 | +5 each | StatGetType opt-ins (0x0050A650) |
| CPersistContextReadSection.h, CStringParser.h, CDataInputStream.h | hand856 | +8/+5/+10 | ReadLine / ReadPresized / ReadUHUGE declarations (0x005253F0) |
| CThingManager.h, NThingManager_CEntityRuntimePersistence.h | hand856 | +16/+8 | LoadGameState members (0x005253F0) |
| C2DCoordI.h, C2DBoxF.h, CNavigationPosition.h | hand856 | +13/+6/+6 | ToC2DCoordF, corners, ctors (0x0050A650) |
| CQ_PrisonRaceScript.h | hand856 | +12 | layout for 0x00DDAE40 |
| C3DMeshFileBoneChunk.h, C3DMeshFileSubMeshChunk.h | hand860 | +9/+7 | bone/sub-mesh chunk layouts (0x00A8B990) |
| CRenderStateManager.h | hand860 | +11 | bookmark members (0x009D8CF0 / 0x009D9C80) |
| CThingSearchTools.h | hand856 **+ chunk 864** | +30 | hand856's TCSINROUGHAREA declarations **plus the new nested opt-in `FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA_BODY`** (see below) |

## New in chunk 864: `FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA_BODY` (CThingSearchTools.h)

Nested inside `FABLE_RETAIL_CTHINGSEARCHTOOLS_TCSINROUGHAREA`. Gives `GetAllTCsInRoughArea<T,Filter,Container>` and
`GetAllTCDsInRoughArea<T,Filter,Container>` same-TU `__declspec(noinline)` bodies (templates are defined in retail's
header and compiled out of line: 0x00518340 / 0x005185F0 / 0x00518900). The bodies are a STAND-IN for the shape (things in
the rough area through the filter, pushed into `result`), not the retail instructions: what the caller row needs is only
their mod/ref summary -- the body writes through `result` and does not capture its address. With declarations only the
caller's vector locals were address-exposed and c2 coalesced the free's `_M_start` reload with the loop iterator (the
0x0050A650 residue); with the bodies the vectors are promoted locals whose `_M_start` web is split by the allocator and
the free reloads into eax as retail. 0x0050A650 went from md 6 to RELOCATION_MATCH with this opt-in alone.

## New in chunk 864: `FABLE_RETAIL_CCHARSTRING_REPLACE_STRSTR_TEXT` (fable_string.h, overlay copy of the repo header)

Nested inside `FABLE_RETAIL_CCHARSTRING_REPLACE`: the Replace loop's second `strstr` spelled on `text` after the `text = found`
copy (retail 0x005257AA `mov edi,esi ; push edi`). Diagnostic only (0x005253F0 variants M4/M5: it raises `text`'s priority by
one loop-weight reference per expansion, which over-corrects the text/search_text pair and flips the function-level
this/context registers). No matched row uses it; kept for the hand-over so the M4/M5 attempts can be reproduced. The rest of
the file is the repo header verbatim.
