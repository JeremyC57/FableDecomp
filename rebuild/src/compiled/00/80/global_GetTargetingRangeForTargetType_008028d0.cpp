// GetTargetingRangeForTargetType(unsigned long type, const CTargetingDef& def, const CThing& targeter, const CThing* carried) -- the largest targeting range that applies to a target of these targeting flags: the def's spell range (type 8) when the targeter's player is in spell mode and the flags are none of 1 / 2 / 4, the def's ranges for flags 2, 1 and 0x10, a carried projectile weapon's own range (and the def's type-8 range) when flag 8 is set, and the def's range for flag 4
// Leaf: ?GetTargetingRangeForTargetType@?A0x
// Retail: Fable.exe (TLC, VC7.1) @ 0x008028D0, 648 bytes (+8 int3 to 0x00802B60), __fastcall (type in ecx, def in edx, targeter
//   and carried on the stack), `ret 8`, float on the x87 stack.
// Identity (proven, two sources): Ego_r.pdb 0x007FD8C0 / FableWin.pdb
//   ?GetTargetingRangeForTargetType@?A0x58ed6966@@YIMKABVCTargetingDef@@ABVCThing@@PBV3@@Z, the file-static of the player-targeting
//   TU (Ego_r donor order GetTargetingRange 0x007FD880 / GetTargetingRangeForTargetType 0x007FD8C0 / GetTargetingRangeForTarget
//   0x007FDB70 = retail 0x00802890 / 0x008028D0 / 0x00802B60); callers 0x00802BBF (GetTargetingRangeForTarget), 0x00802DAF
//   (NPlayerTargeting::IsThingInTargetingRangeForTargetType) and 0x00803A95.
// Callees: CInputEvent::CInputEvent 0x00A04410 / ~CInputEvent 0x00A03B30 (a 0x34-byte frame object the body never reads),
//   CThing::PeekPlayer 0x004C7A10, GetTargetingRange 0x00802890 (twice; the other four sites expand it: the map's find 0x007F9830
//   with a hidden const_iterator, `ret 8`), CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound const 0x004365B0 (PeekTC of
//   TCI_WEAPON 72), CTCWeapon::IsProjectileWeapon 0x005DAA40, CTCWeapon::GetTargetingRange 0x005DA890 (Ego_r ?GetTargetingRange@CTCWeapon@@QBEJXZ).
//   Inline: FableWin.pdb ?IsSpellMode@CPlayer@@QBE_NXZ (+0x218), CThing::IsDead (+0x91 bit 0), STLport max (the reference
//   select `lea eax,[a] ; jnp ; lea eax,[b] ; mov eax,[eax]`). Data: 0.0f 0x0122DEDC.
// Shape: every max keeps the running range first except the weapon pair, which retail compares the new value against
//   (`fst [tmp] ; fcomp [range]`, 0x00802AA4 / 0x00802AD0). GetTargetingRange's body is in the TU (retail expands it at the
//   0.5-frequency sites and calls it at the two nested ones).
// Header overlay (FABLE_HDR_OVERLAY=$A/hdr): engine/CTargetingDef.h (new), FABLE_RETAIL_PLAYER_ISSPELLMODE,
//   FABLE_RETAIL_CTCWEAPON_TARGETINGRANGE.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_PEEKPLAYER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_ISSPELLMODE
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_TARGETINGRANGE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CPlayer.h"
#include "engine/CTCWeapon.h"
#include "engine/CInputEvent.h"
#include "engine/CTargetingDef.h"
#include "stlport/fable_algorithm.h"

namespace {
float __fastcall GetTargetingRange(const CTargetingDef& def, ETargetingType type)   // 0x00802890
{
    return def.GetTargetingRange(type);
}

float __fastcall GetTargetingRangeForTargetType(unsigned long type, const CTargetingDef& def, const CThing& targeter,
                                                const CThing* carried)
{
    float range = 0.0f;
    CInputEvent event;
    if (type != 2 && type != 1 && (type & 4) == 0 && targeter.PeekPlayer().IsSpellMode())
        range = std::max(range, GetTargetingRange(def, (ETargetingType)8));
    if (type & 2)
        range = std::max(range, GetTargetingRange(def, (ETargetingType)2));
    if (type & 1)
        range = std::max(range, GetTargetingRange(def, (ETargetingType)1));
    if (type & 0x10)
        range = std::max(range, GetTargetingRange(def, (ETargetingType)0x10));
    const CTCWeapon* weapon;
    if (carried && !carried->IsDead() && carried->PeekTC(&weapon) && (type & 8) && weapon->IsProjectileWeapon()) {
        range = std::max(GetTargetingRange(def, (ETargetingType)8), range);
        range = std::max((float)weapon->GetTargetingRange(), range);
    }
    if (type & 4)
        range = std::max(range, GetTargetingRange(def, (ETargetingType)4));
    return range;
}
}

float __fastcall GetTargetingRangeForTargetType_Use(unsigned long type, const CTargetingDef& def, const CThing& targeter,
                                                    const CThing* carried)
{
    return GetTargetingRangeForTargetType(type, def, targeter, carried);
}