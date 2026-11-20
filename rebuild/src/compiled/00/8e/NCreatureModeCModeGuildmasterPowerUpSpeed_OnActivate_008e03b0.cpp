// NCreatureMode::CModeGuildmasterPowerUpSpeed::OnActivate() -- guildmaster speed power-up: find nearby bodyguards that hold a melee weapon, give up to three of them a multi-strike component (unless they have one) and a POWER_UP_BEAM effect
// Inline-budget: header opt-ins that set expanded vs called, each removed singly from this source: FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY (the 0x008E4100 instance body visible but out of line: removed, DIFFER(823v811)), FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY (GetSubDef<CSpecialEffectsDef> 0x00669580 visible but out of line: removed, DIFFER(813v811)), FABLE_RETAIL_CINTELLIGENTPOINTER_COPY (the inline CIntelligentPointer copy ctor of the two push_backs, base ctor / GetPItem / SetPItem calls at 0x008E05A8..0x008E05C1 and 0x008E0658..0x008E0671: removed, DIFFER(784v811)). No flag, no pragma.
// Leaf: ?OnActivate@CModeGuildmasterPowerUpSpeed@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E03B0, 811 bytes, thiscall `ret`.
// Naming: Ego_r.pdb 0x00820330 / FableWin.pdb creature_modes.obj #282 (0x02862550, 2481 B = Ego_d's size) ?OnActivate@CModeGuildmasterPowerUpSpeed@NCreatureMode@@UAEXXZ.
//   Locals as Ego_d (guards, effect_index, pEffects, guard_def_index, p_ai_creature, random_guard, p_emitter_thing, it, ptc_multi_strike).
//   The sibling Shield mode's landed OnActivate 0x008E0060 is the model (same TU, creature_modes.obj); its CMainGameComponent-based
//   duration_in_frames has no retail trace here (Ego_d lists one; it is dead and its constant unknown, so it is left out).
// Filter: the search binds CFilter_Bind2<CThingFilter_HasDefGlobalIndex, `anonymous namespace'::CThingFilter_IsHoldingMeleeWeapon> through
//   GFFilter_Bind2 (FableWin.pdb ??$GFFilter_Bind2@..IsHoldingMeleeWeapon@?A0x..@@ 0x0286DF60; inlined in retail: the two pointer words
//   stored at 0x008E0413 / 0x008E0422), the instance the landed 0x008E4100 compiles. The anonymous class is the landed 0x008E4100 text plus
//   its user default constructor (FableWin.pdb ??0CThingFilter_IsHoldingMeleeWeapon@?A0x..@@QAE@XZ 0x02863170, right after this function in
//   creature_modes.obj; Ego_d has it too): without it the temporary is value-initialised, `mov byte ptr [esp+17h],bl`, DIFFER(827v811).
// Callees: CModeStandBase::OnActivate 0x008621A0, CThingAICreature::SuspendAI 0x00833A10, CGameDefinitionManager::Get 0x0044C6B0 /
//   CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, CThing::GetPos 0x004C73D0, __ftol2 0x00BFEA70, CModeBase::GetWorld 0x0073A680,
//   CThingSearchTools::GetAllThingsInRoughArea<CThingAICreature, vector<CThingAICreature*>, CFilter_Bind2<..> > 0x008E4100,
//   GetSubDef<CSpecialEffectsDef> 0x00669580, CSpecialEffectsDef::GetEffectFromString 0x00669D30, CWorld::GetWorldSeed 0x0049D810,
//   GFROR13 0x00497890, CThing::AddTC 0x004C9D60, CBaseIntelligentPointer ctor / GetPItem / SetPItem / dtor 0x00A01B10 / 0x00A01B50 /
//   0x00A01B90 / 0x00A01C10, vector<CIntelligentPointer<CThingAICreature> >::_M_insert_overflow 0x007AFD10, vector<CIntelligentPointer<
//   CThing> >::_M_insert_overflow 0x004AC940, memmove 0x00BFEAE6 (erase), CTCDParticleEmitter::Create 0x006E0880, free 0x00BFEA14.
// Data: "CREATURE_GUILDMASTER_BODYGUARD" 0x01285C70, "POWER_UP_BEAM" 0x01287340, "CTCMultiStrike" 0x0123D374; vtables 0x0123E7E0 /
//   0x01238C6C. The multi-strike test is HasTC(TCI_MULTI_STRIKE = 135): bit 7 of the presence byte +0x30 (`test al,al ; js`, 0x008E0541).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_SKYBEAM_ORIGINAL
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_DRAINED
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CMODESTANDBASE_ACTIVATE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCCARRYING_ISTHINGCARRYINGMELEEWEAPON
#include <string.h>
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/ThingAlive.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CTCMultiStrike.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CWorld.h"
class CTCMapwho;   // forward declaration the default CTCMapwho.h path needs before NThingMap::CMapwhoNode
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasDefGlobalIndex.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CTCCarrying.h"
#include "engine/C2DBoxI.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFRandom.h"
#include "engine/NCreatureMode_CModeGuildmasterPowerUpShield.h"

namespace { class CThingFilter_IsHoldingMeleeWeapon { public: CThingFilter_IsHoldingMeleeWeapon() { } FABLE_FORCEINLINE bool operator()(const CThing& p)const{return CTCCarrying::IsThingCarryingMeleeWeapon(p);}}; }
void NCreatureMode::CModeGuildmasterPowerUpSpeed::OnActivate()
{
 CModeStandBase::OnActivate();
 if (GetParentThing().GetType() == 1) {
  CThingAICreature* p_ai_creature = static_cast<CThingAICreature*>(&GetParentThing());
  p_ai_creature->SuspendAI(true, this);
 }
 std::vector<CThingAICreature*> guards;
 long guard_def_index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("CREATURE_GUILDMASTER_BODYGUARD");
 GetWorld().GetThingSearchTools().GetAllThingsInRoughArea<CThingAICreature>(C2DBoxI(GetParentThing().GetPos().ToC2DCoordI(), 20),
  GFFilter_Bind2(CThingFilter_HasDefGlobalIndex(guard_def_index), CThingFilter_IsHoldingMeleeWeapon()), guards);
 long effect_index = 0;
 CDefPointer<const CSpecialEffectsDef> pEffects;
 GetParentThing().GetPBaseDef()->GetSubDef(&pEffects);
 pEffects->GetEffectFromString("POWER_UP_BEAM", effect_index);
 while (guards.size() > 0 && Guards.size() < 3) {
  long random_guard = GFRandom(guards.size(), GetWorld().GetWorldSeed());
  if (!guards[random_guard]->HasTC(TCI_MULTI_STRIKE)) {
   CTCMultiStrike* ptc_multi_strike = static_cast<CTCMultiStrike*>(guards[random_guard]->AddTC("CTCMultiStrike", false, 0));
  }
  Guards.push_back(CIntelligentPointer<CThingAICreature>(guards[random_guard]));
  std::vector<CThingAICreature*>::iterator it = guards.begin() + random_guard;
  guards.erase(it);
  CThing* p_emitter_thing = CTCDParticleEmitter::Create(effect_index, GetParentThing().GetPos(), false);
  if (IS_THING_ALIVE(p_emitter_thing))
   LightningBeamParticleEmitters.push_back(CIntelligentPointer<CThing>(p_emitter_thing));
 }
}