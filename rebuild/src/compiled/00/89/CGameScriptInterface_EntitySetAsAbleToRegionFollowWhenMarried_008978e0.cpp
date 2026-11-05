// CGameScriptInterface::EntitySetAsAbleToRegionFollowWhenMarried(const CScriptThing& entity, bool able) const -- script call: sets a live wife's permitted-to-region-follow flag (CTCWife, interface 0xd7)
// Leaf: ?EntitySetAsAbleToRegionFollowWhenMarried@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x008978E0, 86 bytes, thiscall `ret 8`.
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 207 holds this body; slot 207 is
//   ?EntitySetAsAbleToRegionFollowWhenMarried@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z. Interface 0xd7 = CTCWife
//   (vtable 0x0125ED5C slot 21 = landed 0x006D4540, returns 0xd7). Setter: FableWin.pdb
//   ?SetAsPermittedToRegionFollow@CTCWife@@QAEX_N@Z, inline (byte +0x1c; member name descriptive).
// Body: t = entity.GetPThing() (slot 11); live t holding interface 0xd7 (UsingComponents word 6 bit 23): GetTC (non-const
//   LowerBound 0x0040F020) and store the flag.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CTCWife {                                         // interface 0xd7
public:
    unsigned char _pad00[0x1c];
    bool PermittedToRegionFollow;                        // +0x1c
    void SetAsPermittedToRegionFollow(bool permitted) { PermittedToRegionFollow = permitted; }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::EntitySetAsAbleToRegionFollowWhenMarried(const CScriptThing& entity, bool able) const
{
    CThing* t = entity.GetPThing();
    CTCWife* wife;
    if (t && !t->IsDead() && t->GetTC(TCI_WIFE, &wife))
        wife->SetAsPermittedToRegionFollow(able);
}