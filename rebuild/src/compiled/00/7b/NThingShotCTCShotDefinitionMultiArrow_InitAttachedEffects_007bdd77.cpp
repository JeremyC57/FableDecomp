// NThingShot::CTCShotDefinitionMultiArrow::InitAttachedEffects() -- spawn the multi-arrow trail effect at the shot and, when it lives, keep it as PEffect and attach its particle emitter to the shot's "weapon_pos_a" helper
// Leaf: ?InitAttachedEffects@CTCShotDefinitionMultiArrow@NThingShot@@MAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BDD77, 186 bytes (plain `ret`; frame-pointer size TU: optimize("y", off) + ("s", on), as the
//   landed shot rows). Ego_r.pdb ?InitAttachedEffects@CTCShotDefinitionMultiArrow@NThingShot@@MAEXXZ (protected virtual).
// Callees: CDefClassBase::GetSubDef<CThingMultiArrowShotDef> 0x007BE7EA (this TU; its landed body is repeated here under auto_inline(off),
//   which lets VC7.1 see that `arrowDef` does not escape: retail's `and [ebp-4],0` / `push 0` immediates and the null-test-free
//   release instead of an ebx zero register), CThing::GetPos 0x004C73D0, CTCDParticleEmitter::Create 0x006E0880 (__fastcall),
//   IS_THING_ALIVE<CThing*> 0x0040DCE0, CIntelligentPointer<CThing>(CThing*) 0x004A96F0 / operator= 0x004A9760 / ~ 0x00A01C10,
//   CThing::GetTCDriver<CTCDParticleEmitter> 0x004ACB50, CCharString 0x0099EBF0 / ~ 0x0099EAE0, CTCDParticleEmitter::AttachToThing
//   0x006E0BE0, the def's release through vtable slot 1.
// Model: the landed sibling SetSoulEffect 0x007BE05C (retail CThing / CTCBase / CIntelligentPointer headers, scoped pointer temporary);
//   CThingMultiArrowShotDef::MultiArrowTrailEffect +0x44 (Ego_r.pdb).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCDriverBase.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
class CThingMultiArrowShotDef : public CSubDefClassBase {             // engine/CThingMultiArrowShotDef.h is the flat Ego_r model
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CThingMultiArrowShotDef", -1); }
    unsigned char _pad_0x25[0x44 - 0x25];
    long          MultiArrowTrailEffect;                            // +0x44 (Ego_r.pdb)
};
class CDefinitionManager {
    friend class CDefClassBase;
protected:
    CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (protected non-const overload; hidden result pointer)
};
// Same-TU body of the landed GetSubDef<CThingMultiArrowShotDef> 0x007BE7EA (this TU, called out of line).
#pragma auto_inline(off)
template <>
bool CDefClassBase::GetSubDef<CThingMultiArrowShotDef>(CDefPointer<const CThingMultiArrowShotDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CThingMultiArrowShotDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#pragma auto_inline(on)
class CTCDParticleEmitter : public CTCDriverBase {                  // no shared header
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_PARTICLE_EMITTER; }
    static CThing* __fastcall Create(long type, const C3DVector& position, bool flag);    // 0x006E0880
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
template <class T> inline bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return &*thing && !thing->IsDead();
}
namespace NThingShot {
class CTCShotDefinitionMultiArrow : public CTCBase {
protected:
    virtual void InitAttachedEffects();
private:
    CIntelligentPointer<CThing> PEffect;                // +0x0c (CTCShotDefinitionBase)
    unsigned char _pad14[0x38 - 0x14];
    CDefPointer<const CDefClassBase> PShotDef;          // +0x38 CDefPointer<const CThingShotDef>
};
}

void NThingShot::CTCShotDefinitionMultiArrow::InitAttachedEffects()
{
    CDefPointer<const CThingMultiArrowShotDef> arrowDef;
    PShotDef->GetSubDef(&arrowDef);
    long effectType = arrowDef->MultiArrowTrailEffect;
    if (effectType) {
        CThing* effect = CTCDParticleEmitter::Create(effectType, GetParentThing().GetPos(), false);
        if (IS_THING_ALIVE(effect)) {
            {
                CIntelligentPointer<CThing> effectPointer(effect);
                PEffect = effectPointer;
            }
            CTCDParticleEmitter* emitter = 0;
            effect->GetTCDriver(&emitter);
            CCharString helperName("weapon_pos_a");
            emitter->AttachToThing(GetParentThing(), 0x3c, helperName, 0, 0.0f);
        }
    }
}