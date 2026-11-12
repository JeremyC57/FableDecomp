// CCombatAction_WillHealInto::FinishAction() -- end of the heal spell's wind-up: spawn the creature def's "HEAL" special effect at the
//   creature and attach its particle emitter to the creature (attach type 10, no helper), run the base combat FinishAction, give
//   the creature a CTCHealLife cast at this action's SpellLevel when it has none (TCI_HEAL_LIFE 153), and hand it the next action,
//   a temporary CCombatAction_WillHealRelease
// Leaf: ?FinishAction@CCombatAction_WillHealInto@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C2AD0, 370 bytes, thiscall, plain `ret`; slot 4 of vtable 0x012948AC.
// Identity: Ego_r.pdb / FableWin.pdb ?FinishAction@CCombatAction_WillHealInto@@UAEXXZ; SpellLevel = Ego_r +0x12c = retail +0x114.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8), CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580,
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CSpecialEffectsDef::GetEffectFromString 0x00669D30,
//   CThing::GetPos 0x004C73D0, CTCDParticleEmitter::Create(long, const C3DVector&, bool) 0x006E0880 (__fastcall static),
//   CTCDParticleEmitter::AttachToThing 0x006E0BE0, CCombatActionBase::FinishAction 0x00859A20 (direct), CThing::AddTC 0x004C9D60,
//   CTCHealLife::SetAICast(long) 0x007C3EA0, CCombatAction_WillHealRelease(CThingCreatureBase&) 0x008C3930,
//   CThingCreatureBase::SetCurrentAction 0x006644F0, ~CCombatActionBase 0x0062E900 (the temporary's implicit destructor).
//   Inline: CThing::GetPBaseDef (the counted copy of PBaseDef +0x70), CDefPointer release (Delete, slot 1), CThing::IsDead (+0x91
//   bit 0), CThing::PeekTCDriver<CTCDParticleEmitter> (TCI_PARTICLE_EMITTER 75: `test ch,8` on +0x28, PTCDriver +0x5C),
//   CThing::HasTC(TCI_HEAL_LIFE) (`test dword ptr [esi+30h],2000000h`).
// Shape: no EH frame (TU built without /GX, as its neighbours 0x008C29F0 / 0x008C2A80). The special-effects def pointer lives for
//   the whole function (released last, after the action temporary). GetSubDef's bool result is not used: retail null-tests the
//   def pointer it filled (`mov esi,[esp+14h] ; cmp esi,ebx`) and keeps it in esi for GetEffectFromString; the shared header's
//   GetSubDef body is visible (called out of line as the COMDAT 0x00669580). The creature is fetched before the base FinishAction
//   and kept in esi. The AddTC result is a named local: the "CTCHealLife" temporary dies before SetAICast is called. The release
//   action temporary is 0x114 bytes (no members of its own), built at the bottom of the 0x120-byte frame. The effect index is
//   block-scoped: its frame word [esp+10h] is reused by the "" and "CTCHealLife" strings (at function scope it takes the "HEAL"
//   string's word instead, unless the GetEffectFromString body opt-in is on -- both spellings were graded).
// Local declarations: no shared header exists for CCombatAction_WillHealInto, CCombatAction_WillHealRelease or CTCHealLife; they
//   are declared here over the shared bases (no layout of a shared class is modelled locally).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_FINISHACTION
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingBaseDef.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"

class CTCHealLife : public CTCBase {
public:
    void SetAICast(long spell_level);                                    // 0x007C3EA0
};
class CCombatAction_WillHealRelease : public CCombatActionBase {
public:
    CCombatAction_WillHealRelease(CThingCreatureBase& creature);         // 0x008C3930
};
class CCombatAction_WillHealInto : public CCombatActionBase {
public:
    virtual void FinishAction();
protected:
    long SpellLevel;                                                     // +0x114
};

void CCombatAction_WillHealInto::FinishAction()
{
    CDefPointer<const CSpecialEffectsDef> pspecial_effects;
    CThing* pfx = 0;
    {
        long effect = 0;
        PCreature->GetPBaseDef()->GetSubDef(&pspecial_effects);
        if (!pspecial_effects.IsNull()) {
            if (pspecial_effects->GetEffectFromString("HEAL", effect))
                pfx = CTCDParticleEmitter::Create(effect, PCreature->GetPos(), false);
        }
    }
    if (pfx && !pfx->IsDead()) {
        CTCDParticleEmitter* pemitter;
        if (pfx->PeekTCDriver(&pemitter))
            pemitter->AttachToThing(*PCreature, 10, "", 0, 0.0f);
    }
    CThingCreatureBase* pcreature = PCreature.Get();
    CCombatActionBase::FinishAction();
    if (!pcreature->HasTC(TCI_HEAL_LIFE)) {
        CTCHealLife* ptc_heal_life = GFDebugPDynamicCast(pcreature->AddTC("CTCHealLife", true, 0), (CTCHealLife*)0);
        ptc_heal_life->SetAICast(SpellLevel);
    }
    pcreature->SetCurrentAction(CCombatAction_WillHealRelease(*pcreature));
}