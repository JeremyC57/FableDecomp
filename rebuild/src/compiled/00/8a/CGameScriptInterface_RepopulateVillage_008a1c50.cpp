// CGameScriptInterface::RepopulateVillage(const CScriptThing& thing) -- for a thing holding a village component (interface 0x22), have the world map repopulate its region with villagers
// Leaf: ?RepopulateVillage@CGameScriptInterface@@UBEXABVCScriptThing@@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x008A1C50, 104 bytes, `ret 4`; Ego_r.pdb same symbol.
// Body: if thing.IsAlive() (slot 75, +0x12c): t = thing.GetPThing() (slot 11); if t holds interface 0x22 (bit 2 of
// UsingComponents word 1, +0x24) and the component from the non-const Components map (LowerBound 0x0040F020, key in
// the dead `thing` slot) is non-null: World (+0x04) slot 12 (+0x30) -> PopulateRegionWithVillagers(*t) (0x00502E90,
// ?PopulateRegionWithVillagers@CWorldMap@@QAEXAAVCThing@@@Z per engine_api.tsv). No null test on t.
// Layout: CThing as the matched 0x0089A330 / 0x0089B0B0 rows. Interface 0x22 is modelled as CTCVillage and World slot 12
// as GetWorldMap(); neither name is confirmed.
// Shape: /O2 (16-aligned entry); the GetTC out-pointer form.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCVillage {                                      // interface 0x22 (name not confirmed)
public:
};
class CWorldMap {
public:
    void PopulateRegionWithVillagers(CThing& village);   // 0x00502E90
};
class CWorld {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap();                    // slot 12 (+0x30, name not confirmed)
};
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::RepopulateVillage(const CScriptThing& thing) const
{
    if (thing.IsAlive())
    {
        CThing* t = thing.GetPThing();
        CTCVillage* village;
        if (t->GetTC(TCI_VILLAGE, &village) && village)
            World->GetWorldMap()->PopulateRegionWithVillagers(*t);
    }
}