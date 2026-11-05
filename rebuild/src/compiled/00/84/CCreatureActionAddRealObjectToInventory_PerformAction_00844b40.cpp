// CCreatureAction_AddRealObjectToInventory::PerformAction() -- if the object exists and is not flagged (bit 0 of its byte at +0x91), add it to the creature's inventory component (interface type 203) as a real item
// Leaf: ?PerformAction@CCreatureAction_AddRealObjectToInventory@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00844B40, length 118 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PObject at +0xa8 three times, PCreature at +8 once),
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 203 = bit 11 of the bitset dword at +0x38,
// `test ch,8`), the inventory component's AddItemToInventory(CThing*, bool) @ 0x007CAB30 (manifest label
// "TCHeroReceiveItems_AddItemToInventory" is a propagated guess; the result is dropped).
// Shape: one `push ecx` frame for the lookup key; the object pointer is fetched afresh for every use (three GetPItem calls);
// the `push 1` precedes the third fetch (arguments right to left). Retail CCreatureActionBase is 0xa8 bytes (PObject at
// +0xa8; no engine header for this class), so `this` is modelled locally.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCPhysicsBase;
struct CTCGraphicAppearance;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCInventory { public:                       // the interface-203 component
    void AddItemToInventory(CThing* item, bool real);   // 0x007CAB30
};
struct CThingFlags91 {                      // the byte at +0x91 of a thing: bit 0 tested
    unsigned char _pad[0x91];
    unsigned char m_isBeingRemoved : 1;
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
};

class CCreatureAction_AddRealObjectToInventory : public CCreatureActionBase { public:
    CIntelligentPointer<CThing> PObject;        // +0xa8 CIntelligentPointer<CThing>: the real object to add
    virtual void PerformAction();
};

void CCreatureAction_AddRealObjectToInventory::PerformAction()
{
    if (PObject.Get()) {
        if (!((CThingFlags91*)PObject.Get())->m_isBeingRemoved) {
            CTCInventory* pInventory;
            if (PCreature.Get()->GetTC(TCI_HERO_RECEIVE_ITEMS, &pInventory))
                pInventory->AddItemToInventory(PObject.Get(), true);
        }
    }
}