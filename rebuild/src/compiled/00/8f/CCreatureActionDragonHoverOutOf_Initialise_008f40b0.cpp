// CCreatureAction_DragonHoverOutOf::Initialise() -- animate descent, disable lifting/targeting, and stop solidity against the player.
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
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
// NF47 (round 7): 0x013B86A0 is ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static, read through the inline
//   CMainGameComponent::Get(), engine/CMainGameComponent.h), its PPlayerManager scoped_ptr +0x1C through the inline GetPlayerManager() (FableWin.pdb ?GetPlayerManager@CMainGameComponent@@QAEAAVCPlayerManager@@XZ); was the stand-in CGame* g_Game.
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#include "engine/CMainGameComponent.h"
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
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CCombatActionBase { public:
 void* IntelligentPointer;
 CBaseIntelligentPointer PCreature;
 void SetCombatAnimation(const CCharString&); //008574D0
 virtual void Initialise(); //00857520
};
class CCreatureAction_DragonHoverOutOf : public CCombatActionBase { public:void Initialise();};
// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x007E4330 ?SetThingIsEpicSpellable@CTCWillResponse@@SIXPAVCThing@@_N@Z (was ?SetThingIsAbleToBeLifted@@YIXPAVCThing@@_N@Z)
class CTCWillResponse { public: static void __fastcall SetThingIsEpicSpellable(CThing*, bool); };
void CCreatureAction_DragonHoverOutOf::Initialise()
{
 {CCharString animation("HOVER_OUT_OF",-1);SetCombatAnimation(animation);}
 CCombatActionBase::Initialise();
 CTCWillResponse::SetThingIsEpicSpellable(((CThing*)PCreature.GetPItem()),false);
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
 if(solid)solid->AddThingNotSolidTo(CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().PeekPControlledCreature());
}