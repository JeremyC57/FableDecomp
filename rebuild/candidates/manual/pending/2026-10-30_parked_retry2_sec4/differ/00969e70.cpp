// CAIStateGroup_SleepOnFloor::IsRunnable() -- gather the sleeping spots in the 20-unit box around the creature (not
// Grade: DIFFER(324v311) nd=100 (was 124) -- lever: the creature read into a named local before the vector (its load precedes the zero stores like retail); tried: vector after the position (124), an STLport alloc-proxy end-of-storage (124); remaining: retail zeroes the vector through a zero register (push edi ; xor edi,edi, reused by the dtor's null test) where ours stores immediates, and loads both position components before the box arithmetic (fld x ; fld y ; fld st(1))
// the creature itself) and latch a random one; runnable when there was one.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00969E70, length 311 bytes (+ int3 padding).
// Callees: CThing::GetPosition @ 0x004C73D0, the float box's integer-box conversion @ 0x00A55A30 (C2DBoxI by hidden
// pointer; manifest "CEditWorldMap::GetPenArea" is propagated), CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000,
// CThingSearchTools::GetAllThingsInRoughArea<CThing, vector, CFilter_Bind2<CThingFilter_IsNot, ...>> @ 0x00969FB0,
// CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890, CBaseIntelligentPointer ctor/SetPItem/GetPItem/
// dtor (vtable 0x01238C6C), free @ 0x00BFEA14. Constant 20.0f @ 0x0129669C.
// Shape: the float box is built from the position kept on the FPU; the filter chain binds pointers to the IsNot
// filter and the empty filter byte; the random index is the landed `range ? seed % range : 0` step.
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
    // a NON-template lookup core shared by every interface type (its key local packs into one frame slot across
    // several inlined lookups); the typed wrappers only cast
    __forceinline bool GetInterfaceCore(CTCBase** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = ((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterfaceN(T** ppOut, long interfaceType)
    {
        return GetInterfaceCore((CTCBase**)ppOut, interfaceType);
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
struct CThingPos : CThing {
    const C3DVector& GetPosition();             // 0x004C73D0
};
struct C2DBoxI { long x0, y0, x1, y1; };
struct C2DBox {
    float x0, y0, x1, y1;
    C2DBox(float ax0, float ay0, float ax1, float ay1) : x0(ax0), y0(ay0), x1(ax1), y1(ay1) { }
    C2DBoxI GetIntBox() const;                  // 0x00A55A30
};
extern "C" void __cdecl free(void* block);      // 0x00BFEA14
struct CThingVector {                           // std::vector<CThing*> (STLport: first, last, end)
    CThing** m_pFirst;
    CThing** m_pLast;
    CThing** m_pEnd;
    CThingVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    ~CThingVector() { if (m_pFirst) free(m_pFirst); }
};
struct CThingFilter_IsNot {
    const CThing* m_pThing;
    CThingFilter_IsNot(const CThing* thing) : m_pThing(thing) { }
};
struct CThingFilter_IsSleepingSpot {
    CThingFilter_IsSleepingSpot() { }
};
template<class A, class B> struct CFilter_Bind2 {
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
struct CThingSearchTools {
    void GetAllThingsInRoughArea(const C2DBoxI& box, const CFilter_Bind2<CThingFilter_IsNot, CThingFilter_IsSleepingSpot>& filter,
        CThingVector& out);                     // 0x00969FB0
};

struct CAIStateGroup_SleepOnFloor : CAIStateGroupBase {
    CIntelligentPointerThing PSleepSpot;        // +0x14
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    virtual bool IsRunnable();
};

bool CAIStateGroup_SleepOnFloor::IsRunnable()
{
    CThingPos* creature = (CThingPos*)PBrain->PCreature;
    CThingVector spots;
    const C3DVector& pos = creature->GetPosition();
    PeekThingSearchTools()->GetAllThingsInRoughArea(
        C2DBox(pos.x - 20.0f, pos.y - 20.0f, pos.x + 20.0f, pos.y + 20.0f).GetIntBox(),
        CThingFilter_IsNot(PBrain->PCreature) && CThingFilter_IsSleepingSpot(), spots);
    if (spots.m_pFirst == spots.m_pLast)
        return false;
    unsigned long count = spots.m_pLast - spots.m_pFirst;
    unsigned long* seed = GetPWorldSeed();
    unsigned long next = *seed * 0x24A1U + 0x24DFU;
    *seed = next;
    unsigned long rotated = GFROR13(next);
    *seed = rotated;
    unsigned long index = count ? rotated % count : 0;
    PSleepSpot = CIntelligentPointerThing(spots.m_pFirst[index]);
    return true;
}
