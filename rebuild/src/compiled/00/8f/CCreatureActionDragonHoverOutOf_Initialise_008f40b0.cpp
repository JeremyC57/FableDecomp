// CCreatureAction_DragonHoverOutOf::Initialise() -- animate descent, disable lifting/targeting, and stop solidity against the player.
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's pointer members First/Last are the header CArray base's begin() / end() (STLport vector start +0 / finish +4).
// Retail008F40B0 length277. Targeted interface9, SolidToThing77, native component vector+44.
#define CCreatureAction_DragonHoverOutOf CCreatureAction_DragonHoverOutOf_PDB
#include "engine/CCreatureAction_DragonHoverOutOf.h"
#undef CCreatureAction_DragonHoverOutOf
#define CThing CThing_PDB
#include "engine/_quarantine/CThing.h"
#undef CThing
#define CPlayer CPlayer_PDB
#include "engine/CPlayer.h"
#undef CPlayer
#define CPlayerManager CPlayerManager_PDB
#include "engine/CPlayerManager.h"
#undef CPlayerManager
#define CCombatActionBase CCombatActionBase_PDB
#include "engine/_quarantine/CCombatActionBase.h"
#undef CCombatActionBase
#include "engine/CTCTargeted.h"
#include "fable_string.h"
class CThing;
class CTCParamsBase;
class CThingPlayerCreature;
class CTCSolidToThing { public:void AddThingNotSolidTo(const CThing*);}; //00770FA0 (Ego_r.pdb: PBVCThing)
class CPlayer { public:const CThingPlayerCreature* PeekPControlledCreature() const;}; //00487DD0, native intelligent pointer+2c
class CPlayerManager { public:public:
    CPlayer& GetMainPlayer();   // 0x00449970
}; //00449970 selects player ID at+1c
class CGame { public:unsigned char _nativeBeforePlayerManager[0x1c];CPlayerManager* PPlayerManager;};
extern CGame* g_Game; //013B86A0
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
 unsigned char _nativeBeforeUsingComponents[0x20];
 unsigned long UsingComponents[9];
 CInterfaceMap Components;
 CTCBase* AddTC(CCharString const&a0, bool a1, CTCParamsBase const*a2); //004C9D60
};
class CThingPlayerCreature : public CThing { };
class CBaseObject;
struct CBaseIntelligentPointer {
 virtual ~CBaseIntelligentPointer(); //00A01C10
 void* PData;
 CBaseObject* GetPItem() const; //00A01B50
};
class CCombatActionBase { public:
 void* IntelligentPointer;
 CBaseIntelligentPointer PCreature;
 void SetCombatAnimation(const CCharString&); //008574D0
 virtual void Initialise(); //00857520
};
extern void FABLE_FASTCALL SetThingIsAbleToBeLifted(CThing*,bool); //007E4330
class CCreatureAction_DragonHoverOutOf : public CCombatActionBase { public:void Initialise();};
void CCreatureAction_DragonHoverOutOf::Initialise()
{
 {CCharString animation("HOVER_OUT_OF",-1);SetCombatAnimation(animation);}
 CCombatActionBase::Initialise();
 SetThingIsAbleToBeLifted(((CThing*)PCreature.GetPItem()),false);
 CThing* thing=((CThing*)PCreature.GetPItem());
 CTCTargeted* targeted;
 if(thing->UsingComponents[0]&0x200){
  CInterfaceMap* map=&thing->Components;ETCInterfaceType type = (ETCInterfaceType)(9);
  CInterfaceMap::value_type* p=map->LowerBound(type);
  if(p==map->end() || p->first>9)p=map->end();
  targeted=static_cast<CTCTargeted*>(((void*)p->second));
 }
 targeted->Targetable=false; // native requires Targeted component on dragon
 thing=((CThing*)PCreature.GetPItem());
 CTCSolidToThing* solid;
 if(thing->UsingComponents[2]&0x2000){
  CInterfaceMap* map=&thing->Components;ETCInterfaceType type = (ETCInterfaceType)(0x4d);
  CInterfaceMap::value_type* p=map->LowerBound(type);
  if(p==map->end() || p->first>0x4d)p=map->end();
  solid=static_cast<CTCSolidToThing*>(((void*)p->second));
 } else {
  CCharString name("CTCSolidToThing",-1);
  solid=((CTCSolidToThing*)((CThing*)PCreature.GetPItem())->AddTC(name, false, 0));
 }
 if(solid)solid->AddThingNotSolidTo(g_Game->PPlayerManager->GetMainPlayer().PeekPControlledCreature());
}