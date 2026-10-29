// CAIStateGroup_MinionSitOutside::IsRunnable() -- runnable when the idle scheduler prefers this state group and (for a
// villager with a village) it is the village's sit-outside time; then pick a sitting duration of 225..374 frames and
// defer to CAIStateGroup_MinionSitOnSeatBase::IsRunnable.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00938F70, length 238 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface: type 0xD3 = CTCIdleScheduler, bit 19 of the dword
// at +0x38, the preference test is skipped without it; type 0x23 = CTCVillager, bit 3 of +0x24), CTCIdleScheduler::
// IsPreferredIdle @ 0x00801BE0, GFGetPVillageInterface @ 0x007EE540 (twice), the village time test @ 0x00658D5D
// (manifest "CTCVillage::IsCurfewTime" is propagated; the landed curfew test is 0x00658DB3), CAIStateGroupBase::GetPWorld
// @ 0x008FCFC0 (result dropped), GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890, CWorld::GetFrame @ 0x0049D870,
// CAIStateGroup_MinionSitOnSeatBase::IsRunnable @ 0x0090F9C0 (qualified base call, tail result).
// Shape: the seed step is the landed free inline `WorldRandomRange(unsigned long& seed, unsigned long range)` (0x009284B0:
// seed = seed*0x24A1+0x24DF; seed = GFROR13(seed); `range == 0` test folded for 150; `seed % range` through the reference),
// called as `225 + WorldRandomRange(*GetPWorldSeed(), 150) + GFGetCurrentFrame()`: the inline's own return path keeps the
// unsigned `div` (remainder in esi) BEFORE the frame call, then `lea edx,[eax+esi+0E1h]`. A member helper returning
// `*seed % range` or `rotated % range` (with or without the zero test) sinks the div past the call.
// FrameToStopSitting +0x40 (engine header name); the header flattens the bases, so the class is modelled locally.
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

class CWorld { public: static long __fastcall GetFrame(); };            // 0x0049D870
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

inline unsigned long WorldRandomRange(unsigned long& seed, unsigned long range)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    if (range == 0)
        return 0;
    return seed % range;
}

class CTCIdleScheduler { public:
    bool IsPreferredIdle(CAIStateGroupBase* group);     // 0x00801BE0
};
struct CTCVillager;
class CTCVillage { public:
    bool IsSitOutsideTime();                    // 0x00658D5D
};
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540

struct CAIStateGroup_MinionSitOnSeatBase : CAIStateGroupBase {
    unsigned char _seat[0x2c];                  // +0x14..+0x3f UsesSeat / SitOnSeatBase members
    long FrameToStopSitting;                    // +0x40
    CWorld* GetPWorld();                        // 0x008FCFC0 (CAIStateGroupBase)
    unsigned long* GetPWorldSeed();             // 0x008FCFD0 (CAIStateGroupBase)
    virtual bool IsRunnable();                  // 0x0090F9C0
};

struct CAIStateGroup_MinionSitOutside : CAIStateGroup_MinionSitOnSeatBase {
    virtual bool IsRunnable();
};

bool CAIStateGroup_MinionSitOutside::IsRunnable()
{
    CTCIdleScheduler* scheduler;
    if (PBrain->PCreature->GetInterface(&scheduler, 0xd3)) {
        if (!scheduler->IsPreferredIdle(this))
            return false;
    }
    CTCVillager* villager;
    if (PBrain->PCreature->GetInterface(&villager, 0x23)) {
        if (GFGetPVillageInterface(villager) != 0) {
            if (!GFGetPVillageInterface(villager)->IsSitOutsideTime())
                return false;
        }
    }
    GetPWorld();
    FrameToStopSitting = 225 + WorldRandomRange(*GetPWorldSeed(), 150) + CWorld::GetFrame();
    return CAIStateGroup_MinionSitOnSeatBase::IsRunnable();
}