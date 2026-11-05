// CGameScriptInterface::IsChestOpen(const CScriptThing& chest) const -- script call: whether a valid chest thing's chest component (interface 0x3c) is open
// Leaf: ?IsChestOpen@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x00896330, 88 bytes, thiscall `ret 4`.
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 434 holds this body; slot 434 is
//   ?IsChestOpen@CGameScriptInterface@@UBE_NABVCScriptThing@@@Z. Interface 0x3c: the CTCChest vtable 0x0126957C slot 21 is
//   0x004E48C3 (returns 0x3c; the same body is CTCSearchableContainer's slot 21). Getter: FableWin.pdb
//   ?IsOpen@CTCChest@@QBE_NXZ, inline (byte +0x1c; member name descriptive).
// Body: if chest.IsAlive() (CScriptThing slot 75, +0x12c): t = chest.GetPThing() (slot 11, no null or dead test); if t
//   holds interface 0x3c (UsingComponents word 1 bit 28): PeekTC (const LowerBound 0x004365B0) and return IsOpen().
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CTCChest {                                        // interface 0x3c
public:
    unsigned char _pad00[0x1c];
    bool Opened;                                         // +0x1c (descriptive)
    bool IsOpen() const { return Opened; }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
bool CGameScriptInterface::IsChestOpen(const CScriptThing& chest) const
{
    if (chest.IsAlive())
    {
        const CThing* t = chest.GetPThing();
        const const CTCChest* tc;
        if (t->PeekTC(TCI_CONTAINER, &tc))
            return tc->IsOpen();
    }
    return false;
}