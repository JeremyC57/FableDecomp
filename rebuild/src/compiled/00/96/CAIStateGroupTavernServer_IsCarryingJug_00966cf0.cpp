// CAIStateGroup_TavernServer::IsCarryingJug() -- with the jug alive, ask the creature's inventory component
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retry 10b (getsw sweep, FIXES B-15): the item id is read through an inline getter `GetItemID()`; the plain member read gives the parked register/scheduling difference.
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

class CDefPointeeBase { public:
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1: `call [eax+4]` at refcount zero
    long m_refCount;                            // +0x04
};

template <class T>
class CDefPointer { public:                            // counted def pointer (GetSubDef's out pointer, released by hand)
    CDefPointeeBase* Object;                    // +0x00
    CDefPointer() : Object(0) {}
    T* operator->() const { return (T*)Object; }
};

struct CJugDef : CDefPointeeBase {              // the jug object's sub-def
    unsigned char _pad08[0x28 - 0x08];
    long ItemID;                                // +0x28
    long GetItemID() const { return ItemID; }
};

class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<const CJugDef>* out);   // 0x005CC10E (manifest "GetSubDef<CAbilityDef>", propagated)
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const CJugDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020.
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
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

class CTCInventory { public:                           // interface 0x46
    bool HasItemWithID(long id);                // 0x006A1230
};

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
    CTCInventory* inventory;
    PBrain->PCreature->GetInterface(&inventory, 0x46);
    bool result;
    {
        CDefPointer<const CJugDef> def;
        CThing* jug = PJug.GetPItem();
        ((CDefClassBase*)jug->PDef.Object)->GetSubDef(&def);
        CDefPointeeBase* object = def.Object;
        result = !inventory->HasItemWithID(def->GetItemID());
        if (--object->m_refCount == 0)
            object->Delete();
    }
    return result;
}