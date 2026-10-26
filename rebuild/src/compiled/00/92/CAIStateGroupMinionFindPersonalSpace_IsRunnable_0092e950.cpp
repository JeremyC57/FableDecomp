// CAIStateGroup_MinionFindPersonalSpace::IsRunnable() -- when the creature's personal space is being invaded, search the rough box of the personal-space radius around it for the invader and latch the first one found
// Leaf: ?IsRunnable@CAIStateGroup_MinionFindPersonalSpace@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092E950, length 308 bytes (+ int3 padding).
// Callees: IsSpaceInvader(CThingCreatureBase*) @ 0x0092E890, the creature's physics-info accessor @ 0x006A4D00
// (+0x60 sub-object + 0xc; manifest "DoSizeof" is propagated), the static radius getter @ 0x00661CC0 (`fld [g] ; ret`,
// so the object expression is evaluated and ignored), CThing::GetPosition @ 0x004C73D0, __ftol2 @ 0x00BFEA70 (x3),
// CAIStateGroupBase::GetPWorld @ 0x008FCFC0, the world's thing-search tools (+0x20) GetThingsInRoughArea @ 0x00725F10,
// free @ 0x00BFEA14 (the vector block), CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor (vtable 0x0123E7D8).
// Shape: the box is built by a centre/radius C2DBoxI constructor from the truncated position and the truncated radius
// (x and y then become the far corner in place); the result vector is a POD {first, last, end} with a freeing
// destructor; the loop re-reads `last` each pass and returns from inside. The PCreature reads go through an inline
// GetCreature() accessor and the world pointer is a named local (GetPWorld runs before the argument pushes).
// GetThingsInRoughArea is written above as a same-TU stand-in (not a reconstruction of 0x00725F10): it takes the box
// and filter by address (they escape, so PCreature is re-read after the filter store) but reads and rewrites the
// result vector's three words by value, so `found` does not escape (its start stays in ebx across the loop, the zero
// stays in ebp). The filter, box and search sit in a nested block: VC7.1 packs escaped locals by lexical scope, so
// the latch temporary in the loop then shares the filter's frame slot as in retail.
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
class CThingCreatureBase;
struct CPersonalSpaceInfo {
    static float GetPersonalSpaceRadius();      // 0x00661CC0
};
class CThingCreatureBase : public CThing { public:
    CPersonalSpaceInfo* GetPersonalSpaceInfo(); // 0x006A4D00
    const C3DVector& GetPosition();             // 0x004C73D0
};
class C2DCoordI { public:
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
class C2DBoxI { public:
    long x0, y0, x1, y1;
    C2DBoxI(const C2DCoordI& centre, long radius) : x0(centre.X - radius), y0(centre.Y - radius), x1(centre.X + radius), y1(centre.Y + radius) { }
};
extern "C" void __cdecl free(void* block);      // 0x00BFEA14
struct CCreatureVector {                        // std::vector<CThingCreatureBase*> (STLport: first, last, end)
    CThingCreatureBase** m_pFirst;
    CThingCreatureBase** m_pLast;
    CThingCreatureBase** m_pEnd;
    CCreatureVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
    ~CCreatureVector() { if (m_pFirst) free(m_pFirst); }
};
class CThingFilter_IsNot { public:
    CThing* m_pThing;
    CThingFilter_IsNot(CThing* thing) : m_pThing(thing) { }
};
class CThingSearchTools { public:
    bool GetThingsInRoughArea(const C2DBoxI& box, const CThingFilter_IsNot& filter, CCreatureVector& out);  // 0x00725F10
};
class CWorld { public:
    unsigned char _pad[0x20];
    CThingSearchTools* m_pSearchTools;          // +0x20
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>
    virtual void v00();
    CIntelligentPointerCreature(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CAIStateGroup_MinionFindPersonalSpace : CAIStateGroupBase {
    CIntelligentPointerCreature PSpaceInvader;  // +0x14
    CWorld* GetPWorld();                        // 0x008FCFC0
    bool IsSpaceInvader(CThingCreatureBase* creature);  // 0x0092E890
    CThingCreatureBase* GetCreature() const { return (CThingCreatureBase*)PBrain->PCreature; }
    virtual bool IsRunnable();
};

bool SearchRoughArea_00725f10(CThingSearchTools* tools, const C2DBoxI* box, const CThingFilter_IsNot* filter, CThingCreatureBase** first, CThingCreatureBase** last, CThingCreatureBase** end);
CThingCreatureBase** SearchResultFirst_00725f10();
CThingCreatureBase** SearchResultLast_00725f10();
CThingCreatureBase** SearchResultEnd_00725f10();
__declspec(noinline) bool CThingSearchTools::GetThingsInRoughArea(const C2DBoxI& box, const CThingFilter_IsNot& filter, CCreatureVector& out)
{
    bool found = SearchRoughArea_00725f10(this, &box, &filter, out.m_pFirst, out.m_pLast, out.m_pEnd);
    out.m_pFirst = SearchResultFirst_00725f10();
    out.m_pLast = SearchResultLast_00725f10();
    out.m_pEnd = SearchResultEnd_00725f10();
    return found;
}

bool CAIStateGroup_MinionFindPersonalSpace::IsRunnable()
{
    if (IsSpaceInvader(GetCreature())) {
        float radius = GetCreature()->GetPersonalSpaceInfo()->GetPersonalSpaceRadius();
        CCreatureVector found;
        {
            CThingFilter_IsNot notSelf(GetCreature());
            const C3DVector& pos = GetCreature()->GetPosition();
            long x = (long)pos.x;
            long y = (long)pos.y;
            C2DBoxI box(C2DCoordI(x, y), (long)radius);
            CWorld* world = GetPWorld();
            world->m_pSearchTools->GetThingsInRoughArea(box, notSelf, found);
        }
        for (CThingCreatureBase** it = found.m_pFirst; it != found.m_pLast; ++it) {
            CThingCreatureBase* creature = *it;
            if (IsSpaceInvader(creature)) {
                PSpaceInvader = CIntelligentPointerCreature(creature);
                return true;
            }
        }
    }
    return false;
}