// CAIStateGroup_ScorpionKing::Cleanup() -- leave the combat manager if the creature is a combatant; unless the
// creature is alive with at least 1.0 health, reset every live trap thing that has the trap interface (0x7D).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097B780, length 236 bytes (+ int3 padding).
// Callees: the creature's combatant test @ 0x00662410 (manifest "C3DAnimationBlendState::AddCombinationBlend" is
// propagated), CAIStateGroupBase::GetPWorld @ 0x008FCFC0 (+0x4c = CCombatManager), CCombatManager::DeregisterCombatant
// @ 0x006ED9C0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (IS_THING_ALIVE, then the thing), CVectorMap::LowerBound
// @ 0x0040F020 (inlined HasTC + entry lookup, type 0x7D, bit 29 of the dword at +0x2c), the trap component's reset
// @ 0x00787C07 (manifest "CXMVPlaybackInfo" is propagated); 1.0f @ 0x0122DED8.
// Shape: `for (unsigned long i = 0; i < TrapThings.size(); ++i)` over the vector<CIntelligentPointer<CThing>> at +0x1c
// (size re-read every pass, `sar 3`, unsigned `jb`), the element reloaded through the begin pointer each time.
// Engine header names (PArenaCentre +0x14, TrapThings +0x1c); the header flattens the base, so modelled locally.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
// Shape: the combat manager is reached by naming the GetPWorld() CALL result and chaining the member in the call expression
//   (`CWorld* world = GetPWorld(); world->PCombatManager->Deregister...`, retail `mov ecx,[eax+4Ch]`), and the trap vector's
//   operator[] is STLport's `*(begin() + i)` (retail `mov eax,[edi+1Ch] ; lea ecx,[eax+ebp]`; `First[i]` folds to add).
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

inline bool IS_THING_ALIVE(const CBaseIntelligentPointerCtor& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CCombatManager {
    void DeregisterCombatant(CThing* creature); // 0x006ED9C0
};
struct CWorld {
    unsigned char _pad[0x4c];
    CCombatManager* PCombatManager;             // +0x4c
};
struct CThingCombatant {
    unsigned char _pad[0xb4];
    float Health;                               // +0xb4
    bool IsCombatant();                         // 0x00662410
};
struct CTCTrap {                                // the interface-0x7D component
    void Reset();                               // 0x00787C07
};

struct CIntelligentPointerVector {              // std::vector<CIntelligentPointer<CThing>> (16 bytes)
    CBaseIntelligentPointerCtor* First;         // +0x00
    CBaseIntelligentPointerCtor* Last;          // +0x04
    CBaseIntelligentPointerCtor* End;           // +0x08
    unsigned long _alloc;                       // +0x0c
    unsigned long size() const { return Last - First; }
    CBaseIntelligentPointerCtor* begin() { return First; }
    CBaseIntelligentPointerCtor& operator[](unsigned long i) { return *(begin() + i); }
};

struct CAIStateGroup_ScorpionKing : CAIStateGroupBase {
    CIntelligentPointerThing PArenaCentre;      // +0x14
    CIntelligentPointerVector TrapThings;       // +0x1c
    CWorld* GetPWorld();                        // 0x008FCFC0 (CAIStateGroupBase)
    virtual void Cleanup();
};

void CAIStateGroup_ScorpionKing::Cleanup()
{
    if (((CThingCombatant*)PBrain->PCreature)->IsCombatant()) {
        CThing* combatant = PBrain->PCreature;
        CWorld* world = GetPWorld();
        world->PCombatManager->DeregisterCombatant(combatant);
    }
    CThing* creature = PBrain->PCreature;
    if (creature != 0 && !(creature->m_flags91 & 1) && !(((CThingCombatant*)creature)->Health < 1.0f))
        return;
    for (unsigned long i = 0; i < TrapThings.size(); ++i) {
        if (IS_THING_ALIVE(TrapThings[i])) {
            CThing* trap = TrapThings[i].GetPItem();
            if (trap->HasTC(0x7d))
                ((CTCTrap*)trap->FindEntry(0x7d)->component)->Reset();
        }
    }
}