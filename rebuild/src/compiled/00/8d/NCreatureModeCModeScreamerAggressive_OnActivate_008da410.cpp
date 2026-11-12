// NCreatureMode::CModeScreamerAggressive::OnActivate() -- the screamer's aggressive-mode activation: look up the "AGGRESSIVE"
// effect in the creature's CSpecialEffectsDef sub-def, kill any live previous effect thing (PEffect), spawn the new particle
// effect at the creature and attach its emitter to the "HIT_LOCATION_CYLINDER_01" helper, mark the combat component ready for
// melee, then run the stand-mode activation.
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DA410, 511 bytes (0x1FF), plain `ret` (virtual slot 5 of vtable 0x012681EC).
// Callees: CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580, CSpecialEffectsDef::GetEffectFromString 0x00669D30,
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem
//   0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C), CThing::Kill 0x004C9B80,
//   CThing::GetPos 0x004C73D0, CTCDParticleEmitter::Create 0x006E0880 (static __fastcall), CVectorMap::LowerBound 0x0040F020
//   (GetTC expanded, TCI_PARTICLE_EMITTER 0x4B and TCI_COMBAT 0x30), CTCDParticleEmitter::AttachToThing 0x006E0BE0,
//   CModeStandBase::OnActivate 0x008621A0; the def's release is CDefPointer's inline dtor (slot 1).
// Shape: the GetSubDef && GetEffectFromString expression statement makes the "AGGRESSIVE" name a conditional temporary (flag in bl).
//   Both literals convert implicitly to const CCharString& (an explicit CCharString(...) temp moves the arg pushes before the
//   construction). The combat pointer is declared uninitialised right before its GetTC (retail reads the slot on the false path).
//   GetSubDef / GetEffectFromString real bodies are visible as noinline (header opt-ins), as retail's caller behaves.
// Flags: none
// Leaf: ?OnActivate@CModeScreamerAggressive@NCreatureMode@@UAEXXZ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODESTANDBASE_ACTIVATE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_READYFORMELEE
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_CREATE
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/C3DVector.h"
#include "engine/CDefPointer.h"
#include "engine/CTCCombat.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/NCreatureMode_CModeScreamerAggressive.h"
#include "engine/ThingAlive.h"
void NCreatureMode::CModeScreamerAggressive::OnActivate()
{
 CDefPointer<const CSpecialEffectsDef> pdef;
 long effect=0;
 GetParentThing().PeekBaseDef().GetSubDef(&pdef)&&pdef->GetEffectFromString("AGGRESSIVE",effect);
 if(IS_THING_ALIVE(PEffect)){
  PEffect->Kill(true);
  PEffect=CIntelligentPointer<CThing>(0);
 }
 if(effect>0){
  PEffect=CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(effect,GetParentThing().GetPos(),false));
  if(IS_THING_ALIVE(PEffect)){
   CTCDParticleEmitter* pemitter=0;
   PEffect->GetTC(&pemitter);
   pemitter->AttachToThing(GetParentThing(),12,"HIT_LOCATION_CYLINDER_01",0,0.0f);
  }
 }
 CTCCombat* pcombat;
 GetParentThing().GetTC(&pcombat);
 pcombat->SetIsReadyForMelee(true);
 CModeStandBase::OnActivate();
}