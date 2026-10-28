// behaviour test for 0x00930EE0 CAIStateGroup_MinionReactToCriminal::IsValid (body verbatim, callees stubbed)
#include <stdio.h>
#include <string.h>
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
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)


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
struct CTCVillager;
class CTCVillage { public:
    unsigned char _pad[0x7c];
    bool m_crimeReported;                       // +0x7c (descriptive)
    unsigned char _pad7d[0x88 - 0x7d];
    void* m_pGuardPost;                         // +0x88 (descriptive)
};
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);  // 0x007EE540
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
struct CWorldO {
    unsigned char _pad[0x74];
    COpinionReactionManager* m_pOpinionReactionManager;   // +0x74
    COpinionReactionManager* GetPOpinionReactionManager() { return m_pOpinionReactionManager; }
};

struct CAIStateGroup_MinionReactToCriminal : CAIStateGroupBase {
    CBaseIntelligentPointer PHero;              // +0x14 CIntelligentPointer<CThingPlayerCreature>
    CWorldO* GetPThingWorld();                  // 0x008FCFA0
    bool IsValid();
};

bool CAIStateGroup_MinionReactToCriminal::IsValid()
{
    if (IS_THING_ALIVE(PHero)) {
        CTCVillager* villager;
        if (PBrain->PCreature->GetInterfaceX(&villager, 0x23)) {
            CTCCreatureOpinionOfHero* opinion;
            if (PBrain->PCreature->GetInterfaceX(&opinion, 0x4f)) {
                CTCVillage* village = GFGetPVillageInterface(villager);
                if (village) {
                    long deedIndex = -1;
                    if (village->m_crimeReported || !village->m_pGuardPost
                            || opinion->FindMostRecentDeed(COpinionDeedFilter_IsCrimeToRunAndFindAGuard(), &deedIndex, false)) {
                        CThing* self = PBrain->PCreature;
                        long attitude = GetPThingWorld()->GetPOpinionReactionManager()->GetThingAttitudeToPlayer(self);
                        return attitude == 4 || (attitude == 2 && (village->m_crimeReported || deedIndex >= 0));
                    }
                }
            }
        }
    }
    return false;
}


static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)

static CTCVillage* villageResult;
static CTCVillager* villageArg;
static int villageCalls, deedCalls, attitudeCalls;
static CTCCreatureOpinionOfHero* deedThis;
static long deedSetIndex;
static COpinionDeed* deedResult;
static bool deedForgotten;
static long attitudeResult;
static CThing* attitudeArg;
static CWorldO world;
static COpinionReactionManager reactions;

CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager) { ++villageCalls; villageArg = villager; return villageResult; }
COpinionDeed* CTCCreatureOpinionOfHero::FindMostRecentDeed(const COpinionDeedFilter_IsCrimeToRunAndFindAGuard&, long* index, bool includeForgotten)
{ ++deedCalls; deedThis = this; deedForgotten = includeForgotten; if (deedResult) *index = deedSetIndex; return deedResult; }
long COpinionReactionManager::GetThingAttitudeToPlayer(CThing* thing) { ++attitudeCalls; attitudeArg = thing; return attitudeResult; }
CWorldO* CAIStateGroup_MinionReactToCriminal::GetPThingWorld() { return &world; }

static unsigned char groupMem[sizeof(CAIStateGroup_MinionReactToCriminal)];
static CThing hero, creature;
static CAIBrain brain;
static CInterfaceMap::SEntry entries[3];
static char villagerObj[4], opinionObj[4], endObj[4], deedObj[4];
static CTCVillage village;

static void reset(bool alive, bool heroDead, bool hasVillager, bool hasOpinion)
{
    villageCalls = deedCalls = attitudeCalls = 0; villageArg = 0; deedThis = 0; attitudeArg = 0;
    deedResult = 0; deedSetIndex = -1; attitudeResult = 0; deedForgotten = true;
    memset(&hero, 0, sizeof(hero)); memset(&creature, 0, sizeof(creature)); memset(&village, 0, sizeof(village));
    hero.m_flags91 = heroDead ? 1 : 0;
    entries[0].type = 0x23; entries[0].component = (CTCBase*)villagerObj;
    entries[1].type = 0x4f; entries[1].component = (CTCBase*)opinionObj;
    entries[2].type = 0x7fff; entries[2].component = (CTCBase*)endObj;
    creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 2;
    if (hasVillager) creature.m_presentBits[0x23 >> 5] |= 1UL << (0x23 & 31);
    if (hasOpinion) creature.m_presentBits[0x4f >> 5] |= 1UL << (0x4f & 31);
    brain.PCreature = &creature;
    memset(groupMem, 0, sizeof(groupMem));
    CAIStateGroup_MinionReactToCriminal* g = (CAIStateGroup_MinionReactToCriminal*)groupMem;
    g->PBrain = &brain;
    g->PHero.m_pItem = alive ? &hero : 0;
    world.m_pOpinionReactionManager = &reactions;
    villageResult = &village;
}

int main()
{
    CAIStateGroup_MinionReactToCriminal* g = (CAIStateGroup_MinionReactToCriminal*)groupMem;
    // 1: no hero -> false, nothing looked up
    reset(false, false, true, true); CHECK(!g->IsValid() && villageCalls == 0);
    // 2: dead hero -> false
    reset(true, true, true, true); CHECK(!g->IsValid() && villageCalls == 0);
    // 3: not a villager -> false
    reset(true, false, false, true); CHECK(!g->IsValid() && villageCalls == 0);
    // 4: villager without an opinion -> false
    reset(true, false, true, false); CHECK(!g->IsValid() && villageCalls == 0);
    // 5: no village -> false, the village is asked of the villager component
    reset(true, false, true, true); villageResult = 0;
    CHECK(!g->IsValid() && villageCalls == 1 && villageArg == (CTCVillager*)villagerObj && attitudeCalls == 0);
    // 6: crime reported: attitude 4 / 2 -> true, 1 -> false; no deed search
    reset(true, false, true, true); village.m_crimeReported = true; village.m_pGuardPost = &village; attitudeResult = 4;
    CHECK(g->IsValid() && deedCalls == 0 && attitudeCalls == 1 && attitudeArg == &creature);
    reset(true, false, true, true); village.m_crimeReported = true; attitudeResult = 2; CHECK(g->IsValid());
    reset(true, false, true, true); village.m_crimeReported = true; attitudeResult = 1; CHECK(!g->IsValid());
    // 7: no guard post, not reported: attitude 2 needs a deed index -> false; 4 -> true
    reset(true, false, true, true); attitudeResult = 2; CHECK(!g->IsValid() && deedCalls == 0 && attitudeCalls == 1);
    reset(true, false, true, true); attitudeResult = 4; CHECK(g->IsValid());
    // 8: guard post, not reported: the deed search on the opinion component decides
    reset(true, false, true, true); village.m_pGuardPost = &village; attitudeResult = 4;
    CHECK(!g->IsValid() && deedCalls == 1 && deedThis == (CTCCreatureOpinionOfHero*)opinionObj && !deedForgotten && attitudeCalls == 0);
    reset(true, false, true, true); village.m_pGuardPost = &village; deedResult = (COpinionDeed*)deedObj; deedSetIndex = 3; attitudeResult = 2;
    CHECK(g->IsValid() && deedCalls == 1 && attitudeCalls == 1);
    reset(true, false, true, true); village.m_pGuardPost = &village; deedResult = (COpinionDeed*)deedObj; deedSetIndex = -1; attitudeResult = 2;
    CHECK(!g->IsValid());
    reset(true, false, true, true); village.m_pGuardPost = &village; deedResult = (COpinionDeed*)deedObj; deedSetIndex = 0; attitudeResult = 3;
    CHECK(!g->IsValid());

    if (fails == 0) printf("00930ee0_TEST PASS\n");
    return fails;
}