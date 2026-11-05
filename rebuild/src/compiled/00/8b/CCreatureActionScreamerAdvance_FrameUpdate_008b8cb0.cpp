// CCreatureAction_ScreamerAdvance::FrameUpdate() -- past 30% of the animation, end the action (TotalTime = 1) once the creature's mode manager holds mode 0x44; then the base FrameUpdate
// Leaf: ?FrameUpdate@CCreatureAction_ScreamerAdvance@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B8CB0, length 113 bytes (+ int3 padding).
// Callees: CCreatureActionBase::GetAnimationFraction @ 0x006925E0 (float: elapsed share of the action animation; the manifest
// "CInputTypeMouseMovementEvent::operator==" label is propagated), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8),
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x31 = bit 17 of the bitset dword at +0x24),
// CTCCreatureModeManager::IsModeActive(unsigned long) @ 0x0073A8A0, CCombatActionBase::FrameUpdate @ 0x00859D80 (direct base call).
// Float constant: 0x01238170 = 0.3f. `fcomp ; test ah,41h ; jne` skips on "<=", i.e. the body runs for `fraction > 0.3f`.
// Shape: the mode manager pointer is NULL-INITIALISED (`xor esi,esi` before the GetPItem call), so it lives in esi rather than in the
// frame word of the lookup key and the miss path falls through with esi = 0 into the single HasMode call (no tail duplication, unlike
// the uninitialised form of 0x008C4780). TotalTime is the CCreatureActionBase long at +0x10 (PDB name).
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCCreatureModeManager { public:             // interface type 0x31 component
public:
    bool IsModeActive(unsigned long) const;   // 0x0073A8A0
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void FrameUpdate();             // 0x00859D80
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    long TotalTime;                         // +0x10
    unsigned char _pad14[0x114 - 0x14];     // +0x14
    float GetAnimationFraction();           // 0x006925E0 (CCreatureActionBase)
};

class CCreatureAction_ScreamerAdvance : public CCombatActionBase { public:
    virtual void FrameUpdate();
};

void CCreatureAction_ScreamerAdvance::FrameUpdate()
{
    if (GetAnimationFraction() > 0.3f) {
        CTCCreatureModeManager* pModes = 0;
        PCreature.Get()->GetTC(TCI_ENTITY_MODE_MANAGER, &pModes);
        if (pModes->IsModeActive(0x44))
            TotalTime = 1;
    }
    CCombatActionBase::FrameUpdate();
}