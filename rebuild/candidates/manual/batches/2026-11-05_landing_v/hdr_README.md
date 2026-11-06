# chunk 795 header overlay

Every change is an opt-in `FABLE_RETAIL_*` branch or member block with a `// Retail opt-in (chunk 795 ...)` comment; no default path
changed (`hdrcheck2.py` reports `default=same` for every header, and the existing retail macros alone are unchanged too). Four landed
siblings regraded with this overlay: all RELOCATION_MATCH (SIBLINGS.tsv). Since every edit is opt-in, no INCLUDERS_*.txt list is needed.

| Header | Opt-in macro | Change | Evidence | Rows |
|---|---|---|---|---|
| engine/CQuestManager.h | FABLE_RETAIL_CQUESTMANAGER_LOADGAMESTATE | the LoadGameState members (CQuestCard ctors, lists, RuntimeQuestPersistence, LoadGameState / ActivateMultipleQuests decls) | Ego_r.pdb layout + retail offsets of 0x004B5500 | 0x004B5500 |
| engine/CQuestManager.h | FABLE_RETAIL_CQUESTMANAGER_COMPLETIONUI | AreAllBoastsProcessed 0x004AF9C0, GetIndexFromRegisteredQuest 0x004AF740, RemoveAcceptedBoastsForScript 0x004B0030 (decls), inline GetQuestBoasts | Ego_r / FableWin spellings; retail call targets | 0x005E78F0 (parked) |
| engine/CStringParser.h | FABLE_RETAIL_CSTRINGPARSER_READQUOTED | ReadNextItemAsQuotedString(CCharString&) returning bool (0x009BA1D0) | `?ReadNextItemAsQuotedString@CStringParser@@QAE_NAAVCCharString@@@Z` | 0x004B5500 |
| engine/CDisplayEngine.h | FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE | inline Get3DEngine() | FableWin inline; retail reads the member in place | 0x005E78F0 |
| engine/CMainGameComponent.h | FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE | inline GetDisplayEngine() over the opaque scoped_ptr word +0x28 | FableWin inline; 0x005E790B | 0x005E78F0 |
| engine/CMainGameComponent.h | FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK | inline GetTextBank() / PeekCutSceneMainFont() over the opaque counted pointers +0x14 / +0x1614C | Ego_r spellings; 0x0093033B / 0x00930372; landed 0x00750110 model | 0x0092FAB0 (parked) |
| engine/CWorld.h | FABLE_RETAIL_CWORLD_QUESTCOMPLETIONUI | inline GetMusicManager, SetAsStarting3DRendering, IsPerformingRegionLoad | FableWin inline; retail in place in 0x005E78F0 (0x0092FAB0 calls the COMDAT 0x0048DB13: VC7.1's inline budget) | 0x005E78F0, 0x0092FAB0 |
| engine/CUserProfileManager.h | FABLE_RETAIL_CUSERPROFILEMANAGER_VOLUME | GetSoundVolume / GetDialogueVolume inline ternaries + the volume members (+0x0C, +0x24..+0x2C, +0xE4..+0xEC) | Ego_r names; retail offsets | 0x005E78F0 |
| engine/CASoundManager.h | FABLE_RETAIL_CASOUNDMANAGER_SLOT9 | slot 9 typed as returning the sample owner (descriptive name Virtual09_00a3e3a0) | CSoundManagerLH slot 9 0x00A3E3A0 `mov eax,[ecx+44h]` | 0x005E78F0 |
| engine/CTCHeroStats.h | FABLE_RETAIL_CTCHEROSTATS_BOASTS | AddMoney 0x0057B327, AddRewardMoney 0x0057A59B, NumBoastsCompleted +0xD0 | Ego_r; landed callees | 0x005E78F0 |
| engine/CTCHeroStats.h | FABLE_RETAIL_CTCHEROSTATS_BRIBE (elif) | Money +0x3C, TrainingStatus +0x114 | generated default offsets + Ego_r names; 0x0092FAB0 reads | 0x0092FAB0 |
| engine/CPlayerGui.h | FABLE_RETAIL_CPLAYERGUI_OPEN | Open() 0x004374B0 decl | Ego_r; landed callers | 0x005E78F0 |
| engine/NUISystem_CSprite.h | FABLE_RETAIL_CSPRITE_PUBLIC_TEXTURE | `public:` before PTexture (+0x17C) | the row writes it directly (inline accessor in retail) | 0x0055FEC0 |
| fable_string.h | FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ | forceinline CCharString / CBasicString operator==(same type) | inline budget in big TUs (as FORCEINLINE_TEXT_EQ) | 0x005E78F0 |
| fable_definition_table.h, fable_game_text_bank.h | (none) | unchanged copies, so that the overlaid fable_string.h is the one they include | MSVC includer-directory lookup | all |
| engine/CAIStateGroupBase.h | FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH (with _MEMBERS + _AIACTIONS) | GetWorld 0x008FCFC0, GetPlayerManager 0x008FCF90, PeekWorldMap 0x008FCFF0, AIAction_SpeakToTarget 0x00980BD0, both AIAction_SpeakAISpeechToTarget (0x00981EA0, 0x009812C0) decls | Ego_r manglings (IBE / IAE protected) | 0x0092FAB0 |
| engine/CIntelligentPointer.h | FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT | template converting ctor decl (COMDAT 0x00930D20) | landed 0x00930D20 body; retail out of line | 0x0092FAB0 |
| engine/CSubtitleRenderer.h | FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT | both SetText overloads (0x00492E4B, 0x00492F69) decls | Ego_r manglings | 0x0092FAB0 |
| engine/CRegion.h | FABLE_RETAIL_CREGION_PREGIONDEF | inline GetPRegionDef() by value | FableWin `?GetPRegionDef@CRegion@@QBE?AV?$CDefPointer@$$CBVCRegionDef@@@@XZ`; retail copies +0x24 in place | 0x0092FAB0 |
| engine/CTCWife.h | FABLE_RETAIL_CTCWIFE_GREET | GetGreetDialogueGroupName 0x006D0410, GetGreetConfirmationText 0x006D0590 decls | Ego_r; landed | 0x0092FAB0 |
| engine/CTCShopKeeper.h | FABLE_RETAIL_CTCSHOPKEEPER_SHOP | StatGetInterfaceType (0x90), GetShopTC 0x00798F50, GetHint 0x00798D30 | Ego_r order GetHint < GetShopTC as retail | 0x0092FAB0 |
| engine/CTCShop.h | FABLE_RETAIL_CTCSHOP_AVAILABLE | IsShopKeeperAvailable 0x00773AA0 decl | Ego_r; landed | 0x0092FAB0 |
| engine/CTCVillage.h | FABLE_RETAIL_CTCVILLAGE_BRIBE | GetCostToBribe 0x006588CF decl | Ego_r `?GetCostToBribe@CTCVillage@@QAEJXZ`; body reads PDef->BribeCost | 0x0092FAB0 |
| engine/CTCCreatureOpinionOfHero.h | FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_GREET | GetGreetDialogueGroupIndex 0x00683C10, ResetLastOpinionReactionFrame 0x006CEAE0 decls, inline SetHasInteracted / SetHasGreeted, StatGetInterfaceType when INFORM is off | Ego_r / FableWin | 0x0092FAB0 |

Concurrent edits: chunk 794 also opts into CTCShop.h (SHOPKEEPER), CTCVillage.h (LIMBO) and CTCCreatureOpinionOfHero.h (INFORM);
these copies were taken from the repo with those blocks present and only add new, independent blocks.
