// CAIStateGroup_TavernServer::IsCarryingJug() -- with the jug alive, ask the creature's inventory component
// Retry 10b (getsw sweep, FIXES B-15): the item id is read through an inline getter `GetItemID()`; the plain member read gives the parked register/scheduling difference.
// (interface 0x46) whether the jug's item id is absent; the result is that negation.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009687B0, length 172 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PJug +0x14, three times), CVectorMap::LowerBound
// @ 0x0040F020 (inlined out-parameter GetInterface, 0x46 = bit 6 of +0x28; the miss path reads the never-written
// slot), CDefClassBase::GetSubDef<...> @ 0x005CC10E on the jug's def (+0x70), the inventory's item lookup
// @ 0x006A1230 (bool, item id; manifest "InventoryFindItemByIDLowerBound"), the def's virtual slot 1 at zero.
// Shape: early `return false` for a dead jug; the out def pointer declared (zeroed) before the jug fetch; the
// result `!found` is `neg ; sbb ; inc`; the def released without a null test (`dec [esi+4] ; jne`).
// The engine header is padding only (PDB names PJug +0x14 ...), so a local model is used.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
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
    unsigned char _pad28[8];
    long ItemID;                                // +0x30
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
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound (STLport lower_bound over the 8-byte {type, component} entries): compiled above
// the caller so VC7.1 knows the key's address does not escape and packs the lookup keys as retail (the landed 0x00848730 idiom).
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x70 - 0x4c];
    CDefPointer<CDefClassBase> PDef;            // +0x70 CDefPointer<const CThingDef>
    unsigned char _pad2[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
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
class C3DVector { public: float X, Y, Z; };

class CTCInventory { public:                           // interface 0x46
    bool HasItemWithID(long id);                // 0x006A1230
};

struct CAIStateGroup_TavernServer : CAIStateGroupBase {
    CBaseIntelligentPointer PJug;               // +0x14 CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PBar;               // +0x1c CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PTable;             // +0x24 CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PTankard;           // +0x2c CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PPub;               // +0x34 CIntelligentPointer<CThingBuilding>
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