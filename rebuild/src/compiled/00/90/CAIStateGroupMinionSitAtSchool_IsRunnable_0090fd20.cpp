// CAIStateGroup_MinionSitAtSchool::IsRunnable() -- runnable when the villager's village says it is time to go to
// school and the seat base agrees.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090FD20, length 132 bytes (+ int3 padding). Vtable slot 11.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x23 = CTCVillager, bit 3 of the bitset
// dword at +0x24), GFGetPVillageInterface @ 0x007EE540 (twice: null test, then the call object), the second world
// accessor @ 0x008FCFA0, CTCVillage::IsTimeToGoToSchool(CThingAICreature*, CWorld*, bool) @ 0x0065A8EE,
// CAIStateGroup_MinionSitOnSeatBase::IsRunnable @ 0x0090F9C0.
// Shape: one bool `&&` chain returned through the full register (`mov eax,1` / `xor eax,eax`): interface present,
// village exists, IsTimeToGoToSchool(creature, world, true), base IsRunnable. The villager pointer stays in edi
// across the chain, `this` in ebx; the creature is a named local loaded before the argument pushes (`push 1`
// first: right-to-left) so it rides in esi across the world call (three callee-saved registers).
// Layout local (the engine header flattens the base).
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
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
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

struct CTCVillager;
struct CTCVillage {
    bool IsTimeToGoToSchool(CThing* creature, CWorld* world, bool flag);   // 0x0065A8EE
};
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540

struct CAIStateGroupBaseW : CAIStateGroupBase {
    CWorld* GetPThingWorld();                   // 0x008FCFA0
};

struct CAIStateGroup_MinionSitOnSeatBase : CAIStateGroupBaseW {
    unsigned char _seat[0x28];                  // +0x14..+0x3b UsesSeat members + POwner
    virtual bool IsRunnable();                  // 0x0090F9C0
};

struct CAIStateGroup_MinionSitAtSchool : CAIStateGroup_MinionSitOnSeatBase {
    virtual bool IsRunnable();
    // the creature is a NAMED local taken before the argument pushes (it rides in esi across the world call)
    __forceinline bool IsTimeForSchool(CTCVillager* villager)
    {
        CThing* creature = PBrain->PCreature;
        return GFGetPVillageInterface(villager)->IsTimeToGoToSchool(creature, GetPThingWorld(), true);
    }
};

bool CAIStateGroup_MinionSitAtSchool::IsRunnable()
{
    CTCVillager* villager;
    return PBrain->PCreature->GetInterface(&villager, 0x23)
        && GFGetPVillageInterface(villager) != 0
        && IsTimeForSchool(villager)
        && CAIStateGroup_MinionSitOnSeatBase::IsRunnable();
}