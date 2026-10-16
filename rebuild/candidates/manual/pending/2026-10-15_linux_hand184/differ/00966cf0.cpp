// Grade: DIFFER(174v174) -- the &def push precedes the jug GetPItem call (retail fetches the jug first) and the release loads the vtable into edx (retail eax) | tried: flat locals (8-byte frame, key and def apart), block-scoped def, named jug-def local (extra mov)
// CAIStateGroup_TavernServer::IsCarryingJug() -- with the jug alive, ask the creature's inventory component
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
struct CTCBase;

struct CDefPointeeBase {
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1: `call [eax+4]` at refcount zero
    long m_refCount;                            // +0x04
};

template <class T>
struct CDefPointer {                            // counted def pointer (GetSubDef's out pointer, released by hand)
    CDefPointeeBase* Object;                    // +0x00
    CDefPointer() : Object(0) {}
    T* operator->() const { return (T*)Object; }
};

struct CJugDef : CDefPointeeBase {              // the jug object's sub-def
    unsigned char _pad08[0x28 - 0x08];
    long ItemID;                                // +0x28
};

struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<const CJugDef>* out);   // 0x005CC10E (manifest "GetSubDef<CAbilityDef>", propagated)
};

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020.
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
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

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CRightHandedSet { float m[6]; };         // 0x18 bytes
struct C3DVector { float X, Y, Z; };

struct CTCInventory {                           // interface 0x46
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
        ((CDefClassBase*)PJug.GetPItem()->PDef.Object)->GetSubDef(&def);
        CDefPointeeBase* object = def.Object;
        result = !inventory->HasItemWithID(def->ItemID);
        if (--object->m_refCount == 0)
            object->Delete();
    }
    return result;
}
