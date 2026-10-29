// CAIStateGroup_MinionReactToCriminal::OnReplace() -- drop creature mode 15; if this state group overrode the house locking and the (live, locked) home is not the creature's own home, unlock it again; stamp the leave frame
// Grade: RELOCATION_MATCH -- the unlock is an inline member of the lock (`void Unlock() { Locked = 0; }`) called on the GetPItem() result: VC7.1 then clears the bit in place (`and byte ptr [eax+0D4h],0FEh`); the direct bitfield store on the call result is a load/and/store
// Leaf: OnReplace
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00930E20, length 179 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x31 = CTCCreatureModeManager, bit 17 of
// the dword at +0x24; miss path reads the never-written out slot), CTCCreatureModeManager::RemoveMode @ 0x0073ABA0,
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (IS_THING_ALIVE, the lock test, the creature's own home pointer at
// +0x170, the compare and the unlock), CWorld::GetFrame @ 0x0049D870.
// Shape: the creature's own home is a named local read before the PHome GetPItem it is compared with (held in edi);
// the building lock flag is a 1-bit bitfield at +0xd4 (`test byte` / `and byte ..,0FEh` in place).
// Engine header names (PHome +0x1c, FrameLastLeftStategroup +0x24, OverrodeLocking +0x2c); the header flattens the
// base, so the class is modelled locally.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};

class CThing { public:
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

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
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

class CWorld { public: static long __fastcall GetFrame(); };            // 0x0049D870

class CTCCreatureModeManager { public:
    void RemoveMode(long mode);                 // 0x0073ABA0
};
struct CThingBuildingLock {
    unsigned char _pad[0xd4];
    unsigned char Locked : 1;                   // +0xd4 bit 0
    void Unlock() { Locked = 0; }
};
struct CHomeOwner {
    unsigned char _pad[0x170];
    CBaseIntelligentPointerCtor PHome;          // +0x170
    CThing* GetPHome() { return PHome.GetPItem(); }
};

struct CAIStateGroup_MinionReactToCriminal : CAIStateGroupBase {
    CBaseIntelligentPointerCtor PHero;          // +0x14 CIntelligentPointer<CThingPlayerCreature>
    CBaseIntelligentPointerCtor PHome;          // +0x1c CIntelligentPointer<CThingBuilding>
    long FrameLastLeftStategroup;               // +0x24
    long FrameToNextSpeak;                      // +0x28
    bool OverrodeLocking;                       // +0x2c
    bool HeroInsideHouse;                       // +0x2d
    bool MeInsideHouse;                         // +0x2e
    bool GuardInsideHouse;                      // +0x2f
    virtual void OnReplace();
};

void CAIStateGroup_MinionReactToCriminal::OnReplace()
{
    CTCCreatureModeManager* modes;
    PBrain->PCreature->GetInterface(&modes, 0x31);
    modes->RemoveMode(0xf);
    if (OverrodeLocking && IS_THING_ALIVE(PHome) && ((CThingBuildingLock*)PHome.GetPItem())->Locked) {
        CThing* ownHome = ((CHomeOwner*)PBrain->PCreature)->GetPHome();
        if (PHome.GetPItem() != ownHome)
            ((CThingBuildingLock*)PHome.GetPItem())->Unlock();
    }
    FrameLastLeftStategroup = CWorld::GetFrame();
}