// 0087c580 CPlayer::GlobalEventCreatureDied
// Leaf: ?GlobalEventCreatureDied@CPlayer@@QAEXPAVCThingCreatureBase@@@Z
// Flags:
#define CPlayer CPlayerLayout
#include "engine/CPlayer.h"
#undef CPlayer
#include "engine/CBaseClass.h"
#define CPlayerManager CPlayerManagerFields
#include "engine/CPlayerManager.h"
#undef CPlayerManager
#include "engine/CThingCreatureBase.h"
#include "engine/_quarantine/CThing.h"
class CBaseObject;class CBaseObjectPointer;
class CBaseIntelligentPointer{private:CBaseObjectPointer*PData;public:virtual~CBaseIntelligentPointer();CBaseObject*GetPItem()const;};
enum EPlayerMode{};
class CPlayerManager:public CPlayerManagerFields{public:long GetMainPlayerNumber()const;void RemovePlayerCharacter(long);};
class CThingPlayerCreature;
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:T*Get()const{return reinterpret_cast<T*>(GetPItem());}};
class CInitBaseClass:public CBaseClass{private:bool Valid;protected:bool Initialise();void Uninitialise();public:virtual~CInitBaseClass();virtual void Validate()const;};
// Layout: original CInitBaseClass8 then private CPlayer fields8..34, shared header confirms Number28 and PlayerCharacter34. Unused tail omitted.
class CPlayer:public CInitBaseClass{private:bool DrawingFreeCamDebug,PlayerIsCharacter;CGamePlayerInterface*PlayerInterface;CPlayerManager*PlayerManager;const CMainGameComponent*Component;const CGameDefinitionManager*DefinitionManager;CWorld*World;CThingManager*ThingManager;CWorldMap*PWorldMap;long Number;CIntelligentPointer<CThingPlayerCreature>ControlledCreature,PlayerCharacter;
public:void AddMode(EPlayerMode,bool);void GlobalEventCreatureDied(CThingCreatureBase*);
 long GetNumber()const{return Number;}
 bool IsMainPlayer()const{if(PlayerManager->GetMainPlayerNumber()!=GetNumber())return false;return true;}
};
void CPlayer::GlobalEventCreatureDied(CThingCreatureBase*creature){
 if(creature&&PlayerCharacter.Get()){
  unsigned __int64 id=reinterpret_cast<const CThing*>(creature)->UniqueID;
  if(id==reinterpret_cast<const CThing*>(PlayerCharacter.Get())->UniqueID){
   if(IsMainPlayer())AddMode(static_cast<EPlayerMode>(3),true);
   else PlayerManager->RemovePlayerCharacter(GetNumber());
  }
 }
}