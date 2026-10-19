// CGameScriptInterface::StartMovieSequence -- enter movie state and return an owning movie resource
// Retail0089B110,248 bytes.
#include "engine/CGameScriptInterface.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CTCHeroStats.h"
#include "fable_string.h"
struct ComponentEntry{long Type;void* Component;};struct ComponentMap{ComponentEntry *Start,*Finish,*Capacity;ComponentEntry* LowerBound(const long&);}; //0040F020
struct CThingNative{void* VTable;unsigned char Base[0x1c];unsigned int UsingComponents[9];ComponentMap Components;unsigned char Rest[0x41];unsigned char DeletedFlags;};
struct PlayerMethods:CPlayer{CThingNative* GetHeroThing();}; //00487DC0
struct PlayerManagerMethods:CPlayerManager{PlayerMethods* GetMainPlayer();}; //00449970

#include "engine/CTCBoastUI.h"
struct BoastMethods:CTCBoastUI{void AllowUIToOpen(bool);}; //0056A930
__forceinline BoastMethods* GetBoast(CThingNative* hero){BoastMethods* result;if(hero->UsingComponents[7]&0x100){ComponentMap* map=&hero->Components;ComponentEntry* it=map->LowerBound(232);ComponentEntry* end=map->Finish;result=static_cast<BoastMethods*>((it==end||it->Type>232?end:it)->Component);}return result;}
struct MovieTransitionState{unsigned char Prefix[0xf4];bool Busy;};extern MovieTransitionState* g_MovieTransitionState; //013B8790
extern long g_MovieCameraID; //01380C34
extern long g_MovieState; //013BAD00
struct MovieResource{void* VTable;unsigned long RefCount;void* Data;void* Info;CGameScriptInterface* ScriptInterface;bool Enabled;unsigned char Align[3];MovieResource(CGameScriptInterface*,bool);}; //006E7630, RTTI01260F00 CScriptGameResourceObjectMovie
struct MovieResourceHandle{void* VTable;unsigned long RefCount;void* Data;void* Info;void Reset(MovieResource*);}; //008AB970 delegates counted pointer at+8 to008AB2B0
struct CGameScriptInterfaceMethods:CGameScriptInterface{
 typedef void(CGameScriptInterfaceMethods::*ConstVoidMethod)()const;
 void NewScriptFrame()const{(this->*static_cast<ConstVoidMethod*>(__vftable)[7])();} //006E7410
 void CancelAbilitiesForCutscene()const{(this->*static_cast<ConstVoidMethod*>(__vftable)[396])();} //0089D690
 void StartLetterBox(float,float,float); //006E7280 forwards to DisplayEngine00434BA0
 void StartMovieSequence(const CCharString&,MovieResourceHandle&);
};
FABLE_STATIC_ASSERT(sizeof(CGameScriptInterfaceMethods::ConstVoidMethod)==4);
FABLE_STATIC_ASSERT(sizeof(MovieResource)==24);
void CGameScriptInterfaceMethods::StartMovieSequence(const CCharString& name,MovieResourceHandle& output){
 CThingNative* hero=static_cast<PlayerManagerMethods*>(PlayerManager)->GetMainPlayer()->GetHeroThing();if(hero&&!(hero->DeletedFlags&1)&&(hero->UsingComponents[7]&0x100)){
 GetBoast(hero)->AllowUIToOpen(false);}
 while(InMovieSequence)NewScriptFrame();while(g_MovieTransitionState->Busy)NewScriptFrame();
 InMovieSequence=true;StartLetterBox(0.0f,0.5f,1.7777778f);CancelAbilitiesForCutscene();output.Reset(new MovieResource(this,true));g_MovieCameraID=-1;g_MovieState=0;
}