// NScript::CQ_FireHeartScript::PlayCutscene(CCharString) -- play the named cutscene with the current cast.
// Leaf: ?PlayCutscene@CQ_FireHeartScript@NScript@@QAEXVCCharString@@@Z
// Grade: DIFFER(1056v1578) -- same lane gap as 0x00E137B0: the nine implicit CScriptGameResourceObjectScriptedThingBase assignments (counted-pointer ShareData) stay out of line where retail expands them, and the MovieBase default ctor expands where retail calls it (522 bytes short); 10 shapes (insert/make_pair, map/handle/movie declaration order, explicit key temporaries, no trailing return, header counted-pointer dtor, slot references, assignment order, guest index local, in-place ShareData operator= overlay) leave it. Graded in Codex's header context
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_SCRIPT_QUEST
#define FABLE_STLPORT_HTTP
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CCharString.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
#pragma auto_inline(off)
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer()
{
    if(Info) {
        --Info->RefCount;
        if(Info->RefCount==0) { Info->DeleteFunc(Info->Data);delete Info; }
    }
    Data=0;Info=0;
}
#include "engine/CScriptGameResourceObjectMovieBase.h"
#pragma auto_inline(on)
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CQ_FireHeartScript.h"
#pragma auto_inline(off)
namespace std { template<> void map<CCharString,CCharString>::clear(){_M_t.clear();} }
#pragma auto_inline(on)
void __fastcall RunCutsceneMacro(const CCharString&,std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&,std::map<CCharString,bool>*,std::map<CCharString,CCharString>*,bool,bool);
void NScript::CQ_FireHeartScript::PlayCutscene(CCharString CutsceneDef)
{
    CScriptGameResourceObjectScriptedThingBase seh_Hero;
    Game.StartScriptingEntity(Game.GetHero(),seh_Hero,(EScriptAIPriority)4);
    std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
    mymap["HERO"]=seh_Hero;
    mymap["GUEST"]=seh_Prophet[Guest];
    mymap["PROPHET1"]=seh_Prophet[0];
    mymap["PROPHET2"]=seh_Prophet[1];
    mymap["PROPHET3"]=seh_Prophet[2];
    mymap["PROPHET4"]=seh_Prophet[3];
    mymap["PROPHET5"]=seh_Prophet[4];
    mymap["ROSE"]=seh_BriarRose;
    mymap["SCYTHE"]=seh_Scythe;
    mymap["GM"]=seh_GuildMaster;
    CScriptGameResourceObjectMovieBase speech_movie;
    Game.StartMovieSequence("",speech_movie);
    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
    Game.FixMovieSequenceCamera(true);
    RunCutsceneMacro(CutsceneDef,mymap,&CutsceneFlags,&csargs,false,true);
    Game.FixMovieSequenceCamera(false);
    csargs.clear();
    return;
}
