// CCreatureAction_PutDownJug::PerformAction() -- if the creature carries something in its primary slot, run the table put-down's PerformAction and set the carried jug's "put down" flag
// Leaf: ?PerformAction@CCreatureAction_PutDownJug@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084A200, length 137 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface twice:
// type 0x46 = bit 6 of the word at +0x28 on the creature, type 0xBB = bit 27 of the word at +0x34 on the carried thing),
// CTCCarrying::GetThingInPrimarySlot @ 0x006A1440, CCreatureAction_PutDownThingAtTable::PerformAction @ 0x0084A0A0 (direct base call).
// Shape: both GetInterface results are tested (`je` to the shared epilogue); the carried thing rides in esi across the base call.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CTCCarrying { public:                        // interface type 0x46
public:
    CThing* GetThingInPrimarySlot() const;   // 0x006A1440
};
struct CTCJug {                             // interface type 0xBB
    unsigned char _pad00[0x10];
    bool m_putDown;                         // +0x10 (PickUpJug clears it, PutDownJug sets it)
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
};
class CCreatureAction_PutDownThingAtTable : public CCreatureActionBase { public:
    virtual void PerformAction();           // 0x0084A0A0
};
class CCreatureAction_PutDownJug : public CCreatureAction_PutDownThingAtTable { public:
    virtual void PerformAction();
};

void CCreatureAction_PutDownJug::PerformAction()
{
    CTCCarrying* carrying;
    if (PCreature.GetPItem()->GetTC(TCI_CARRYING, &carrying)) {
        CThing* thing = carrying->GetThingInPrimarySlot();
        if (thing) {
            CCreatureAction_PutDownThingAtTable::PerformAction();
            CTCJug* jug;
            if (thing->GetTC(TCI_TAVERN_JUG, &jug))
                jug->m_putDown = true;
        }
    }
}