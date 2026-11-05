// CTCCreatureOpinionOfHero::FindMostRecentDeed<COpinionDeedFilter_IsCrimeToRunAndFindAGuard>(const filter&, long* outHandle, bool includePending) const -- the most recent (latest time) of the hero's deeds that pass the filter, over the pending deeds when asked and then every deed type's known deeds; its handle is written out
// Leaf: FindMostRecentDeed
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad21[0x44 - 0x21] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006610C7, 302 bytes, `ret 0Ch` (size TU: odd start, frame pointer,
//   `push 51h ; pop edi`, the out-of-line bitset test).
// Callees: CTCBase::PeekPlayerManager() const @ 0x00686D50, CPlayerManager::GetMainPlayer() const @ 0x00449960,
//   CPlayer::PeekPControlledCreature @ 0x00487DD0, std::bitset::test(long) @ 0x00410DE0 (the presence bitset),
//   CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0, CTCHeroOpinionDeedLog::GetDeedFromHandle(long)
//   @ 0x006C58F0, COpinionDeedFilter_IsCrimeToRunAndFindAGuard::operator()(const COpinionDeed*) const @ 0x00681750.
// Pragmas: optimize("y", off) + optimize("s", on) (the size TU keeps the frame pointer).
// Layout: as the landed typed instantiation 0x0093A3C0: PendingDeeds (list<long>, retail 4-byte head) +0x54, KnownDeeds
//   (list<long>[92]) +0x1c8; COpinionDeed time stamp +0x14 (descriptive); deed log = interface 0x51.
// Shape: GetInterface must be `__forceinline` (the size TU calls it out of line otherwise); the best deed and its time are
//   locals zeroed up front (`and [ebp-4],0` / `and [ebp-8],0`), the log is the unchecked
//   out-parameter GetInterface (never written when absent); `deed->Time > bestTime || !best` keeps the latest; the known-deed
//   arrays are walked with a counted outer loop VC7.1 strength-reduces to a list pointer homed in the dead `includePending`
//   slot and a 92-down counter; each list walk tests at the bottom (`jmp` into the test, size TU).
#pragma optimize("y", off)
#pragma optimize("s", on)
class CTCBase;
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    const CThingPlayerCreature* PeekPControlledCreature() const;                    // 0x00487DD0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class COpinionDeed { public:
    long          DeedType;                             // +0x00 EOpinionDeedType
    long          DeedHandle;                           // +0x04
    unsigned char _pad08[0x14 - 0x08];
    unsigned long Time;                                 // +0x14
};
class COpinionDeedFilter_IsCrimeToRunAndFindAGuard { public:
    bool operator()(const COpinionDeed* deed) const;    // 0x00681750
};
class CTCHeroOpinionDeedLog { public:
    const COpinionDeed* GetDeedFromHandle(long handle) const;   // 0x006C58F0
};
struct SHandleNode {                                    // STLport list<long> node
    SHandleNode* next;
    SHandleNode* prev;
    long         value;                                 // +0x08
};
struct CHandleList {                                    // retail list: the sentinel node pointer
    SHandleNode* head;
};
class CTCBase
{
public:
    virtual void v00();
protected:
    const CPlayerManager& PeekPlayerManager() const;   // 0x00686D50
};

class CTCCreatureOpinionOfHero : public CTCBase { public:
    unsigned char _pad04[0x54 - 0x04];
    CHandleList PendingDeeds;                           // +0x54 (PDB +0x70)
    long        LastFrameAppliedDeedEffect[92];         // +0x58
    CHandleList KnownDeeds[92];                         // +0x1c8 (PDB +0x1ec)
    template <class TFilter>
    const COpinionDeed* FindMostRecentDeed(const TFilter& filter, long* outHandle, bool includePending) const;
};

template <class TFilter>
const COpinionDeed* CTCCreatureOpinionOfHero::FindMostRecentDeed(const TFilter& filter, long* outHandle, bool includePending) const
{
    const COpinionDeed* best = 0;
    const CThing* hero = PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
    if (hero) {
        const const CTCHeroOpinionDeedLog* log;
        hero->PeekTC(TCI_HERO_OPINION_DEED_LOG, &log);
        unsigned long bestTime = 0;
        if (includePending) {
            SHandleNode* end = PendingDeeds.head;
            for (SHandleNode* node = end->next; node != end; node = node->next) {
                const COpinionDeed* deed = log->GetDeedFromHandle(node->value);
                if (deed && filter(deed) && (deed->Time > bestTime || !best)) {
                    if (outHandle)
                        *outHandle = node->value;
                    bestTime = deed->Time;
                    best = deed;
                }
            }
        }
        for (long type = 0; type < 92; ++type) {
            SHandleNode* end = KnownDeeds[type].head;
            for (SHandleNode* node = end->next; node != end; node = node->next) {
                const COpinionDeed* deed = log->GetDeedFromHandle(node->value);
                if (deed && filter(deed) && (deed->Time > bestTime || !best)) {
                    if (outHandle)
                        *outHandle = node->value;
                    bestTime = deed->Time;
                    best = deed;
                }
            }
        }
    }
    return best;
}

template const COpinionDeed* CTCCreatureOpinionOfHero::FindMostRecentDeed<COpinionDeedFilter_IsCrimeToRunAndFindAGuard>(
    const COpinionDeedFilter_IsCrimeToRunAndFindAGuard&, long*, bool) const;