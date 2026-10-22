// Grade: DIFFER(813v785) -- retail funnels every Drink/SitIdle exit into one shared call block and packs the 0xcf key with the others (+0x10), the village at +0x14; ours duplicates the tail calls and needs one more frame slot
// CAIStateGroup_GoToPub::ChooseAction(bool) -- pick the next pub activity: keep drinking a non-empty tankard when it is not cooking time and drinking is allowed, thank the barmaid for a full one or drink one time in four; at a busy table tell stories, listen, laugh or sit idle at random; otherwise drink, fall asleep or sit idle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094A3B0, length 783 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020, GFGetPVillageInterface @ 0x007EE540 (`__fastcall`), CTCVillage::IsCookTime
// @ 0x00658C0A, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, Drink @ 0x00948880, ThankBarmaid @ 0x00949380,
// CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890 (`__fastcall`), Laugh @ 0x00947D30, Listen
// @ 0x0094A190, TellStory @ 0x0094A2B0, FallAsleep @ 0x0094A0E0, IS_THING_ALIVE @ 0x004CC340 (`__fastcall`), SitIdle
// @ 0x00947CA0.
// Layout: Ego_r.pdb CAIStateGroup_GoToPub PTable +0x24, PTankard +0x34; CTCTavernTable (0xb8, retail 12-byte vectors)
// NumPeopleSeated +0x40, SomeoneTalking +0x44, SomeoneLaughing +0x45.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
struct CThingCreatureBase : CThing { };
struct CTCVillager;                             // interface 0x23
struct CTCVillage {
    bool IsCookTime();                          // 0x00658C0A
};
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);   // 0x007EE540
struct CTCTankard {                             // interface 0xba
    unsigned char _pad0[0x0c];
    float Fullness;                             // +0x0c
};
struct CTCEating {                              // interface 0xcf
    unsigned char _pad0[0x20];
    bool Busy;                                  // +0x20
};
struct CTCTavernTable {                         // interface 0xb8
    unsigned char _pad0[0x40];
    long NumPeopleSeated;                       // +0x40
    bool SomeoneTalking;                        // +0x44
    bool SomeoneLaughing;                       // +0x45
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
template<class T> bool __fastcall IS_THING_ALIVE_P(const T& thing);   // 0x004CC340
unsigned long __fastcall GFROR13(unsigned long value);      // 0x00497890
inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
};
struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    CBaseIntelligentPointer PPub;               // +0x14
    CBaseIntelligentPointer PBar;               // +0x1c
    CBaseIntelligentPointer PTable;             // +0x24
    CBaseIntelligentPointer PBarman;            // +0x2c
    CBaseIntelligentPointer PTankard;           // +0x34
    bool SitIdle();                             // 0x00947CA0
    bool Laugh();                               // 0x00947D30
    bool Drink();                               // 0x00948880
    bool ThankBarmaid();                        // 0x00949380
    bool FallAsleep();                          // 0x0094A0E0
    bool Listen();                              // 0x0094A190
    bool TellStory();                           // 0x0094A2B0
    bool ChooseAction(bool canDrink);
};

bool CAIStateGroup_GoToPub::ChooseAction(bool canDrink)
{
    CTCVillager* villager;
    GetCreature()->GetInterface(&villager, 0x23);
    CTCVillage* village = GFGetPVillageInterface(villager);
    CTCTankard* tankard = 0;
    if (IS_THING_ALIVE(PTankard))
        PTankard.GetPItem()->GetInterface(&tankard, 0xba);
    CTCEating* eating;
    if (PTable.GetPItem()->GetInterface(&eating, 0xcf) && eating->Busy)
        return SitIdle();
    if (!village->IsCookTime() && canDrink && IS_THING_ALIVE(PTankard)) {
        if (tankard->Fullness != 0.0f)
            return Drink();
        return false;
    }
    if (IS_THING_ALIVE(PTankard) && tankard != 0) {
        if (tankard->Fullness == 1.0f)
        {
            if (ThankBarmaid())
                return Drink();
            return false;
        }
        if (WorldRandomRange(*GetPWorldSeed(), 4) == 0)
            return Drink();
    }
    CTCTavernTable* tavernTable;
    PTable.GetPItem()->GetInterface(&tavernTable, 0xb8);
    if (tavernTable->NumPeopleSeated > 1) {
        if (tavernTable->SomeoneTalking) {
            switch (WorldRandomRange(*GetPWorldSeed(), 4)) {
            case 0: return SitIdle();
            case 1: return Listen();
            case 2: return Laugh();
            default: return TellStory();
            }
        }
        if (tavernTable->SomeoneLaughing && WorldRandomRange(*GetPWorldSeed(), 2) == 0)
            return Laugh();
        if (WorldRandomRange(*GetPWorldSeed(), 2) == 0)
            return SitIdle();
        return TellStory();
    }
    switch (WorldRandomRange(*GetPWorldSeed(), 5)) {
    case 0:
        if (IS_THING_ALIVE_P(PTankard))
            return Drink();
        break;
    case 1:
        return FallAsleep();
    }
    return SitIdle();
}
