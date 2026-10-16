// Grade: DIFFER(188v188) -- frame: retail is 0x1c bytes with the intelligent-pointer temporary overlapping the dead filter slot (+0xc) and the out thing at +0x14; ours 0x20 with spouse +0x10, temp +0x14 | tried: inline search-tools expression, named tools local (fixed call order), 8-byte filter
// CAIStateGroup_MinionBehaviourSad::FoundDeadSpouse() -- search a 10x10 box around the creature for a dead spouse
// (first match of the spouse filter); on a hit remember it in PDeadSpouse.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096F180, length 185 bytes (+ int3 padding).
// Callees: the creature's position getter @ 0x004C73D0 (manifest "CVertexBufferWin32::DoSizeof" is propagated and wrong),
// __ftol2 @ 0x00BFEA70 (x and y truncated to int), CAIStateGroupBase::GetPWorld @ 0x008FCFC0 (+0x20 = the world's
// CThingSearchTools), CThingSearchTools::GetPFirstThingInRoughArea<filter, CThingAICreature const> @ 0x0096F260
// (box, filter, out), the converting CIntelligentPointer temporary (vtable 0x01237F1C): ctor @ 0x00A01B10,
// SetPItem @ 0x00A01B90 (twice), GetPItem @ 0x00A01B50, dtor @ 0x00A01C10.
// Shape: C2DBoxI {x-5, y-5, x+5, y+5} in the frame, a one-word filter holding the creature, the out thing; the
// intelligent-pointer temporary reuses the filter's slot. No engine header for this class; PDeadSpouse is +0x34.
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

struct C3DVector { float X, Y, Z; };
struct C2DBoxI { long MinX, MinY, MaxX, MaxY; };

struct CThingCreaturePos {
    const C3DVector& GetPosition();             // 0x004C73D0
};

struct CThingFilter_IsDeadSpouse {
    CThing* PCreature;
};

struct CThingSearchTools {
    bool GetPFirstThingInRoughArea(const C2DBoxI& box, const CThingFilter_IsDeadSpouse& filter, CThing** ppThing);   // 0x0096F260
};

struct CWorld {
    unsigned char _pad[0x20];
    CThingSearchTools* PThingSearchTools;       // +0x20
};

struct CAIStateGroup_MinionBehaviourSad : CAIStateGroupBase {
    unsigned char _pad_0x14[0x20];              // +0x14..+0x33
    CIntelligentPointerThing PDeadSpouse;       // +0x34
    CWorld* GetPWorld();                        // 0x008FCFC0 (CAIStateGroupBase)
    bool FoundDeadSpouse();
};

bool CAIStateGroup_MinionBehaviourSad::FoundDeadSpouse()
{
    const C3DVector& pos = ((CThingCreaturePos*)PBrain->PCreature)->GetPosition();
    long x = (long)pos.X;
    long y = (long)pos.Y;
    C2DBoxI box;
    box.MinX = x - 5;
    box.MinY = y - 5;
    box.MaxX = x + 5;
    box.MaxY = y + 5;
    CThingFilter_IsDeadSpouse filter;
    filter.PCreature = PBrain->PCreature;
    CThing* spouse;
    CThingSearchTools* tools = GetPWorld()->PThingSearchTools;
    if (tools->GetPFirstThingInRoughArea(box, filter, &spouse)) {
        PDeadSpouse = spouse;
        return true;
    }
    return false;
}
