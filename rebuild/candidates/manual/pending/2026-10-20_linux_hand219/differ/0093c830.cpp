// Grade: DIFFER(322v318) -- frame only: the two inlined lookup keys get separate slots (sub esp,8) where retail shares one (push ecx; the 0x36 key at [esp+0x10] after push ebp)
// CAIStateGroup_StealthSeekEnemy::Update() -- while the enemy is alive and something was perceived (a sound flag or a
// non-zero awareness), finish the current action if allowed, then, while still runnable, move to the perceived
// position, put the enemy's stealth component in danger and play the what's-that reactions; then terminate.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093C830, length 318 bytes.
// Callees: CVectorMap::LowerBound const @ 0x004365B0 (inlined GetInterface 0x34 = CTCPerceiveThing, bit 20 of +0x24,
// result pre-zeroed) and non-const @ 0x0040F020 (0x36 = CTCStealth on the enemy, bit 22 of +0x24), CBaseIntelligentPointer
// ::GetPItem @ 0x00A01B50 (PEnemy x3), CThingCreatureBase::IsFreeToFinishAction @ 0x006621C0 / FinishCurrentAction
// @ 0x00662650, IsStillRunnable(CTCPerceiveThing*) @ 0x0093C4E0, CTCStealth::SetAsInDanger(CThing*, EDangerStatus)
// @ 0x00747B80, SetWhatsThatSound @ 0x0093C600, MoveOrQuit @ 0x0093C7B0, SetWhatsThatAnimation @ 0x0093C6D0,
// Terminate @ 0x008FCF40. Constant 0.0f @ 0x0122DEDC.
// Shape: a rotated `while (IsStillRunnable(p))` loop with &perceive->Position hoisted (ebp); DetectedPos copied
// dword by dword (the memberwise C3DVector assignment). Engine header names (flattened base, modelled locally).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound (this TU's copy) -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020 / 0x004365B0 (per-TU copies)
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x60 - 0x4c];
    void* m_pSub60;                             // +0x60 (typed per use below)
    unsigned char _pad2[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char m_flags92;                    // +0x92
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    bool TestTC(long interfaceType) const       // std::bitset::test: `shr ; and cl,1` (kept as a byte when reused)
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
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
    // the bitset::test form: the test's bool is materialised (`shr ; and dl,1 ; mov [esp+x],dl`) and returned
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
        return has;
    }
    // the ternary form (landed 0x00954690): the end/hit choice is ONE ternary, so the component lands straight in
    // the register of its use (ecx for a call on it); the miss path reads the never-written slot
    template<class T> __forceinline bool GetInterfaceX(T** ppOut, long interfaceType)
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
    // the KidCleanup form (landed 0x00915190): `if (mask) *pp = Lookup();` with an entry-value lookup
    __forceinline CTCBase* LookupValue(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap::SEntry* p = m_interfaces.LowerBound(&type);
        if (p == m_interfaces.last) p = m_interfaces.last;
        else if (p->type > interfaceType) p = m_interfaces.last;
        return p->component;
    }
    template<class T> __forceinline void GetInterfaceK(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            *ppOut = (T*)LookupValue(interfaceType);
    }
    // GetInterface<T>() returning the component: hit = the entry's component, miss = the never-written local
    template<class T> __forceinline T* PeekInterface(long interfaceType)
    {
        T* component;
        if (HasTC(interfaceType))
            component = (T*)FindEntry(interfaceType)->component;
        return component;
    }
    // the out-parameter form whose result is ignored (no bool to keep: the component lands in its use register)
    template<class T> __forceinline void GetInterfaceV(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
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

long __fastcall GFGetCurrentFrame();            // 0x0049D870
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

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
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    // the landed GetRandomObject random step: seed = ror13(seed*0x24A1 + 0x24DF), r = seed % range
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
};

// vtable padding helpers: V4/V16 declare 4/16 placeholder virtual slots
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
struct C3DVector { float x, y, z; };
struct CTCPerceiveThing {                       // interface 0x34
    unsigned char _pad[0x18];
    float m_awareness;                          // +0x18
    unsigned char _pad1c[0x20 - 0x1c];
    C3DVector m_perceivedPos;                   // +0x20
    unsigned char _pad2c[2];
    bool m_heardSound;                          // +0x2e
};
struct CTCStealth {                             // interface 0x36
    void SetAsInDanger(CThing* source, long status);   // 0x00747B80
};
struct CThingCreatureBase : CThing {
    bool IsFreeToFinishAction();                // 0x006621C0
    void FinishCurrentAction();                 // 0x00662650
};

struct CAIStateGroup_StealthSeekEnemy : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14 CIntelligentPointer<CThing>
    C3DVector DetectedPos;                      // +0x1c
    CThingCreatureBase* GetCreature() const { return (CThingCreatureBase*)PBrain->PCreature; }
    bool IsStillRunnable(CTCPerceiveThing* perceive);       // 0x0093C4E0
    void SetWhatsThatSound();                   // 0x0093C600
    void MoveOrQuit(CTCPerceiveThing* perceive);            // 0x0093C7B0
    void SetWhatsThatAnimation(CTCPerceiveThing* perceive); // 0x0093C6D0
    void Terminate();                           // 0x008FCF40
    virtual void Update();
};

void CAIStateGroup_StealthSeekEnemy::Update()
{
    CTCPerceiveThing* perceive = 0;
    PBrain->PCreature->GetInterface(&perceive, 0x34);
    if (IS_THING_ALIVE(PEnemy) && (perceive->m_heardSound || perceive->m_awareness != 0.0f)) {
        if (GetCreature()->IsFreeToFinishAction())
            GetCreature()->FinishCurrentAction();
        while (IsStillRunnable(perceive)) {
            DetectedPos = perceive->m_perceivedPos;
            CTCStealth* stealth;
            if (PEnemy.GetPItem()->GetInterfaceX(&stealth, 0x36))
                stealth->SetAsInDanger(PBrain->PCreature, 1);
            SetWhatsThatSound();
            MoveOrQuit(perceive);
            SetWhatsThatAnimation(perceive);
        }
    }
    Terminate();
}
