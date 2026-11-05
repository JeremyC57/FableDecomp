// CCreatureAction_UndeadRiseUp::FrameUpdate() -- while still rising, once the action is past half way: flag the creature's type-9 component (+0x10 = 1), drop the creature's underground mode and clear the rising flag; then the base FrameUpdate
// Leaf: ?FrameUpdate@CCreatureAction_UndeadRiseUp@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EC3F0, length 118 bytes.
// Callees: CCreatureActionBase::GetProgressRatio @ 0x006925E0 (float (elapsed) / (duration); the manifest's
// "CInputTypeMouseMovementEvent::operator==" / landed "GetMovementRatio" labels are propagated), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 9 =
// bit 9 of the bitset dword at +0x20, `test ch,2`), CCreatureAction_RiseUp::RemoveUndergroundMode @ 0x008B7640 (landed),
// CCombatActionBase::FrameUpdate @ 0x00859D80 (direct base call; the push-ecx frame keeps it a call, not a jump).
// Shape: `fcomp [0.5f] ; test ah,41h ; jne` = `ratio > 0.5f`; the component pointer comes from the landed out-parameter
// GetInterface template (no null test before the byte store). The rising flag at +0x118 is the first member after the
// retail CCreatureAction_RiseUp base (0x118 bytes: CCombatActionBase 0x114 + one word); modelled locally (the generated
// headers flatten the bases).
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CTCType9 {                           // the creature's interface-type-9 component
    unsigned char _pad[0x10];
    unsigned char m_flag;                   // +0x10
};
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
    float GetProgressRatio();               // 0x006925E0 (CCreatureActionBase)
public:
    virtual void FrameUpdate();   // 0x00859D80
};

class CCreatureAction_RiseUp : public CCombatActionBase { public:   // retail sizeof 0x118
    unsigned char _pad114[4];               // +0x114
    void RemoveUndergroundMode();           // 0x008B7640
};

class CCreatureAction_UndeadRiseUp : public CCreatureAction_RiseUp { public:
    unsigned char m_rising;                 // +0x118
    void FrameUpdate();
};

void CCreatureAction_UndeadRiseUp::FrameUpdate()
{
    if (m_rising) {
        if (GetProgressRatio() > 0.5f) {
            CTCType9* component;
            if (PCreature.Get()->GetTC(TCI_TARGETED, &component))
                component->m_flag = 1;
            RemoveUndergroundMode();
            m_rising = 0;
        }
    }
    CCombatActionBase::FrameUpdate();
}