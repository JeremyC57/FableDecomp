// CAIStateGroup_MinionWander::FaceAwayFromBuildings() -- find the nearest building leaf thing within 10 units; when
// Grade: DIFFER(289v287) nd=68 (was 317v287 nd=84) -- C3DVector user copy ctor (the away+position sum is built straight into the by-value argument slot); residue: frame 0x28 v 0x24 (retail spills the difference into a gap slot + the dead compare/filter slots), building kept in edi
// it is alive and in the creature's field of view, turn to look directly away from it. Returns the look action's
// result, or 2 (done) when there is nothing to face away from.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092BF50, length 287 bytes (+ int3 padding).
// Callees: the physics-position accessor @ 0x006A4D00 (x5), CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000,
// CThingSearchTools::GetPBestLeafThing<within-radius filter, CThingCompare_Nearest, CThingBuilding> @ 0x0092C260
// (manifest label propagated from another instantiation), CThingCreatureBase::IsThingInFOV(CThing*, bool)
// @ 0x00665190, AIAction_LookAtPos @ 0x0097D580 (C3DVector by value built in its slot).
// Shape: the away vector `(creature - building) + creature` is one argument expression: the difference stays on the
// FPU and is spilled (z first) across the third position call, then the sum is stored into the argument slot.
// The filter is {position, radius} and the compare {position}, both by-value copies built right to left.
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
struct C3DVector {
    float x, y, z;
    C3DVector() { }
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) { }
    C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(x + o.x, y + o.y, z + o.z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(x - o.x, y - o.y, z - o.z); }
};
struct CThingPhysical : CThing {
    const C3DVector& GetPhysicsPosition();      // 0x006A4D00
};
struct CThingCreatureFOV : CThing {
    bool IsThingInFOV(CThing* thing, bool full);    // 0x00665190
};
struct CThingFilter_WithinRadius {
    C3DVector m_pos;
    float m_radius;
    CThingFilter_WithinRadius(const C3DVector& pos, float radius) : m_pos(pos), m_radius(radius) { }
};
struct CThingCompare_Nearest {
    C3DVector m_pos;
    CThingCompare_Nearest(const C3DVector& pos) : m_pos(pos) { }
};
struct CThingBuilding : CThingPhysical { };
struct CThingSearchTools {
    bool GetPBestLeafThing(const CThingFilter_WithinRadius& filter, const CThingCompare_Nearest& compare,
        CThingBuilding** out);                  // 0x0092C260
};

struct CAIStateGroup_MinionWander : CAIStateGroupBase {
    CThingPhysical* GetCreature() const { return (CThingPhysical*)PBrain->PCreature; }
    CThingSearchTools* PeekThingSearchTools();  // 0x008FD000
    long AIAction_LookAtPos(long repeat, C3DVector pos, long frames, float speed);   // 0x0097D580
    long FaceAwayFromBuildings();
};

long CAIStateGroup_MinionWander::FaceAwayFromBuildings()
{
    CThingBuilding* building = 0;
    PeekThingSearchTools()->GetPBestLeafThing(CThingFilter_WithinRadius(GetCreature()->GetPhysicsPosition(), 10.0f),
        CThingCompare_Nearest(GetCreature()->GetPhysicsPosition()), &building);
    if (building && !(building->m_flags91 & 1) && ((CThingCreatureFOV*)PBrain->PCreature)->IsThingInFOV(building, true)) {
        CThingPhysical* creature = GetCreature();
        const C3DVector& buildingPos = building->GetPhysicsPosition();
        const C3DVector& creaturePos = creature->GetPhysicsPosition();
        return AIAction_LookAtPos(1, (creaturePos - buildingPos) + GetCreature()->GetPhysicsPosition(), 0x11, 0.0027777f);
    }
    return 2;
}
