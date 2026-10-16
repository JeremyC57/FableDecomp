// Grade: DIFFER(219v219) -- register only: retail tests the presence byte through eax (`mov eax,[eax+18h] ; test byte [eax+28h],40h`), ours through ecx | tried: while/return loop (231 bytes), do/while loop + chair branch order (all else fixed), raw bitset test
// CAIStateGroup_SitForMeal::Update() -- with a live cooking pot, take food from it, then either sit on a free chair
// and keep eating while the state group says so, or (no chair) stand around and eat; always finish the update.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00945BD0, length 219 bytes (+ int3 padding).
// Callees: GetPCookingPot @ 0x00945A90, the converting CIntelligentPointer temporary (vtable 0x01238C6C): ctor @
// 0x00A01B10, SetPItem @ 0x00A01B90 (twice), GetPItem @ 0x00A01B50, dtor @ 0x00A01C10; GetFoodFromCookingPot @
// 0x009456A0, GetPFreeChair @ 0x00945B10, ReleaseControlOfPot @ 0x00945530, the seat setter @ 0x009106E0 (manifest
// "CVillageTaskCarryCrate::AssignCreatureToTask" is propagated), CAIStateGroup_UsesSeat::MoveToSeatAndSitDown @
// 0x009387C0, CAIStateGroupBase::AIAction_DoNothing @ 0x0097E200, own virtual slot 14 (+0x38), StandAroundAndEat @
// 0x009454C0, the finish call @ 0x008FCF40.
// Shape: inline HasTC(0x46) (bit 6 of the byte at +0x28) with an early finish; IS-alive test inline; the eat loop
// `do { if (DoNothing() != 4) break; } while (KeepEating());` (VC duplicates the finish tail).
// No engine header for this class; PCookingPot is +0x34.
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
    __forceinline unsigned long HasTC(long interfaceType) const
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

struct CThingObject;

struct CAIStateGroup_SitForMeal : CAIStateGroupBase {
    unsigned char _pad_0x14[0x20];              // +0x14..+0x33 seat members
    CIntelligentPointerThing PCookingPot;       // +0x34
    virtual bool KeepEating();                  // slot 14 (+0x38)
    void FinishUpdate();                        // 0x008FCF40
    long AIAction_DoNothing();                  // 0x0097E200
    void SetSeat(CThingObject* chair);          // 0x009106E0
    bool MoveToSeatAndSitDown(long mode);       // 0x009387C0
    bool StandAroundAndEat();                   // 0x009454C0
    void ReleaseControlOfPot();                 // 0x00945530
    bool GetFoodFromCookingPot();               // 0x009456A0
    CThing* GetPCookingPot();                   // 0x00945A90
    CThingObject* GetPFreeChair();              // 0x00945B10
    virtual void Update();
};

void CAIStateGroup_SitForMeal::Update()
{
    if (!PBrain->PCreature->HasTC(0x46)) {
        FinishUpdate();
        return;
    }
    CThing* pot = GetPCookingPot();
    if (pot != 0 && !(pot->m_flags91 & 1)) {
        PCookingPot = pot;
        if (GetFoodFromCookingPot()) {
            CThingObject* chair = GetPFreeChair();
            if (chair != 0) {
                ReleaseControlOfPot();
                SetSeat(chair);
                if (MoveToSeatAndSitDown(3)) {
                    do {
                        if (AIAction_DoNothing() != 4)
                            break;
                    } while (KeepEating());
                }
            } else {
                StandAroundAndEat();
            }
        }
    }
    FinishUpdate();
}
