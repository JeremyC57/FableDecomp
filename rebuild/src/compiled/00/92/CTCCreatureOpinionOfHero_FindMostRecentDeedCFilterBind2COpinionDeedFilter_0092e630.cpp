// CTCCreatureOpinionOfHero::FindMostRecentDeed<CFilter_Bind2<COpinionDeedFilter_IsStillHappening,COpinionDeedFilter_TargetIs> >(EOpinionDeedType, const filter&, long* outHandle, bool includePending) const -- the first of the hero's deeds of the type that is still happening and targets the filter's thing (pending deeds first when asked, then the known deeds of that type); its handle is written out
// Leaf: FindMostRecentDeed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092E630, 283 bytes (+ padding), `ret 10h`.
// Callees: CTCBase::GetPlayerManager() const @ 0x00686D50, CPlayerManager::GetMainPlayer() const @ 0x00449960,
// CPlayer::GetControlledCreature @ 0x00487DD0, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound const @ 0x004365B0,
// CTCHeroOpinionDeedLog::GetDeedFromHandle(long) @ 0x006C58F0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the deed's target).
// Layout: as the landed IsStillHappening instantiation 0x0093A3C0: PendingDeeds (list<long>) +0x54, KnownDeeds (list<long>[92])
// +0x1c8; COpinionDeed (Ego_r.pdb): DeedType +0, PTarget (CIntelligentPointer<CThing>) +0x18 (descriptive), IsStillHappening
// +0x21; deed log = interface 0x51 (bit 17 of the +0x28 word). CFilter_Bind2 holds references to its two filters
// (TargetIs at +4, its thing at +0).
// Shape: the landed 0x0093A3C0 body with the Bind2 functor (`First(deed) && Second(deed)`, the target compared after
//   loading the filter's thing into a named local before the GetPItem call -- read inline it is compared from memory
//   after the call); with the extra call the deed log stays in its frame
//   word (the GetInterface key's slot) and `this` is parked in the frame.
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
struct CBaseIntelligentPointer {
    void* __vftable;                                    // +0x00
    void* m_pItem;                                      // +0x04
    CThing* GetPItem() const;                           // 0x00A01B50
};
class COpinionDeed { public:
    long          DeedType;                             // +0x00 EOpinionDeedType
    long          DeedHandle;                           // +0x04
    unsigned char _pad08[0x18 - 0x08];
    CBaseIntelligentPointer PTarget;                    // +0x18
    unsigned char _pad20;
    bool          IsStillHappening;                     // +0x21
};
class COpinionDeedFilter_IsStillHappening { public:
    bool operator()(const COpinionDeed& deed) const { return deed.IsStillHappening; }
};
class COpinionDeedFilter_TargetIs { public:
    CThing* Target;                                     // +0x00
    bool operator()(const COpinionDeed& deed) const
    {
        CThing* target = Target;
        return deed.PTarget.GetPItem() == target;
    }
};
template <class F1, class F2> class CFilter_Bind2 { public:
    const F1& First;                                    // +0x00
    const F2& Second;                                   // +0x04
    CFilter_Bind2(const F1& first, const F2& second) : First(first), Second(second) { }
    bool operator()(const COpinionDeed& deed) const { return First(deed) && Second(deed); }
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

typedef CFilter_Bind2<COpinionDeedFilter_IsStillHappening, COpinionDeedFilter_TargetIs> CStillHappeningTargetFilter;
template const COpinionDeed* CTCCreatureOpinionOfHero::FindMostRecentDeed<CStillHappeningTargetFilter>(
    long, const CStillHappeningTargetFilter&, long*, bool) const;