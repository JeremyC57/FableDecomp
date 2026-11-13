// CCreatureAction_SpiritAttack::PerformAction() -- the co-op spirit's strike: retarget PTarget from the creature's targeting TC
//   (CTCTargetingBase::GetPTargetedThing, vslot 33); when the target is alive and a creature (ThingTypeFlags bit 3) and within
//   the spirit's melee target range plus the target's physics radius of the creature, create the attack effect and hit the
//   target from one unit behind the creature along the creature-to-target direction (attribute-0 damage 1.0, blockable); give
//   one experience point unless the hit was blocked or negated; always finish with the spirit TC's OnStrike.
// Leaf: ?PerformAction@CCreatureAction_SpiritAttack@@UAEXXZ
// Retail: Fable.exe 0x00854DD0, 720 bytes, plain `ret` (tail duplicated), no frame pointer, no EH state. Flags: none.
// Shape: the natural source (`direction = target - creature; direction.Normalise(); CHitParameters hit(0, creature, 0,
//   creaturePos - direction, direction, ...)`) only gives retail's bytes when the compiler SEES the bodies of three callees that
//   retail still calls out of line (inline functions of the headers, emitted as COMDATs):
//   - CHitParameters' nine-argument constructor 0x00789238 (header opt-in FABLE_RETAIL_CHITPARAMETERS_HIT_CTOR_BODY: the landed
//     row's initialiser list in declaration order, Position(position) / Direction(direction) on the real C3DVector members): VC7.1 then
//     knows from the callee's mod/ref that the two `const C3DVector&` arguments are only read, so `direction` and the position
//     temporary do not count as escaped: both subtractions are built X first, the unscaled X/Y stay on the x87 stack and only Z
//     is homed in direction.Z (`fst [esp+28h] ; fmul [esp+28h]`, 0x00854F14..0x00854FAD), and the position's three stores are
//     interleaved with the constructor's argument pushes (0x00854FBD..0x00854FEF). With a declaration only, both vectors are
//     built Z, Y, X as escaped objects (710v720).
//   - CThing::GetPos 0x004C73D0 and CThingPhysical::IsInRange 0x006A4DF0 (real bodies below; this TU's copies of both compile
//     to the retail bytes of 0x004C73D0 / 0x006A4DF0 exactly): with them the range sum is `fld range ; fadd [physics+34h]`
//     (0x00854ED2) instead of the radius loaded first.
//   The range, the target's physics TC and the creature are read into locals in that order (retail: GetMeleeTargetRange call,
//   `mov edi,[esi+60h]`, GetPItem, then the sum); the first spirit TC lives in the target block, the final strike re-reads it.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 / ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (temp vtable
//   0x01238C6C), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (GetTC<T> expanded), CTCTargetingBase::GetPTargetedThing
//   (vslot 33), CTCCoopSpirit::GetMeleeTargetRange 0x00670000 / AddScore 0x0066FB10 / OnStrike 0x006707E0, CThing::GetPos
//   0x004C73D0, CThingPhysical::IsInRange 0x006A4DF0, CCreatureAction_SpiritAttack::CreateAttackEffect 0x008548B0, CHitParameters
//   ctor 0x00789238 / AddDamageWithAttribute 0x00868CA0 / dtor 0x00789358, CThingGameObject::OnHit (vslot 65, +0x104).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_ISINRANGE
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_PCREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_ZYX
#define FABLE_RETAIL_CHITPARAMETERS_CTOR
#define FABLE_RETAIL_CHITPARAMETERS_HIT_CTOR
#define FABLE_RETAIL_CHITPARAMETERS_VECTOR_MEMBERS
#define FABLE_RETAIL_CHITPARAMETERS_HIT_CTOR_BODY
#define FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP
#define FABLE_RETAIL_CHITPARAMETERS_AUGMENTATIONS
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CHitParameters.h"
#define FABLE_RETAIL_CCREATUREACTION_SPIRITATTACK_TYPED
#define FABLE_RETAIL_CCREATUREACTION_SPIRITATTACK_PERFORMACTION
#include "engine/CCreatureAction_SpiritAttack.h"
#define CTCCoopSpirit CTCCoopSpiritFields
#include "engine/_quarantine/CTCCoopSpirit.h"
#undef CTCCoopSpirit

class CTCCoopSpirit : public CTCCoopSpiritFields {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return (ETCInterfaceType)0xc7; }
    // 0x0066FB10 is ?AddScore@CTCCoopSpirit@@QAEXJ@Z, not AddExperience: the two bodies are byte-identical (0x0066FB00 / 0x0066FB10,
    // both `[ecx+10h] += amount`) and Ego_r.pdb orders them ApplyMovementVector, AddExperience, AddScore, GetScore (0x006771F0..
    // 0x00677230) exactly as retail 0x0066FAE0, 0x0066FB00, 0x0066FB10, 0x0066FB20; FableWin.pdb has the same symbol.
    void AddScore(long amount);                 // 0x0066FB10
    float GetMeleeTargetRange() const;          // 0x00670000
    void OnStrike();                            // 0x006707E0
};

// CThing::GetPos() const 0x004C73D0 -- real body (landed row CThing_GetPosition_004c73d0.cpp): out of line in retail, but
// visible to the compiler (an inline of the header emitted as a COMDAT).
inline __declspec(noinline) const C3DVector& CThing::GetPos() const
{
    return PhysicsTC->GetPosition();
}

// CThingPhysical::IsInRange(const C3DVector&, float) const 0x006A4DF0 -- real body, out of line in retail but visible to the
// compiler. Spelled as the landed row CThingPhysical_IsInRange_006a4df0.cpp spells it: the physics TC's position read in place
// (retail `mov eax,[ecx+60h] ; fld [eax+0Ch] ; fsub [ecx]`, no callee -- not through the out-of-line GetPos), here through the
// shared header's inline accessor CTCPhysicsBase::GetPosition(). The result is `bool` as both PDBs have it
// (?IsInRange@CThingPhysical@@QBE_NABVC3DVector@@M@Z; the caller tests `al`, 0x00854EEC); the landed row's local model says
// `long`, and both spellings give retail's `mov eax,1` / `xor eax,eax` (this copy grades MATCH against 0x006A4DF0).
inline __declspec(noinline) bool CThingPhysical::IsInRange(const C3DVector& pos, float range) const
{
    C3DVector d = PhysicsTC->GetPosition() - pos;
    return d.GetSquaredMagnitude() < range * range;
}

void CCreatureAction_SpiritAttack::PerformAction()
{
    CTCTargetingBase* tc_targeting;
    PCreature->GetTC(&tc_targeting);
    CThing* target = tc_targeting->GetPTargetedThing();
    PTarget = CIntelligentPointer<CThing>(target);
    if (target && !target->IsDead() && (target->GetTypeFlags() & 8)) {
        CTCCoopSpirit* tc_spirit;
        PCreature->GetTC(&tc_spirit);
        float range = tc_spirit->GetMeleeTargetRange();
        CTCPhysicsBase* physics = target->GetPhysicsTC();
        CThingCreatureBase* creature = PCreature.Get();
        if (static_cast<CThingCreatureBase*>(target)->IsInRange(creature->GetPos(), range + physics->GetRadius())) {
            CreateAttackEffect();
            C3DVector direction = target->GetPos() - PCreature->GetPos();
            direction.Normalise();
            CHitParameters hit(0, PCreature.Get(), 0, PCreature->GetPos() - direction, direction, false, true, false, false);
            hit.AddDamageWithAttribute((EDamageAttribute)0, 1.0f);
            static_cast<CThingCreatureBase*>(target)->OnHit(hit);
            if (!hit.IsBlocked() && !hit.IsHitNegated())
                tc_spirit->AddScore(1);
        }
    }
    CTCCoopSpirit* tc_strike;
    PCreature->GetTC(&tc_strike);
    tc_strike->OnStrike();
}