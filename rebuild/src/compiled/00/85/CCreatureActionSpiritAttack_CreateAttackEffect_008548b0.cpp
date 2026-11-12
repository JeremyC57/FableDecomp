// CCreatureAction_SpiritAttack::CreateAttackEffect() -- look up the lightning-spell ability def and its special-effects sub-def,
//   create the effect the co-op spirit names as its attack effect at the target's position and keep it as the action's
//   PParticleEmitter; when that thing is alive and has a particle emitter, attach the emitter to the creature's
//   MAIN_FOCAL_POSITION_NAME helper and feed it LIGHTNING_START (the spirit's focal point) and LIGHTNING_END (the target's
//   graphic-appearance focal point).
// Leaf: ?CreateAttackEffect@CCreatureAction_SpiritAttack@@AAEXXZ
// Retail: Fable.exe @ 0x008548B0, 723 bytes, __fastcall, plain `ret` (Ego_r.pdb + FableWin.pdb: private, non-virtual).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CCharString(const char*,long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetGlobalIndexFromClassIndex 0x009AD390,
//   CDefinitionManager::GetDef<CSpecialAbilitiesLightningSpellDef> 0x007A35D0, CDefClassBase::GetSubDef<CSpecialEffectsDef>
//   0x00669580, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020, CTCCoopSpirit::GetAttackEffectName 0x00670010 /
//   GetFocalPos 0x0066FFD0, CSpecialEffectsDef::GetEffectFromString 0x00669D30, CThing::GetPos 0x004C73D0,
//   CTCDParticleEmitter::Create(long,const C3DVector&,bool) 0x006E0880 / AttachToThing 0x006E0BE0 /
//   SetEmitterUpdateParameters 0x006E11A0, CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10,
//   NGraphicAppearance::CTCGraphicAppearance::GetFocalPos 0x004C1060, CCRC::Calc 0x004014A0,
//   NParticleEngine::CParticleUpdateParameters<unsigned long,unsigned long> ctor 0x006E24D0 / Update 0x0076C750 / dtor 0x006E2500,
//   vector<pair<unsigned long,C3DVector> >::_M_insert_overflow 0x0076ADA0, CArray<pair<unsigned long,C3DVector> >::push_back
//   0x0076B120 (Ego_r.pdb + FableWin.pdb carry only this instance; landed label respelled via NAME_FIXES.tsv). Global MAIN_FOCAL_POSITION_NAME 0x013BAC48.
// Shape: the first vector parameter goes in through CVectorMap::Insert (STLport push_back expanded in place, then Dirty = 1);
//   the second through the map's CArray::push_back, called out of line (Ego_r.pdb ?push_back@?$CArray@U?$pair@KVC3DVector@@...
//   is an out-of-line instance), so retail marks Dirty only once -- the same TU's FrameUpdate 0x00854B90 Inserts both and
//   stores Dirty twice. The parameter block's constructor is called out of line (0x006E24D0) here.
// Header opt-ins (overlay): FABLE_RETAIL_CVECTORMAP_INSERT_VECTOR_PUSHBACK (Insert expands STLport's push_back, not CArray's),
//   FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_CTOR_OUTOFLINE.
#include "fable_compiler.h"
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_SKYBEAM_ORIGINAL
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_CVECTORMAP_INSERT_FORCEINLINE
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE
#define FABLE_RETAIL_CVECTORMAP_INSERT_VECTOR_PUSHBACK
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMETERS_CTOR_OUTOFLINE
#define FABLE_RETAIL_CCREATUREACTION_SPIRITATTACK_TYPED
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/C3DVector.h"

#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/ThingAlive.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NParticleEngine_CParticleUpdateParameters.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_SpiritAttack.h"

// Ego_r.pdb def class of "SPECIAL_ABILITIES_LIGHTNING_SPELL_DEF" (engine/_quarantine/CSpecialAbilitiesLightningSpellDef.h is a
// flattened generated layout); only its CDefClassBase part is used here (GetSubDef<CSpecialEffectsDef>).
class CSpecialAbilitiesLightningSpellDef : public CParentDefClassBase {};
#define CTCCoopSpirit CTCCoopSpiritFields
#include "engine/_quarantine/CTCCoopSpirit.h"
#undef CTCCoopSpirit
class CTCCoopSpirit : public CTCCoopSpiritFields { public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return (ETCInterfaceType)0xc7; }
    CCharString GetAttackEffectName() const;     // 0x00670010
    C3DVector GetFocalPos() const;               // 0x0066FFD0
};
extern const CCharString MAIN_FOCAL_POSITION_NAME;   // 0x013BAC48

void CCreatureAction_SpiritAttack::CreateAttackEffect()
{
    CThing* target = PTarget.Get();
    long index;
    {
        CCharString name("SPECIAL_ABILITIES_LIGHTNING_SPELL_DEF");
        index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
    }
    CDefPointer<const CSpecialAbilitiesLightningSpellDef> def;
    CGameDefinitionManager::Get().GetDef(index, &def);
    CDefPointer<const CSpecialEffectsDef> effects;
    if (def->GetSubDef(&effects)) {
        const CTCCoopSpirit* tc_spirit = 0;

        PCreature->GetTC(&tc_spirit);
        long effect_index;
        effects->GetEffectFromString(tc_spirit->GetAttackEffectName(), effect_index);
        PParticleEmitter = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(effect_index, target->GetPos(), false));
        if (IS_THING_ALIVE(PParticleEmitter)) {
            CTCDParticleEmitter* emitter = 0;
            PParticleEmitter->GetTC(&emitter);
            if (emitter) {
                emitter->AttachToThing(*PCreature, 12, MAIN_FOCAL_POSITION_NAME, 0, 0.0f);
                C3DVector start_pos = tc_spirit->GetFocalPos();
                C3DVector end_pos = target->GetTCGraphicAppearance()->GetFocalPos(1.0f);
                std::pair<unsigned long, C3DVector> u_start_pos(CCRC::Calc(0, "LIGHTNING_START", 15), start_pos);
                std::pair<unsigned long, C3DVector> u_end_pos(CCRC::Calc(0, "LIGHTNING_END", 13), end_pos);
                NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> update_params;
                update_params.VectorParams.Insert(u_start_pos);
                update_params.VectorParams.push_back(u_end_pos);
                update_params.Update();
                emitter->SetEmitterUpdateParameters(update_params);
            }
        }
    }
}