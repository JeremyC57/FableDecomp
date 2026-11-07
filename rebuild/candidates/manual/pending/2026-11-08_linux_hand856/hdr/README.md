# hdr/ overlay (chunk 856)

Chunk 856 made **no header edits of its own**. Every row in this chunk is a retry of a parked row from an earlier chunk whose
row source `#define`s opt-in macros that live only in that chunk's (not yet landed) pending overlay. To grade those sources this
overlay carries those pending copies **unchanged** (byte-identical to the pending file named below). All of them are
additive opt-in branches (`#ifdef FABLE_RETAIL_<X>`), so the default path of each header is the repo text and no landed includer
can change bytes. When the coordinator lands the originating chunk's hdr/, these copies become redundant.

The overlay was confirmed in effect (brief rule): a DIAG probe that changed the overlay CDataInputStream.h Read body (range check
dropped) turned 0x00AAD650 from 1575 to 1303 bytes; a DIAG empty `_Vector_base` dtor in a temporary hdr/stlport copy turned
0x0050A650 from 2233 to 2185 bytes. Both probe edits were removed again (CDataInputStream.h is back to byte-identical with
2026-11-07_linux_hand848; the stlport copy was deleted).

| header | identical to pending copy | needed by row | opt-in macro(s) |
|---|---|---|---|
| engine/CQ_EndGameBossBattleScript.h | 2026-11-07_linux_hand838 | 0x00D33730 | FABLE_RETAIL_CQ_ENDGAMEBOSSBATTLESCRIPT_CTHEREALGUILDMASTER |
| engine/CPersistContextReadSection.h | 2026-11-07_linux_hand848 | 0x005253F0 | FABLE_RETAIL_CPERSISTCONTEXTREADSECTION_IMPLICIT_DTOR |
| engine/CQ_PrisonRaceScript.h | 2026-11-07_linux_hand837 | 0x00DDAE40 | FABLE_RETAIL_CQ_PRISONRACESCRIPT_MAIN |
| engine/CQ_FireHeartScript.h | 2026-11-07_linux_hand837 | 0x00D3A4A0 | FABLE_RETAIL_CQ_FIREHEARTSCRIPT_PRISONER |
| engine/CV_ExposeMayorScript.h | 2026-11-07_linux_hand836 | 0x00E799F0 | FABLE_RETAIL_CV_EXPOSEMAYORSCRIPT_CEM_RHODRI |
| engine/CV_DemonDoorsScript.h | 2026-11-07_linux_hand834 | 0x00E72630 | FABLE_RETAIL_CDF_THREESUITS |
| engine/CV_RockTrollFirstEncounterScript.h (new header) | 2026-11-07_linux_hand842 | 0x00EC4A00 | (new file, FABLE_RETAIL_NSCRIPT) |
| engine/CDataInputStream.h, engine/CThingManager.h, engine/CStringParser.h, engine/NThingManager_CEntityRuntimePersistence.h | 2026-11-07_linux_hand848 | 0x005253F0 (0x00AAD650 reads CDataInputStream.h's default path) | CDATAINPUTSTREAM_READPRESIZED / _READUHUGE, CTHINGMANAGER_LOADGAMESTATE, CSTRINGPARSER_READLINE, CENTITYRUNTIMEPERSISTENCE_LOADSCRIPT |
| engine/C2DBoxF.h, engine/C2DCoordI.h, engine/CNavigationPosition.h, engine/CThingSearchTools.h, engine/CThingVillage.h | 2026-11-08_linux_hand853 | 0x0050A650 | C2DBOXF_COPY, C2DCOORDI_TOFLOAT, CNAVIGATIONPOSITION_CTORS, CTHINGSEARCHTOOLS_TCSINROUGHAREA, CTHINGVILLAGE_STATGETTYPE |
| engine/CThingAICreature.h | repo copy + chunk 853's `FABLE_RETAIL_CTHINGAICREATURE_STATGETTYPE` block (the repo text already carries chunk 854's IMMUNETOATTACK change; 853's pending copy predates it) | 0x0050A650 | FABLE_RETAIL_CTHINGAICREATURE_STATGETTYPE |

The ten parked sources were regraded with exactly this overlay (attempts.log tag `final2`): same grades as with the larger
first overlay. ef4b40 (chunk 837) needs no overlay header of its own beyond the shared NScript ones in the repo.

Not carried: the pending copies of stlport/fable_map.h and fable_retail_banks.h (the repo copies are newer), every pending
header that is byte-identical to the repo, and twelve pending copies (chunks 836 / 837 / 838 / 842 / 853) none of these rows includes. Includers rechecked: none needed (no default path changed by this chunk).
An experimental `engine/CIsThingConscious.h` opt-in (in-class / implicit predicate dtor, for 0x00E799F0) did not help and was removed.
