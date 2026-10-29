// CAIStateGroup_StealStealableItem::FindPEscapePoint(bool nearest) -- pick an escape point thing: the nearest one to
// the thief, or a random one of all of them.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00964CA0, length 245 bytes (+ int3 padding), `ret 4`.
// Callees: CThing::GetPosition @ 0x004C73D0, CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000 (x2),
// CThingSearchTools::GetPBestThing<filter, CThingCompare_Nearest, CThing> @ 0x009642D0 and GetAllLeafThings<CThing,
// vector, filter> @ 0x00964EF0 (both manifest labels are propagated from other instantiations),
// CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890, free @ 0x00BFEA14 (the vector block).
// Shape: the one-word filter object is homed in the dead `nearest` argument slot (`lea eax,[esp+24h]`); the result
// is zero-initialised up front (edi, also the best-thing out slot); the random index is the landed
// `range ? seed % range : 0` step over the vector size.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound (this TU's copy) -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020 / 0x004365B0 (per-TU copies)
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class C3DVector;
class CThing { public:
    const C3DVector& GetPos() const;    // 0x004C73D0
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
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->first <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
    // the bitset::test form: the test's bool is materialised (`shr ; and dl,1 ; mov [esp+x],dl`) and returned
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
        }
        return has;
    }
    // the ternary form (landed 0x00954690): the end/hit choice is ONE ternary, so the component lands straight in
    // the register of its use (ecx for a call on it); the miss path reads the never-written slot
    template<class T> __forceinline bool GetInterfaceX(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
    // the KidCleanup form (landed 0x00915190): `if (mask) *pp = Lookup();` with an entry-value lookup
    __forceinline CTCBase* LookupValue(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap::SEntry* p = m_interfaces.LowerBound(type);
        if (p == m_interfaces.last) p = m_interfaces.last;
        else if (p->first > interfaceType) p = m_interfaces.last;
        return p->second;
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
            component = (T*)FindEntry(interfaceType)->second;
        return component;
    }
    // the out-parameter form whose result is ignored (no bool to keep: the component lands in its use register)
    template<class T> __forceinline void GetInterfaceV(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
        }
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
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
long __fastcall GFROR13(unsigned long);   // 0x00497890

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
    protected:
    unsigned long* GetPWorldSeed() const;             // 0x008FCFD0
    public:
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
struct CThingPos : CThing {
};
extern "C" void __cdecl free(void* block);      // 0x00BFEA14
struct CThingVector {                           // std::vector<CThing*> (STLport: first, last, end)
    CThing** m_pFirst;
    CThing** m_pLast;
    CThing** m_pEnd;
    CThingVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    ~CThingVector() { if (m_pFirst) free(m_pFirst); }
    bool empty() const { return m_pFirst == m_pLast; }
    unsigned long size() const { return m_pLast - m_pFirst; }
};
struct CThingFilter_EscapePoint {               // a one-word thing-type filter (type 1)
    long m_type;
    CThingFilter_EscapePoint(long type) : m_type(type) { }
};
class CThingCompare_Nearest { public:
    C3DVector m_pos;
    CThingCompare_Nearest(const C3DVector& pos) : m_pos(pos) { }
};
long GetThingType(const CThing* thing);        // stand-in model: the thing's type id
class CThingSearchTools { public:
    CThing** m_things;                          // candidate list (stand-in model)
    CThing** m_thingsEnd;
    bool GetPBestThing(const CThingFilter_EscapePoint& filter, const CThingCompare_Nearest& compare, CThing** out);  // 0x009642D0
    void GetAllLeafThings(const CThingFilter_EscapePoint& filter, CThingVector& out);   // 0x00964EF0
};

struct CAIStateGroup_StealStealableItem : CAIStateGroupBase {
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    CThing* FindPEscapePoint(bool nearest);
};

void LeafThings_00964ef0(CThingSearchTools* tools, const CThingFilter_EscapePoint* filter, CThing** first, CThing** last, CThing** end);
CThing** LeafResultFirst_00964ef0();
CThing** LeafResultLast_00964ef0();
CThing** LeafResultEnd_00964ef0();
__declspec(noinline) void CThingSearchTools::GetAllLeafThings(const CThingFilter_EscapePoint& filter, CThingVector& out)
{
    LeafThings_00964ef0(this, &filter, out.m_pFirst, out.m_pLast, out.m_pEnd);
    out.m_pFirst = LeafResultFirst_00964ef0();
    out.m_pLast = LeafResultLast_00964ef0();
    out.m_pEnd = LeafResultEnd_00964ef0();
}

// GetPBestThing stand-in: the nearest candidate of the filter's thing type.
#pragma auto_inline(off)
bool CThingSearchTools::GetPBestThing(const CThingFilter_EscapePoint& filter, const CThingCompare_Nearest& compare, CThing** out)
{
    CThing* best = 0;
    float bestDistance = 0.0f;
    for (CThing** it = m_things; it != m_thingsEnd; ++it) {
        CThing* thing = *it;
        if (GetThingType(thing) != filter.m_type)
            continue;
        const C3DVector& pos = ((CThingPos*)thing)->GetPos();
        float dx = pos.x - compare.m_pos.x;
        float dy = pos.y - compare.m_pos.y;
        float dz = pos.z - compare.m_pos.z;
        float distance = dx * dx + dy * dy + dz * dz;
        if (!best || distance < bestDistance) {
            best = thing;
            bestDistance = distance;
        }
    }
    *out = best;
    return best != 0;
}
#pragma auto_inline(on)

CThing* CAIStateGroup_StealStealableItem::FindPEscapePoint(bool nearest)
{
    CThing* result = 0;
    if (nearest) {
        PeekThingSearchTools()->GetPBestThing(CThingFilter_EscapePoint(1),
            CThingCompare_Nearest(((CThingPos*)PBrain->PCreature)->GetPos()), &result);
        return result;
    } else {
    CThingVector points;
    PeekThingSearchTools()->GetAllLeafThings(CThingFilter_EscapePoint(1), points);
    if (!points.empty()) {
        unsigned long count = points.size();
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        unsigned long index = count ? rotated % count : 0;
        result = points.m_pFirst[index];
    }
    return result;
    }
}