// CAIStateGroup_KidWanderNearKids::IsRunnable() -- gate idle modes, then find the first eligible child creature.
// Retail @ 0x00914c60, 279 bytes. The first and second component-map lookups call byte-identical
// LowerBound bodies at 0x0040f020 and 0x004365b0 respectively. Other callees: CTCIdleScheduler::IsPreferredIdle
// 0x00801be0, CTCCreatureModeManager::IsModeActive 0x0073a8a0 (the propagated CPlayer label is wrong),
// CAIStateGroupBase::GetPThingWorld 0x008fcfa0, CWorld::GetPRandomSeed 0x0049d810, GFROR13 0x00497890,
// PeekThingSearchTools 0x008fd000, CThingSearchTools::GetPFirstThing 0x00914aa0. The generated
// CThingAICreature header is opaque over the native CThing prefix; the local CThing view follows landed
// 0x00914aa0/0x0092e890 and the retail flag/map offsets. The filter types follow 0x00914aa0; its empty
// child object has a user-provided empty ctor so value-initialization does not zero an unused byte.
#ifdef _MSC_VER
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIStateGroup_KidWanderNearKids.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingSearchTools.h"
#include "engine/CWorld.h"
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_KidWanderNearKids) == sizeof(CAIStateGroupBase));
#else
#define __forceinline inline
#define __fastcall
struct CThingAICreature;
struct CWorld { };
struct CAIBrain { char pad[0x18]; CThingAICreature* Creature; };
struct CAIStateGroupBase { void* vtable; CAIBrain* PBrain; char rest[0xc]; };
struct CAIStateGroup_KidWanderNearKids { void* vtable; char base[0x10]; };
struct CThingSearchTools { };
#endif

struct CTCBase { };
struct KidEntry { int type; CTCBase* component; };
struct KidMap { KidEntry* first; KidEntry* last; KidEntry* LowerBound(const int* key); }; // 0x0040f020 / 0x004365b0
// CThing is quarantined and CThingAICreature's PDB header has only opaque base bytes here. This
// exact retail prefix view records UsingComponents at +0x20 and the interface vector at +0x44.
struct CThing {
    unsigned char pad[0x20]; unsigned int bits[9]; KidMap map;
    bool HasTC(int id) const { return (bits[id >> 5] & (1U << (id & 31))) != 0; }
    template<class T> __forceinline T* GetTC(int id) {
        int key=id; KidMap* m=&map; KidEntry* p=m->LowerBound(&key);
        if (p==m->last || id<p->type) p=m->last; return (T*)p->component;
    }
};
struct CWorldMethods : CWorld { unsigned int* GetPRandomSeed(); }; // 0x0049d810, actual CWorld method adapter
struct KidScheduler:CTCBase { bool IsPreferredIdle(CAIStateGroupBase* group); }; // 0x00801be0
struct CTCCreatureModeManager:CTCBase { bool IsModeActive(unsigned int mode); }; // 0x0073a8a0
struct KidWanderMethods : CAIStateGroup_KidWanderNearKids {
    CWorld* GetPThingWorld(); // 0x008fcfa0
    CThingSearchTools* PeekThingSearchTools(); // 0x008fd000
    bool IsRunnable();
};
struct CThingFilter_IsNot {
    const CThing* Excluded;
    CThingFilter_IsNot(const CThing* thing):Excluded(thing){}
    bool operator()(const CThingAICreature* creature) const;
};
struct CThingFilter_IsChild {
    CThingFilter_IsChild() { }
    bool operator()(const CThingAICreature*) const;
};
struct CFilter_Bind2 {
    const CThingFilter_IsNot& First; const CThingFilter_IsChild& Second;
    CFilter_Bind2(const CThingFilter_IsNot& a,const CThingFilter_IsChild& b):First(a),Second(b){}
};
inline CFilter_Bind2 operator&&(const CThingFilter_IsNot& a,const CThingFilter_IsChild& b) { return CFilter_Bind2(a,b); }
struct CThingSearchToolsMethods : CThingSearchTools {
    bool GetPFirstThing(const CFilter_Bind2& filter,CThingAICreature** out); // 0x00914aa0
};
unsigned int __fastcall GFROR13(unsigned int); // 0x00497890

bool KidWanderMethods::IsRunnable() {
    if (((CThing*)((CAIStateGroupBase*)this)->PBrain->Creature)->bits[6] & 0x80000) {
        KidScheduler* scheduler=((CThing*)(((CAIStateGroupBase*)this)->PBrain->Creature))->GetTC<KidScheduler>(0xd3);
        if (!scheduler->IsPreferredIdle((CAIStateGroupBase*)this)) return false;
    }
    if (((CThing*)((CAIStateGroupBase*)this)->PBrain->Creature)->bits[1] & 0x20000) {
        CTCCreatureModeManager* modes=((CThing*)(((CAIStateGroupBase*)this)->PBrain->Creature))->GetTC<CTCCreatureModeManager>(0x31);
        if (modes->IsModeActive(8) || modes->IsModeActive(9)) {
            unsigned int* seed=((CWorldMethods*)GetPThingWorld())->GetPRandomSeed();
            *seed=*seed*0x24a1+0x24df;
            *seed=GFROR13(*seed);
            if (*seed % 100 > 10) return false;
        }
    }
    CThingAICreature* found;
    return ((CThingSearchToolsMethods*)PeekThingSearchTools())->GetPFirstThing(
        CThingFilter_IsNot((const CThing*)((CAIStateGroupBase*)this)->PBrain->Creature) && CThingFilter_IsChild(),
        &found);
}

#include <cstdio>

#ifdef _MSC_VER
typedef unsigned __int64 TestWide;
#else
typedef unsigned long long TestWide;
#endif

struct FixtureCreature { CThing state; };

static CWorldMethods testWorld;
static CThingSearchTools testTools;
static KidEntry entries[3];
static KidEntry* modesEntry;
static CAIBrain* activeBrain;
static CThingAICreature* schedulerReplacement;
static CThingAICreature* searchCandidate;
static const CThing* expectedExcluded;
static CThingAICreature* foundThing;
static CTCCreatureModeManager* replacementModes;
static CTCCreatureModeManager* seenMode8;
static CTCCreatureModeManager* seenMode9;
static unsigned int expectedSeed;
static bool preferredResult, mode8Result, mode9Result;
static bool searchResult, childResult, swapCreatureOnScheduler, replaceModesOn8;
static bool filtersChecked;
static int preferredCalls, modeCalls, searchCalls;

KidEntry* KidMap::LowerBound(const int* key)
{
    for (KidEntry* p=first; p!=last; ++p)
        if (*key <= p->type) return p;
    return last;
}

bool KidScheduler::IsPreferredIdle(CAIStateGroupBase*)
{
    ++preferredCalls;
    if (swapCreatureOnScheduler) activeBrain->Creature=schedulerReplacement;
    return preferredResult;
}

bool CTCCreatureModeManager::IsModeActive(unsigned int mode)
{
    ++modeCalls;
    if (mode == 8) {
        seenMode8=this;
        if (replaceModesOn8) modesEntry->component=(CTCBase*)replacementModes;
        return mode8Result;
    }
    seenMode9=this;
    return mode9Result;
}

unsigned int* CWorldMethods::GetPRandomSeed() { return &expectedSeed; }
CWorld* KidWanderMethods::GetPThingWorld() { return (CWorld*)&testWorld; }
CThingSearchTools* KidWanderMethods::PeekThingSearchTools() { return &testTools; }
unsigned int __fastcall GFROR13(unsigned int value) { return (value >> 13) | (value << 19); }

bool CThingFilter_IsNot::operator()(const CThingAICreature* creature) const
{
    return Excluded != (const CThing*)creature;
}

bool CThingFilter_IsChild::operator()(const CThingAICreature*) const
{
    return childResult;
}

bool CThingSearchToolsMethods::GetPFirstThing(const CFilter_Bind2& filter, CThingAICreature** out)
{
    ++searchCalls;
    filtersChecked = filter.First.Excluded == expectedExcluded;
    if (!filtersChecked || !filter.First(searchCandidate) || !filter.Second(searchCandidate) || !searchResult)
        return false;
    *out=searchCandidate;
    foundThing=*out;
    return true;
}

static unsigned int updatedSeed(unsigned int seed)
{
    return seed * 0x24a1U + 0x24dfU;
}

static unsigned int rotatedSeed(unsigned int seed)
{
    unsigned int value=updatedSeed(seed);
    return (value >> 13) | (value << 19);
}

static unsigned int seedForRemainder(unsigned int remainder)
{
    for (unsigned int seed=0; seed<1000000U; ++seed)
        if (rotatedSeed(seed) % 100U == remainder) return seed;
    return 0xffffffffU;
}

static CThingAICreature* asCreature(FixtureCreature* fixture)
{
    return (CThingAICreature*)&fixture->state;
}

static void setupView(FixtureCreature* fixture, int count)
{
    for (int i=0; i<9; ++i) fixture->state.bits[i]=0;
    fixture->state.map.first=entries;
    fixture->state.map.last=entries+count;
}

static void reset(CAIStateGroup_KidWanderNearKids* group, CAIBrain* brain,
                  FixtureCreature* creature, FixtureCreature* replacement,
                  FixtureCreature* candidate, KidScheduler* scheduler,
                  CTCCreatureModeManager* modes, int haveScheduler, unsigned int seed)
{
    int count=0;
    entries[count].type=0x31; entries[count].component=(CTCBase*)modes; ++count;
    modesEntry=&entries[0];
    if (haveScheduler) {
        entries[count].type=0xd3; entries[count].component=(CTCBase*)scheduler; ++count;
    }
    entries[count].type=0x7fffffff; entries[count].component=0;
    setupView(creature,count);
    setupView(replacement,count);
    ((CAIStateGroupBase*)group)->PBrain=brain;
    brain->Creature=asCreature(creature);
    activeBrain=brain;
    schedulerReplacement=asCreature(replacement);
    searchCandidate=asCreature(candidate);
    expectedExcluded=&creature->state;
    foundThing=0;
    expectedSeed=seed;
    preferredResult=true; mode8Result=false; mode9Result=false;
    searchResult=true; childResult=true;
    swapCreatureOnScheduler=false; replaceModesOn8=false;
    replacementModes=0; seenMode8=0; seenMode9=0;
    filtersChecked=false;
    preferredCalls=modeCalls=searchCalls=0;
}

static bool run(CAIStateGroup_KidWanderNearKids* group)
{
    return ((KidWanderMethods*)group)->IsRunnable();
}

int main()
{
    CAIStateGroup_KidWanderNearKids group;
    CAIBrain brain;
    FixtureCreature current, replacement, candidate;
    KidScheduler scheduler;
    CTCCreatureModeManager modes, alternateModes;
    unsigned int seed, expected;

    // No scheduler component and no scheduler bit: take the ordinary search path.
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,0,0x12345678U);
    if (!run(&group) || preferredCalls!=0 || modeCalls!=0 || searchCalls!=1 || !filtersChecked) goto fail;

    // A present scheduler can veto before mode lookup and search.
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,1,0x12345678U);
    current.state.bits[6]=0x80000U; preferredResult=false;
    if (run(&group) || preferredCalls!=1 || modeCalls!=0 || searchCalls!=0) goto fail;

    // Accepting the scheduler continues, while the mode bit remains clear.
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,1,0x12345678U);
    current.state.bits[6]=0x80000U;
    if (!run(&group) || preferredCalls!=1 || searchCalls!=1) goto fail;

    // With the mode bit set and neither queried mode active, skip RNG and continue searching.
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,0,0x12345678U);
    current.state.bits[1]=0x20000U;
    if (!run(&group) || modeCalls!=2 || expectedSeed!=0x12345678U || searchCalls!=1) goto fail;

    // Mode 8 short-circuits mode 9; an active mode still enters the random gate.
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,0,seedForRemainder(10));
    current.state.bits[1]=0x20000U; mode8Result=true; mode9Result=true;
    seed=expectedSeed; expected=rotatedSeed(seed);
    if (!run(&group) || modeCalls!=1 || seenMode8!=&modes || seenMode9!=0 || expectedSeed!=expected || expectedSeed%100U!=10U || searchCalls!=1) goto fail;

    // A rotated seed with remainder 10 is accepted; the bound is inclusive.
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,0,seedForRemainder(10));
    current.state.bits[1]=0x20000U; mode9Result=true;
    seed=expectedSeed; expected=rotatedSeed(seed);
    if (!run(&group) || modeCalls!=2 || expectedSeed!=expected || expectedSeed%100U!=10U || searchCalls!=1) goto fail;

    // Remainder 11 is rejected and search is skipped.
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,0,seedForRemainder(11));
    current.state.bits[1]=0x20000U; mode9Result=true;
    seed=expectedSeed; expected=rotatedSeed(seed);
    if (run(&group) || modeCalls!=2 || expectedSeed!=expected || expectedSeed%100U!=11U || searchCalls!=0) goto fail;

    // The seed update wraps at 32 bits before the rotate and remainder test.
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,0,0xffffffffU);
    current.state.bits[1]=0x20000U; mode9Result=true;
    seed=expectedSeed; expected=rotatedSeed(seed);
    if ((TestWide)seed*0x24a1U+0x24dfU <= (TestWide)0xffffffffU) goto fail;
    if (run(&group) != (expected%100U<=10U) || expectedSeed!=expected || searchCalls!=(expected%100U<=10U?1:0)) goto fail;

    // Scheduler callback replaces the brain creature; mode and filter must see the replacement.
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,1,seedForRemainder(10));
    current.state.bits[6]=0x80000U; replacement.state.bits[1]=0x20000U;
    swapCreatureOnScheduler=true; mode8Result=true;
    expectedExcluded=&replacement.state;
    if (!run(&group) || preferredCalls!=1 || modeCalls!=1 || !filtersChecked || searchCalls!=1) goto fail;

    // Mode 9 uses the cached mode manager even if mode 8 replaces the map entry.
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,0,seedForRemainder(10));
    current.state.bits[1]=0x20000U; mode9Result=true;
    replacementModes=&alternateModes; replaceModesOn8=true;
    if (!run(&group) || modeCalls!=2 || seenMode8!=&modes || seenMode9!=&modes || searchCalls!=1) goto fail;

    // Search honors both filters and propagates the underlying search result.
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,0,0x12345678U);
    searchCandidate=asCreature(&current);
    if (run(&group) || searchCalls!=1 || !filtersChecked) goto fail;
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,0,0x12345678U);
    childResult=false;
    if (run(&group) || searchCalls!=1 || !filtersChecked) goto fail;
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,0,0x12345678U);
    searchResult=false;
    if (run(&group) || searchCalls!=1 || !filtersChecked) goto fail;
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,0,0x12345678U);
    if (!run(&group) || foundThing!=asCreature(&candidate) || searchCalls!=1 || !filtersChecked) goto fail;

    // Scheduler bit set but no exact entry: LowerBound runs to end and GetTC uses the valid sentinel component.
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,0,0x12345678U);
    current.state.bits[6]=0x80000U;
    entries[0].type=0x31; entries[0].component=(CTCBase*)&modes;
    entries[1].type=0xd2; entries[1].component=0;
    entries[2].type=0x7fffffff; entries[2].component=(CTCBase*)&scheduler;
    current.state.map.first=entries; current.state.map.last=entries+2;
    if (!run(&group) || preferredCalls!=1 || searchCalls!=1) goto fail;

    // Mode bit set but the lower-bound candidate is a higher type; lookup falls back to the sentinel mode manager.
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,0,0x12345678U);
    current.state.bits[1]=0x20000U;
    entries[0].type=0x32; entries[0].component=0;
    entries[1].type=0x7fffffff; entries[1].component=(CTCBase*)&modes;
    modesEntry=&entries[1]; current.state.map.first=entries; current.state.map.last=entries+1;
    if (!run(&group) || modeCalls!=2 || expectedSeed!=0x12345678U || searchCalls!=1) goto fail;

    std::printf("00914c60_TEST PASS\n");
    return 0;
fail:
    std::printf("00914c60_TEST FAIL\n");
    return 1;
}