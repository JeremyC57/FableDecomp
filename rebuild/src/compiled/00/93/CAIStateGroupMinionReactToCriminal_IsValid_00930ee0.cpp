// CAIStateGroup_MinionReactToCriminal::IsValid() -- the reaction stays valid while the hero is alive, the creature is a villager with an opinion of the hero, its village exists, and the hero is hostile (attitude 4) or wanted (attitude 2) for a crime the village already knows about or this creature saw (a run-and-find-a-guard deed).
// Grade: RELOCATION_MATCH -- same-TU noinline LowerBound body (0x0040F020) plus both interface lookups spelled on the `PBrain->PCreature` chain (no named creature local: a named one CSEs the map address and keeps it live across the first lookup)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00930EE0, length 297 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PHero x2), CVectorMap::LowerBound const @ 0x004365B0
// (0x23 = CTCVillager bit 3 of +0x24, 0x4F = CTCCreatureOpinionOfHero bit 15 of +0x28; both after their own bit
// tests), GFGetPVillageInterface @ 0x007EE540, CTCCreatureOpinionOfHero::FindMostRecentDeed<COpinionDeedFilter_
// IsCrimeToRunAndFindAGuard>(filter, long*, bool) @ 0x006610C7, CAIStateGroupBase::GetPThingWorld @ 0x008FCFA0
// (manifest "CPlayer::GetCurrentMode" is propagated), COpinionReactionManager::GetThingAttitudeToPlayer @ 0x00701840.
// Shape: nested tests with one shared `return false` (`xor al,al`); the final `return a || (b && (c || d))` widens to eax; the
// deed index is pre-set to -1 and the empty filter is one frame byte.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
// Name fix (agentNF7 2026-11-10): the local CInterfaceMap stand-in replaced by the shared CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<...> > (engine/CVectorMap.h), so LowerBound binds the PDB spelling (0x0040F020 / const 0x004365B0).
#include "fable_compiler.h"
class CTCBase;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound (this TU's copy) -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;   // LowerBound 0x0040F020 / const 0x004365B0
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
// (the CVectorMap LowerBound bodies are the shared header's same-TU noinline templates)
#pragma auto_inline(on)


class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x60 - 0x54];
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
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = static_cast<const CInterfaceMap*>(map)->LowerBound(reinterpret_cast<const ETCInterfaceType&>(type));
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    FABLE_FORCEINLINE const CInterfaceMap::value_type* FindEntry(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = static_cast<const CInterfaceMap*>(map)->LowerBound(reinterpret_cast<const ETCInterfaceType&>(type));
        const CInterfaceMap::value_type* sel;
        if (it == map->end()) sel = map->end();
        else if (it->first <= interfaceType) sel = it;
        else sel = map->end();
        return sel;
    }
    // the bitset::test form: the test's bool is materialised (`shr ; and dl,1 ; mov [esp+x],dl`) and returned
    template<class T> FABLE_FORCEINLINE bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = static_cast<const CInterfaceMap*>(map)->LowerBound(reinterpret_cast<const ETCInterfaceType&>(type));
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
        }
        return has;
    }
    // the ternary form (landed 0x00954690): the end/hit choice is ONE ternary, so the component lands straight in
    // the register of its use (ecx for a call on it); the miss path reads the never-written slot
    template<class T> FABLE_FORCEINLINE bool GetInterfaceX(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = static_cast<const CInterfaceMap*>(map)->LowerBound(reinterpret_cast<const ETCInterfaceType&>(type));
            *ppOut = (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
            return true;
        }
        return false;
    }
    // the KidCleanup form (landed 0x00915190): `if (mask) *pp = Lookup();` with an entry-value lookup
    FABLE_FORCEINLINE CTCBase* LookupValue(long interfaceType)
    {
        long type = interfaceType;
        const CInterfaceMap::value_type* p = static_cast<const CInterfaceMap&>(m_interfaces).LowerBound(reinterpret_cast<const ETCInterfaceType&>(type));
        if (p == m_interfaces.end()) p = m_interfaces.end();
        else if (p->first > interfaceType) p = m_interfaces.end();
        return p->second;
    }
    template<class T> FABLE_FORCEINLINE void GetInterfaceK(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            *ppOut = (T*)LookupValue(interfaceType);
    }
    // GetInterface<T>() returning the component: hit = the entry's component, miss = the never-written local
    template<class T> FABLE_FORCEINLINE T* PeekInterface(long interfaceType)
    {
        T* component;
        if (HasTC(interfaceType))
            component = (T*)FindEntry(interfaceType)->second;
        return component;
    }
    // the out-parameter form whose result is ignored (no bool to keep: the component lands in its use register)
    template<class T> FABLE_FORCEINLINE void GetInterfaceV(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::value_type* it = static_cast<const CInterfaceMap*>(map)->LowerBound(reinterpret_cast<const ETCInterfaceType&>(type));
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
        }
    }
};

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(((CThing*)other.GetPItem())); return *this; }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!(((CThing*)p.GetPItem())->m_flags91 & 1))
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
protected:
    const CWorld& PeekWorld() const;    // 0x008FCFA0
public:
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
struct CTCVillager;
class CTCVillage { public:
    unsigned char _pad[0x7c];
    bool m_crimeReported;                       // +0x7c (descriptive)
    unsigned char _pad7d[0x88 - 0x7d];
    void* m_pGuardPost;                         // +0x88 (descriptive)
};
class CTCVillageMember { public: CTCVillage* GetPVillage() const; };   // 0x007EE540 (Ego_r.pdb CTCVillageMember::GetPVillage, const)
class COpinionDeedFilter_IsCrimeToRunAndFindAGuard { public:
    COpinionDeedFilter_IsCrimeToRunAndFindAGuard() { }
};
class COpinionDeed;
class CTCCreatureOpinionOfHero { public:               // interface 0x4F
    COpinionDeed* FindMostRecentDeed(const COpinionDeedFilter_IsCrimeToRunAndFindAGuard& filter, long* index,
        bool includeForgotten);                 // 0x006610C7
};
class COpinionReactionManager { public:
    long GetThingAttitudeToPlayer(CThing* thing);   // 0x00701840
};
class CWorld { public:
    unsigned char _pad[0x74];
    COpinionReactionManager* m_pOpinionReactionManager;   // +0x74
    COpinionReactionManager* GetPOpinionReactionManager() const { return m_pOpinionReactionManager; }
};

struct CAIStateGroup_MinionReactToCriminal : CAIStateGroupBase {
    CBaseIntelligentPointer PHero;              // +0x14 CIntelligentPointer<CThingPlayerCreature>
    bool IsValid();
};

bool CAIStateGroup_MinionReactToCriminal::IsValid()
{
    if (IS_THING_ALIVE(PHero)) {
        CTCVillager* villager;
        if (PBrain->PCreature->GetInterfaceX(&villager, 0x23)) {
            CTCCreatureOpinionOfHero* opinion;
            if (PBrain->PCreature->GetInterfaceX(&opinion, 0x4f)) {
                CTCVillage* village = ((CTCVillageMember*)villager)->GetPVillage();
                if (village) {
                    long deedIndex = -1;
                    if (village->m_crimeReported || !village->m_pGuardPost
                            || opinion->FindMostRecentDeed(COpinionDeedFilter_IsCrimeToRunAndFindAGuard(), &deedIndex, false)) {
                        CThing* self = PBrain->PCreature;
                        long attitude = PeekWorld().GetPOpinionReactionManager()->GetThingAttitudeToPlayer(self);
                        return attitude == 4 || (attitude == 2 && (village->m_crimeReported || deedIndex >= 0));
                    }
                }
            }
        }
    }
    return false;
}