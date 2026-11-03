# Codex pass 13 landing (2026-11-03)

This batch lands the Codex pass 13 package (`../../pending/2026-11-02_codex_pass13/`, audit in
`../2026-11-03_codex_pass13/`). Nothing was committed by the landing session.

## Result
- **47 of 51 rows landed RELOCATION_MATCH** on the final tree headers: 46 NScript / CWorldMap / CPersistContext rows plus
  0x0121AF20, the CPlayer::PlayerModeInfo dynamic initialiser. **0x004FDA60 CWorldMap::SaveToFile was re-landed**
  (RELOCATION_MATCH) with its callees respelled IAE.
- **4 rows held** (`HEADER_NEEDS.md`):
  - 0x00501450: the CVectorMap::LowerBound body conflicts with the tree model.
  - 0x00989CB0, 0x0098ACF0, 0x009DA9F0: their render header set overlaps the pending chunk 723 overlays.
- **Checks:**
  - `bindings_multi_check.py rebuild/oracles/reloc_bindings.tsv` prints 0.
  - Per-row grades are in `STATUS.tsv`; logs are in `logs/` (scratch paths replaced by `<scratch>`).

## Why the coordinator's overlay-only regrade showed 6 SRC_FAIL + 1 missing grade
The coordinator's overlay-only regrade showed SRC_FAIL for 00e36d50, 00e70bc0, 00d43450, 00e32540, 00ec5de0 and 00ecb0e0,
and no grade line for 00501450. The cause was the overlay, not edits made in the tree during the run.

`FABLE_HDR_OVERLAY` was a *partial* tree (only the changed headers). A quoted `#include "x.h"` resolves next to the
*including* header first. So once a header was reached through a tree header, its copy in the tree was included as well
as its copy in the overlay: `fable_string.h` came in from both (error C2011, class redefinition).

Wine's cl prints its error only in the per-row work directory, so the vland log shows SRC_FAIL with no compiler message.
We compiled each row alone with the cl output visible and found C2011 in each. 00501450 actually graded DIFFER(1334v1334).

On a *full mirror* (rebuild/include copied, the stage copied on top) all 7 compile, and the six PASS. All later grading
used full mirrors and, at the end, the tree itself.

## Header consolidation (one class, one model)
**Method.** Codex's CURRENT_MANIFEST set (one version per header) was proved consistent first: 51/51 PASS on a full
mirror (`logs/grades_res_cur.txt`). 3-way merges against Codex's bases replaced pass 12 work wholesale, because Codex's
base predates pass 12. So the landing stage instead starts from the tree headers and adds only what the rows need.

**Changed headers (21).**
- `fable_string.h`
  - Under FABLE_RETAIL_CCHARSTRING: `SaveToFile(CAFile&) const`, `EqualsCaseInsensitive`, `operator<`, and the free
    `operator+(const char*, const CCharString&)`.
  - The per-TU opt-in FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ on `operator==(const char*)`.
  - Default path unchanged.
- `stlport/fable_map.h`: the STLport 4.6 `_Rb_tree::insert_unique(const V&)` body under FABLE_RETAIL_STLP_MAP.
- `engine/C3DVector.h`: `GetAngleXY() const` under FABLE_RETAIL_C3DVECTOR (FableWin 0x01ACF1E0).
- `engine/CCounter.h`: `operator=(float)` under FABLE_RETAIL_CCOUNTER.
- `engine/CDefString.h`: the per-TU opt-in FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION (calls ??B 0x00415D70).
- `engine/CEntityScriptBinding.h`: the per-TU opt-in FABLE_RETAIL_CENTITYSCRIPTBINDING_FORCEINLINE_CTOR.
- `engine/CParsedItem.h`: the class branch is FABLE_RETAIL_CPARSEDITEM (implicit ctor/copy/dtor).
- `engine/CPersistContext.h`, under FABLE_RETAIL_CPERSISTCONTEXT:
  - inline default ctor and dtor, `Init(CStringParser&,...)`;
  - `GetDefaultVal<T>` (folded in from `_DEFAULT_VALUES`);
  - `TransferVectorLoadText`, `TransferSubComponentLoadText`.
- `engine/CPlayer.h`, under FABLE_RETAIL_PLAYER: `static const CPlayerModeInfo PlayerModeInfo[MAX_NO_PLAYER_MODES]` and
  the 94 private Init/Uninit<Mode> members.
- `engine/CQ_GuildTrainingPreMeleeScript.h`: Codex's FABLE_RETAIL_CQ_GUILDTRAININGPREMELEESCRIPT class branch.
- `engine/CQ_SunnyvaleMasterData.h`: typed members (EGameflowPosition, EPlaceInHeroSouls, EHeroNames, ArcheryOpenState
  from Ego_r / FableWin LF_ENUM); layout unchanged.
- `engine/CRGBColour.h`: the three-channel ctor under FABLE_RETAIL_CRGBCOLOUR.
- `engine/CRegion.h`: Codex's typed FABLE_RETAIL_CREGION branch (the REGION_SERIALIZATION accessors folded in).
- `engine/CScriptGameResourceObjectMovieBase.h` and `engine/CScriptGameResourceObjectScriptedThingBase.h`: per-TU
  `_OUTOFLINE_CTOR` / `_OUTOFLINE_DTOR` / `_OUTOFLINE_ASSIGN` opt-ins.
- `engine/CScriptThing.h`: per-TU `_IMPLICIT_COPY` / `_IMPLICIT_ASSIGNMENT` opt-ins.
- `engine/CStringParser.h`, under FABLE_RETAIL_CSTRINGPARSER (all in both PDBs):
  - includes CParsedItem;
  - ctors and `Init(const CCharString&,...)`;
  - `GetLine` (inline), `PeekNextItemType`, `ReadNextItem`, `ReadAsInteger`, `NextItemExists`,
    `ReadAsStringUntilWhitespace`.
- `engine/CThingManager.h`: inline `GetCurrentThingUID() const` (FableWin).
- `engine/CWideScreenMagicPauseEntities.h`: the per-TU opt-in `_FORCEINLINE_CTOR`.
- `engine/CWorldMap.h`, under FABLE_RETAIL_CWORLDMAP:
  - Codex's typed members;
  - `ELoadType`;
  - LoadRegion, GetRegionNumberMapIsIn, GenerateRegionConnectivityGraph (public);
  - SaveMapsToString, SaveRegionsToString, UnloadRegion, LoadLevelFromFiles (protected, IAE as in both PDBs).
- `engine/NHeroInformationScreens_CBase.h`: includes the new `engine/EPlayerMode.h` instead of declaring
  `enum EPlayerMode {}`. The old empty enum collided (C2011) with the full one in 0x0047A9F0 / 0x00606300.

**New headers (35).**
- Script classes: CMarkTestScriptsScript, COracleMinigameTestScript, CPersonalScript_GlobalThingsScript,
  CPersonalScript_alextestScript, CQS_GuardianSisterInfo2_SisterInBanditCampScript, CQ_ArenaScript, CQ_BreakSiegeScript,
  CQ_GuildTrainingScript, CQ_HeroSoulsArenaScript, CQ_OakValeBanditRaidScript, CQ_OpeningGraveyardSecretPassageScript,
  CQ_PrisonEscapeRescueMotherScript, CQ_RansomVictimChiefsHouseScript, CQ_RansomVictimScript, CQ_SummoningTheShipScript,
  CQ_TraderEscortScript, CTestQuestCard1Script, CV_ArcheryCompetitionScript, CV_BanditTollScript, CV_DemonDoorsScript,
  CV_GhostGrannyNecklaceScript, CV_LostTraderScript, CV_MurderTwistScript, CV_ParanoidWhispersScript, CV_SickChildScript,
  CV_TravellingHeroesScript.
- Others: CPersistContext_TransferableVectorGenericType, CPlayerModeInfo, EPlayerMode.
- Script helpers: fable_script_animation_defaults / cutscene / def_global / distance / fade / text.

Headers needed only by the held rows were **not** installed (`HEADER_NEEDS.md`).

**hdrcheck2** (`logs/hdrcheck2_final.txt`) prints default=same for 18 of the 21 changed headers. The other three are
retail-only class headers with no branch macro: CQ_SunnyvaleMasterData.h, CThingManager.h and
NHeroInformationScreens_CBase.h. Every includer of those three was rechecked (below).

## Macros: folded vs kept
**Codex's set.** The Codex sources defined 79 FABLE_RETAIL_ macros. The landed rows define 35.

**Folded or dropped.**
- Renamed into a class branch:
  - VECTOR_ANGLE became C3DVECTOR.
  - REGION_SERIALIZATION became CREGION.
  - CPARSEDITEM_IMPLICIT_CONSTRUCTION became CPARSEDITEM.
  - STLPORT_MAP and STLPORT_TREE_INSERT_BODY became STLP_MAP.
  - CPLAYER_TYPED and PLAYER_MODE_TABLE became PLAYER.
  - CRGBCOLOUR_THREE_CHANNEL_CTOR became CRGBCOLOUR.
  - CPERSISTCONTEXT_DEFAULT_VALUES became CPERSISTCONTEXT.
  - CV_SICKCHILDSCRIPT_COMPLETE was removed; the new header is one model.
- Replaced by row-local inline bodies (the established form for header-declared members), so they are gone:
  - CCHARSTRING_PARSER_HELPERS;
  - CBASICSTRING_COMPARE_NONINLINE_BODY;
  - CCHARSTRING_TEXT_COMPARE and _EXPANDED;
  - CSCRIPTTHING_EQUAL.
- Dropped because the default already matches: CCHARSTRING_CHAR_ARRAY_BODY, CSPAWNEDFUNC, CSPAWNEDFUNC_EXPAND_CTOR.
- 183 defines of access-only macros that no header reads (CSCRIPTTHING, CENTITYSCRIPTBASE, CTIMER, ...) were removed
  from the rows.

**Kept as per-TU inline or visibility opt-ins.** Retail has one declaration, and VC7.1 expanded it in some TUs and called
the COMDAT in others. These follow the FABLE_RETAIL_CBASICSTRING_EQ_OUTOFLINE precedent and are named `_OUTOFLINE_*` /
`_FORCEINLINE_*` / `_IMPLICIT_*`.

The evidence comes from forcing each macro on in every landed row that lacks it: first all at once
(`logs/grades_res_allon.txt`), then one macro at a time (`logs/grades_res_iso.txt`, `logs/iso_plan.tsv`).

| Opt-in | Rows using it | Forced on in rows without it | Decision |
|---|---|---|---|
| CSCRIPTTHING_IMPLICIT_COPY | 5 | 7 of the 9 single-opt-in rows DIFFER | per-TU |
| CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR | 15 | 13 match / 9 DIFFER | per-TU |
| CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR | 8 | 8 match / 9 DIFFER | per-TU |
| CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR | 11 | 10 match / 4 DIFFER | per-TU |
| CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR | 14 | 6 match / 5 DIFFER | per-TU |
| CDEFSTRING_OUTOFLINE_CONVERSION | 2 | 4 match / 0 DIFFER | per-TU: landed CTCShop::OnSerialise 0x00775C60 expands the conversion inline, so the default must stay inline |
| CENTITYSCRIPTBINDING_FORCEINLINE_CTOR | 1 | 9 match / 0 DIFFER | kept: folding changes every landed NSCRIPT binder TU; fold candidate after that recheck |
| CSCRIPTTHING_IMPLICIT_ASSIGNMENT, CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_CTOR / _OUTOFLINE_ASSIGN, CCHARSTRING_FORCEINLINE_TEXT_EQ | 1 each | not isolated | kept, same mechanism as their siblings |

## Row rebases (no change to retail behaviour)
**All 42 NScript rows: the pointer model.** They were rebased onto the tree's pointer model of
CLevelScriptBase::Game / MasterScriptData and CEntityScriptBase::Game (the pass 12 precedent):
- `Game.` becomes `Game->`;
- `(Game)` becomes `(*Game)`;
- `&MasterScriptData` becomes `MasterScriptData`.

**Per-row edits.** The other edits are listed per row in `STATUS.tsv`:
- macro renames;
- row-local inline bodies for header-declared members;
- `#include` order fixes for CV_BordelloScript, which needs fable_string / CCountedPointer first;
- the CDataOutputStream include for CPersistTraits.

**Class keys.** `keypayload.py` changed only five test doubles (`struct` became `class`); no source changed.

## Includer recheck
- **Scope.** The landed includers of the 21 changed headers: 5,140 sources.
- **Token-identical preprocess.** 5,070 preprocess token-identically against the tree and the final headers, so they
  give the same object.
- **Rechecked.** 70 differ. All 70 were rechecked with `vland --recheck-landed`, three in parallel, and all 70 are
  RELOCATION_MATCH (`includer_recheck_grades.tsv`). The first pass gave 0x0047A9F0 and 0x00606300 SRC_FAIL (EPlayerMode
  C2011). They match after the EPlayerMode unification, and their 4 includers were rechecked again.

## Names
See `NAME_DECISIONS.tsv`. Queue lines are in `../../naming/NAME_FIXES_QUEUE.tsv`, tag `cx13`: 64 lines, 46 PRIORITY and 18
REVIEW.

**Decisions on specific addresses.**
- **0x004FB990 / 0x004FD040 (IAE).** The single landed caller, 0x004FDA60, was respelled and re-landed.
- **0x00989CB0 (AAE).** The row is held; its binder 0x009E1280 is queued PRIORITY.
- **0x009DA9F0.** Its binders are already queued PRIORITY (cx11m).
- **0x0121AF20.** Named `CPlayer::`dynamic initializer for 'PlayerModeInfo''` (object symbol `_$E1`) from three
  pieces of evidence:
  - it sits in the .CRT$XCU slot at 0x013743EC;
  - Ego_r 0x0100CFE0 and FableWin 0x03FE1960 name it;
  - its body is 0xA90 bytes in both retail and Ego_r.
- **0x007E72A0.** `??0CScriptGameResourceObjectScriptedThingBase@@QAE@XZ` (agrees with mig5). Its Construct binders
  are queued.
- **0x00415D70.** `??BCDefString@@QBE?AVCCharString@@XZ`; 0x00415DD0 is GetString. This answers mig9; the other
  spellings are queued.

**Second spellings.** Codex's "492" was measured over all 51 rows before the rebase (`second_spellings.tsv`: 600
row/target entries over 111 targets). For the 47 landed rows, 70 targets still have another landed spelling. They are queued, together with the 24 that `spellcheck.py` reports after landing:
- PRIORITY when the symbol is in both PDB name sets;
- otherwise REVIEW;
- also REVIEW when the landed row at the address carries another member name (a possible identical-COMDAT fold: the 5
  CPlayer::UninitMode* slots of the PlayerModeInfo table).

**Codex NAME_FIXES (136 rows).**
- 6 applied as labels (both PDBs, no other landed spelling).
- 24 covered by a second-spelling line.
- 128 entries were already in the queue.
- The rest are queued: PRIORITY when the symbol is in both PDBs, REVIEW otherwise.

**Data names accepted from Codex's PDB DATA / PUBLIC32 citations.** Verify each with cvdump on the owner's PDBs:
- NPCAttitudes 0x0143E928;
- the seven CRenderStateManager D3D*Table statics: 0x01396DC8, 0x01396DF8, 0x01396E58, 0x01396E64, 0x01396F9C,
  0x01396FA4, 0x01396FB0;
- CRenderManager2D::LibraryStateBlockFunctions 0x01396510;
- NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations 0x01375748;
- CShaderRenderManager::SingletonInstance 0x013BC470.

**Labels.**
- `ghidra_out/labels_codex_pass13_2026-11-03.tsv` (63 lines) was applied with `apply_labels.py`, then
  `sync_catalog_names.py`, then `sync_manifest_from_catalog.py --labels`.
- Labels for addresses whose landed row carries its own Leaf were left out (they are queue lines instead). Their
  catalog and manifest rows were restored.
