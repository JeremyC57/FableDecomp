// CAIStateGroup_ServeFood::Update() -- serve at a tavern table: mark the table's 0xB8 component occupied; state 1 walks to the table, makes a food object, puts it in the creature's carry slot (the food's CCarryableDef slot) and puts the tankard down, telling the table the food was served; state 2 walks to the table and, if the table's food for our task lives, takes it off the table, drops and destroys it and tells the table; then replace and terminate the state group
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009236C0, length 756 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface: 0xB8 on the table, bit 24 of +0x34; 0x46 =
// CTCCarrying on the creature, bit 6 of +0x28), CAIStateGroup_ServeFood::WalkTo @ 0x00922FC0 (by-value
// C3DVector + CRightHandedSet), CTCTavernTable::GetFood @ 0x007B85C0, CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10 /
// 0x00A01B90 / 0x00A01B50 / 0x00A01C10 (CIntelligentPointer<CThingObject>, vtable 0x0125A858),
// CCreatureAction_RemoveFoodFromTable ctor @ 0x00922EE0, CCreatureAction_PutDownTankard ctor @ 0x00922F50,
// AIAction_PerformActionAndWait @ 0x0097DB70, ~CCreatureActionBase @ 0x00693EF0, CTCCarrying::RemoveThingFromPrimarySlot
// @ 0x006A1E00 / PutThingInCarrySlot @ 0x006A1F80, IS_THING_ALIVE<CIntelligentPointer<...>> @ 0x0078B2A0 (__fastcall),
// CThing::Kill(bool) @ 0x004C9B80, CThing::GetPos @ 0x004C73D0, MakeFood @ 0x00923510, GetSubDef<CCarryableDef> @
// 0x005CC10E, the table's served notification @ 0x007B85F0, Terminate @ 0x008FCF40 (manifest "CXMVPlaybackInfo" is
// propagated), virtual OnReplace (slot 10).
// Layout: no engine header for this class; PTable +0x18, TaskIndex +0x1c, ServePos +0x20, ServeRHSet +0x2c
// (Ego_r.pdb), Mode +0x48; retail CCreatureActionBase is 0xa8 bytes, the
// pick-up action keeps a CIntelligentPointer at +0xa8.
// Shape: the bit-24 presence test reads the bitset word into a named local and shifts it (the masked / unnamed forms narrow to
// `mov cl,byte [eax+37h]`; the landed 0x0094BB20 TestTC form), bound to a `const bool&` (frame byte at +0x17); the action temporaries' generated destructors run inline (member intelligent pointer, then ~CCreatureActionBase);
// a `break` out of the state-2 block skips the notification but still destroys the food pointer.
class CTCBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound 0x0040F020 (STLport lower_bound over the 8-byte entries).
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
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
class CCarryableDef { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long RefCount;                              // +0x04
    unsigned char _pad08[0x28 - 0x08];
    long CarrySlot;                             // +0x28
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->RefCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
class CDefinitionManager { public:
    CDefPointer<CCarryableDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
class CDefClassBase { public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual long* GetSubDefIndex(const CCharString& name);   // slot 14 (+0x38)
    unsigned char _pad04[0x1c - 4];
    CDefinitionManager* m_pManager;                   // +0x1c
    bool GetSubDef(CDefPointer<CCarryableDef>* out);   // 0x005CC10E
};
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef<CCarryableDef> 0x005CC10E (the landed 0x00848500 copy): VC7.1 then knows the def
// pointer's address does not escape, so the def stays in a register after the call.
bool CDefClassBase::GetSubDef(CDefPointer<CCarryableDef>* out)
{
    long* pIndex;
    {
        CCharString name("CCarryableDef");
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<CCarryableDef> result = m_pManager->GetPDefFromGlobalIndex(*pIndex);
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                      // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;       // 0x004C73D0
    void Kill(bool silent);                     // 0x004C9B80
    bool HasTC(long interfaceType) const
    {
        unsigned long word = m_presentBits[interfaceType >> 5];
        return ((word >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        const bool& present = HasTC(interfaceType);
        if (present) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
        }
        return present;
    }
    template<class T> __forceinline bool FindInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CThingObject : public CThing { public: };
class CThingCreatureBase : public CThing { public: };
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CIntelligentPointerThingObject : CBaseIntelligentPointer {   // CIntelligentPointer<CThingObject> (vtable 0x0125A858)
    virtual void v00();
    __forceinline CIntelligentPointerThingObject(CThingObject* item) : CBaseIntelligentPointer() { SetPItem(item); }
};
bool __fastcall IS_THING_ALIVE(const CIntelligentPointerThingObject& pointer);   // 0x0078B2A0
class CTCCarrying { public:                            // interface 0x46
    CThing* RemoveThingFromPrimarySlot(long flags);                     // 0x006A1E00
    void PutThingInCarrySlot(CThingObject* thing, long slot, bool immediately);   // 0x006A1F80
};
class CTCTavernTable { public:                         // interface 0xb8
    unsigned char _pad[0xc];
    bool Occupied;                              // +0x0c
    CThingObject* GetFood(long taskIndex);      // 0x007B85C0
    void NotifyFoodServed(long taskIndex, CThing* food);   // 0x007B85F0
};
class CCreatureActionBase { public:                    // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0xa8 - 4];
    virtual ~CCreatureActionBase();                     // 0x00693EF0
};
class CCreatureAction_RemoveFoodFromTable : public CCreatureActionBase { public:   // retail sizeof 0xb4
    CBaseIntelligentPointer PThing;             // +0xa8
    long Kind;                                  // +0xb0
    CCreatureAction_RemoveFoodFromTable(CThingCreatureBase* owner, CThing* thing);   // 0x00922EE0
};
class CCreatureAction_PutDownTankard : public CCreatureActionBase { public:   // retail sizeof 0xac
    long Slot;                                  // +0xa8
    CCreatureAction_PutDownTankard(CThingCreatureBase* owner);     // 0x00922F50
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11
    virtual void Update();                      // slot 12 (+0x30)
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    void Terminate();                           // 0x008FCF40
};
struct CAIStateGroup_ServeFood : CAIStateGroupBase {
    unsigned char _pad_0x14[0x4];               // +0x14
    CThing* PTable;                             // +0x18
    long TaskIndex;                             // +0x1c
    C3DVector ServePos;                         // +0x20
    CRightHandedSet ServeRHSet;                 // +0x2c
    long PlaceIndex;                            // +0x44
    long Mode;                                  // +0x48 (ServingMode)
    bool WalkTo(C3DVector pos, CRightHandedSet rhSet);   // 0x00922FC0
    CThingObject* MakeFood(const C3DVector* position, CThing* owner);     // 0x00923510
    virtual void Update();
};

void CAIStateGroup_ServeFood::Update()
{
    CTCTavernTable* table;
    PTable->GetInterface(&table, 0xb8);
    table->Occupied = true;
    switch (Mode) {
    case 1:
        if (WalkTo(ServePos, ServeRHSet)) {
            CThingObject* food = MakeFood(&PTable->GetPos(), PTable);
            CTCCarrying* carrying;
            GetCreature()->FindInterface(&carrying, 0x46);
            CDefPointer<CCarryableDef> carryDef;
            food->m_pDef->GetSubDef(&carryDef);
            carrying->PutThingInCarrySlot(food, carryDef->CarrySlot, true);
            if (AIAction_PerformActionAndWait(&CCreatureAction_PutDownTankard(GetCreature()), false) != 2)
                break;
            table->NotifyFoodServed(TaskIndex, food);
        }
        break;
    case 2:
        if (WalkTo(ServePos, ServeRHSet)) {
            CIntelligentPointerThingObject food(table->GetFood(TaskIndex));
            if (food.GetPItem() && !(food.GetPItem()->m_flags91 & 1)) {
                if (AIAction_PerformActionAndWait(&CCreatureAction_RemoveFoodFromTable(GetCreature(), food.GetPItem()), false) != 2)
                    break;
                CTCCarrying* carrying;
                GetCreature()->GetInterface(&carrying, 0x46);
                carrying->RemoveThingFromPrimarySlot(0);
                if (IS_THING_ALIVE(food))
                    food.GetPItem()->Kill(true);
            }
            table->NotifyFoodServed(TaskIndex, 0);
        }
        break;
    }
    OnReplace();
    Terminate();
}