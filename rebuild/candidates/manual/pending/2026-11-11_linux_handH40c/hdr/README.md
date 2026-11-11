# Chunk H40c hdr/ overlay (third pass) -- no header file changed; this is the H40b overlay as handed over (grade every row of this hand-back with `FABLE_HDR_OVERLAY=<this dir>`)

### Opt-ins the three MATCHED rows of H40c need (all carried unchanged; default path = HEAD)
| header | opt-in | change | evidence | used by |
|---|---|---|---|---|
| engine/CScriptGameResourceObjectScriptedThingBase.h | `FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_FORCEINLINE_ASSIGN` | the inline `operator=` (CBaseObject::operator= 0x0099A3B0 + PImp share), `FABLE_FORCEINLINE`, placed after HEAD's `..._OUTOFLINE_DTOR` block | the CV_ChickenKickingScript TU expands it at 0x00E65052 (hand818 / 820; re-based in H40b) | 0x00E64FB0 |
| engine/NScript_CV_ChickenKickingScript.h | `FABLE_RETAIL_CV_CHICKENKICKINGSCRIPT_CHICKENMASTER` | the nested CChickenMaster entity script (0x20 bytes: ParentClass +0x14, PMasterData +0x18, HaveTalked +0x1c, HeroHasPlayed +0x1d, KickingChickens +0x1e, GhostChat +0x1f) | Ego_r.pdb layout, entity vtable 0x012E747C, both PDB name sets (chunk 818) | 0x00E64FB0 |
| engine/CQ_GuildTrainingSkillScript.h | new header, body under `FABLE_RETAIL_NSCRIPT` | NScript::CQ_GuildTrainingSkillScript and its nested CTheRealGuildmaster | Ego_r.pdb layouts, entity vtable 0x012D09D8, both PDB name sets (hand816 / 818) | 0x00D5AE70 |
| engine/CQ_GuildTrainingWoodsDepartureScript.h | new header, body under `FABLE_RETAIL_NSCRIPT` | NScript::CQ_GuildTrainingWoodsDepartureScript with the nested CArtifactThief and CFinalMaze | Ego_r.pdb layouts, entity vtables 0x012D16DC / 0x012D16C0, both PDB name sets (hand810 / 814 / 817) | 0x00D647F0 |

# Chunk H40b hdr/ overlay (the chunk H40 overlay re-based onto HEAD, plus three re-based opt-ins) (grade every row of this hand-back with `FABLE_HDR_OVERLAY=<this dir>`)

Every file was copied from `rebuild/include` at HEAD (2026-11-11) or from the newest pending hand-back that carries the row's opt-in; every
change sits behind an `#ifdef`, the default path is textually HEAD's. `diff <file> rebuild/include/<file>` shows added lines only.

## Chunk H40b (second pass)
Every grade of this hand-back was made with `FABLE_HDR_OVERLAY=<this dir>`. CPredicateBase.h, CV_BeggarAndChildScript.h,
CQ_PrisonEscapeRescueMotherScript.h and CQ_GuildTrainingWillScript.h are now byte-identical to HEAD (their opt-ins landed); every other
file is HEAD plus added `#ifdef` blocks only (`diff` against rebuild/include shows `>` lines only).

### Opt-ins the four MATCHED rows need (all carried unchanged from older hand-backs, default path = HEAD)
| header | opt-in | change | evidence | used by |
|---|---|---|---|---|
| engine/CV_BordelloScript.h | `FABLE_RETAIL_CV_BORDELLOSCRIPT_CBORDELLOLADY` | private `CCharString GetLHTSTag(const CCharString&)` (0x00E403D0) in the nested CBordelloLady | Ego_r.pdb + FableWin.pdb `?GetLHTSTag@CBordelloLady@CV_BordelloScript@NScript@@AAE?AVCCharString@@ABV4@@Z` (chunk 810) | 0x00E3EB10 |
| engine/CQ_TraderConflictGoodScript.h | `FABLE_RETAIL_CQ_TRADERCONFLICTGOODSCRIPT_TRADERTORESCUE` | the nested CTraderToRescue entity script (0x20 bytes: ParentClass +0x14, PMasterData +0x18, BarIndex +0x1c; Alloc, GetParentScript, Main, Init) | Ego_r.pdb layout, entity vtable 0x012DEC68, both PDB name sets (chunk 815) | 0x00DFE0F0 |
| engine/CQ_HerosOldHouseScript.h | `FABLE_RETAIL_CQ_HEROSOLDHOUSESCRIPT_CFISHERMANSWIFE` | the nested CFishermansWife entity script (36 bytes: ParentClass +0x14, PMasterData +0x18, TalkedTo +0x1c, LeavingHappy +0x1d, initial_angle +0x20) | Ego_r.pdb layout, entity vtable 0x012D50DC, both PDB name sets (chunk 810) | 0x00D8BCD0 |

0x00E84B80 needs no overlay file (its header opt-ins are at HEAD); 0x00E3EB10 and 0x00D8BCD0 also use the landed
`FABLE_RETAIL_CPREDICATEBASE_NOVTABLE`.

### Header debt re-based in this pass (parked rows compile again; default path = HEAD)
| header | from | opt-in | note | used by |
|---|---|---|---|---|
| engine/CScriptGameResourceObjectScriptedThingBase.h | hand818 / hand820 | `FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_FORCEINLINE_ASSIGN` | the inline `operator=` (CBaseObject::operator= 0x0099A3B0 + PImp share) the CV_ChickenKickingScript TU expands at 0x00E65052; now spelled `FABLE_FORCEINLINE` (fable_compiler.h is included under the same opt-in) and placed AFTER HEAD's `..._OUTOFLINE_DTOR` block (tools/hmerge.py had nested it inside that block: check the nesting after every hmerge) | 0x00E64FB0 (differ, 461 -> 8 lines) |
| engine/CV_BodyGuardScript.h | hand815 | `FABLE_RETAIL_CV_BODYGUARDSCRIPT_CBODYGUARD_MAIN` | three blocks (includes, nested CBodyGuard 0x3c bytes, size assert) | 0x00E5FD40 (differ) |
| engine/C3DVector.h | hand741 | `FABLE_RETAIL_C3DVECTOR_EMITTER` | `GetRandomPointWithinUnitSphere` (0x00A146C0, FableWin.pdb) and inline `Clear()` | 0x00B10B10 (differ) |

Still not re-based: CQ_BountyHuntScript.h (hand812 `_MAIN`; HEAD has since split that block into per-entity opt-ins: 0x00D13580) and the
screen-builder / CWorldMap overlays of hand729 / 879 / 882 / 883 (0x00603000, 0x00502E90, 0x0061E6D0, 0x006096E0).

## Chunk H40 (first pass) -- kept for reference
### New opt-ins of this chunk (needed by the matched rows)
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

(First-pass text; the H40b section above lists what has since been re-based.) Not carried (still header debt, the draft does not compile on HEAD): engine/CScriptGameResourceObjectScriptedThingBase.h (hand818 / 820: 0x00E64FB0),
CQ_BountyHuntScript.h (hand812: 0x00D13580), CV_BodyGuardScript.h (hand815: 0x00E5FD40), C3DVector.h (hand741 / 835: 0x00B10B10) -- their
HEAD copies changed since; and the screen-builder / CWorldMap overlays of hand729 / 855 / 879 / 882 / 883 (0x00603000, 0x006096E0, 0x0061E6D0, 0x00502E90).
