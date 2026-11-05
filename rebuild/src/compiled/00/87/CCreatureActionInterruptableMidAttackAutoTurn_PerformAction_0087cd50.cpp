// CCreatureAction_InterruptableMidAttackAutoTurn::PerformAction() -- if the animation entry's parameters allow it and the target is alive, put the target's mode manager into mode 0x1F
// Leaf: ?PerformAction@CCreatureAction_InterruptableMidAttackAutoTurn@@QAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087CD50, length 145 bytes (+ int3 padding).
// Model: rebuild/include/engine/CCreatureAction_InterruptableMidAttackAutoTurn.h is the flattened PDB layout and sits high against the bytes (CCreatureActionBase is 0xb4 bytes in the PDB, 0xa8 in retail: retail containers are 12 bytes), so `this` is a local model with the retail offsets.
// Callees: the animation entry's parameter getter @ 0x00738A00 (thiscall on PAnimEntry.Data at +0x74, `void (T** out)`: a type-6
// lookup in the entry's own map with a global default map as fallback; manifest "CTCInterface_GetAnimation"),
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the target pointer at +0xa8, three times), CVectorMap::LowerBound @ 0x0040F020
// (inlined GetInterface for type 0x31), CTCCreatureModeManager::AddMode @ 0x0073AE10.
// Shape: the parameters pointer (fetched in an inline accessor, so its out pointer is an inner-scope local) and the GetInterface key share the single `push ecx` frame word (the absent GetInterface arm
// reads it back as the out pointer); IS_THING_ALIVE inlined; AddMode tail-duplicated into both GetInterface arms.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined, landed 0x0094DBC0 form).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}
class CTCCreatureModeManager { public:             // interface type 0x31
    bool AddMode(unsigned long mode);       // 0x0073AE10
};
struct CAnimationParameters {
    unsigned char _pad00[8];
    bool m_autoTurn;                        // +0x08 flag tested before turning the target
};
class CAnimationEntry { public:
    void GetParameters(CAnimationParameters** out);   // 0x00738A00
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0x74 - 4];         // +0x04
    CAnimationEntry* PAnimEntry_Data;       // +0x74 CCountedPointer<CAnimationEntry>::Data
    unsigned char _pad78[0xa8 - 0x78];      // +0x78
};
class CCreatureAction_InterruptableMidAttackAutoTurn : public CCreatureActionBase { public:
    CIntelligentPointer<CThing> PTarget;        // +0xa8 the creature being attacked
    void PerformAction();
    // inline accessor: its out pointer lives in an inner scope, so it shares the frame word with the lookup key
    bool GetAutoTurn() const
    {
        CAnimationParameters* params;
        PAnimEntry_Data->GetParameters(&params);
        return params->m_autoTurn;
    }
};

void CCreatureAction_InterruptableMidAttackAutoTurn::PerformAction()
{
    if (GetAutoTurn() && IS_THING_ALIVE(PTarget)) {
        CTCCreatureModeManager* modes;
        PTarget.Get()->GetTC(TCI_ENTITY_MODE_MANAGER, &modes);
        modes->AddMode(0x1F);
    }
}