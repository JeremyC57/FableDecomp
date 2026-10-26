// CAIStateGroup_MinionReactToUnconsciousPerson::FindUndiscoveredUnconsciousPerson() -- once a second (staggered by the
// creature's UID), find the nearest not-yet-discovered unconscious ally the creature is aware of within a 7-unit box.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00937B10, length 261 bytes (+ int3 padding).
// Callees: GFGetCurrentFrame @ 0x0049D870, __allrem @ 0x00BFEC30 (signed 64-bit `(frame + UID) % fps`), CThing::
// GetPosition @ 0x004C73D0 (x2), __ftol2 @ 0x00BFEA70 (x2), CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000,
// CThingSearchTools::GetPBestThingInRoughArea<CFilter_Bind4<CThingFilter_IsAICreatureAwareOf, CThingFilter_IsUnconscious,
// CThingFilter_IsAnAlly, std::unary_negate<CThingFilter_HasBeenDiscovered>>, CThingCompare_Nearest, CThingCreatureBase>
// @ 0x00937420. Global: the frame-rate long
// @ 0x01375550 (read first).
// Shape: the landed filter-chain model (`a && b && c` binding pointers to frame temporaries; the empty filter is one
// byte); the result is zeroed up front and returned through its out slot.
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
    SEntry* LowerBound(const long* type);       // 0x0040F020 / 0x004365B0 (per-TU copies)
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
extern long g_framesPerSecond;                  // 0x01375550
class C3DVector { public: float x, y, z; };

struct CThingWithUID {                          // a thing seen through its 64-bit UID (+0x18)
    unsigned char _pad[0x18];
    __int64 UID;                                // +0x18 (read signed: the frame stagger uses __allrem)
};
class CThingCreatureBase;
struct CThingPos : CThing {
    const C3DVector& GetPosition();             // 0x004C73D0
};
class C2DBoxI { public:
    long x0, y0, x1, y1;
    C2DBoxI(long ax0, long ay0, long ax1, long ay1) : x0(ax0), y0(ay0), x1(ax1), y1(ay1) { }
};
class CThingFilter_IsAICreatureAwareOf { public:
    const CThing* m_pCreature;
    CThingFilter_IsAICreatureAwareOf(const CThing* creature) : m_pCreature(creature) { }
};
class CThingFilter_IsUnconscious { public:
    CThingFilter_IsUnconscious() { }
};
class CThingFilter_IsAnAlly { public:
    const CThing* m_pCreature;
    CThingFilter_IsAnAlly(const CThing* creature) : m_pCreature(creature) { }
};
class CThingFilter_HasBeenDiscovered { public:
    CThingFilter_HasBeenDiscovered() { }
};
template<class F> struct unary_negate_ {        // std::unary_negate<F>: holds the predicate (one empty byte)
    F m_pred;
    explicit unary_negate_(const F& f) : m_pred(f) { }
};
template<class F> inline unary_negate_<F> not1_(const F& f) { return unary_negate_<F>(f); }
template<class A, class B> class CFilter_Bind2 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template<class A, class B, class C> class CFilter_Bind3 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    CFilter_Bind3(const CFilter_Bind2<A, B>& ab, const C& c) : m_pFirst(ab.m_pFirst), m_pSecond(ab.m_pSecond), m_pThird(&c) { }
};
template<class A, class B, class C, class D> class CFilter_Bind4 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    const D* m_pFourth;
    CFilter_Bind4(const CFilter_Bind3<A, B, C>& abc, const D& d)
        : m_pFirst(abc.m_pFirst), m_pSecond(abc.m_pSecond), m_pThird(abc.m_pThird), m_pFourth(&d) { }
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }
template<class A, class B, class C, class D> inline CFilter_Bind4<A, B, C, D> operator&&(const CFilter_Bind3<A, B, C>& abc, const D& d) { return CFilter_Bind4<A, B, C, D>(abc, d); }
class CThingCompare_Nearest { public:
    C3DVector m_pos;
    CThingCompare_Nearest(const C3DVector& pos) : m_pos(pos) { }
};
typedef CFilter_Bind3<CThingFilter_IsAICreatureAwareOf, CThingFilter_IsUnconscious, CThingFilter_IsAnAlly> TUnconsciousFilter;
typedef CFilter_Bind4<CThingFilter_IsAICreatureAwareOf, CThingFilter_IsUnconscious, CThingFilter_IsAnAlly,
    unary_negate_<CThingFilter_HasBeenDiscovered> > TUndiscoveredFilter;
bool ThingInBox(const CThing* thing, const C2DBoxI& box);
bool IsAwareOf(const CThing* creature, const CThing* thing);
bool IsUnconscious(const CThing* thing);
bool IsAlly(const CThing* creature, const CThing* thing);
bool HasBeenDiscovered(const CThing* thing);
const C3DVector& PositionOf(const CThing* thing);
class CThingSearchTools { public:
    CThing** m_things;                          // candidate list (stand-in model)
    CThing** m_thingsEnd;
    bool GetPBestThingInRoughArea(const C2DBoxI& box, const TUnconsciousFilter& filter,
        const CThingCompare_Nearest& compare, CThingCreatureBase** out);    // 0x00937690
    bool GetPBestThingInRoughArea(const C2DBoxI& box, const TUndiscoveredFilter& filter,
        const CThingCompare_Nearest& compare, CThingCreatureBase** out);    // 0x00937420
};
struct CAIStateGroup_MinionReactToUnconsciousPerson : CAIStateGroupBase {
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    CThingCreatureBase* FindUnconsciousPerson();
    CThingCreatureBase* FindUndiscoveredUnconsciousPerson();
};
// GetPBestThingInRoughArea stand-in: the nearest candidate in the box passing every bound filter.
#pragma auto_inline(off)
bool CThingSearchTools::GetPBestThingInRoughArea(const C2DBoxI& box, const TUndiscoveredFilter& filter,
    const CThingCompare_Nearest& compare, CThingCreatureBase** out)
{
    CThing* best = 0;
    float bestDistance = 0.0f;
    for (CThing** it = m_things; it != m_thingsEnd; ++it) {
        CThing* thing = *it;
        if (!ThingInBox(thing, box) || !IsAwareOf(filter.m_pFirst->m_pCreature, thing) || !IsUnconscious(thing)
                || !IsAlly(filter.m_pThird->m_pCreature, thing) || HasBeenDiscovered(thing))
            continue;
        const C3DVector& pos = PositionOf(thing);
        float dx = pos.x - compare.m_pos.x;
        float dy = pos.y - compare.m_pos.y;
        float dz = pos.z - compare.m_pos.z;
        float distance = dx * dx + dy * dy + dz * dz;
        if (!best || distance < bestDistance) {
            best = thing;
            bestDistance = distance;
        }
    }
    *out = (CThingCreatureBase*)best;
    return best != 0;
}
#pragma auto_inline(on)

CThingCreatureBase* CAIStateGroup_MinionReactToUnconsciousPerson::FindUndiscoveredUnconsciousPerson()
{
    __int64 uid = ((CThingWithUID*)PBrain->PCreature)->UID;
    long framesPerSecond = g_framesPerSecond;
    CThingCreatureBase* found = 0;
    if ((GFGetCurrentFrame() + uid) % framesPerSecond == 0) {
        const C3DVector& pos = ((CThingPos*)PBrain->PCreature)->GetPosition();
        long x = (long)pos.x;
        long y = (long)pos.y;
        C2DBoxI box(x - 7, y - 7, x + 7, y + 7);
        PeekThingSearchTools()->GetPBestThingInRoughArea(box,
            CThingFilter_IsAICreatureAwareOf(PBrain->PCreature) && CThingFilter_IsUnconscious()
                && CThingFilter_IsAnAlly(PBrain->PCreature) && not1_(CThingFilter_HasBeenDiscovered()),
            CThingCompare_Nearest(((CThingPos*)PBrain->PCreature)->GetPosition()), &found);
    }
    return found;
}