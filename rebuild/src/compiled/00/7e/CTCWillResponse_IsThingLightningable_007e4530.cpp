// CTCWillResponse::IsThingLightningable(const CThing* thing) -- true when the thing can be hit by the lightning spell: its will-response component decides (not when not bloody-willable at all), else a smashable thing, a targeted thing with a targeting flag 0x18 bit, or a thing type flag 8
// Leaf: ?IsThingLightningable@CTCWillResponse@@SI_NPBVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E4530, 159 bytes (+ int3 padding), public static `__fastcall` (thing in ecx), plain `ret`.
// Identity: FableWin.pdb ?IsThingLightningable@CTCWillResponse@@SI_NPBVCThing@@@Z (public static bool __fastcall (const CThing*)),
//   between IsThingLifeDrainable 0x007E44E0 and IsThingNotBloodyWillableAtAll 0x007E45D0 (landed, same TU); the catalog `_global` label is wrong.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const 0x004365B0 (twice, key by address).
// Layout: CTCWillResponse (Ego_r.pdb, 0x14): CTCBase, PDef +0x0c, the seven response bits at +0x10 in PDB field order (no engine
//   header exists; the landed siblings model it locally). CThing from engine/CThing.h, CTCTargeted (TargetingFlags +0x0c) from
//   engine/CTCTargeted.h; TCI_WILL_RESPONSE 156 (word 4 bit 28 at +0x30), TCI_SMASHABLE 78 (+0x28 bit 14), TCI_TARGETED 9 (+0x20 bit 9).
// Shape: the shared header's FABLE_RETAIL_CTHING_GETTC_EXPANDED PeekTC<T> (key local inside the test, map through a pointer local,
//   end-or-greater conditional) for both components: rebasing the parked local model (a hand-written HasTC/GetTC/Find) onto it moved
//   the targeted component load from edx to retail's eax (`mov eax,[eax+4] ; test byte ptr [eax+0Ch],18h`, 0x007E45BA).
class CTCBase;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#define FABLE_RETAIL_CTCTARGETED
#include "engine/CTCTargeted.h"
class CWillResponseDef;

class CTCWillResponse : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_WILL_RESPONSE; }
    static bool __fastcall IsThingLightningable(const CThing* thing);
private:
    bool IsThingLightningable() const
    {
        if (IsNotBloodyWillable)
            return false;
        return IsLightningable;
    }
    CDefPointer<const CWillResponseDef> PDef;   // +0x0c
    bool IsForcePushable : 1;                   // +0x10 bit 0
    bool IsLifeDrainable : 1;                   // bit 1
    bool IsLightningable : 1;                   // bit 2
    bool IsNotBloodyWillable : 1;               // bit 3
    bool IsAbleToBeLifted : 1;                  // bit 4
    bool IsEpicPowerExplodable : 1;             // bit 5
    bool IsEpicSpellable : 1;                   // bit 6
};

bool __fastcall CTCWillResponse::IsThingLightningable(const CThing* thing)
{
    const CTCWillResponse* tc;
    if (thing->PeekTC(&tc))
        return tc->IsThingLightningable();
    if (thing->HasTC(TCI_SMASHABLE))
        return true;
    const CTCTargeted* targeted;
    if (thing->PeekTC(&targeted)) {
        if (targeted->GetTargetingFlags() & 0x18)
            return true;
    }
    if (thing->GetTypeFlags() & 8)
        return true;
    return false;
}