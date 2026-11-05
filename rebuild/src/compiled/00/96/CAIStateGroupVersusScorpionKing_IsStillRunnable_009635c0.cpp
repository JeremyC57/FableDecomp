// CAIStateGroup_VersusScorpionKing::IsStillRunnable() -- base check, then "target not in creature mode 0x38"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009635C0, length 113 bytes (+ int3 padding).
// Callees: CAIStateGroup_VersusEnemyBase::IsStillRunnable @ 0x00962AF0 (PDB-mangled manifest name),
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface,
// type 0x31 = bit 17 of the presence word at +0x24), CTCCreatureModeManager::IsModeActive(long) @ 0x0073A8A0.
//
// Shape: the base-class check returns its own al when false (`if (!Base::IsStillRunnable()) return false;`
// needs no `xor al,al`: VC reuses the tested byte). Then the target's mode manager comes from the out-parameter
// GetInterface whose result is not tested, so retail tail-duplicates `HasMode(0x38)` + the `neg ; sbb ; inc`
// negation (`return HasMode(0x38) ? false : true;` -- `!IsModeActive()` widens to `sbb eax,eax ; inc eax`) into
// the hit arm and the miss arm (which reads the unwritten frame slot). Modelled over the
// VersusEnemyBase PDB header (PTargetThing +0x14); the derived members follow the ScorpionKing header offsets.
// Leaf: ?IsStillRunnable@CAIStateGroup_VersusScorpionKing
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
#include "engine/CAIStateGroup_VersusEnemyBase.h"

class CTCBase;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with LowerBound @ 0x0040F020 -- the landed out-parameter
// GetInterface template (0x007504E0 / 0x00930DC0 shape): the component lands in eax, and a miss leaves the
// caller's out pointer unwritten (the frame slot that also holds the key).
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

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
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

class CTCCreatureModeManager { public:                 // interface type 0x31 component
public:
    bool IsModeActive(unsigned long) const;   // 0x0073A8A0
};

struct CAIStateGroup_VersusEnemyBase_Methods : CAIStateGroup_VersusEnemyBase {
    const CIntelligentPointer<CThing>& Target() const { return *(const CIntelligentPointer<CThing>*)PTargetThing; }
    bool IsStillRunnable();                     // 0x00962AF0
};

struct CAIStateGroup_VersusScorpionKing : CAIStateGroup_VersusEnemyBase_Methods {
    bool GoClockwise;                           // +0x1c
    unsigned char PArenaCentreThing[0xb];       // +0x1d CIntelligentPointer<CThing>
    bool IsStillRunnable();
};

bool CAIStateGroup_VersusScorpionKing::IsStillRunnable()
{
    if (!CAIStateGroup_VersusEnemyBase_Methods::IsStillRunnable())
        return false;
    CTCCreatureModeManager* modes;
    Target().GetPItem()->GetTC(TCI_ENTITY_MODE_MANAGER, &modes);
    return modes->IsModeActive(0x38) ? false : true;
}