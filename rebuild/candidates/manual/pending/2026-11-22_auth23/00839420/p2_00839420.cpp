// NThingHelpers::CreateEffectOnHit(const CThing& thing, const CHitParameters& hit, const CWorldMap& worldMap, float strength) -- spawn the hit particle effect: melee hits on creatures whose def does not use the actual hit position move it to 0.75 x the physics radius towards the striker (direction swung to the striker's last action hand), then, when in the map and within the def's render fade distance of the game LOD centre, create the material's blended (region-weighted) or random sword-hit effect and hand it the DIRECTION vector
// Grade: DIFFER(1304v1308) -- 75 masked diff lines (best p of 17 grades): same calls, branches and x87 order; residue is frame-slot order (offset/direction and material/creatureDef/effect slots permuted), the CDefPointer<const CCreatureDef> release in load/dec/store shape where retail decrements in place, and the early returns that retail ends with their own release + epilogue. Next idea: declaration/scope orders for creatureDef and material, and the creatureDef test written as a separate GetSubDef statement.
// Leaf: ?CreateEffectOnHit@NThingHelpers@@YIXABVCThing@@ABVCHitParameters@@ABVCWorldMap@@M@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00839420, 1308 bytes, __fastcall (thing ecx, hit edx; map, strength on the stack), `ret 8`.
// Identity: Ego_r.pdb ?CreateEffectOnHit@NThingHelpers@@YIXABVCThing@@ABVCHitParameters@@ABVCWorldMap@@M@Z (the catalog's _global::CreateEffectOnHit);
//   the landed CMaterialDef::GetRandomSwordHitParticleEffect 0x008EE600 names this caller.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, NCombatActionHelpers::IsToCreateCombatParticleEffects 0x008F7860, CHitParameters::IsMeleeStrike
//   0x00868860, GetSubDef<CCreatureDef> 0x00485815, CThing::GetPos 0x004C73D0, C3DVector::AccurateNormalise 0x00A14440, CWorldMap::IsPosInMap 0x004FC600 /
//   GetLoadedRegion 0x004FB150 / GetRegion 0x004FC180, CMainGameComponent::PeekDefinitionManager 0x00415FBC, CDefString::GetString 0x00415DD0,
//   CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410, CMaterialDef::GetBlendedSwordHitEffect 0x008EE790 / GetRandomBerserkedSwordHitParticleEffect
//   0x008EE720 / GetRandomSwordHitParticleEffect 0x008EE600, CreateScaledEffect 0x006E11F0, CTCDParticleEmitter::Create 0x006E0880, IS_THING_ALIVE
//   0x00436380 / 0x0040DCE0, CThing::GetTCDriver<CTCDParticleEmitter> 0x004ACB50, CCRC::Calc 0x004014A0, CParticleUpdateParameters() 0x006E24D0 /
//   ~ 0x006E2500, push_back 0x0076B120, SetEmitterUpdateParameters 0x006E11A0; CWorld slot 8 GetGameLODCentre, CDefClassBase slot 10
//   GetInstantiationName. Globals NGlobalConsole::CombatParticleEffects 0x013756F5, DELTA_NOTIONAL_ZERO 0x0129BA3C; literals 0.75f, -1.0f.
// Headers: FABLE_RETAIL_CHITPARAMETERS_EFFECTONHIT (IsCreateParticleEffectOnHit / GetPHitMaterialDef, FableWin.pdb) and
//   FABLE_RETAIL_CTHINGCREATUREBASE_LASTHANDEDNESS (GetLastActionHandedness, Ego_r/FableWin) in the chunk 778 overlay. GFSqr is ?GFSqr@@YIMM@Z.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CROSSWITHUP
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CHITPARAMETERS_EFFECTONHIT
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_LASTHANDEDNESS
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCDriverBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CDefString.h"
#include "engine/CThingBaseDef.h"
#include "engine/CCreatureDef.h"
#include "engine/CHitParameters.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CThingCreatureBase.h"

extern const float DELTA_NOTIONAL_ZERO;                             // 0x0129BA3C
namespace NGlobalConsole { extern bool CombatParticleEffects; }    // 0x013756F5
namespace NCombatActionHelpers { bool __fastcall IsToCreateCombatParticleEffects(const CThing& thing, const CThing* hitter); }   // 0x008F7860

class CDefinitionManager { public: long GetDefGlobalIndexFromName(const CCharString& name) const; };   // 0x009AD410
class CGameDefinitionManager : public CDefinitionManager {};
class CWorld { public:
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    virtual C3DVector GetGameLODCentre() const;                     // slot 8 0x0049DBE0
};
class CMainGameComponent { public:
    unsigned char _pad00[0x24];
    CWorld* PWorld;                                                 // +0x24 boost::scoped_ptr<CWorld>
    const CWorld& PeekWorld() const { return *PWorld; }
    const CGameDefinitionManager& PeekDefinitionManager() const;    // 0x00415FBC
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }
    static CMainGameComponent* PMainGameComponent;                  // 0x013B86A0
};
class CRegion { public:
    unsigned char _pad00[0x24];
    CDefPointer<const CDefClassBase> PDef;                          // +0x24 CDefPointer<const CRegionDef>
    CDefPointer<const CDefClassBase> GetPDef() const { return PDef; }
};
class CWorldMap { public:
    bool IsPosInMap(const C3DVector& pos) const;                    // 0x004FC600
    long GetLoadedRegion() const;                                   // 0x004FB150
    const CRegion& GetRegion(long region) const;                    // 0x004FC180
};
class CMaterialDef : public CDefClassBase { public:
    bool GetBlendedSwordHitEffect(long regionDef, float strength, long& effectA, long& effectB, float& blend) const;   // 0x008EE790
    long GetRandomBerserkedSwordHitParticleEffect() const;          // 0x008EE720
    long GetRandomSwordHitParticleEffect() const;                   // 0x008EE600
};
CThing* __fastcall CreateScaledEffect(long effectA, long effectB, float blend, const C3DVector& position, bool flag);   // 0x006E11F0
class CCRC { public: static unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length); };   // 0x004014A0
#include "engine/CBaseClass.h"
namespace NParticleEngine {
struct SVectorParam {
    unsigned long m_key;
    C3DVector     m_value;
    SVectorParam(const unsigned long& key, const C3DVector& value) : m_key(key), m_value(value) {}
};
struct SUpdateParam;
struct CVectorParamVector {
    SVectorParam* _M_start; SVectorParam* _M_finish; SVectorParam* _M_end_of_storage;
    __forceinline CVectorParamVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    void push_back(const SVectorParam& value);                      // 0x0076B120
};
struct CUpdateParamVector {
    SUpdateParam* _M_start; SUpdateParam* _M_finish; SUpdateParam* _M_end_of_storage;
    __forceinline CUpdateParamVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
struct CCompare {};
struct CVectorParamMap { CVectorParamVector Vector; CCompare Compare; bool Dirty; __forceinline CVectorParamMap() : Dirty(false) {} };
struct CUpdateParamMap { CUpdateParamVector Vector; CCompare Compare; bool Dirty; __forceinline CUpdateParamMap() : Dirty(false) {} };
struct CParticleUpdateParameters : CBaseClass {
    CVectorParamMap VectorParams;
    CUpdateParamMap UpdateParams;
    CParticleUpdateParameters();                                    // 0x006E24D0
    virtual ~CParticleUpdateParameters();                           // 0x006E2500
};
}
class CTCDParticleEmitter : public CTCDriverBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_PARTICLE_EMITTER; }
    static CThing* __fastcall Create(long type, const C3DVector& position, bool flag);    // 0x006E0880
    void SetEmitterUpdateParameters(const NParticleEngine::CParticleUpdateParameters& parameters);   // 0x006E11A0
};
template <class T> inline bool __fastcall IS_THING_ALIVE(const T& thing) { return &*thing && !thing->IsDead(); }
inline float __fastcall GFSqr(float value) { return value * value; }   // ?GFSqr@@YIMM@Z
namespace NThingHelpers {
void __fastcall CreateEffectOnHit(const CThing& thing_hit, const CHitParameters& hit_params, const CWorldMap& world_map, float delta_health)
{
    const CThing* pthing_hitting = 0;
    pthing_hitting = hit_params.GetPThingHitting();
    if (!NCombatActionHelpers::IsToCreateCombatParticleEffects(thing_hit, pthing_hitting))
    {
        return;
    }

    C3DVector point_of_hit = hit_params.GetHitPosition();

    if (!NGlobalConsole::CombatParticleEffects)
    {
        return;
    }
    if (!hit_params.IsCreateParticleEffectOnHit())
    {
        return;
    }

    C3DVector attack_direction = hit_params.GetHitDirection();
    CDefPointer<const CMaterialDef> pmaterial_def = hit_params.GetPHitMaterialDef();

    if (IS_THING_ALIVE(pthing_hitting) && (thing_hit.GetTypeFlags() & 8) && hit_params.IsMeleeStrike())
    {
        CDefPointer<const CCreatureDef> pcreature_def;
        if (thing_hit.GetPBaseDef()->GetSubDef(&pcreature_def) && !pcreature_def->UseActualHitPosForHitEffects)
        {
            C3DVector line_out = pthing_hitting->GetPos() - thing_hit.GetPos();
            line_out.Z = 0.0f;
            if (line_out.IsNonZero())
            {
                line_out.AccurateNormalise();

                C3DVector new_point_of_hit = thing_hit.GetPos() + line_out * thing_hit.GetPhysicsTC()->GetRadius() * 0.75f;
                new_point_of_hit.Z = point_of_hit.Z;

                C3DVector new_direction_of_hit = attack_direction;
                if (pthing_hitting->GetTypeFlags() & 8)
                {
                    C3DVector perp = line_out.CrossWithUp();
                    const CThingCreatureBase* phitter = static_cast<const CThingCreatureBase*>(pthing_hitting);
                    if (phitter->GetLastActionHandedness() == 0)
                    {
                        new_direction_of_hit = perp * -1.0f;
                    }
                    else if (phitter->GetLastActionHandedness() == 1)
                    {
                        new_direction_of_hit = perp * 1.0f;
                    }
                }

                point_of_hit = new_point_of_hit;
                attack_direction = new_direction_of_hit;
            }
        }
    }

    if (!world_map.IsPosInMap(point_of_hit))
    {
        return;
    }

    if (!thing_hit.GetPBaseDef().IsNull())
    {
        C3DVector lod_centre = CMainGameComponent::Get()->PeekWorld().GetGameLODCentre();
        if ((point_of_hit - lod_centre).GetSquaredMagnitude() > GFSqr(thing_hit.GetPBaseDef()->RenderFadeDistance))
        {
            return;
        }
    }

    if (!pmaterial_def.IsNull())
    {
        const CGameDefinitionManager& def_man = CMainGameComponent::Get()->PeekDefinitionManager();
        const CRegion& region = world_map.GetRegion(world_map.GetLoadedRegion());
        const CDefClassBase* pregion_def = region.GetPDef().Get();
        long region_def_index = 0;
        if (pregion_def)
        {
            region_def_index = def_man.GetDefGlobalIndexFromName(pregion_def->GetInstantiationName());
        }

        CThing* pthing_emitter = 0;
        long from_effect_id = 0;
        long to_effect_id = 0;
        float interpolate = 0.0f;

        if (pmaterial_def->GetBlendedSwordHitEffect(region_def_index, delta_health, from_effect_id, to_effect_id, interpolate))
        {
            if (from_effect_id && to_effect_id)
            {
                pthing_emitter = CreateScaledEffect(from_effect_id, to_effect_id, interpolate, point_of_hit, false);
            }
        }
        else
        {
            if (IS_THING_ALIVE(pthing_hitting) && pthing_hitting->HasTC((ETCInterfaceType)132))
            {
                from_effect_id = pmaterial_def->GetRandomBerserkedSwordHitParticleEffect();
            }
            else
            {
                from_effect_id = pmaterial_def->GetRandomSwordHitParticleEffect();
            }
            if (from_effect_id > 0)
            {
                pthing_emitter = CTCDParticleEmitter::Create(from_effect_id, point_of_hit, false);
            }
        }

        if (IS_THING_ALIVE(pthing_emitter))
        {
            CTCDParticleEmitter* tcd_particle_emitter = 0;
            pthing_emitter->GetTCDriver(&tcd_particle_emitter);
            NParticleEngine::SVectorParam parameter(CCRC::Calc(0, "DIRECTION", 9), attack_direction);
            NParticleEngine::CParticleUpdateParameters params;
            params.VectorParams.Vector.push_back(parameter);
            tcd_particle_emitter->SetEmitterUpdateParameters(params);
        }
    }
}
}
