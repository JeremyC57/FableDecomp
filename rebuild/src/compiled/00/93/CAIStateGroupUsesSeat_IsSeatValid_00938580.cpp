// CAIStateGroup_UsesSeat::IsSeatValid() -- the seat thing exists and its action-point component reports usable
// Leaf: ?IsSeatValid@CAIStateGroup_UsesSeat@@QAE_NXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00938580, length 98 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x2), CVectorMap::LowerBound @ 0x0040F020 (inlined
// GetInterface, type 0x2D = bit 13 of the presence word at +0x24), CTCUserActionPointBase::IsActionPointFree
// @ 0x007FEBF0 (manifest label; the shared 0x007FEBF0 is also called by the WorkSawingWood/HammerAnvil
// OnActivate bodies of this chunk).
//
// Shape: PSeat (+0x14, PDB name) is null-tested with one GetPItem, the second GetPItem feeds the inlined
// GetInterface whose miss leaves the component pointer unwritten (`mov eax,[esp+4]`, the key's frame slot);
// `if (f()) return true;` inside the null test with one shared `return false` tail gives the separate
// `mov al,1` exit and the `xor al,al` tail. No engine header exists for this class: the base is modelled
// with the CAIStateGroupBase PDB members, the derived members with their PDB names and offsets.
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

struct CTCUserActionPointBase {                 // interface type 0x2D component
    bool IsActionPointFree();                   // 0x007FEBF0
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    bool Terminated;                            // +0x10
    unsigned char Priority;                     // +0x11
    bool Enabled;                               // +0x12
};

struct CAIStateGroup_UsesSeat : CAIStateGroupBase {
    CIntelligentPointer<CThing> PSeat;              // +0x14 CIntelligentPointer<CThingObject>
    bool GotSeatPos;                            // +0x1c
    bool Sitting;                               // +0x1d
    long SeatPosIndex;                          // +0x20
    float SeatPos[3];                           // +0x24 C3DVector
    bool SittingOnBench;                        // +0x30
    bool IsSeatValid();
};

bool CAIStateGroup_UsesSeat::IsSeatValid()
{
    if (PSeat.GetPItem() != 0) {
        CTCUserActionPointBase* actionPoint;
        PSeat.GetPItem()->GetTC(TCI_SINGLE_USER_ACTION_POINTS, &actionPoint);
        if (actionPoint->IsActionPointFree())
            return true;
    }
    return false;
}