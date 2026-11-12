// NScript::CQ_EndGameBossBattleScript::JackOfBladesBossBattle() -- the Jack of Blades boss battle: wait for HeroGuildComplex,
//   hide the guild complex's sconces / chandeliers / bookcases / candles (EntitySetInLimbo), open the secret bookcase, fade into
//   FrescoDome, take Jack under script control, run the intro cut-scene (JackOfBladesBossFightCutscene), spawn
//   JackOfBladesPhaseWatcher, fight until boss phase 4 or health <= 10, kill the wardogs, give the XP, wait for the final cut-scene,
//   play the evil / good epilogue movie by JackBossBattleResult and morality, then restore the guild complex (second limbo pass,
//   map / teleporter / doors / villagers / guards) and move the hero to Oakvale's memorial garden.
// Leaf: ?JackOfBladesBossBattle@CQ_EndGameBossBattleScript@NScript@@QAEXXZ
// Flags: /d2inlS25
// Inline-budget: /d2inlS25 -- the implicit ~CScriptGameResourceObjectMovieBase in the cold epilogue (retail 0x00D317F3 / 0x00D318BB:
//   `lea ecx,[esp+98h]; call 0x006E7AB0; lea ecx,[esp+90h]; mov [esp+90h],0x0126008C; call 0x0099A430`) is a call to the ??1 COMDAT
//   in the plain build (c2 score ~3e5 at static frequency ~1e-5, so no /d2inlT fits: pass-2 ~vector sites retail calls score
//   2227..83964); S25 is the smallest N that expands it (S24 leaves the call), as /d2inlS15 did for CBH_GangDeputy2::Main 0x00D18DF0.
// Retail: Fable.exe @ 0x00D30030, 8589 bytes (0x00D30030..0x00D321BC), thiscall, plain `ret`, no EH, /O2 /Oy, frame 0xC8.
// Identity (DOSSIER.md): Ego_r.pdb + FableWin.pdb leaf; only caller CQ_EndGameBossBattleScript::Main 0x00D2F7F0 (0x00D2FA20);
//   FableWin source order Main, JackOfBladesBossBattle, JackOfBladesBossFightCutscene (landed 0x00D321C0), PhaseWatcher 0x00D32630.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, IsActiveThreadTerminating 0x00CB7940,
//   ~CScriptThing 0x004AA840, ~vector<CScriptThing> 0x008AC970, vector::erase(iterator, iterator) 0x008AC9D0, free 0x00BFEA14,
//   operator delete 0x00BFE9BC, ~CBaseClass 0x0099A2E0, CCountedPointer<CScriptThing>::operator= 0x00704580,
//   CScriptGameResourceObjectScriptedThingBase::IsValid 0x00CD23B9 / Release 0x00CD2770, CScriptGameResourceObjectBase() 0x006E7A60,
//   ~CCountedPointer<CScriptGameResourceObjectMovieBase> 0x006E7AB0, ~CBaseObject 0x0099A430, JackOfBladesBossFightCutscene
//   0x00D321C0, operator new 0x00BFEA1A, CSpawnedFuncBase 0x00CDD450, CScriptBase::AddSpawnedFunction 0x00CB7E50,
//   KillAllThingsInVector 0x00CBED82, AddLogbookStoryEntryString 0x00CBE960; CGameScriptInterfaceBase slots per DOSSIER.md section 3.
// Shape (generated quest-script style): `TRetVoid _Fail_Val; ... return _Fail_Val.Return();` at every terminate check. Each
//   first-pass limbo loop is `{ long i = 0; if (n > 0) for (; i < n; ++i) ... }` -- the index is zeroed before the entry test
//   (retail `xor edi,edi; cmp ebp,ebx; jle`, the first one coalesced with the zero constant in ebx) and the guard gives c2 the
//   static frequency that splits retail's ~vector / ~CScriptThing expand-or-call at each exit. Inline growth: STLport 4.6's
//   allocator layers (FABLE_RETAIL_STLP_ALLOC_PROXY_REBIND: _STLP_alloc_proxy::deallocate through __stl_alloc_rebind;
//   FABLE_RETAIL_STLP_MALLOC_ALLOC_DEALLOCATE: allocator::deallocate through __malloc_alloc<0>::deallocate) make c2's growth
//   counter cross its 9000 penalty between the 4th and 5th ~vector of the loop-5 after-exit (retail 0x00D30A60: four expanded, the
//   chandeliers / sconces destructors called through the shared tail 0x00D32197), which also fixes the vector frame order.
//   `while (!(phase == 4 || health <= 10.0f))` gives `test ah,41h; jnp`; ActivateQuest inside each morality arm gives retail's
//   cross-jumped arm tails and their `push -1; lea ecx; push` schedule; clear() is spelled `erase(begin(), end())` (called 0x008AC9D0).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_STLP_MALLOC_ALLOC_DEALLOCATE
#define FABLE_RETAIL_STLP_ALLOC_PROXY_REBIND
#include "fable_string.h"
#include "engine/CCountedPointer.h"
class CScriptThing;
template <> CCountedPointer<CScriptThing>& CCountedPointer<CScriptThing>::operator=(const CCountedPointer<CScriptThing>& other);   // 0x00704580 (out of line in retail)
#include "engine/CScriptThing.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0 (out of line in retail)
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CQ_EndGameBossBattleScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/EMusicSetType.h"
#include "engine/TRetVoid.h"
inline CScriptThing::~CScriptThing() { }
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
extern CDefPointer<const CScriptDef> pGlobalScriptDef;   // 0x0143E90C
void __fastcall KillAllThingsInVector(CGameScriptInterfaceBase& game, const std::vector<CScriptThing>& things, bool fade);   // 0x00CBED82
void __fastcall AddLogbookStoryEntryString(const CCharString& text);   // 0x00CBE960

void NScript::CQ_EndGameBossBattleScript::JackOfBladesBossBattle()
{
    TRetVoid _Fail_Val;
    Game->SetTeleportingAsActive(false);
    while (!Game->IsLevelLoaded("HeroGuildComplex")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game->TransitionToTheme("ENVIRONMENT_EOW_04", 0.0f);
    CScriptThing guildComplex = Game->GetNearestWithDefName(Game->GetHero(), "VILLAGE_GUILD_COMPLEX_INSIDE");
    Game->SetVillageLimbo(guildComplex, true);
    Game->SetThingAsUsable(Game->GetThingWithScriptName("MainGuildMap"), false);
    std::vector<CScriptThing> sconces;
    std::vector<CScriptThing> chandeliers;
    std::vector<CScriptThing> chandeliersLong;
    std::vector<CScriptThing> bookcases;
    std::vector<CScriptThing> candleRounds;
    std::vector<CScriptThing> candleTrios;
    long nSconces = Game->GetAllThingsWithDefName("GENERIC_SCONCE", sconces);
    long nChandeliers = Game->GetAllThingsWithDefName("GENERIC_CHANDELIER", chandeliers);
    long nChandeliersLong = Game->GetAllThingsWithDefName("GENERIC_CHANDELIER_LONG", chandeliersLong);
    long nBookcases = Game->GetAllThingsWithDefName("OBJECT_GUILD_BOOKCASE_SELECT", bookcases);
    long nCandleRounds = Game->GetAllThingsWithDefName("Candle_Flame_Round_01", candleRounds);
    long nCandleTrios = Game->GetAllThingsWithDefName("Candle_Flame_Trio_01", candleTrios);
    {
    long i = 0;
    if (nSconces > 0) for (; i < nSconces; ++i) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game->EntitySetInLimbo(sconces[i], true, true);
    }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    {
    long i = 0;
    if (nChandeliers > 0) for (; i < nChandeliers; ++i) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game->EntitySetInLimbo(chandeliers[i], true, true);
    }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    {
    long i = 0;
    if (nChandeliersLong > 0) for (; i < nChandeliersLong; ++i) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game->EntitySetInLimbo(chandeliersLong[i], true, true);
    }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    {
    long i = 0;
    if (nCandleRounds > 0) for (; i < nCandleRounds; ++i) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game->EntitySetInLimbo(candleRounds[i], true, true);
    }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    {
    long i = 0;
    if (nCandleTrios > 0) for (; i < nCandleTrios; ++i) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game->EntitySetInLimbo(candleTrios[i], true, true);
    }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    {
    long i = 0;
    if (nBookcases > 0) for (; i < nBookcases; ++i) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game->EntitySetInLimbo(bookcases[i], true, true);
    }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game->SetTeleporterAsActive(Game->GetThingWithScriptName("HERO_GUILD_TELEPORT_MARKER"), false);
    CScriptThing secretBookcase = Game->GetThingWithScriptName("SecretBookcase");
    Game->OpenDoor(secretBookcase);
    Game->SetThingPersistent(secretBookcase, true);
    Game->CacheMusicSet(MUSIC_SET_CUTSCENE_JACK_BOSS_FIGHT_INTRO);
    Game->CacheMusicSet(MUSIC_SET_CUTSCENE_JACK_BOSS_FIGHT_OUTRO);
    Game->FadeScreenIn();
    while (!Game->IsLevelLoaded("FrescoDome")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game->TransitionToTheme("ENVIRONMENT_JACK_OF_BLADES_FADE", 0.0f);
    Game->OverrideMusic(MUSIC_SET_BOSS, false, false);
    Game->SetAllowScreenFadingIfAlreadyFaded(true);
    Game->FadeScreenOutUntilNextCallToFadeScreenIn(0.5f, 0.0f);
    Jack = Game->GetThingWithScriptName("EndGameJackOfBladesBoss");
    if (seh_Jack.IsValid())
        seh_Jack.Release();
    while (!Game->StartScriptingEntity(Jack, seh_Jack, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game->EntitySetAsDrawable(Jack, false);
    Game->EntityTeleportToThing(Jack, Game->GetThingWithScriptName("MK_JACK1_HOLDJACK"), false);
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game->EntityEnableGravity(Game->GetThingWithScriptName("CS_AEONS"), false);
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game->EntityTeleportToThing(Game->GetThingWithScriptName("CS_AEONS"), Game->GetThingWithScriptName("CS_AEONS_HOMEPOS"), false);
    Game->SetThingAsUsable(Game->GetThingWithScriptName("FrescoDoor"), false);
    JackOfBladesBossFightCutscene();
    AddSpawnedFunction(new CSpawnedFunc<CQ_EndGameBossBattleScript>("JackOfBladesPhaseWatcher", 0, this, &CQ_EndGameBossBattleScript::JackOfBladesPhaseWatcher), CCharString(""));
    seh_Jack.Release();
    Game->EntitySetAsDrawable(Jack, true);
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    IntroFinished = true;
    Game->DisplayQuestInfo(true);
    Game->AddQuestInfoBarHealth(Jack, CRGBColour(255, 255, 255, 255), "HUD_QUEST_ICON_JACK_OF_BLADES", 1.0f);
    while (!(Game->EntityGetBossPhase(Jack) == 4 || Game->GetHealth(Jack) <= 10.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game->RemoveThing(Jack, false, true);
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    std::vector<CScriptThing> wardogs;
    Game->GetAllThingsWithDefName("CREATURE_MINION_WARDOG", wardogs);
    KillAllThingsInVector(*Game, wardogs, false);
    Game->GiveHeroExperience(pGlobalScriptDef->JackFightDefeatJackXP);
    FinalDecision = true;
    Game->StopOverrideMusic(false);
    while (!FinalCutscenePlayed) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, false, false);
    if (MasterScriptData->JackBossBattleResult == 1) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CGameScriptInterfaceBase* game = Game;
        game->PauseAllNonScriptedEntities(true);
        if (Game->GetHeroMorality() < 0.5f) {
            if (IsActiveThreadTerminating()) {
                game->PauseAllNonScriptedEntities(false);
                return _Fail_Val.Return();
            }
            MasterScriptData->JackBossBattleHeroGoodAtEnd = false;
            Game->PlayAVIMovie("Data\\Video\\9_evil_epilogue_comp_Sisterkilled.xmv");
            Game->ActivateQuest("Hook_Fresco_01_EvilEpilogue");
        } else {
            if (IsActiveThreadTerminating()) {
                game->PauseAllNonScriptedEntities(false);
                return _Fail_Val.Return();
            }
            MasterScriptData->JackBossBattleHeroGoodAtEnd = true;
            Game->PlayAVIMovie("Data\\Video\\9_evil_epilogue_comp_Sistersaved.xmv");
            Game->ActivateQuest("Hook_Fresco_01_EvilEpilogue");
        }
        AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_295");
        Game->GiveHeroMorality(pGlobalScriptDef->EGJ_MoralityLoss);
        game->PauseAllNonScriptedEntities(false);
    } else if (MasterScriptData->JackBossBattleResult == 2) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CGameScriptInterfaceBase* game = Game;
        game->PauseAllNonScriptedEntities(true);
        if (Game->GetHeroMorality() < 0.5f) {
            if (IsActiveThreadTerminating()) {
                game->PauseAllNonScriptedEntities(false);
                return _Fail_Val.Return();
            }
            MasterScriptData->JackBossBattleHeroGoodAtEnd = false;
            Game->PlayAVIMovie("Data\\Video\\10_good_epilogue_comp_Sisterkilled.xmv");
            Game->ActivateQuest("Hook_Fresco_02_GoodEpilogue");
        } else {
            if (IsActiveThreadTerminating()) {
                game->PauseAllNonScriptedEntities(false);
                return _Fail_Val.Return();
            }
            MasterScriptData->JackBossBattleHeroGoodAtEnd = true;
            Game->PlayAVIMovie("Data\\Video\\10_good_epilogue_comp_Sistersaved.xmv");
            Game->ActivateQuest("Hook_Fresco_02_GoodEpilogue");
        }
        AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_290");
        Game->GiveHeroMorality(pGlobalScriptDef->EGJ_MoralityGain);
        game->PauseAllNonScriptedEntities(false);
    }
    Game->DeactivateQuest("Q_EndGameBossBattleScenery", 0);
    Game->SetAllowScreenFadingOnNextRegionChange(false);
    Game->FadeScreenOut(1.0f, 0.0f, CRGBColour(0, 0, 0, 255));
    Game->OverrideMusic(MUSIC_SET_GUILD_DAY, false, false);
    Game->EnableSounds(false);
    Game->SetThingAsUsable(Game->GetThingWithScriptName("FrescoDoor"), true);
    Game->SetTeleportingAsActive(true);
    Game->SetRegionTextDisplayAsActive(false);
    Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("GuildTrainingHSP"), false);
    while (!Game->IsRegionLoaded("HeroGuildComplexInside")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game->SetAllowScreenFadingOnNextRegionChange(false);
    sconces.erase(sconces.begin(), sconces.end());
    chandeliers.erase(chandeliers.begin(), chandeliers.end());
    chandeliersLong.erase(chandeliersLong.begin(), chandeliersLong.end());
    bookcases.erase(bookcases.begin(), bookcases.end());
    candleRounds.erase(candleRounds.begin(), candleRounds.end());
    candleTrios.erase(candleTrios.begin(), candleTrios.end());
    nSconces = Game->GetAllThingsWithDefName("GENERIC_SCONCE", sconces);
    nChandeliers = Game->GetAllThingsWithDefName("GENERIC_CHANDELIER", chandeliers);
    nChandeliersLong = Game->GetAllThingsWithDefName("GENERIC_CHANDELIER_LONG", chandeliersLong);
    nBookcases = Game->GetAllThingsWithDefName("OBJECT_GUILD_BOOKCASE_SELECT", bookcases);
    nCandleRounds = Game->GetAllThingsWithDefName("Candle_Flame_Round_01", candleRounds);
    nCandleTrios = Game->GetAllThingsWithDefName("Candle_Flame_Trio_01", candleTrios);
    Game->ResetToDefaultTheme(5.0f);
    for (long i = 0; i < nSconces; ++i) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game->EntitySetInLimbo(sconces[i], false, true);
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    for (long i = 0; i < nChandeliers; ++i) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game->EntitySetInLimbo(chandeliers[i], false, true);
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    for (long i = 0; i < nBookcases; ++i) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game->EntitySetInLimbo(bookcases[i], false, true);
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    for (long i = 0; i < nChandeliersLong; ++i) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game->EntitySetInLimbo(chandeliersLong[i], false, true);
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    for (long i = 0; i < nCandleRounds; ++i) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game->EntitySetInLimbo(candleRounds[i], false, true);
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    for (long i = 0; i < nCandleTrios; ++i) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game->EntitySetInLimbo(candleTrios[i], false, true);
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    CScriptThing mainGuildMap = Game->GetThingWithScriptName("MainGuildMap");
    Game->SetThingAsUsable(mainGuildMap, true);
    CScriptThing teleportMarker = Game->GetThingWithScriptName("HERO_GUILD_TELEPORT_MARKER");
    Game->SetTeleporterAsActive(teleportMarker, true);
    CScriptThing guildDoors = Game->GetThingWithScriptName("GuildDoors");
    Game->SetThingAsUsable(guildDoors, true);
    guildComplex = Game->GetNearestWithDefName(Game->GetHero(), "VILLAGE_GUILD_COMPLEX_INSIDE");
    Game->EnableVillagerDefTypes(guildComplex, true, "CREATURE_GUILD_APPRENTICE_MALE");
    Game->EnableVillagerDefTypes(guildComplex, true, "CREATURE_GUILD_APPRENTICE_FEMALE");
    Game->EnableGuards(guildComplex, true);
    Game->SetVillageLimbo(guildComplex, false);
    Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("MemorialGardenHSP"), false);
    while (!Game->IsRegionLoaded("OakvaleMemorialGarden")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game->ResetToDefaultTheme(0.0f);
    Game->FadeScreenIn();
    Game->SetRegionTextDisplayAsActive(true);
    Game->SetAllowScreenFadingOnNextRegionChange(true);
    Game->EnableSounds(true);
    Game->StopOverrideMusic(false);
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
