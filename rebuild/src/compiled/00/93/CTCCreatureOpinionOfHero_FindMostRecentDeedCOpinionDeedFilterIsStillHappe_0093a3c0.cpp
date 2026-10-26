// CTCCreatureOpinionOfHero::FindMostRecentDeed<COpinionDeedFilter_IsStillHappening>(EOpinionDeedType, const filter&, long* outHandle, bool includePending) const -- the first of the hero's deeds of the type that is still happening (pending deeds first when asked, then the known deeds of that type); its handle is written out
// Leaf: FindMostRecentDeed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093A3C0, 228 bytes (`ret 10h`).
// Callees: CTCBase::GetPlayerManager() const @ 0x00686D50, CPlayerManager::GetMainPlayer() const @ 0x00449960,
// CPlayer::GetControlledCreature @ 0x00487DD0, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0,
// CTCHeroOpinionDeedLog::GetDeedFromHandle(long) @ 0x006C58F0.
// Layout: local model -- engine/CTCCreatureOpinionOfHero.h is Ego_r.pdb (vector<bool> 0x14, lists 0xc); with retail 12-byte
// vectors/sets and 4-byte lists, PendingDeeds (list<long>) sits at +0x54 and KnownDeeds (list<long>[92]) at +0x1c8 (PDB +0x70 /
// +0x1ec). COpinionDeed (Ego_r.pdb): DeedType +0, IsStillHappening +0x21; deed log = interface 0x51 (bit 17 of the +0x28 word).
// Shape: the deed log is the unchecked out-parameter GetInterface (ebx; the absent arm reads the never-written local's home,
// the `includePending` slot); the filter is the inline functor reading IsStillHappening; both walks return through one
// shared found tail (`if (outHandle) *outHandle = handle; return deed;`, merged by VC7.1); each walk caches its sentinel in a
// local (edi), so `this` is parked in the frame during the pending walk.
class CTCBase;
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (HasTC(interfaceType)) {
            *ppOut = (T*)GetTC(interfaceType);
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
    unsigned char _pad08[0x21 - 0x08];
    bool          IsStillHappening;                     // +0x21
};
class COpinionDeedFilter_IsStillHappening { public:
    bool operator()(const COpinionDeed& deed) const { return deed.IsStillHappening; }
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
class CTCCreatureOpinionOfHero { public:
    virtual void v00();
    unsigned char _pad04[0x54 - 0x04];
    CHandleList PendingDeeds;                           // +0x54 (PDB +0x70)
    long        LastFrameAppliedDeedEffect[92];         // +0x58
    CHandleList KnownDeeds[92];                         // +0x1c8 (PDB +0x1ec)
    const CPlayerManager* GetPlayerManager() const;     // 0x00686D50 (CTCBase)
    template <class TFilter>
    const COpinionDeed* FindMostRecentDeed(long deedType, const TFilter& filter, long* outHandle, bool includePending) const;
};

template <class TFilter>
const COpinionDeed* CTCCreatureOpinionOfHero::FindMostRecentDeed(long deedType, const TFilter& filter, long* outHandle, bool includePending) const
{
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetControlledCreature();
    if (hero) {
        const CTCHeroOpinionDeedLog* log;
        hero->GetInterface(&log, 0x51);
        if (includePending) {
            SHandleNode* end = PendingDeeds.head;
            for (SHandleNode* node = end->next; node != end; node = node->next) {
                const COpinionDeed* deed = log->GetDeedFromHandle(node->value);
                if (deed && deed->DeedType == deedType && filter(*deed)) {
                    if (outHandle)
                        *outHandle = node->value;
                    return deed;
                }
            }
        }
        SHandleNode* end = KnownDeeds[deedType].head;
        for (SHandleNode* node = end->next; node != end; node = node->next) {
            const COpinionDeed* deed = log->GetDeedFromHandle(node->value);
            if (deed && filter(*deed)) {
                if (outHandle)
                    *outHandle = node->value;
                return deed;
            }
        }
    }
    return 0;
}

template const COpinionDeed* CTCCreatureOpinionOfHero::FindMostRecentDeed<COpinionDeedFilter_IsStillHappening>(
    long, const COpinionDeedFilter_IsStillHappening&, long*, bool) const;