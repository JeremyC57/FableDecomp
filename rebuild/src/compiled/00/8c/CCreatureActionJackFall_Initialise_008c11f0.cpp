// CCreatureAction_JackFall::Initialise() -- clear the effects on Jack's boss component (interface type 0xAD), then the base Initialise
// Leaf: ?Initialise@CCreatureAction_JackFall@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C11F0, length 103 bytes (+ int3 padding).
// Model: rebuild/include/engine/CCreatureAction_JackFall.h is the flattened PDB layout and sits high against the bytes (CCombatActionBase is 0x12c bytes in the PDB, 0x114 in retail: retail containers are 12 bytes), so `this` is a local model with the retail offsets.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface
// for type 0xAD = bit 13 of the presence word at +0x34, `test ch,20h`), CTCBoss::ClearEffects @ 0x007AEF60,
// CCombatActionBase::Initialise @ 0x00857520 (direct base call, last).
// Shape: the GetInterface result is ignored -- the absent arm reads the uninitialised out pointer from the key's frame word
// (`mov eax,[esp+4]`), and VC tail-duplicates the two calls into both arms; esi is only pushed inside the present arm.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCBoss { public:                            // interface type 0xAD
    void ClearEffects();                    // 0x007AEF60
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Initialise();              // slot 12: 0x00857520
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

class CCreatureAction_JackFall : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_JackFall::Initialise()
{
    CTCBoss* boss;
    PCreature.Get()->GetTC(TCI_BOSS, &boss);
    boss->ClearEffects();
    CCombatActionBase::Initialise();
}