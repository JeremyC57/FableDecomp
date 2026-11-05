// CAIStateGroup_TavernServer::GetOldJugPoint() -- with the bar alive, ask its named-action-point component
// Leaf: ?GetOldJugPoint@CAIStateGroup_TavernServer@@QAE_NXZ
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// (interface 0x2E) for the free point "ACTION_POINT_MAID_0<JugIndex+1>" and store it as the bar point.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009672C0, length 233 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PBar +0x1c, three times), CVectorMap::LowerBound
// @ 0x0040F020 (inlined out-parameter GetInterface, 0x2E = bit 14 of +0x24; the miss path reads the never-written
// slot), CCharString(const char*, long) @ 0x0099EBF0, operator+=(const char*) @ 0x0099F100, GFIntToCharString
// @ 0x0099F830 (__fastcall, hidden result), operator+=(const CCharString&) @ 0x0099F0A0, ~CCharString @ 0x0099EAE0,
// the by-value copy @ 0x0099EC30, CTCNamedUserActionPoints::GetFreeNamedActionPoint @ 0x007FF190 (creature, &BarPos,
// &BarRHSet, CCharString name, &BarActionPointIndex, 0, true). Literals 0x0126DCC0 "ACTION_POINT_MAID", 0x0126DCBC "_0".
// Shape: early `return false` for a dead bar; the creature a named local read before the by-value string copy;
// the integer string temporary shares the lookup-key slot. Engine header is padding only, so a local model.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020.
#define FABLE_RETAIL_CCHARSTRING
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

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

class CRightHandedSet { public: float m[6]; };         // 0x18 bytes
#include "engine/C3DVector.h"

CCharString __fastcall GFIntToCharString(long value);   // 0x0099F830 (hidden result in ecx)

class CTCNamedUserActionPoints { public:               // interface 0x2E
    bool GetFreeNamedActionPoint(CThing* creature, C3DVector* pos, CRightHandedSet* rhSet, CCharString name,
                                 long* actionPointIndex, long slot, bool reserve);   // 0x007FF190
};

struct CAIStateGroup_TavernServer : CAIStateGroupBase {
    CIntelligentPointer<CThing> PJug;               // +0x14 CIntelligentPointer<CThingObject>
    CIntelligentPointer<CThing> PBar;               // +0x1c CIntelligentPointer<CThingObject>
    CIntelligentPointer<CThing> PTable;             // +0x24 CIntelligentPointer<CThingObject>
    CIntelligentPointer<CThing> PTankard;           // +0x2c CIntelligentPointer<CThingObject>
    CIntelligentPointer<CThing> PPub;               // +0x34 CIntelligentPointer<CThingBuilding>
    long JugIndex;                              // +0x3c
    long ServingActionPointIndex;               // +0x40
    C3DVector ServingPos;                       // +0x44
    CRightHandedSet ServingRHSet;               // +0x50
    long BarActionPointIndex;                   // +0x68
    C3DVector BarPos;                           // +0x6c
    CRightHandedSet BarRHSet;                   // +0x78
    bool GetOldJugPoint();
};

bool CAIStateGroup_TavernServer::GetOldJugPoint()
{
    if (!IS_THING_ALIVE(PBar))
        return false;
    CTCNamedUserActionPoints* points;
    PBar.GetPItem()->GetTC(TCI_NAMED_USER_ACTION_POINTS, &points);
    CCharString name("ACTION_POINT_MAID", -1);
    name += "_0";
    name += GFIntToCharString(JugIndex + 1);
    CThing* creature = PBrain->PCreature;
    return points->GetFreeNamedActionPoint(creature, &BarPos, &BarRHSet, name, &BarActionPointIndex, 0, true);
}