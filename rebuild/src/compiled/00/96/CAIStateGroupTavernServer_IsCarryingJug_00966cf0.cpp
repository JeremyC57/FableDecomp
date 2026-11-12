// CAIStateGroup_TavernServer::IsCarryingJug() -- with the jug alive, ask the creature's inventory component
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Leaf: ?IsCarryingJug@CAIStateGroup_TavernServer@@QAE_NXZ
// agentNF25b (2026-11-12): GetSubDef<CCarryableDef> const 0x005CC10E (the body pushes 'CCarryableDef'; was a local CJugDef) from
//   engine/CDefClassBase_GetSubDef.h under the per-TU no-inline opt-in; the +0x28 word is Ego_r CCarryableDef::ActiveCarrySlot, read into a named
//   local (the earlier made-up GetItemID() getter); 0x006A1230 is CTCCarrying::IsCarrySlotFree(long) const (Ego_r + FableWin; landed row,
//   was HasItemWithID on a local CTCInventory) from engine/CTCCarrying.h.
// (interface 0x46) whether the jug's item id is absent; the result is that negation.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00966CF0, length 172 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PJug +0x14, three times), CVectorMap::LowerBound
// @ 0x0040F020 (inlined out-parameter GetInterface, 0x46 = bit 6 of +0x28; the miss path reads the never-written
// slot), CDefClassBase::GetSubDef<...> @ 0x005CC10E on the jug's def (+0x70), the inventory's item lookup
// @ 0x006A1230 (bool, item id; manifest "InventoryFindItemByIDLowerBound"), the def's virtual slot 1 at zero.
// Shape: early `return false` for a dead jug; the out def pointer declared (zeroed) before the jug fetch; the
// result `!found` is `neg ; sbb ; inc`; the def released without a null test (`dec [esi+4] ; jne`).
// The engine header is padding only (PDB names PJug +0x14 ...), so a local model is used.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
#include "fable_compiler.h"
class CTCBase;

#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCARRYABLEDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE   // agentNF25b: the GetSubDef<CCarryableDef> template body (0x005CC10E) kept out of line, visible to the TU
#include "fable_string.h"
#include "engine/CDefPointer.h"
#include "engine/CCarryableDef.h"
#include "engine/CDefClassBase_GetSubDef.h"

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020.
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad9[0x1c];
    CDefPointer<CDefClassBase> PDef;            // +0x70 CDefPointer<const CThingDef>
    unsigned char _pad2[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

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
        if (!(p.GetPItem()->m_flags91 & 1))
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

#define FABLE_RETAIL_CTCCARRYING_SLOTFREE   // ?IsCarrySlotFree@CTCCarrying@@QBE_NJ@Z 0x006A1230 (agentNF25b)
#include "engine/CTCCarrying.h"                 // interface 0x46

struct CAIStateGroup_TavernServer : CAIStateGroupBase {
    CIntelligentPointer<CThing> PJug;               // +0x14 CIntelligentPointer<CThingObject>
    CIntelligentPointer<CThing> PBar;               // +0x1c CIntelligentPointer<CThingObject>
    CIntelligentPointer<CThing> PTable;             // +0x24 CIntelligentPointer<CThingObject>
    CIntelligentPointer<CThing> PTankard;           // +0x2c CIntelligentPointer<CThingObject>
    CIntelligentPointer<CThing> PPub;               // +0x34 CIntelligentPointer<CThingBuilding>
    long JugIndex;                              // +0x3c
    bool IsCarryingJug();
};

bool CAIStateGroup_TavernServer::IsCarryingJug()
{
    if (!IS_THING_ALIVE(PJug))
        return false;
    CTCCarrying* carrying;
    PBrain->PCreature->GetInterface(&carrying, 0x46);
    bool result;
    {
        CDefPointer<const CCarryableDef> def;
        CThing* jug = PJug.GetPItem();
        jug->PDef.Get()->GetSubDef(&def);
        long slot = def->ActiveCarrySlot;                   // named: read before the call (was a made-up inline getter)
        result = !carrying->IsCarrySlotFree(slot);
    }
    return result;
}