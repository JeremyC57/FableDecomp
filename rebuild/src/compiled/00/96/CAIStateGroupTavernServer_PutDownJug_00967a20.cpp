// CAIStateGroup_TavernServer::PutDownJug() -- put the empty jug down on the bar (PUT_EMPTY_JUG_ON_BAR, mode 0x29)
// and free the bar point; on success clear the jug's holder component, forget the jug and the serving point.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00967A20, length 288 bytes.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CCreatureAction_PutDownThingAtTable
// (creature, const CCharString&, mode) @ 0x00842A20, AIAction_PerformActionAndWait @ 0x0097DB70, ~CCreatureActionBase
// @ 0x00693EF0, CAIStateGroup_TavernKeeper::FreeBarPoint @ 0x00967110, CBaseIntelligentPointer GetPItem/ctor/SetPItem/
// dtor (PJug; null temporary vtable 0x0125A858), CVectorMap::LowerBound @ 0x0040F020 (0xBB = bit 27 of +0x34).
// Shape: CCreatureAction_PutDownJug (vtable 0x012964DC) is a derived action whose inline constructor builds the
// animation-name CCharString as an IMPLICIT conversion temporary inside its base mem-initializer (built before
// any argument push) (so the string dies BEFORE the derived vtable store);
// the action is a named local in its own block with the `!= 2` result in bl across its destructor and FreeBarPoint.
// Engine header names (rebuild/include/engine/CAIStateGroup_TavernServer.h; flattened base, modelled locally).
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
class CThingCreatureBase;
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];                      // +0x04..+0xa7
    virtual ~CCreatureActionBase();                         // 0x00693EF0
};
class CCreatureAction_PutDownThingAtTable : public CCreatureActionBase { public:
    long m_thing;                                   // +0xa8
    CCreatureAction_PutDownThingAtTable(CThingCreatureBase* owner, const CCharString& animation, long mode);   // 0x00842A20
};
class CCreatureAction_PutDownJug : public CCreatureAction_PutDownThingAtTable { public:   // vtable 0x012964DC
    virtual void v00();
    CCreatureAction_PutDownJug(CThingCreatureBase* owner)
        : CCreatureAction_PutDownThingAtTable(owner, "PUT_EMPTY_JUG_ON_BAR", 0x29) { }
};
struct CTCJugHolder {                               // interface 0xBB on the jug
    unsigned char _pad[0x0c];
    CThing* m_pHolder;                              // +0x0c
};
struct CIntelligentPointerObject : CBaseIntelligentPointer {   // CIntelligentPointer<CThingObject> (vtable 0x0125A858)
    virtual void v00();
    CIntelligentPointerObject(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerObject& operator=(const CIntelligentPointerObject& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CAIStateGroup_TavernServer : CAIStateGroupBase {
    CIntelligentPointerObject PJug;                 // +0x14
    CBaseIntelligentPointer PBar;                   // +0x1c
    CBaseIntelligentPointer PTable;                 // +0x24
    CBaseIntelligentPointer PTankard;               // +0x2c
    CBaseIntelligentPointer PPub;                   // +0x34
    long JugIndex;                                  // +0x3c
    long ServingActionPointIndex;                   // +0x40
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool interruptible);   // 0x0097DB70
    void FreeBarPoint();                            // 0x00967110
    bool PutDownJug();
};

bool CAIStateGroup_TavernServer::PutDownJug()
{
    CThingCreatureBase* creature = (CThingCreatureBase*)PBrain->PCreature;
    bool failed;
    {
        CCreatureAction_PutDownJug action(creature);
        failed = AIAction_PerformActionAndWait(&action, false) != 2;
    }
    FreeBarPoint();
    if (failed)
        return false;
    if (IS_THING_ALIVE(PJug)) {
        CThing* jug = PJug.GetPItem();
        if (jug->HasTC(0xbb)) {
            CTCJugHolder* holder;
            jug->GetInterfaceV(&holder, 0xbb);
            holder->m_pHolder = 0;
        }
    }
    PJug = CIntelligentPointerObject(0);
    ServingActionPointIndex = -1;
    return true;
}