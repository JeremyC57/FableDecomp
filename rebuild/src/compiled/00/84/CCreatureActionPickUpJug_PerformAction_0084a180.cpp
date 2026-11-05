// CCreatureAction_PickUpJug::PerformAction() -- the table pick-up's PerformAction, then if the jug is still alive clear its jug component's "put down" flag
// Leaf: ?PerformAction@CCreatureAction_PickUpJug@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084A180, length 119 bytes (+ int3 padding).
// Callees: CCreatureAction_PickUpThingAtTable::PerformAction @ 0x00849F70 (direct base call, first), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 (the thing pointer at +0xa8, three times), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface for type 0xBB =
// bit 27 of the presence word at +0x34).
// Shape: IS_THING_ALIVE inlined (null test, then the +0x91 dead bit); the GetInterface result is ignored, so the absent arm stores
// through the uninitialised out pointer read from the key's frame word (`mov eax,[esp+4]`), and the byte store is tail-duplicated.
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

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined, landed 0x0094DBC0 form).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
struct CTCJug {                             // interface type 0xBB
    unsigned char _pad00[0x10];
    bool m_putDown;                         // +0x10 (PickUpJug clears it, PutDownJug sets it)
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();           // slot 3
    unsigned char _pad04[0xa8 - 4];
};
class CCreatureAction_PickUpThingAtTable : public CCreatureActionBase { public:
    CIntelligentPointer<CThing> PThing;         // +0xa8 the thing picked up
    virtual void PerformAction();           // 0x00849F70
};
class CCreatureAction_PickUpJug : public CCreatureAction_PickUpThingAtTable { public:
    virtual void PerformAction();
};

void CCreatureAction_PickUpJug::PerformAction()
{
    CCreatureAction_PickUpThingAtTable::PerformAction();
    if (IS_THING_ALIVE(PThing)) {
        CTCJug* jug;
        PThing.GetPItem()->GetTC(TCI_TAVERN_JUG, &jug);
        jug->m_putDown = false;
    }
}