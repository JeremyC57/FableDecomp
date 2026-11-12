// (anonymous namespace)::CThingFilter_CombatIsStrikeToStrike::operator()(const CThing& thing) const -- the combat strike search's target filter: a creature fails when it evades all hits or is rolling, fails when its creature def defers hits from behind and it stands behind the striker's facing, passes otherwise when it is vulnerable to the striker's melee strikes; a non-creature passes when the striker's class type is 2 or its def always includes it in object strike scans
// Leaf: ??RCThingFilter_CombatIsStrikeToStrike@?A0x
// Retail: Fable.exe (TLC, VC7.1) @ 0x008565E0, 406 bytes, __fastcall `ret 4`, bool in al, no frame pointer, no SEH.
// Name (FableWin.pdb and Ego_r.pdb): ??RCThingFilter_CombatIsStrikeToStrike@?A0xad05d24c@@QBE_NABVCThing@@@Z; the leaf above is a
//   prefix because the anonymous-namespace hash is per TU. Member (Ego_r.pdb): CThing* ThingStriking +0.
// Callees: NCreatureMode::IsToEvadeAllHits 0x0085FA00, NCreatureMode::IsRolling 0x0085EEB0 (both __fastcall, const CThing&),
//   CThing::GetPos() const 0x004C73D0 (twice), CTCPhysicsBase::GetFacingVector() const (virtual slot 70, +0x118, hidden result),
//   NTargeting::IsThingVulnerableToMeleeStrikesFromThing 0x007F59F0 (__fastcall), CDefPointeeBase::Delete (virtual slot 1, +4) when a
//   def count reaches zero. Inline: CThing::GetTypeFlags (+0x6C), GetPBaseDef (+0x70, by value), GetPhysicsTC (+0x60),
//   CClass::GetType (+0x10), the CDefPointer members, C3DVector::operator-.
// Shape: the two mode tests share one `return false` tail (one `||`). The creature def handle is a null CDefPointer<const
//   CThingCreatureDef> local filled by SetDynamicCast from the by-value GetPBaseDef() temporary: retail's three count stores
//   (+1 temporary, +1 local, -1 temporary's destructor) with the local in ebx from a zeroed start; it lives to the end of the
//   creature arm, so each exit releases it (the vulnerability result waits in the argument's home slot across the release).
//   DeferHitsIfBehindHitter is bit 0 of +0x138. The thing's position is a named reference taken before the striker's (retail calls
//   GetPos on the thing first). The dot product is written out: retail multiplies facing.Z by delta.Z and facing.Y by delta.Y but
//   delta.X by facing.X (0x008566AA `fld [delta.X] ; fmul [facing.X]`), which no shared C3DVector::Dot spelling gives (the ordered,
//   grouped and default forms all load facing.X there); c2 emits a one-line sum last term first with each product's operands
//   swapped. `dot < 0.0f` is `fcomp ; test ah,5 ; jp`. The creature bit is tested again before the vulnerability call (retail
//   re-reads +0x6C). The non-creature arm tests the bool bitfield AlwaysIncludeInObjectStrikeScans (bit 7 of +0x42) through a
//   temporary handle in an if / return true / return false (`shr bl,7`, the release, then `test bl,bl ; setne al`); a plain
//   `return <bitfield>` gives `mov al,bl`.
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGCREATUREDEF_MEMBERS
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_NCREATUREMODE_QUERIES
#define FABLE_RETAIL_NTARGETING_MELEE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NCreatureMode.h"
#include "engine/NTargeting.h"

namespace NCreatureMode { bool __fastcall IsRolling(const CThing& thing); }   // 0x0085EEB0

namespace {
class CThingFilter_CombatIsStrikeToStrike {
public:
    CThing* ThingStriking;                      // +0x00
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_CombatIsStrikeToStrike::operator()(const CThing& thing) const
{
    if (thing.GetTypeFlags() & 8) {
        if (NCreatureMode::IsToEvadeAllHits(thing) || NCreatureMode::IsRolling(thing))
            return false;
        CDefPointer<const CThingCreatureDef> def; def.SetDynamicCast(thing.GetPBaseDef());
        if (!def.IsNull() && def->DeferHitsIfBehindHitter) {
            const C3DVector& pos = thing.GetPos();
            C3DVector delta = pos - ThingStriking->GetPos();
            C3DVector facing = ThingStriking->GetPhysicsTC()->GetFacingVector();
            if (facing.X * delta.X + delta.Y * facing.Y + delta.Z * facing.Z < 0.0f)
                return false;
        }
        if (!(thing.GetTypeFlags() & 8))
            return true;
        return NTargeting::IsThingVulnerableToMeleeStrikesFromThing(thing, *ThingStriking);
    }
    if (ThingStriking->GetType() == 2)
        return true;
    if (thing.GetPBaseDef()->AlwaysIncludeInObjectStrikeScans)
        return true;
    return false;
}
}