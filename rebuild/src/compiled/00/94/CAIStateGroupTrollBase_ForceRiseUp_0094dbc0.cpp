// CAIStateGroup_TrollBase::ForceRiseUp() -- a troll lying in mode 0x1E gets up (rise-up action, wait for it)
// Leaf: ?ForceRiseUp@CAIStateGroup_TrollBase_Methods@@QAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094DBC0, length 122 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface on the brain's creature, type 0x31 = bit 17
// of the presence word at +0x24), CTCCreatureModeManager::IsModeActive(long) @ 0x0073A8A0,
// CCreatureAction_RiseUp::CCreatureAction_RiseUp(CThingCreatureBase*) @ 0x008BD880,
// CAIStateGroupBase::AIAction_PerformActionAndWait(CCreatureActionBase*, bool) @ 0x0097DB70,
// CCombatActionBase::~CCombatActionBase @ 0x0062E900 (the landed 0x00920960 / 0x0095B680 identification).
//
// Shape: the mode manager comes from the bool GetInterface, `&& HasMode(0x1E)`; then the landed
// PerformActionAndWait shape: `false` pushed first, the rise-up action built as a temporary inside the call
// (frame 0x11c = lookup key 4 + action 0x118), its base destructor after the call. The creature is re-read
// through an inline accessor. `this` is the PDB header layout; PBrain lives in the flattened base bytes.
#include "engine/CAIStateGroup_TrollBase.h"

class CTCBase;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with LowerBound @ 0x0040F020 -- the landed out-parameter
// GetInterface template (0x007504E0 / 0x00930DC0 shape): the component lands in eax, and a miss leaves the
// caller's out pointer unwritten (the frame slot that also holds the key).
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CBaseIntelligentPointer {                // CIntelligentPointer<T>: vptr + item
    void*   __vftable;                          // +0x00
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CThingCreatureBase;

class CTCCreatureModeManager { public:                 // interface type 0x31 component
public:
    bool IsModeActive(unsigned long) const;   // 0x0073A8A0
};

class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];                  // +0x04..+0xa7
};

class CCombatActionBase : public CCreatureActionBase { public:
    unsigned char _combat[0x6c];                // +0xa8..+0x113
    virtual ~CCombatActionBase();                       // 0x0062E900
};

class CCreatureAction_RiseUp : public CCombatActionBase { public:
    long m_riseState;                           // +0x114 (descriptive; sizeof 0x118 from the frame)
    CCreatureAction_RiseUp(CThingCreatureBase* creature);   // 0x008BD880
};

struct CAIStateGroup_TrollBase_Methods : CAIStateGroup_TrollBase {
    CAIBrain* GetPBrain() const { return *(CAIBrain* const*)_pad_0x04; }   // CAIStateGroupBase::PBrain (+0x04)
    CThingCreatureBase* GetCreature() const { return (CThingCreatureBase*)GetPBrain()->PCreature; }
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    void ForceRiseUp();
};

void CAIStateGroup_TrollBase_Methods::ForceRiseUp()
{
    CTCCreatureModeManager* modes;
    if (GetPBrain()->PCreature->GetTC(TCI_ENTITY_MODE_MANAGER, &modes) && modes->IsModeActive(0x1E))
        AIAction_PerformActionAndWait(&CCreatureAction_RiseUp(GetCreature()), false);
}