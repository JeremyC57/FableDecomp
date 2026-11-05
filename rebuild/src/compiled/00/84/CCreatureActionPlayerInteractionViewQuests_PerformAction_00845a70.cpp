// CCreatureAction_PlayerInteractionViewQuests::PerformAction() -- with both the player and the guide thing alive, open the guide's quest-select interface (type 0x6B component) and finish the action
// Leaf: ?PerformAction@CCreatureAction_PlayerInteractionViewQuests@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00845A70, length 155 bytes (+ int3 padding).
// Model: rebuild/include/engine/CCreatureAction_PlayerInteractionViewQuests.h is the flattened PDB layout and sits high against the bytes (CCreatureActionBase is 0xb4 bytes in the PDB, 0xa8 in retail: retail containers are 12 bytes), so `this` is a local model with the retail offsets.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8 twice, the guide pointer at +0xa8 three times),
// CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface for type 0x6B = bit 11 of the presence word at +0x2c, `test ch,8`),
// CTCHeroGuide::OpenQuestSelectInterface @ 0x007800B0, this action's virtual slot 4 (`call [edx+10h]`, FinishAction).
// Shape: two inlined IS_THING_ALIVE tests joined by `&&`; the GetInterface result is ignored (the absent arm reads the out pointer
// from the key's frame word) and the out pointer is null-tested instead.
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
class CTCHeroGuide { public:                       // interface type 0x6B
    void OpenQuestSelectInterface();        // 0x007800B0
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();            // slot 4
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
};
class CCreatureAction_PlayerInteractionViewQuests : public CCreatureActionBase { public:
    CIntelligentPointer<CThing> PGuide;         // +0xa8 the thing whose quest guide is opened
    virtual void PerformAction();
};

void CCreatureAction_PlayerInteractionViewQuests::PerformAction()
{
    if (IS_THING_ALIVE(PCreature) && IS_THING_ALIVE(PGuide)) {
        CTCHeroGuide* guide;
        PGuide.GetPItem()->GetTC(TCI_HERO_GUIDE, &guide);
        if (guide) {
            guide->OpenQuestSelectInterface();
            FinishAction();
        }
    }
}