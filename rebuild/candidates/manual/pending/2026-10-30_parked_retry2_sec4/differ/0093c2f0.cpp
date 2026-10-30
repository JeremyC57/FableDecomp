// CAIStateGroup_StealthSeekEnemy::IsRunnable() -- seek the thing the creature perceives when it is alive, an enemy
// Grade: DIFFER(303v303) IMPROVED (48 -> 32 difflines): a same-TU noinline CInterfaceMap::LowerBound stand-in (GOTCHAS chunk 240 / 008328d0 idiom) lets the two lookup keys share one frame slot (sub esp,8 as retail). Left: scratch rotation eax/ecx/edx in the three brain loads, the two key pushes and the DetectedPos copy. Tried: filter declared at the top (=), GetCreature() accessor on top of the stand-in (302v303, 81 lines). Next: per-load accessor mix.
// the creature is willing to attack, actually heard or noticed, and accepted by the creature's interface-8 component;
// latch it as the enemy and remember the perceived position.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093C2F0, length 297 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound const @ 0x004365B0 (0x34 = CTCPerceiveThing bit 20 of +0x24; 8 = bit 8 of +0x20),
// the perceived-thing getter @ 0x007446A0 (manifest label propagated), AreThingsEnemies @ 0x0076D2F0 and
// IsWillingToAttackTarget @ 0x008648A0 (__fastcall), CTCPerceiveThing::IsAwareOfThing @ 0x00643B3B, the interface-8
// component's target test @ 0x007F6170 (manifest "CTCFollowed::IsAIFollower" is propagated), the out-of-line
// CIntelligentPointer<const CThing>(CThing*) @ 0x004A96F0 / converting operator= @ 0x004A9760 / ~CBaseIntelligentPointer
// @ 0x00A01C10.
// Shape: `PEnemy = target;` is an IMPLICIT conversion temporary (operator= gets `lea eax,[temp]`); DetectedPos is a
// dword-by-dword C3DVector copy through advanced pointers. Engine header names (flattened base, modelled locally).
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
// Same-TU stand-in for CVectorMap::LowerBound (not the retail body; GOTCHAS chunk 240 / 008328d0 idiom).
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) 
{
    CInterfaceMap::SEntry* it = first;
    long count = last - it;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) { it += half + 1; count -= half + 1; }
        else count = half;
    }
    return it;
}
#pragma auto_inline(on)

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
    unsigned char _pad[0x20];
    C3DVector m_perceivedPos;                   // +0x20
    unsigned char _pad2c[2];
    bool m_heardSound;                          // +0x2e
    CThing* GetPerceivedThing();                // 0x007446A0
    bool IsAwareOfThing();                      // 0x00643B3B
};
struct CTCTargetFilter {                        // interface 8
    bool IsValidTarget(CThing* target);         // 0x007F6170
};
bool __fastcall AreThingsEnemies(CThing* a, CThing* b);           // 0x0076D2F0
bool __fastcall IsWillingToAttackTarget(CThing* attacker, CThing* target);   // 0x008648A0
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {     // CIntelligentPointer<const CThing>
    virtual void v00();
    CIntelligentPointerConstThing(CThing* item);                        // 0x004A96F0 (out of line)
};
struct CIntelligentPointerEnemy : CBaseIntelligentPointer {          // CIntelligentPointer<CThing>
    virtual void v00();
    CIntelligentPointerEnemy& operator=(const CIntelligentPointerConstThing& other);   // 0x004A9760 (out of line)
};

struct CAIStateGroup_StealthSeekEnemy : CAIStateGroupBase {
    CIntelligentPointerEnemy PEnemy;            // +0x14
    C3DVector DetectedPos;                      // +0x1c
    virtual bool IsRunnable();
};

bool CAIStateGroup_StealthSeekEnemy::IsRunnable()
{
    CTCPerceiveThing* perceive;
    if (PBrain->PCreature->GetInterface(&perceive, 0x34)) {
        CThing* target = perceive->GetPerceivedThing();
        if (target && !(target->m_flags91 & 1) && AreThingsEnemies(PBrain->PCreature, target)
                && IsWillingToAttackTarget(PBrain->PCreature, target)
                && (perceive->m_heardSound || perceive->IsAwareOfThing())) {
            CTCTargetFilter* filter;
            if (PBrain->PCreature->GetInterface(&filter, 8) && filter->IsValidTarget(target)) {
                PEnemy = target;
                DetectedPos = perceive->m_perceivedPos;
                return true;
            }
        }
    }
    return false;
}
