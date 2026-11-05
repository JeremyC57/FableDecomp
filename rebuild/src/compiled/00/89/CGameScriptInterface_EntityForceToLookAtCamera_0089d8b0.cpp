// CGameScriptInterface::EntityForceToLookAtCamera(const CScriptThing& thing, bool look) const -- set the valid thing's targeting component (interface 8) camera-look flag
// Leaf: ?EntityForceToLookAtCamera@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x0089D8B0, 85 bytes, `ret 8`; virtual (vtable reference 0x012616D8); Ego_r.pdb
// ?EntityForceToLookAtCamera@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z.
// Body: if thing.IsAlive() (slot 75, +0x12c): t = thing.GetPThing() (slot 11, +0x2c); if t holds interface 8 (bit 8 of
// UsingComponents +0x20), the component from the Components map (LowerBound 0x0040F020, key in the dead `thing` slot)
// gets its +0x34 byte = look.
// Layout: as the landed 0x0089D830 EntityForceToLookAtThing sibling (same slots, interface 8 = the targeting component)
// and the 0x0089D7C0 row's CThing model (UsingComponents +0x20, Components +0x44). The +0x34 member name is not confirmed.
// Shape: /O2 (16-aligned entry); the GetTC out-pointer form.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCTargetingBase {
public:
    unsigned char _pad00[0x34];
    bool          LookAtCamera;                          // +0x34 (name not confirmed)
};
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::EntityForceToLookAtCamera(const CScriptThing& looker, bool look) const
{
    if (looker.IsAlive())
    {
        CTCTargetingBase* tc;
        if (looker.GetPThing()->GetTC(TCI_TARGETING, &tc))
            tc->LookAtCamera = look;
    }
}