// NScript::CV_BanditCampPathScript::Main() -- the bandit camp path village script: bind BCP_Bandit, post it, wait for the BanditCampPath_1 level, take the hero under scripted control, fade out, drop Q_OakValeBanditRaid, set the save marker at the hero and auto-save, collect the bandits and pick out the two eavesdroppers, play the CS_BANDITPATH_OVERHEARD cut-scene once while both live, release the hero, wait until he wears the whole bandit camp outfit, then wait for the level to unload and deactivate the active quest
// Leaf: ?Main@CV_BanditCampPathScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E46D30, 1971 bytes, thiscall, plain `ret` (two epilogue copies), no EH frame; ends at 0x00E474E2.
// Identity (proven): slot 2 (Main) of the village-script vtable stored by the landed ctor 0x00E477F0 (slot 1 the landed RegisterMain
//   0x00E46C40, slot 3 the landed Init 0x00E46C30); Ego_r.pdb / FableWin.pdb name set ?Main@CV_BanditCampPathScript@NScript@@UAEXXZ;
//   the one binding is the landed CBCP_Bandit::Alloc 0x00E47500 ("BCP_Bandit", the class name less its `C`); the members
//   OverheardBanditConversation / Bandits / Bandit1 / Bandit2 are the PDB's (engine/CV_BanditCampPathScript.h).
// Callees: operator new 0x00BFEA1A, CCharString 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0, CBasicString<char>::Compare 0x00411570
//   (inline CCharString == literal), CScriptBase AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities 0x00CB8930 /
//   IsActiveThreadTerminating 0x00CB7940, CBaseObject 0x0099A380 (the first handle's inline ctor), CScriptGameResourceObjectScriptedThingBase
//   ctor 0x007E72A0 / dtor 0x007E74D0 / operator= 0x008ABD10 / Release 0x00CD2770, CScriptThing::operator= 0x008AB980, cast map ctor
//   0x00CDBF70 / operator[] 0x00CD3D2E / dtor 0x00CDBFB0, movie ctor 0x006E7B60 / dtor 0x006E7B80, RunCutsceneMacro 0x00CBFB7D (__fastcall);
//   CGameScriptInterfaceBase slots 7 NewScriptFrame, 8 StartScriptingEntity, 11 IsLevelLoaded, 64 PostAddScriptedEntities, 70 GetHero,
//   75 GetAllThingsWithScriptName, 184 IsObjectInThingsPossession, 280 DeactivateQuest, 281 DeactivateQuestLater, 370 StartMovieSequence,
//   371 FixMovieSequenceCamera, 373 FadeScreenOut, 379 PauseAllNonScriptedEntities, 655 GetActiveQuestName, 703 AutoSaveCheckPoint,
//   708 SetSaveGameMarkerPos; CScriptThing slots 3 GetDataString, 6 GetPos, 75 IsAlive.
// Shape: the landed village-Main binding model (CV_BanditTollScript::Main 0x00E479B0) with WAIT_UNTIL loops; the eavesdropper search is
//   an index loop over Bandits with `GetDataString() == "literal"` (FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ +
//   FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE: repe cmpsb against "" on the null arm, Compare + neg/sbb/inc otherwise); the final wait is
//   `while (!(!IsLevelLoaded(...)))` (test al,al / setne bl).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_STLP_MAP
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CV_BanditCampPathScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CRGBColour.h"
#include "engine/fable_script_cutscene.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::map();                       // 0x00CDBF70
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::~map();                      // 0x00CDBFB0

namespace NScript {
class CV_BanditCampPathScript::CBCP_Bandit : public CEntityScriptBase {
public:
    static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BanditCampPathScript&, CScriptDataBase*, const CScriptThing&);   // 0x00E47500
};
}

void NScript::CV_BanditCampPathScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BanditCampPathScript>("BCP_Bandit", this, &CBCP_Bandit::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    while (!Game->IsLevelLoaded("BanditCampPath_1")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase seh_hero;
    Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
    Game->FadeScreenOut(0.5f, 0.0f, CRGBColour(0, 0, 0, 255));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->DeactivateQuest("Q_OakValeBanditRaid", 0);
    Game->SetSaveGameMarkerPos(Game->GetHero().GetPos());
    Game->AutoSaveCheckPoint();
    Game->GetAllThingsWithScriptName("BCP_Bandit", Bandits);
    for (unsigned int i = 0; i < Bandits.size(); ++i) {
        if (Bandits[i].GetDataString() == "BCP_EavesDropBandit1") {
            if (IsActiveThreadTerminating())
                return;
            Bandit1 = Bandits[i];
        } else if (Bandits[i].GetDataString() == "BCP_EavesDropBandit2") {
            if (IsActiveThreadTerminating())
                return;
            Bandit2 = Bandits[i];
        }
    }
    if (Bandit1.IsAlive() && Bandit2.IsAlive() && !OverheardBanditConversation) {
        if (IsActiveThreadTerminating())
            return;
        CScriptGameResourceObjectScriptedThingBase seh_bandit1;
        CScriptGameResourceObjectScriptedThingBase seh_bandit2;
        Game->StartScriptingEntity(Bandit1, seh_bandit1, SCRIPT_AI_PRIORITY_HIGHEST);
        Game->StartScriptingEntity(Bandit2, seh_bandit2, SCRIPT_AI_PRIORITY_HIGHEST);
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"] = seh_hero;
        mymap["BAN1"] = seh_bandit1;
        mymap["BAN2"] = seh_bandit2;
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_BANDITPATH_OVERHEARD", mymap, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
        OverheardBanditConversation = true;
    }
    seh_hero.Release();
    while (!(Game->IsObjectInThingsPossession("OBJECT_HERO_GLOVES_BANDITCAMP", Game->GetHero()) &&
             Game->IsObjectInThingsPossession("OBJECT_HERO_HAT_BANDITCAMP", Game->GetHero()) &&
             Game->IsObjectInThingsPossession("OBJECT_HERO_BOOTS_BANDITCAMP", Game->GetHero()) &&
             Game->IsObjectInThingsPossession("OBJECT_HERO_SHIRT_BANDITCAMP", Game->GetHero()) &&
             Game->IsObjectInThingsPossession("OBJECT_HERO_TROUSERS_BANDITCAMP", Game->GetHero()))) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!(!Game->IsLevelLoaded("BanditCampPath_1"))) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}