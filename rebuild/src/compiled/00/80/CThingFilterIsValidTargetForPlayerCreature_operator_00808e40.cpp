// (anonymous namespace)::CThingFilter_IsValidTargetForPlayerCreature::operator()(const CThing&) const -- the player-targeting filter
// (NPlayerTargeting TU, anon ns _anon_58ED6966): true when `thing` may be targeted by the player creature. A fixed target
// (PTargetOnlyThing) short-circuits to an identity test; a child player may not target doors, beds or window action-use things
// (HasTC + GetClassTypeName() == CTCActionUseWindow::StatGetClassTypeName()); a shop's stock item needs Stealable and
// AcceptStealableStockItems; a type-13 thing is rejected while the player's stealth TC is in danger; with the PC "left click is king" control-scheme flag
// set and a weapon carried, melee-flagged things (ThingTypeFlags & 8) defer to NTargeting::IsThingVulnerableToMeleeStrikesFromThing,
// others need a CTCTargeted with TargetingFlags & 0x1C (when they have one).
// Retail: Fable.exe @ 0x00808E40, 488 bytes. Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const @ 0x004365B0,
// CCharString::CCharString(const char*,long) @ 0x0099EBF0, CCharString::~CCharString @ 0x0099EAE0,
// CBasicString<char>::Compare @ 0x00411570, CTCStockItem::GetShopTC @ 0x007F4FC0,
// NTargeting::IsThingVulnerableToMeleeStrikesFromThing @ 0x007F59F0, CTCBase::GetClassTypeName (vslot 22, +0x58).
// Shape: PeekTC expanded (FABLE_RETAIL_CTHING_GETTC_EXPANDED) keys home into the dead `thing` arg slot; the class-name test is a
// nested if (an `&&` with it adds a temporaries-destruction flag); the identity and melee tests are written as two-return / if-else
// (the `return a==b;` form emits xor/sete cl, and `return Vuln(...)` drops retail's `test al,al ; jne` before the 3rd epilogue).
// Header overlay: FABLE_RETAIL_CTCSTEALTH_ISINDANGER (inline CTCStealth::IsInDanger, timer >= 0).
// Leaf: ??RCThingFilter_IsValidTargetForPlayerCreature
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCSTOCKITEM_TARGETING
#define FABLE_RETAIL_NTARGETING_MELEE
#define FABLE_RETAIL_CTCSTEALTH_ISINDANGER
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CVectorMap.h"
#include "engine/CTCActionUseWindow.h"
#include "engine/CTCStockItem.h"
#include "engine/CTCStealth.h"
#include "engine/CTCTargeted.h"
#include "engine/NTargeting.h"
#include "engine/CThingPlayerCreature.h"

extern bool g_bControlSchemeLeftClickIsKing;   // 0x013B85F4 (descriptive: no PDB name; the second reader, CUserProfileManager::ResetAssignedInputs 0x004085F4, tests it before loading the def "FABLE_PC_CONTROL_SCHEME_GDD_LEFT_CLICK_IS_KING" 0x0122E1A8)

namespace {
class CThingFilter_IsValidTargetForPlayerCreature { public:
    bool AcceptStealableStockItems;          // +0x00
    bool PlayerIsChild;                      // +0x01
    bool PlayerCarryingWeapon;               // +0x02
    const CThingPlayerCreature* PlayerCreature; // +0x04
    const CThing* PTargetOnlyThing;          // +0x08
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_IsValidTargetForPlayerCreature::operator()(const CThing& thing) const
{
    if (PTargetOnlyThing) {
        if (PTargetOnlyThing == &thing)
            return true;
        return false;
    }
    if (PlayerIsChild) {
        if (thing.HasTC(TCI_DOOR))
            return false;
        if (thing.HasTC(TCI_BED))
            return false;
        const CTCBase* use;
        if (thing.PeekTC(TCI_ON_ACTION_USE, &use)) {
            if (use->GetClassTypeName() == CTCActionUseWindow::StatGetClassTypeName())
                return false;
        }
    }
    const CTCStockItem* stock;
    if (thing.PeekTC(TCI_STOCK_ITEM, &stock) && stock->GetShopTC()) {
        if (!stock->IsStealable())
            return false;
        if (!AcceptStealableStockItems)
            return false;
    }
    const CTCStealth* stealth;
    if (thing.GetType() == 13 && PlayerCreature->PeekTC(TCI_STEALTH, &stealth) && stealth->IsInDanger())
        return false;
    if (!g_bControlSchemeLeftClickIsKing || !PlayerCarryingWeapon)
        return true;
    if (thing.GetTypeFlags() & 8) {
        if (!NTargeting::IsThingVulnerableToMeleeStrikesFromThing(thing, *PlayerCreature))
            return false;
    } else {
        const CTCTargeted* targeted;
        if (thing.PeekTC(TCI_TARGETED, &targeted) && !(targeted->TargetingFlags & 0x1c))
            return false;
    }
    return true;
}
}
