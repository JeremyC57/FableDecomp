// CAIStateGroup_MinionSitOnSeatBase::IsRunnable() -- when it is time to sit and an owner object exists, join the
// Grade: DIFFER(150v148) -- two things: retail loads the gather-point component straight into ecx and the creature argument AFTER it (`mov edx,[edi+4] ; mov ecx,[eax+4] ; mov eax,[edx+18h] ; push eax`), ours loads the argument first and moves the component (`mov eax,[eax+4] ; push edx ; mov ecx,eax`); and retail's AddAsUser failure jumps to the shared `xor al,al` tail at the end while ours lays that exit inline (`jne` over it).
// owner's gather point (if the group uses one), pick a seat from the owner and check the seat is valid.
// Leaf: ?IsRunnable@CAIStateGroup_MinionSitOnSeatBase@@MAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090F9C0, length 148 bytes (+ int3 padding). Vtable slot 11.
// Callees: this class's virtuals slot 14 (+0x38, bool "is it sit time"), slot 16 GetPOwnerObject(CThing**) (+0x40,
// 0x0090FAE0 in MinionSitAtInn), slot 17 (+0x44, bool "uses a gather point"), slot 18 GetSeat(CThing*) (+0x48,
// 0x009104D0 here); CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface on the OWNER, type 0x7B =
// CTCGatherPointBase, bit 27 of the bitset dword at +0x2c), CTCGatherPointBase::AddAsUser(CThing*) @ 0x007FFEB0
// with PBrain->PCreature, CAIStateGroup_UsesSeat::IsSeatValid @ 0x00938580 (tail, its bool returned).
// Shape: nested ifs with the single textual `return false` LAST (the merged `xor al,al` block sits after the
// IsSeatValid tail); the owner is a frame local passed by address (re-read from its slot for GetSeat); the
// gather-point join is `if (UsesGatherPoint() && owner && owner->HasTC(0x7b)) { gp = FindTC(0x7b); if
// (!gp->AddAsUser(creature)) return false; }` with the pointer-returning lookup in a named local (the component
// lands in ecx before the creature argument is loaded); then `GetSeat(owner); return IsSeatValid();`.
// Layout local (the engine header flattens the base and has no vtable).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;
struct CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
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
    unsigned long HasTC(long interfaceType) const     // the masked word (a bool return narrows to `shr ; test dl,1`)
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    // the pointer-returning lookup: the component lands straight in ecx for the call that follows
    template<class T> __forceinline T* FindTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
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
    CThing* GetPItem() const;                   // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
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
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
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
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
    unsigned long* GetPWorldSeed();             // 0x008FCFD0 (world random seed)
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated)
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

struct CTCGatherPointBase {                     // interface type 0x7B
    bool AddAsUser(CThing* user);               // 0x007FFEB0
};

struct CAIStateGroup_UsesSeat : CAIStateGroupBase {
    unsigned char _seat[0x20];                  // +0x14 PSeat, GotSeatPos, Sitting, SeatPosIndex, SeatPos, SittingOnBench
    bool IsSeatValid();                         // 0x00938580
};

struct CAIStateGroup_MinionSitOnSeatBase : CAIStateGroup_UsesSeat {
    CBaseIntelligentPointer POwner;             // +0x34 CIntelligentPointer<CThing>
    virtual bool IsTimeToSit();                 // slot 14 (+0x38)
    virtual void v15();                         // slot 15
    virtual bool GetPOwnerObject(CThing** ppOwner);     // slot 16 (+0x40)
    virtual bool UsesGatherPoint();             // slot 17 (+0x44)
    virtual void GetSeat(CThing* owner);        // slot 18 (+0x48)
    virtual bool IsRunnable();
};

bool CAIStateGroup_MinionSitOnSeatBase::IsRunnable()
{
    if (!IsTimeToSit())
        return false;
    CThing* owner;
    if (!GetPOwnerObject(&owner))
        return false;
    if (UsesGatherPoint()) {
        if (owner != 0) {
            if (owner->HasTC(0x7b)) {
                if (!owner->FindTC<CTCGatherPointBase>(0x7b)->AddAsUser(PBrain->PCreature))
                    return false;
            }
        }
    }
    GetSeat(owner);
    return IsSeatValid();
}
