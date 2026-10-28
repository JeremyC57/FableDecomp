// CCreatureAction_DragonHoverOutOf::Initialise() -- animate descent, disable lifting/targeting, and stop solidity against the player.
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
struct CInterfaceMap {
 struct Entry {long Type;void* Component;};
 Entry* First;Entry* Last;
 Entry* LowerBound(const long*); //0040F020
};
class CThing { public:
 unsigned char _nativeBeforeUsingComponents[0x20];
 unsigned long UsingComponents[9];
 CInterfaceMap Components;
 CTCSolidToThing* AddTC(const CCharString&,bool,CTCParamsBase*); //004C9D60
};
class CThingPlayerCreature : public CThing { };
struct CBaseIntelligentPointer {
 virtual ~CBaseIntelligentPointer(); //00A01C10
 void* PData;
 CThing* GetPItem() const; //00A01B50
};
class CCombatActionBase { public:
 void* __vftable;void* IntelligentPointer;
 CBaseIntelligentPointer PCreature;
 void SetAnimation(const CCharString&); //008574D0
 void Initialise(); //00857520
};
extern void FABLE_FASTCALL SetThingIsAbleToBeLifted(CThing*,bool); //007E4330
class CCreatureAction_DragonHoverOutOf : public CCombatActionBase { public:void Initialise();};
void CCreatureAction_DragonHoverOutOf::Initialise()
{
 {CCharString animation("HOVER_OUT_OF",-1);SetAnimation(animation);}
 CCombatActionBase::Initialise();
 SetThingIsAbleToBeLifted(PCreature.GetPItem(),false);
 CThing* thing=PCreature.GetPItem();
 CTCTargeted* targeted;
 if(thing->UsingComponents[0]&0x200){
  CInterfaceMap* map=&thing->Components;long type=9;
  CInterfaceMap::Entry* p=map->LowerBound(&type);
  if(p==map->Last || p->Type>9)p=map->Last;
  targeted=static_cast<CTCTargeted*>(p->Component);
 }
 targeted->Targetable=false; // native requires Targeted component on dragon
 thing=PCreature.GetPItem();
 CTCSolidToThing* solid;
 if(thing->UsingComponents[2]&0x2000){
  CInterfaceMap* map=&thing->Components;long type=0x4d;
  CInterfaceMap::Entry* p=map->LowerBound(&type);
  if(p==map->Last || p->Type>0x4d)p=map->Last;
  solid=static_cast<CTCSolidToThing*>(p->Component);
 } else {
  CCharString name("CTCSolidToThing",-1);
  solid=PCreature.GetPItem()->AddTC(name,false,0);
 }
 if(solid)solid->AddThingNotSolidTo(g_Game->PPlayerManager->GetMainPlayer().PeekPControlledCreature());
}