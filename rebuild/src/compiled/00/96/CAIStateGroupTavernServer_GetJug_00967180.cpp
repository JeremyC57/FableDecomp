// CAIStateGroup_TavernServer::GetJug() -- keep a live jug; otherwise ask the bar's tavern-bar component
// (interface 0xB9) for a free jug point for this creature and adopt the jug standing there.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00967180, length 206 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PJug +0x14 twice, PBar +0x1c, the returned temporary),
// CVectorMap::LowerBound @ 0x0040F020 (inlined HasTC + entry-returning FindEntry, 0xB9 = bit 25 of +0x34),
// CTCTavernBar::GetFreeJugPoint @ 0x007BA000 (creature, &BarPos, &BarRHSet, &BarActionPointIndex, &JugIndex, true),
// CTCTavernBar::GetJug(index) @ 0x007B9F60 (CIntelligentPointer<CThingObject> returned by hidden pointer),
// SetPItem @ 0x00A01B90 on PJug, the temporary's dtor @ 0x00A01C10.
// Shape: early `return true` while the jug is alive (inline IS_THING_ALIVE); the component kept in edi; the
// creature a named local (its load is scheduled ahead of the component load); the
// returned pointer temporary shares the lookup-key slot; `PJug = bar->GetJug(JugIndex)` is the inline
// operator= (`SetPItem(other.GetPItem())`). Engine header is padding only (PDB names from it), so a local model.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
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
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->type <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5  (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CBaseIntelligentPointerCtor {            // CBaseIntelligentPointer (8 bytes: vptr + item)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointerCtor();             // 0x00A01C10
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};


inline bool IS_THING_ALIVE(const CBaseIntelligentPointerCtor& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CRightHandedSet { float m[6]; };         // 0x18 bytes
struct C3DVector { float X, Y, Z; };

struct CIntelligentPointerObject : CBaseIntelligentPointerCtor { };   // CIntelligentPointer<CThingObject>

struct CTCTavernBar {                           // interface 0xB9
    bool GetFreeJugPoint(CThing* creature, C3DVector* pos, CRightHandedSet* rhSet, long* actionPointIndex,
                         long* jugIndex, bool reserve);              // 0x007BA000
    CIntelligentPointerObject GetJug(long index);                    // 0x007B9F60
};

struct CAIStateGroup_TavernServer : CAIStateGroupBase {
    CBaseIntelligentPointerCtor PJug;           // +0x14 CIntelligentPointer<CThingObject>
    CBaseIntelligentPointerCtor PBar;           // +0x1c CIntelligentPointer<CThingObject>
    CBaseIntelligentPointerCtor PTable;         // +0x24 CIntelligentPointer<CThingObject>
    CBaseIntelligentPointerCtor PTankard;       // +0x2c CIntelligentPointer<CThingObject>
    CBaseIntelligentPointerCtor PPub;           // +0x34 CIntelligentPointer<CThingBuilding>
    long JugIndex;                              // +0x3c
    long ServingActionPointIndex;               // +0x40
    C3DVector ServingPos;                       // +0x44
    CRightHandedSet ServingRHSet;               // +0x50
    long BarActionPointIndex;                   // +0x68
    C3DVector BarPos;                           // +0x6c
    CRightHandedSet BarRHSet;                   // +0x78
    long ActionPointIndex;                      // +0x90
    long SeatIndex;                             // +0x94
    float AverageWalkSpeed;                     // +0x98
    bool GetJug();
};

bool CAIStateGroup_TavernServer::GetJug()
{
    if (IS_THING_ALIVE(PJug))
        return true;
    CThing* bar = PBar.GetPItem();
    if (bar->HasTC(0xB9)) {
        CTCTavernBar* tavernBar = (CTCTavernBar*)bar->FindEntry(0xB9)->component;
        CThing* creature = PBrain->PCreature;
        if (tavernBar->GetFreeJugPoint(creature, &BarPos, &BarRHSet, &BarActionPointIndex, &JugIndex, true)) {
            PJug.SetPItem(tavernBar->GetJug(JugIndex).GetPItem());
            return true;
        }
    }
    return false;
}