# Chunk H40 hdr/ overlay (grade every row of this hand-back with `FABLE_HDR_OVERLAY=<this dir>`)

Every file was copied from `rebuild/include` at HEAD (2026-11-11) or from the newest pending hand-back that carries the row's opt-in; every
change sits behind an `#ifdef`, the default path is textually HEAD's. `diff <file> rebuild/include/<file>` shows added lines only.

## New opt-ins of this chunk (needed by the matched rows)
| header | opt-in | change | evidence | used by |
|---|---|---|---|---|
| engine/CPredicateBase.h | `FABLE_RETAIL_CPREDICATEBASE_NOVTABLE` | `class __declspec(novtable) CPredicateBase` under `_MSC_VER` (other compilers see the plain class) | inline-budget device (owner decision 2026-11-08): with it the base ctor is empty, so a derived predicate ctor compiled under a row-local `#pragma inline_depth(0)` region has no `call CPredicateBase::CPredicateBase` (first witness 0x00E5B3B0; without it the region leaves that one call, chunk 835 NOTES). **Not retail's declaration**: retail stores the base vtable 0x012C2FF4 in 41 places (xref.py: ~CPredicateBase 0x00CDDB28, 0x00E20696 ...), so a row whose retail body has that store must not define it (0x00E20600) | 0x00E5B3B0, 0x00E58D40, 0x00DD6A60, 0x00DE4CB0 |
| engine/CV_BeggarAndChildScript.h | `FABLE_RETAIL_CV_BEGGARANDCHILDSCRIPT_ENTITY_MAINS` | adds to the nested CLookoutPointBeggar / CBeggarBully: `GetParentScript() const` (0x00E58D00 / 0x00E5B370), `ParentClass` +0x14, `PMasterData` +0x18, private `Init()` (0x00E58D10 / 0x00E5B380) and `Main()` (0x00E58D40 / 0x00E5B3B0) | Ego_r.pdb layouts (28 bytes each), entity vtables 0x012E68C4 / 0x012E6C1C, both PDB name sets (chunks 813 / 816 hdr, re-based on the chunk 845 header now at HEAD) | 0x00E58D40, 0x00E5B3B0 |

## Carried from pending hand-backs (unchanged opt-ins, re-based on HEAD)
| header | from | opt-in | used by |
|---|---|---|---|
| engine/CQ_PrisonEscapeRescueMotherScript.h | hand811 (HEAD + additions) | `FABLE_RETAIL_CQ_PRISONESCAPERESCUEMOTHERSCRIPT_MOTHERTORTURE` | 0x00DD6A60 (matched) |
| engine/CQ_GuildTrainingWillScript.h | hand819 (new header, not at HEAD) | whole header under `FABLE_RETAIL_NSCRIPT` | 0x00D5E0C0 (matched) |
| engine/CQ_GuildTrainingScript.h | hand816 block moved onto HEAD (after the ENTITIES block) | `FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_SPEEDTEST` (+ `_MAIN`) | 0x00D3E2E0 (differ) |
| engine/CQ_NewOakValeIntroScript.h | hand813 (HEAD + additions) + the hand811 `_BULLY` block | `FABLE_RETAIL_CQ_NEWOAKVALEINTROSCRIPT_BARRELMAN_THERESA`, `_BULLY` | 0x00DB97A0, 0x00DBB310 (differ) |
| engine/CQ_HobbeCaveScript.h | hand815 blocks moved onto HEAD | `FABLE_RETAIL_CQ_HOBBECAVESCRIPT_CBANDITSACRIFICE_MAIN` | 0x00D973D0 (differ) |
| engine/CQ_HerosOldHouseScript.h, CV_BordelloScript.h | hand810 (HEAD + additions) | `..._CFISHERMANSWIFE`, `..._CBORDELLOLADY` | 0x00D8BCD0, 0x00E3EB10, 0x00E3BB70 (differ) |
| engine/CV_AmbushScamScript.h, CV_ArcheryCompetitionScript.h, CV_BeardyBaldyScript.h (new), CQ_TraderConflictGoodScript.h, CV_BanditTollScript.h, CV_ChapelOfEvilScript.h, CQ_ArenaHoldingScriptScript.h, NScript_CQR_EscortTraderScript.h, NScript_CV_ChickenKickingScript.h, CQ_GuildTrainingSkillScript.h (new), CQ_GuildTrainingWoodsDepartureScript.h (new), CQ_TraderConflictEvilScript.h (new) | hand810..hand835 (HEAD + additions, or new) | as in those hand-backs' hdr/README.md | the parked drafts of this chunk (differ / untouched) |

Not carried (still header debt, the draft does not compile on HEAD): engine/CScriptGameResourceObjectScriptedThingBase.h (hand818 / 820: 0x00E64FB0),
CQ_BountyHuntScript.h (hand812: 0x00D13580), CV_BodyGuardScript.h (hand815: 0x00E5FD40), C3DVector.h (hand741 / 835: 0x00B10B10) -- their
HEAD copies changed since; and the screen-builder / CWorldMap overlays of hand729 / 855 / 879 / 882 / 883 (0x00603000, 0x006096E0, 0x0061E6D0, 0x00502E90).
