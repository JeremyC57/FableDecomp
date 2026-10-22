// Behaviour test for retail 0x00914c60: the candidate source below is embedded verbatim (from src/00914c60.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// CAIStateGroup_KidWanderNearKids::IsRunnable() -- gate idle modes, then find the first eligible child creature.
// Leaf: IsRunnable
// Retail: Fable.exe (TLC, VC7.1) @ 0x00914C60, 279 bytes (virtual; manifest ?IsRunnable@CAIStateGroup_KidWanderNearKids@@UAE_NXZ).
// Callees: the two component-map lookups call two distinct retail copies of CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
//   -- 0x0040F020 (first lookup, interface 0xd3) and 0x004365B0 (second lookup, interface 0x31). The copies are separate
//   COMDATs in retail, so they must be separate symbols here; they are modelled as the non-const / const overloads of
//   the same lookup (a modelling device: which retail instance is the const one is not proven).
//   CTCIdleScheduler::IsPreferredIdle 0x00801BE0, CTCCreatureModeManager::HasMode(ulong) 0x0073A8A0 (manifest "CPlayer::HasMode"
//   is propagated), CAIStateGroupBase::GetPWorld 0x008FCFA0 (manifest "CPlayer::GetCurrentMode" is propagated; body
//   PBrain->+0x1c->+0x24, as landed), CWorld::GetPRandomSeed 0x0049D810, GFROR13 0x00497890,
//   CAIStateGroupBase::PeekThingSearchTools 0x008FD000, CThingSearchTools::GetPFirstThing<CFilter_Bind2<CThingFilter_IsNot,
//   CThingFilter_IsChild>,CThingAICreature> 0x00914AA0 (manifest).
// Layout: engine/ headers for the state group, brain, world and search tools; the generated CThingAICreature header is
//   opaque over the native CThing prefix, so the local CThing view records UsingComponents at +0x20 and the interface
//   vector at +0x44 (as landed 0x00914AA0/0x0092E890). The filter types follow 0x00914AA0; its empty child object has a
//   user-provided empty ctor so value-initialization does not zero an unused byte. The *_Methods structs are the lane's
//   method carriers for generated data-only headers.
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIStateGroup_KidWanderNearKids.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingSearchTools.h"
#include "engine/CWorld.h"
FABLE_STATIC_ASSERT(sizeof(CAIStateGroup_KidWanderNearKids) == sizeof(CAIStateGroupBase));

struct CTCBase { };
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType,CTCBase*>
    struct SEntry { int type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const int* key);                 // 0x0040F020
    const SEntry* LowerBound(const int* key) const;     // 0x004365B0
};
struct CThing {
    unsigned char pad[0x20]; unsigned int UsingComponents[9]; CInterfaceMap Interfaces;
    template<class T> __forceinline T* GetTC(int id) {
        int key=id; CInterfaceMap* m=&Interfaces; CInterfaceMap::SEntry* p=m->LowerBound(&key);
        if (p==m->last || id<p->type) p=m->last; return (T*)p->component;
    }
    template<class T> __forceinline T* GetTCConst(int id) const {
        int key=id; const CInterfaceMap* m=&Interfaces; const CInterfaceMap::SEntry* p=m->LowerBound(&key);
        if (p==m->last || id<p->type) p=m->last; return (T*)p->component;
    }
};
struct CWorld_Methods : CWorld { unsigned int* GetPRandomSeed(); };          // 0x0049D810
struct CTCIdleScheduler : CTCBase { bool IsPreferredIdle(CAIStateGroupBase* group); };   // 0x00801BE0
struct CTCCreatureModeManager : CTCBase { bool HasMode(unsigned long mode); };          // 0x0073A8A0
struct CAIStateGroupBase_Methods : CAIStateGroupBase {
    CWorld* GetPWorld();                                // 0x008FCFA0
    CThingSearchTools* PeekThingSearchTools();          // 0x008FD000
};
struct CAIStateGroup_KidWanderNearKids_Methods : CAIStateGroup_KidWanderNearKids {
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
template<class A, class B> struct CFilter_Bind2 {
    const A& First; const B& Second;
    CFilter_Bind2(const A& a,const B& b):First(a),Second(b){}
};
inline CFilter_Bind2<CThingFilter_IsNot, CThingFilter_IsChild> operator&&(const CThingFilter_IsNot& a,const CThingFilter_IsChild& b)
{ return CFilter_Bind2<CThingFilter_IsNot, CThingFilter_IsChild>(a,b); }
struct CThingSearchTools_Methods : CThingSearchTools {
    template<class F, class T> bool GetPFirstThing(const F& filter, T** out);   // <Bind2<IsNot,IsChild>,CThingAICreature>: 0x00914AA0
};
unsigned int __fastcall GFROR13(unsigned int);          // 0x00497890

bool CAIStateGroup_KidWanderNearKids_Methods::IsRunnable() {
    CAIStateGroupBase_Methods* self = (CAIStateGroupBase_Methods*)(CAIStateGroupBase*)this;
    if (((CThing*)self->PBrain->Creature)->UsingComponents[6] & 0x80000) {
        CTCIdleScheduler* scheduler=((CThing*)(self->PBrain->Creature))->GetTC<CTCIdleScheduler>(0xd3);
        if (!scheduler->IsPreferredIdle(self)) return false;
    }
    if (((CThing*)self->PBrain->Creature)->UsingComponents[1] & 0x20000) {
        CTCCreatureModeManager* modes=((CThing*)(self->PBrain->Creature))->GetTCConst<CTCCreatureModeManager>(0x31);
        if (modes->HasMode(8) || modes->HasMode(9)) {
            unsigned int* seed=((CWorld_Methods*)self->GetPWorld())->GetPRandomSeed();
            *seed=*seed*0x24a1+0x24df;
            *seed=GFROR13(*seed);
            if (*seed % 100 > 10) return false;
        }
    }
    CThingAICreature* found;
    return ((CThingSearchTools_Methods*)self->PeekThingSearchTools())->GetPFirstThing(
        CThingFilter_IsNot((const CThing*)self->PBrain->Creature) && CThingFilter_IsChild(),
        &found);
}

// ===== harness =====

#include <cstdio>

#ifdef _MSC_VER
typedef unsigned __int64 TestWide;
#else
typedef unsigned long long TestWide;
#endif

struct FixtureCreature { CThing state; };

static CWorld_Methods testWorld;
static CThingSearchTools testTools;
static CInterfaceMap::SEntry entries[3];
static CInterfaceMap::SEntry* modesEntry;
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

static int lbCalls[2];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const int* key)
{
    ++lbCalls[0];
    for (SEntry* p=first; p!=last; ++p)
        if (*key <= p->type) return p;
    return last;
}
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const int* key) const
{
    ++lbCalls[1];
    for (const SEntry* p=first; p!=last; ++p)
        if (*key <= p->type) return p;
    return last;
}

bool CTCIdleScheduler::IsPreferredIdle(CAIStateGroupBase*)
{
    ++preferredCalls;
    if (swapCreatureOnScheduler) activeBrain->Creature=schedulerReplacement;
    return preferredResult;
}

bool CTCCreatureModeManager::HasMode(unsigned long mode)
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

unsigned int* CWorld_Methods::GetPRandomSeed() { return &expectedSeed; }
CWorld* CAIStateGroupBase_Methods::GetPWorld() { return (CWorld*)&testWorld; }
CThingSearchTools* CAIStateGroupBase_Methods::PeekThingSearchTools() { return &testTools; }
unsigned int __fastcall GFROR13(unsigned int value) { return (value >> 13) | (value << 19); }

bool CThingFilter_IsNot::operator()(const CThingAICreature* creature) const
{
    return Excluded != (const CThing*)creature;
}

bool CThingFilter_IsChild::operator()(const CThingAICreature*) const
{
    return childResult;
}

template<class F, class T> bool CThingSearchTools_Methods::GetPFirstThing(const F& filter, T** out)
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
    for (int i=0; i<9; ++i) fixture->state.UsingComponents[i]=0;
    fixture->state.Interfaces.first=entries;
    fixture->state.Interfaces.last=entries+count;
}

static void reset(CAIStateGroup_KidWanderNearKids* group, CAIBrain* brain,
                  FixtureCreature* creature, FixtureCreature* replacement,
                  FixtureCreature* candidate, CTCIdleScheduler* scheduler,
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
    return ((CAIStateGroup_KidWanderNearKids_Methods*)group)->IsRunnable();
}

int main()
{
    CAIStateGroup_KidWanderNearKids group;
    CAIBrain brain;
    FixtureCreature current, replacement, candidate;
    CTCIdleScheduler scheduler;
    CTCCreatureModeManager modes, alternateModes;
    unsigned int seed, expected;

    // No scheduler component and no scheduler bit: take the ordinary search path.
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,0,0x12345678U);
    if (!run(&group) || preferredCalls!=0 || modeCalls!=0 || searchCalls!=1 || !filtersChecked) goto fail;

    // A present scheduler can veto before mode lookup and search.
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,1,0x12345678U);
    current.state.UsingComponents[6]=0x80000U; preferredResult=false;
    if (run(&group) || preferredCalls!=1 || modeCalls!=0 || searchCalls!=0) goto fail;

    // Accepting the scheduler continues, while the mode bit remains clear.
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,1,0x12345678U);
    current.state.UsingComponents[6]=0x80000U;
    if (!run(&group) || preferredCalls!=1 || searchCalls!=1) goto fail;

    // With the mode bit set and neither queried mode active, skip RNG and continue searching.
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,0,0x12345678U);
    current.state.UsingComponents[1]=0x20000U;
    if (!run(&group) || modeCalls!=2 || expectedSeed!=0x12345678U || searchCalls!=1) goto fail;

    // Mode 8 short-circuits mode 9; an active mode still enters the random gate.
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,0,seedForRemainder(10));
    current.state.UsingComponents[1]=0x20000U; mode8Result=true; mode9Result=true;
    seed=expectedSeed; expected=rotatedSeed(seed);
    if (!run(&group) || modeCalls!=1 || seenMode8!=&modes || seenMode9!=0 || expectedSeed!=expected || expectedSeed%100U!=10U || searchCalls!=1) goto fail;

    // A rotated seed with remainder 10 is accepted; the bound is inclusive.
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,0,seedForRemainder(10));
    current.state.UsingComponents[1]=0x20000U; mode9Result=true;
    seed=expectedSeed; expected=rotatedSeed(seed);
    if (!run(&group) || modeCalls!=2 || expectedSeed!=expected || expectedSeed%100U!=10U || searchCalls!=1) goto fail;

    // Remainder 11 is rejected and search is skipped.
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,0,seedForRemainder(11));
    current.state.UsingComponents[1]=0x20000U; mode9Result=true;
    seed=expectedSeed; expected=rotatedSeed(seed);
    if (run(&group) || modeCalls!=2 || expectedSeed!=expected || expectedSeed%100U!=11U || searchCalls!=0) goto fail;

    // The seed update wraps at 32 bits before the rotate and remainder test.
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,0,0xffffffffU);
    current.state.UsingComponents[1]=0x20000U; mode9Result=true;
    seed=expectedSeed; expected=rotatedSeed(seed);
    if ((TestWide)seed*0x24a1U+0x24dfU <= (TestWide)0xffffffffU) goto fail;
    if (run(&group) != (expected%100U<=10U) || expectedSeed!=expected || searchCalls!=(expected%100U<=10U?1:0)) goto fail;

    // Scheduler callback replaces the brain creature; mode and filter must see the replacement.
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,1,seedForRemainder(10));
    current.state.UsingComponents[6]=0x80000U; replacement.state.UsingComponents[1]=0x20000U;
    swapCreatureOnScheduler=true; mode8Result=true;
    expectedExcluded=&replacement.state;
    if (!run(&group) || preferredCalls!=1 || modeCalls!=1 || !filtersChecked || searchCalls!=1) goto fail;

    // Mode 9 uses the cached mode manager even if mode 8 replaces the map entry.
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,0,seedForRemainder(10));
    current.state.UsingComponents[1]=0x20000U; mode9Result=true;
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
    current.state.UsingComponents[6]=0x80000U;
    entries[0].type=0x31; entries[0].component=(CTCBase*)&modes;
    entries[1].type=0xd2; entries[1].component=0;
    entries[2].type=0x7fffffff; entries[2].component=(CTCBase*)&scheduler;
    current.state.Interfaces.first=entries; current.state.Interfaces.last=entries+2;
    if (!run(&group) || preferredCalls!=1 || searchCalls!=1) goto fail;

    // Mode bit set but the lower-bound candidate is a higher type; lookup falls back to the sentinel mode manager.
    reset(&group,&brain,&current,&replacement,&candidate,&scheduler,&modes,0,0x12345678U);
    current.state.UsingComponents[1]=0x20000U;
    entries[0].type=0x32; entries[0].component=0;
    entries[1].type=0x7fffffff; entries[1].component=(CTCBase*)&modes;
    modesEntry=&entries[1]; current.state.Interfaces.first=entries; current.state.Interfaces.last=entries+1;
    if (!run(&group) || modeCalls!=2 || expectedSeed!=0x12345678U || searchCalls!=1) goto fail;

    if (!lbCalls[0] || !lbCalls[1]) goto fail;   /* both retail lookup copies were exercised */
    std::printf("00914c60_TEST PASS\n");
    return 0;
fail:
    std::printf("00914c60_TEST FAIL\n");
    return 1;
}