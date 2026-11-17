// NCombatActionHelpers::CreateDustEffectForCreature(CThingCreatureBase&, const CWorldMap&, const CGameDefinitionManager&, const C3DVector&,
// vector<CThing*>*) -- with combat particle effects on: the strongest engine theme under the creature gives its surface material, whose
// random slide-foot particle effect is created at the creature (optionally recorded) and pointed along the direction
// Flags: /O2 /Oy /W3 /Gr
// Leaf: ?CreateDustEffectForCreature@NCombatActionHelpers@@YIXAAVCThingCreatureBase@@ABVCWorldMap@@ABVCGameDefinitionManager@@ABVC3DVector@@PAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F7950, __fastcall, `ret 0Ch`. Callees: CThing::GetPos 0x004C73D0, CWorldMap::GetStrongestEngineThemeAt
//   0x0063ADF0, CDefinitionManager::GetDef<CEngineThemeDef> 0x0063F3A0 / GetDef<CMaterialDef> 0x0066C340, CMaterialDef::
//   GetRandomSlideFootParticleEffect 0x008EE5A0, CTCDParticleEmitter::Create(long, const C3DVector&, bool) 0x006E0880,
//   vector<CThing*>::push_back 0x00436C80, CThing::GetTCDriver<CTCDParticleEmitter> 0x004ACB50, CCRC::Calc 0x004014A0,
//   NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> ctor 0x006E24D0 / dtor 0x006E2500,
//   CArray<pair<unsigned long, C3DVector> >::push_back 0x0076B120, CTCDParticleEmitter::SetEmitterUpdateParameters 0x006E11A0.
//   Data: NGlobalConsole::CombatParticleEffects 0x013756F5 (the name the landed sibling 0x008F7860 IsToCreateCombatParticleEffects uses).
// NF49 (round 7): the def classes, the def handle and the manager on the shared headers (GETDEF_GLOBAL_BODY); CEngineThemeDef has no shared
//   header (engine/_quarantine only): modelled on the shared CDefClassBase with MaterialDef +0x8c as landed 0x007F0300.
// NF67 (round 7): the NDust008f7950 namespace (local C3DVector / CThing / CThingCreatureBase / CWorldMap / CThingVector with a malloc
//   push_back body / CTCDParticleEmitter / CParticleUpdateParameters / SVectorParam / CCRC models, the stand-in GetTCDriver body) is gone:
//   ported onto the shared headers with the 0x008548B0 particle-parameter block; the self takes the queued q10436 PDB spelling.
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMATERIALDEF_GETRANDOMSLIDEFOOTPARTICLEEFFECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_STRONGESTTHEME
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_CTOR_OUTOFLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMaterialDef.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CWorldMap.h"
#include "engine/CCRC.h"
#include "engine/NParticleEngine_CParticleUpdateParameters.h"
#include "engine/CTCDParticleEmitter.h"
class CEngineThemeDef : public CDefClassBase { public:            // Ego_r.pdb CEngineThemeDef (no shared header); retail offsets
    unsigned char _pad_0x25[0x8c - 0x25];
    long          MaterialDef;                                  // +0x8c
};
namespace NGlobalConsole { extern bool CombatParticleEffects; }    // 0x013756F5

namespace NCombatActionHelpers {
void __fastcall CreateDustEffectForCreature(CThingCreatureBase& creature, const CWorldMap& world_map, const CGameDefinitionManager& def_manager,
                                            const C3DVector& direction, std::vector<CThing*>* created_things)
{
    if (!NGlobalConsole::CombatParticleEffects)
        return;
    long strength = 0;
    long theme = world_map.GetStrongestEngineThemeAt(creature.GetPos(), strength);
    CDefPointer<const CEngineThemeDef> p_theme;
    if (def_manager.GetDef(theme, &p_theme)) {
        if (p_theme->MaterialDef) {
            CDefPointer<const CMaterialDef> p_material;
            def_manager.GetDef(p_theme->MaterialDef, &p_material);
            long effect = p_material->GetRandomSlideFootParticleEffect();
            if (effect) {
                CThing* p_emitter = CTCDParticleEmitter::Create(effect, creature.GetPos(), false);
                if (p_emitter) {
                    if (created_things)
                        created_things->push_back(p_emitter);
                    CTCDParticleEmitter* ptc_emitter = 0;
                    p_emitter->GetTCDriver(&ptc_emitter);
                    std::pair<unsigned long, C3DVector> direction_param(CCRC::Calc(0, "DIRECTION", 9), direction);
                    NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> update_params;
                    update_params.VectorParams.push_back(direction_param);
                    ptc_emitter->SetEmitterUpdateParameters(update_params);
                }
            }
        }
    }
}
}
