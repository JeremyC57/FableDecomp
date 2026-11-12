// CTCBulletTime::PlayEffect(CIntelligentPointer<CThing>* effect, const CCharString& name, CThingCreatureBase* target, long attachType)
// -- look the named effect up in the bullet-time def's CSpecialEffectsDef sub-def, create its particle effect at the target's position
// and, if the effect thing is alive, attach its particle emitter to the target (attach type `attachType`, no helper, offset 0); when
// the caller passes an intelligent pointer the new effect thing is kept in it.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079CC80, 512 bytes, thiscall `ret 10h`, plain /O2 (no EH frame).
// Identity: Ego_r.pdb (0x007A26B0) + FableWin.pdb (0x026A7DD0) ?PlayEffect@CTCBulletTime@@AAEXPAV?$CIntelligentPointer@VCThing@@@@ABVCCharString@@PAVCThingCreatureBase@@J@Z.
// Callees: CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580 (template body visible in the TU, called out of line);
//   CSpecialEffectsDef::GetEffectFromString 0x00669D30 (same-TU noinline body, FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY);
//   CThing::GetPos 0x004C73D0; CTCSpecialAbilities::CreateParticleEffect 0x0072AB70 (static __fastcall);
//   CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound 0x0040F020 (CThing::GetTC(TCI_PARTICLE_EMITTER, T**) expanded);
//   CCharString(const char*, long) 0x0099EBF0 ("" 0x0122D70E) / ~CCharString 0x0099EAE0; CTCDParticleEmitter::AttachToThing 0x006E0BE0;
//   CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10
//   (CIntelligentPointer<CThing> vtable 0x01238C6C); the effects def's release (pointee slot 1, inline CDefPointer dtor).
// Layout: engine/CTCBulletTime.h (+ overlay opt-in FABLE_RETAIL_CTCBULLETTIME_PLAYEFFECT: PDef typed
//   CDefPointer<const CSpecialAbilitiesBulletTimeDef> at +0x20 and the PlayEffect declaration); engine/CThing.h (UsingComponents
//   +0x20, Components +0x44, Dead +0x91).
// Shape: the effect==0 test is the OUTER branch and each arm calls PDef->GetSubDef(&effects) itself; VC7.1 hoists the identical
//   call above the branch (hence the call before `cmp esi,ebx`, `found` re-tested in each arm), and the zero register (ebx) falls
//   out of that. The arm-2 effect thing goes straight into the CIntelligentPointer temporary (no named local), so the temporary
//   takes the dead effects slot and the index keeps the third one. The emitter local stays uninitialised (retail reads the slot
//   on the not-present path).
// Leaf: ?PlayEffect@CTCBulletTime@@AAEXPAV?$CIntelligentPointer@VCThing@@@@ABVCCharString@@PAVCThingCreatureBase@@J@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTCBULLETTIME
#define FABLE_RETAIL_CTCBULLETTIME_MEMBERS
#define FABLE_RETAIL_CTCBULLETTIME_PLAYEFFECT
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/C3DVector.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CDefClassBase.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CSpecialAbilitiesBulletTimeDef.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/ThingAlive.h"
#include "engine/CTCBulletTime.h"

// CDefClassBase::GetSubDef<T>: copied verbatim from the landed CTCUnholyPower ctor row 0x007941D0
//   (rebuild/src/compiled/00/79/CTCUnholyPower_CTCUnholyPower_007941d0.cpp lines 86-95; same text as the 0x00669580
//   specialization). Retail's TU saw this template body (the specialization is emitted as an out-of-line COMDAT at
//   0x00669580 and called, not inlined), so the real body goes here, not a declaration-only stand-in.
template <class T> bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(T::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}

void CTCBulletTime::PlayEffect(CIntelligentPointer<CThing>* effect, const CCharString& name, CThingCreatureBase* target, long attachType)
{
    CDefPointer<const CSpecialEffectsDef> effects;
    long effect_index = 0;
    if (effect == 0) {
        if (PDef->GetSubDef(&effects) && effects->GetEffectFromString(name, effect_index)) {
            CThing* thing = CTCSpecialAbilities::CreateParticleEffect(effect_index, target->GetPos());
            if (IS_THING_ALIVE(thing)) {
                CTCDParticleEmitter* emitter;
                thing->GetTC(TCI_PARTICLE_EMITTER, &emitter);
                emitter->AttachToThing(*target, attachType, "", 0, 0);
            }
        }
    } else {
        if (PDef->GetSubDef(&effects) && effects->GetEffectFromString(name, effect_index)) {
            *effect = CIntelligentPointer<CThing>(CTCSpecialAbilities::CreateParticleEffect(effect_index, target->GetPos()));
            if (IS_THING_ALIVE(*effect)) {
                CTCDParticleEmitter* emitter;
                (*effect)->GetTC(TCI_PARTICLE_EMITTER, &emitter);
                emitter->AttachToThing(*target, attachType, "", 0, 0);
            }
        }
    }
}