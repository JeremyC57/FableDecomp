// Grade: DIFFER(436v432) -- VC7.1 hoists the -1 of the SeatIndex test and both index stores into ebp for the whole body (retail: cmp [ebx+64h],-1 immediate, or esi,-1 late), which rotates carrying/tankard registers and the ebp/edi push point
// CAIStateGroup_GoToPub::FreeSeat() -- give the creature's seat back to the seating area, put a carried tankard back, and clear the seat/tankard pointers
// Retail @ 0x009486D0, 432 bytes (`ret`).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 / ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10,
//   CVectorMap::LowerBound 0x0040F020 (inlined GetInterface: 0xB8 seating area = bit 24 of +0x34, 0x46 carrying =
//   bit 6 of +0x28, 0xBA tavern tankard = bit 26 of +0x34), CTCSeatingArea::FreeSeat(long, long) 0x007B8AA0,
//   CTCCarrying::GetThingInPrimarySlot 0x006A1440 / RemoveThingFromPrimarySlot(bool) 0x006A1E00,
//   CTCTavernTankard::ReplaceTankard 0x00947C30.
// Shape: the landed GoToPub/GameMaster AI model (FreeBarPoint 0x00949320, OnReplace 0x0094CA80): out-parameter
// GetInterface lookups whose miss path reads the never-written slot; IS_THING_ALIVE inline; the two pointer
// members are cleared through `member = CIntelligentPointer<T>(0)` temporaries (vtables 0x01238C6C
// CIntelligentPointer<CThing> and 0x0125A858 CIntelligentPointer<CThingObject>, RTTI); -1 kept in esi for both
// index stores. No engine header for CAIStateGroup_GoToPub: local layout (seating thing +0x24, tankard +0x34,
// seat area index +0x3c, seat index +0x64).
struct CTCBase;
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
    unsigned char _pad1[0x45];
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
struct CThingObject : CThing {};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CTCSeatingArea { void FreeSeat(long seatArea, long seat); };          // 0x007B8AA0
struct CTCCarrying {
    CThing* GetThingInPrimarySlot();                                        // 0x006A1440
    CThing* RemoveThingFromPrimarySlot(bool drop);                          // 0x006A1E00
};
struct CTCTavernTankard { void ReplaceTankard(); };                         // 0x00947C30

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

struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    unsigned char _pad14[0x10];
    CIntelligentPointer<CThingObject> PSeating; // +0x24
    unsigned char _pad2c[8];
    CIntelligentPointer<CThing> PTankard;       // +0x34
    long SeatAreaIndex;                         // +0x3c
    unsigned char _pad40[0x24];
    long SeatIndex;                             // +0x64
    bool HasSeat() const { return SeatIndex != -1; }
    void FreeSeat();
};

void CAIStateGroup_GoToPub::FreeSeat()
{
    if (PSeating.GetPItem() && HasSeat()) {
        CTCSeatingArea* seating;
        PSeating.GetPItem()->GetInterface(&seating, 0xB8);
        seating->FreeSeat(SeatAreaIndex, SeatIndex);
        CTCCarrying* carrying;
        PBrain->PCreature->GetInterface(&carrying, 0x46);
        if (IS_THING_ALIVE(PTankard)) {
            if (carrying->GetThingInPrimarySlot() == PTankard.GetPItem())
                carrying->RemoveThingFromPrimarySlot(false);
            CTCTavernTankard* tankard;
            PTankard.GetPItem()->GetInterface(&tankard, 0xBA);
            tankard->ReplaceTankard();
        }
        SeatIndex = -1;
        PTankard = CIntelligentPointer<CThing>(0);
        PSeating = CIntelligentPointer<CThingObject>(0);
        SeatAreaIndex = -1;
    }
}
