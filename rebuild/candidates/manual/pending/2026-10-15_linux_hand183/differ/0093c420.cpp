// Grade: DIFFER(184v184) -- argument scheduling only (same as 0x0093BDF0): retail `mov ecx,[eax+4] ; mov eax,[edi+4] ; mov edx,[eax+18h]`, ours loads PBrain->PCreature through ecx/edx before the component | tried: FindEntry inline, GetInterface for 0x45 (fixed that call), named component local, named creature local
// CAIStateGroup_StealthSeekEnemy::Cleanup() -- reset the creature's interface-0x45 state (mode 8, full weight, 0xFF),
// and if the enemy is still alive with interface 0x36, tell that component the creature stopped seeking it.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093C420, length 180 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (two inlined lookups: type 0x45, bit 5 of the dword at +0x28; type 0x36,
// bit 22 of +0x24), the 0x45 component's setter @ 0x0071AAB0 (8, 0, 0, 1.0f, 0xFF), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 (IS_THING_ALIVE, then the thing), the 0x36 component's RemoveSeeker @ 0x00747B80.
// Shape: the 0x45 lookup is an out-parameter GetInterface (component to eax, `mov ecx,eax` after the pushes); the 0x36
// one is HasTC + FindEntry into a named local (ecx loaded before the argument loads); IS_THING_ALIVE
// inlined. The engine header flattens the base, so the class is modelled locally; PEnemy is +0x14.
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

inline bool IS_THING_ALIVE(const CBaseIntelligentPointerCtor& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CTCStealthState {                        // the interface-0x45 component
    void SetState(long mode, long a, long b, float weight, long alpha);   // 0x0071AAB0
};
struct CTCSeekTarget {                          // the interface-0x36 component
    void RemoveSeeker(CThing* seeker, long flags);  // 0x00747B80
};

struct CAIStateGroup_StealthSeekEnemy : CAIStateGroupBase {
    CIntelligentPointerThing PEnemy;            // +0x14
    virtual void Cleanup();
};

void CAIStateGroup_StealthSeekEnemy::Cleanup()
{
    CTCStealthState* stealth;
    if (PBrain->PCreature->GetInterface(&stealth, 0x45))
        stealth->SetState(8, 0, 0, 1.0f, 0xff);
    if (IS_THING_ALIVE(PEnemy)) {
        CThing* enemy = PEnemy.GetPItem();
        if (enemy->HasTC(0x36)) {
            CTCSeekTarget* seek = (CTCSeekTarget*)enemy->FindEntry(0x36)->component;
            seek->RemoveSeeker(PBrain->PCreature, 0);
        }
    }
}
