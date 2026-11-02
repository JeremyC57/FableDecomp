// NScript::CQ_HangingTreeEvilScript::Introduction() -- stages the bandit introduction and opens the quest-start screen
// Leaf: ?Introduction@CQ_HangingTreeEvilScript@NScript@@QAEXXZ
// Grade: DIFFER(1500v1516) -- inline budget: retail resize(count, CScriptGameResourceObjectScriptedThingBase()) calls the element ctor 0x007E72A0 and dtor 0x007E74D0 but inlines both elsewhere; the explicit 2-arg form inlines the dtor (1580), the one-arg wrapper is not inlined at all (this file)
// Context: grades against the Codex row context $S/codex11_dl/x/rows/00d6d080/selected_context/hdr (Codex per-feature macros; NOT rebased on rebuild/include); source = retry11a probe p01; 00d6d080 probes in probe/00d6d080.log
#include "engine/fable_hanging_tree.h"
#include "engine/CQ_HangingTreeEvilScript.h"
#include "engine/TRetVoid.h"
void NScript::CQ_HangingTreeEvilScript::Introduction()
{
    TRetVoid _Fail_Val;
    {
        std::vector<CScriptThing> Bandits;
        std::vector<CScriptGameResourceObjectScriptedThingBase> BanditHandles;
        Game.GetAllThingsWithScriptName("HTE_FreedomFighter",Bandits);
        std::vector<CScriptThing>::size_type count=Bandits.size();
        BanditHandles.resize(count);
        for(long i=0;i<Bandits.size();++i)
            Game.StartScriptingEntity(Bandits[i],BanditHandles[i],SCRIPT_AI_PRIORITY_HIGHEST);
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        Game.StartScriptingEntity(Game.GetHero(),seh_hero,SCRIPT_AI_PRIORITY_HIGHEST);
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"]=seh_hero;
        mymap["BAN1"]=BanditHandles[0];
        mymap["BAN2"]=BanditHandles[1];
        mymap["BAN3"]=BanditHandles[2];
        mymap["BAN4"]=BanditHandles[3];
        CScriptGameResourceObjectMovieBase speech_movie;
        Game.StartMovieSequence("",speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
        Game.FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_HANGINGTREE_EVIL_INTRO",mymap,0,0,false,true);
        Game.FixMovieSequenceCamera(false);
    }
    Game.NewScriptFrame();
    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.NewScriptFrame();
    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game.NewScriptFrame();
    if(IsActiveThreadTerminating()) return _Fail_Val.Return();
    if(!QuestStartScreenShown) {
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game.KickOffQuestStartScreen("Q_HangingTreeEvil",true,false);
        QuestStartScreenShown=true;
    }
    IntroCutscenePlayed=true;
    while(!BanditsStarted) {
        Game.NewScriptFrame();
        if(IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    IsActiveThreadTerminating();
}
