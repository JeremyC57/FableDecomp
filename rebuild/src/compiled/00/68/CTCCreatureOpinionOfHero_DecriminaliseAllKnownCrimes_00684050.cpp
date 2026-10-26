// CTCCreatureOpinionOfHero::DecriminaliseAllKnownCrimes() -- for a villager with a village, clear every crime deed this creature knows about (crime deed types 1..9 and the pending list) from the hero's opinion deed log
// Retail 0x00684050, 448 bytes (plain `ret`). Callees: CVectorMap LowerBound @ 0x0040F020 (inlined GetInterface: CTCVillager 0x23
// on the parent, the hero's CTCHeroOpinionDeedLog 0x51), GFGetPVillageInterface(CTCVillager*) @ 0x007EE540 (__fastcall),
// CTCBase::PeekPlayerManager() const @ 0x00686D50, CPlayerManager::GetMainPlayer() const @ 0x00449960, CPlayer::GetHeroCreature
// @ 0x00487DC0, CTCHeroOpinionDeedLog::GetDeedFromHandle(long) @ 0x006C58F0, the log's per-handle decriminalise @ 0x006C59A0
// (manifest "CScreenEffectPriorityStack::SetAreaTrackPos" is propagated).
// Layout: Ego_r.pdb CTCCreatureOpinionOfHero has PendingDeeds +0x70, LastFrameAppliedDeedEffect long[92] +0x7c, KnownDeeds
// list<long>[92] +0x1ec; retail's STLport list is 4 bytes and its vector<bool> masks are smaller, so PendingDeeds is +0x54,
// LastFrameAppliedDeedEffect +0x58 and KnownDeeds +0x1c8 (the loop starts at KnownDeeds[1], +0x1cc). COpinionDeed: type +0,
// Decriminalised +0x22, IsCrime flag +0x23, linked handle +0x24 (deed type 0x35 links to another deed).
// Shape: the deed log lookup result is unchecked (the miss path reads the stale out local); the per-list walk is an inline
// helper used for the nine known-deed lists and the pending list; the village lookup is called a second time with its
// result unused; `this` is spilled to a frame word and re-read for the pending list. IsCrime tests `Type > 9` first (retail
// lays the 2..9 arm second); the linked handle is swapped in BEFORE the first decriminalise call (`first = handle; handle =
// linked; Decriminalise(first)`), which keeps the deed log in ebx and the list sentinel in ebp (no extra frame word).
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CPlayer { public:
    CThing* GetHeroCreature();                      // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer() const;                 // 0x00449960
};
struct CTCVillager;
class CTCVillage;
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);   // 0x007EE540

class COpinionDeed { public:
    long          Type;                             // +0x00 EOpinionDeedType
    unsigned char _pad04[0x22 - 0x04];
    bool          Decriminalised;                   // +0x22
    bool          IsCrime;                          // +0x23
    long          LinkedHandle;                     // +0x24
};
class CTCHeroOpinionDeedLog { public:
    COpinionDeed* GetDeedFromHandle(long handle);   // 0x006C58F0
    void DecriminaliseDeed(long handle);            // 0x006C59A0
};

struct SListNode {
    SListNode* _M_next;
    SListNode* _M_prev;
    long       _M_data;
};
struct CLongList {                                  // retail STLport list<long>, 4 bytes
    SListNode* _M_node;
};

__forceinline bool IsCrime(const COpinionDeed* deed)
{
    if (deed->Type < 2)
        return false;
    if (deed->Type > 9)
        return deed->Type == 0x35 && deed->LinkedHandle != -1;
    return deed->IsCrime && !deed->Decriminalised;
}

__forceinline void DecriminaliseList(CTCHeroOpinionDeedLog* log, CLongList& deeds)
{
    SListNode* end = deeds._M_node;
    for (SListNode* it = end->_M_next; it != end; it = it->_M_next) {
        long handle = it->_M_data;
        COpinionDeed* deed = log->GetDeedFromHandle(handle);
        if (deed && IsCrime(deed)) {
            if (deed->Type == 0x35) {
                long linked = deed->LinkedHandle;
                if (linked != -1) {
                    long first = handle;
                    handle = linked;
                    log->DecriminaliseDeed(first);
                }
            }
            log->DecriminaliseDeed(handle);
        }
    }
}

class CTCBase
{
protected:
    const CPlayerManager& PeekPlayerManager() const;   // 0x00686D50
};

class CTCCreatureOpinionOfHero : public CTCBase { public:
    void*         __vftable;                        // +0x00
    CThing*       Thing;                            // +0x04 CTCBase parent thing
    unsigned char _pad08[0x54 - 0x08];
    CLongList     PendingDeeds;                     // +0x54
    long          LastFrameAppliedDeedEffect[92];   // +0x58
    CLongList     KnownDeeds[92];                   // +0x1c8
    void DecriminaliseAllKnownCrimes();
};

void CTCCreatureOpinionOfHero::DecriminaliseAllKnownCrimes()
{
    CTCVillager* villager;
    if (!Thing->GetInterface(&villager, 0x23))
        return;
    if (!GFGetPVillageInterface(villager))
        return;
    CTCVillage* village = GFGetPVillageInterface(villager);
    CThing* hero = PeekPlayerManager().GetMainPlayer()->GetHeroCreature();
    if (!hero)
        return;
    CTCHeroOpinionDeedLog* log;
    hero->GetInterface(&log, 0x51);
    for (int i = 1; i < 10; ++i)
        DecriminaliseList(log, KnownDeeds[i]);
    DecriminaliseList(log, PendingDeeds);
}