// CTCCreatureOpinionOfHero::FindMostRecentDeed<COpinionDeedFilter_IsCrimeToRunAndFindAGuard>(const filter&, long* outHandle, bool includePending) const -- the most recent (latest time) of the hero's deeds that pass the filter, over the pending deeds when asked and then every deed type's known deeds; its handle is written out
// Leaf: FindMostRecentDeed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006610C7, 302 bytes, `ret 0Ch` (size TU: odd start, frame pointer,
//   `push 51h ; pop edi`, the out-of-line bitset test).
// Callees: CTCBase::PeekPlayerManager() const @ 0x00686D50, CPlayerManager::GetMainPlayer() const @ 0x00449960,
//   CPlayer::GetControlledCreature @ 0x00487DD0, std::bitset::test(long) @ 0x00410DE0 (the presence bitset),
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
struct CPresenceBits {
    bool test(long bit) const;                          // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CThing { public:
    unsigned char _pad00[0x20];
    CPresenceBits m_present;                            // +0x20
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                         // +0x44
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_present.test(interfaceType)) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CPlayer { public:
    CThing* GetControlledCreature();                    // 0x00487DD0
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer() const;                     // 0x00449960
};
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
    CThing* hero = PeekPlayerManager().GetMainPlayer()->GetControlledCreature();
    if (hero) {
        const CTCHeroOpinionDeedLog* log;
        hero->GetInterface(&log, 0x51);
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