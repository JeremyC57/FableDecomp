// CAIStateGroup_TrollBase::HasEnemyBeenKnockedDown() -- the (alive) enemy was knocked down since the last check when
// its entity events hold a newer CEventHitBy that this troll hit-and-knocked-down, or one caused by an explosion;
// remember that event's frame.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094D9C0, length 246 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PEnemy x3), CVectorMap::LowerBound const @ 0x004365B0
// (inlined GetInterface 0x42 = CTCEntityEvents, bit 2 of +0x28, unconditional), CTCEntityEvents::GetEvent<CEventHitBy,
// CFilter_Bind2<CIsEventCreatedAfter, CIsHitEventHitAndKnockedDownBy>> @ 0x0094E480 and <..., CIsHitEventHitByExplosion>
// @ 0x0094E530.
// Shape: the landed filter chain (`a && b` binds pointers to the two frame temporaries; the explosion filter is
// empty, one byte); each search has its own event local in its own block.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound (this TU's copy) -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;       // 0x0040F020 / 0x004365B0 (per-TU copies)
};

class CThing { public:
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

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

class CAIStateGroupBase { public:
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
class C3DVector { public: float x, y, z; };
class C2DVector { public:
    float x, y;
    float GetLength() const { return (float)sqrt(x * x + y * y); }
};
struct CWorldF {
    static long GetWorldFrame();                // 0x0049D870 (does not use `this`)
};
struct CTCEnemyTracker {                        // interface 8
    V16(a) V4(b) V4(c)
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThing* GetEnemy();                 // slot 29 (+0x74)
};
inline CTCEnemyTracker* GetEnemyTracker(CThing* thing)
{
    CTCEnemyTracker* tracker;
    thing->GetInterfaceX(&tracker, 8);
    return tracker;
}
class CThingPhysical : public CThing { public:
    const C3DVector& GetPhysicsPosition();      // 0x006A4D00 (the +0x60 physics sub-object's position)
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>
    virtual void v00();
    CIntelligentPointerCreature(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CAIStateGroup_TrollBase : CAIStateGroupBase {
    CIntelligentPointerCreature PEnemy;         // +0x14
    long EnemyKnockDownFrame;                   // +0x1c
    long ShotCounter;                           // +0x20
    float TROLL_MAX_THROWING_DISTANCE;          // +0x24
    float TROLL_MIN_THROWING_DISTANCE;          // +0x28
    float TROLL_STOP_POUNDING_DISTANCE;         // +0x2c
    float TROLL_STOP_THROWING_DISTANCE;         // +0x30
    CWorldF* GetPWorld();                       // 0x008FCFC0
    bool HasEnemyBeenKnockedDown();             // 0x0094D9C0
};
namespace NEntityEvents {
class CEventHitBy { public:
    void* __vftable;
    long CreationFrame;                         // +0x04
};
class CIsEventCreatedAfter { public:
    long Frame;
    CIsEventCreatedAfter(long frame) : Frame(frame) { }
};
class CIsHitEventHitAndKnockedDownBy { public:
    const CThing* m_pAttacker;
    CIsHitEventHitAndKnockedDownBy(const CThing* attacker) : m_pAttacker(attacker) { }
};
class CIsHitEventHitByExplosion { public:
    CIsHitEventHitByExplosion() { }
};
}
template<class A, class B> class CFilter_Bind2 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
class CTCEntityEvents { public:                        // interface 0x42
    bool GetEvent(NEntityEvents::CEventHitBy** out, const CFilter_Bind2<NEntityEvents::CIsEventCreatedAfter,
        NEntityEvents::CIsHitEventHitAndKnockedDownBy>& filter);           // 0x0094E480
    bool GetEvent(NEntityEvents::CEventHitBy** out, const CFilter_Bind2<NEntityEvents::CIsEventCreatedAfter,
        NEntityEvents::CIsHitEventHitByExplosion>& filter);                // 0x0094E530
};

bool CAIStateGroup_TrollBase::HasEnemyBeenKnockedDown()
{
    if (IS_THING_ALIVE(PEnemy)) {
        CTCEntityEvents* events;
        PEnemy.GetPItem()->GetInterfaceV(&events, 0x42);
        {
            NEntityEvents::CEventHitBy* hit;
            if (events->GetEvent(&hit, NEntityEvents::CIsEventCreatedAfter(EnemyKnockDownFrame)
                    && NEntityEvents::CIsHitEventHitAndKnockedDownBy(PBrain->PCreature))) {
                EnemyKnockDownFrame = hit->CreationFrame;
                return true;
            }
        }
        {
            NEntityEvents::CEventHitBy* hit;
            if (events->GetEvent(&hit, NEntityEvents::CIsEventCreatedAfter(EnemyKnockDownFrame)
                    && NEntityEvents::CIsHitEventHitByExplosion())) {
                EnemyKnockDownFrame = hit->CreationFrame;
                return true;
            }
        }
    }
    return false;
}