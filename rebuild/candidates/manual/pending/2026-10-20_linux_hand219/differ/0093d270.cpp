// Grade: DIFFER(297v295) -- only &PEnemy: ours hoists lea esi,[ebx+0x14] above the temporary's GetPItem and moves it to ecx (retail: lea ecx,[ebx+0x14] after GetPItem); the tracker uses the ternary lookup
// CAIStateGroup_MeleeCombatNearbyBase::IsRunnable() -- latch the creature's current enemy (when alive) into PEnemy,
// and run while the group is still runnable and the navigation component can path from the creature to the enemy.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093D270, length 295 bytes (+ int3 padding). Vtable slot 11 of
// CAIStateGroup_MeleeCombatNearbyBase (0x0128B954).
// Callees: CVectorMap::LowerBound const @ 0x004365B0 (type 8, `test ch,1` on +0x20) and non-const @ 0x0040F020
// (type 7 = CTCCreatureNavigation, sign bit of byte +0x20), the type-8 component's virtual slot 29 (+0x74, GetEnemy),
// CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10, own virtual
// slot 14 (+0x38, IsStillRunnable(bool) const @ 0x0093D800), CThingCreatureBase::GetCombatManager @ 0x00661ED0,
// CThing::GetPosition @ 0x004C73D0 (x2), CTCCreatureNavigation::IsAbleToNavigateFromPosToPos @ 0x006CB620.
// Shape: both components are used unconditionally (the miss arms read the never-written key slot); the enemy is
// assigned through a CIntelligentPointer<CThingCreatureBase> temporary (vtable 0x0123E7D8); the creature that owns
// the navigation component is a named local reused for GetCombatManager, the start position re-reads the brain.
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
struct CTCEnemyTracker {                        // interface type 8
    V16(a) V4(b) V4(c)
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetEnemy();                 // slot 29 (+0x74)
};
struct CCombatManager {
    unsigned char _pad[0x28];
    float m_reach;                              // +0x28
};
inline CTCEnemyTracker* GetTracker(CThing* thing)
{
    CTCEnemyTracker* tracker;
    thing->GetInterfaceX(&tracker, 8);
    return tracker;
}
struct CTCCreatureNavigation {                  // interface type 7
    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float reach);  // 0x006CB620
};
struct CThingCreatureBase : CThing {
    CCombatManager* GetCombatManager();         // 0x00661ED0
    const C3DVector& GetPosition();             // 0x004C73D0
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>
    virtual void v00();
    CIntelligentPointerCreature(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CAIStateGroup_MeleeCombatNearbyBase : CAIStateGroupBase {
    CIntelligentPointerCreature PEnemy;         // +0x14
    bool Archer;                                // +0x1c
    virtual bool IsRunnable();
    CThingCreatureBase* GetPEnemy() const { return (CThingCreatureBase*)PEnemy.GetPItem(); }
};
struct CAIStateGroup_MeleeCombatNearbyBaseV {
    V4(a) V4(b) V4(c)
    virtual void v12(); virtual void v13();
    virtual bool IsStillRunnable(bool starting) const;   // slot 14 (+0x38)
};

bool CAIStateGroup_MeleeCombatNearbyBase::IsRunnable()
{
    CThing* enemy = GetTracker(PBrain->PCreature)->GetEnemy();
    if (enemy && !(enemy->m_flags91 & 1)) {
        PEnemy = CIntelligentPointerCreature(enemy);
        if (((CAIStateGroup_MeleeCombatNearbyBaseV*)this)->IsStillRunnable(false)) {
            CThingCreatureBase* creature = (CThingCreatureBase*)PBrain->PCreature;
            CTCCreatureNavigation* navigation;
            creature->GetInterfaceV(&navigation, 7);
            CCombatManager* manager = creature->GetCombatManager();
            CThingCreatureBase* target = GetPEnemy();
            CThingCreatureBase* self = (CThingCreatureBase*)PBrain->PCreature;
            if (navigation->IsAbleToNavigateFromPosToPos(self->GetPosition(), target->GetPosition(), manager->m_reach))
                return true;
        }
    }
    return false;
}
