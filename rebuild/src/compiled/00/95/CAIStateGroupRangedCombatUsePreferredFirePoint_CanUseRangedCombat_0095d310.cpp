// CAIStateGroup_RangedCombatUsePreferredFirePoint::CanUseRangedCombat() -- the thing in the creature's primary
// carrying slot is a projectile weapon.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095D310, length 138 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x004365B0 (twice: inlined GetInterface for type 0x46 = CTCCarrying, bit 6 of
// the bitset dword at +0x28, on the creature -- the miss path reads the never-written slot -- and for type 0x48 =
// CTCWeapon, bit 8 of +0x28 (`test ch,1`), on the carried thing), CTCCarrying::GetThingInPrimarySlot @ 0x006A1440,
// CTCWeapon::IsProjectileWeapon @ 0x005DAA40.
// Shape: `GetInterface(&carry, 0x46); item = carry->GetThingInPrimarySlot(); if (item && item->GetInterface(&w,
// 0x48) && w->IsProjectileWeapon()) return true; return false;` with the component loaded straight into ecx for
// IsProjectileWeapon (`mov ecx,[eax+4]`) and the shared `xor al,al` exit. Layout local (the engine header
// flattens the base).
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

struct CTCWeapon {                              // interface type 0x48
    bool IsProjectileWeapon();                  // 0x005DAA40
};

struct CInterfaceMapB {                         // the same vector map, this module's LowerBound instantiation
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};

struct CThingB {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMapB m_interfaces;                // +0x44
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* FindTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMapB* map = &m_interfaces;
        CInterfaceMapB::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            *ppOut = FindTC<T>(interfaceType);
            return true;
        }
        return false;
    }
};

struct CTCCarrying {                            // interface type 0x46
    CThingB* GetThingInPrimarySlot();           // 0x006A1440
};

struct CAIStateGroup_RangedCombatUsePreferredFirePoint : CAIStateGroupBase {
    bool CanUseRangedCombat();
};

bool CAIStateGroup_RangedCombatUsePreferredFirePoint::CanUseRangedCombat()
{
    CTCCarrying* pCarrying;
    ((CThingB*)PBrain->PCreature)->GetInterface(&pCarrying, 0x46);
    CThingB* item = pCarrying->GetThingInPrimarySlot();
    if (item != 0) {
        if (item->HasTC(0x48)) {
            if (item->FindTC<CTCWeapon>(0x48)->IsProjectileWeapon())
                return true;
        }
    }
    return false;
}