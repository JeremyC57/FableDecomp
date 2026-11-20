// CGameScriptInterface::StartMovieSequence -- enter movie state and return an owning movie resource
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail0089B110,248 bytes.
// Leaf: ?StartMovieSequence@CGameScriptInterfaceMethods@@QAEXABVCCharString@@AAVCScriptGameResourceObjectMovieBase@@@Z
#include "fable_compiler.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CGameScriptInterface.h"
#include "engine/CPlayerManager.h"
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer
#include "engine/CTCHeroStats.h"
#include "fable_string.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef std::pair<ETCInterfaceType, CTCBase*> ComponentEntry;template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:ComponentEntry *Start,*Finish,*Capacity;ComponentEntry* LowerBound(const ETCInterfaceType& key);};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > ComponentMap; //0040F020
struct CThingNative{void* VTable;unsigned char Base[0x1c];unsigned int UsingComponents[9];ComponentMap Components;unsigned char Rest[0x41];unsigned char DeletedFlags;};
class CThingPlayerCreature : public CThingNative { };
class CPlayer : public CPlayerPdb { public: CThingPlayerCreature* GetPControlledCreature() const; }; //00487DC0

#define CTCBoastUI CTCBoastUILayout
#include "engine/CTCBoastUI.h"
#undef CTCBoastUI

class CTCBoastUI : public CTCBoastUILayout
{
public:
    void AllowUIToOpen(bool);   // 0x0056A930
};
struct BoastMethods:CTCBoastUI{}; //0056A930
FABLE_FORCEINLINE BoastMethods* GetBoast(CThingNative* hero){BoastMethods* result;if(hero->UsingComponents[7]&0x100){ComponentMap* map=&hero->Components;ComponentEntry* it=map->LowerBound((ETCInterfaceType)232);ComponentEntry* end=map->Finish;result=static_cast<BoastMethods*>(((void*)(it==end||it->first>232?end:it)->second));}return result;}
struct MovieTransitionState{unsigned char Prefix[0xf4];bool Busy;};extern MovieTransitionState* g_MovieTransitionState; //013B8790
extern long g_MovieCameraID; //01380C34
extern long g_MovieState; //013BAD00
// The output handle is the shared engine/CScriptGameResourceObjectMovieBase.h class; 0x008AB970 is its SetImplementation (FableWin.pdb
// ?SetImplementation@CScriptGameResourceObjectMovieBase@@QAEXPAV1@@Z; PImp.Reset body, twin of the ScriptedThingBase one 0x008AB960).
#include "engine/CCountedPointer.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
struct MovieResource:CScriptGameResourceObjectMovieBase{CGameScriptInterface* ScriptInterface;bool Enabled;unsigned char Align[3];MovieResource(CGameScriptInterface*,bool);}; //006E7630, RTTI01260F00 CScriptGameResourceObjectMovie (no shared header; on the shared base)
struct CGameScriptInterfaceMethods:CGameScriptInterface{
 typedef void(CGameScriptInterfaceMethods::*ConstVoidMethod)()const;
 void NewScriptFrame()const{(this->*static_cast<ConstVoidMethod*>(__vftable)[7])();} //006E7410
 void CancelAbilitiesForCutscene()const{(this->*static_cast<ConstVoidMethod*>(__vftable)[396])();} //0089D690
 void StartLetterBox(float,float,float); //006E7280 forwards to DisplayEngine00434BA0
 void StartMovieSequence(const CCharString&,CScriptGameResourceObjectMovieBase&);
};
FABLE_STATIC_ASSERT(sizeof(CGameScriptInterfaceMethods::ConstVoidMethod)==4);
FABLE_STATIC_ASSERT(sizeof(MovieResource)==24);
void CGameScriptInterfaceMethods::StartMovieSequence(const CCharString& name,CScriptGameResourceObjectMovieBase& output){
 CThingPlayerCreature* hero=PlayerManager->GetMainPlayer().GetPControlledCreature();if(hero&&!(hero->DeletedFlags&1)&&(hero->UsingComponents[7]&0x100)){
 GetBoast(hero)->AllowUIToOpen(false);}
 while(InMovieSequence)NewScriptFrame();while(g_MovieTransitionState->Busy)NewScriptFrame();
 InMovieSequence=true;StartLetterBox(0.0f,0.5f,1.7777778f);CancelAbilitiesForCutscene();output.SetImplementation(new MovieResource(this,true));g_MovieCameraID=-1;g_MovieState=0;
}