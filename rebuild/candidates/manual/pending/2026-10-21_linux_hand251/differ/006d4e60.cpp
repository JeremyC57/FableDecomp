// Grade: DIFFER(363v363) -- register choice only: the component-key address temp is eax/ecx/edx in retail at the three lookups (ours ecx/eax/ecx) and RenownLevel is pushed through eax (ours edx)
// CTCFollowed::IsCreatureValidFollower(CThingAICreature* creature, bool* temporary) const -- can this creature follow the hero: it reports through `temporary` whether it would be a temporary (opinion-driven) follower; a creature not allowed to follow fails, a non-temporary one passes, otherwise the follower count must be under the hero's renown limit (unless it already follows) and its opinion of the hero must be willing to start (or continue) following
// Leaf: IsCreatureValidFollower
//
// Retail 0x006D4E60, 361 bytes (`ret 8`).
// Callees: CThing::IsUnderScriptSupervision @ 0x004C7E90, the const component-map LowerBound @ 0x004365B0 (kinds 4 hero stats,
// 0x4f opinion of hero, 0xbc followed), CTCFollowed::IsAIFollower(CThingAICreature*) const @ 0x006D4D70 (on this, then on the
// thing's own followed component), CTCHeroStats::GetMaxBoastingCrowdForRenownLevel(long) @ 0x0057AB14,
// CTCCreatureOpinionOfHero::IsWillingToContinueFollowingHero @ 0x006818F0 / IsWillingToStartFollowingHero @ 0x006818A0.
// Layout: CThing interface bitset +0x20 and map +0x44 (as the landed component lookups); Ego_r.pdb CTCFollowed Followers +0x0c
// (retail one-pointer STLport list); CTCHeroStats RenownLevel +0x70 (PDB +0x78); CThingAICreature AllowedToFollowHero bit 0 of
// +0x1c9 (PDB +0x1f9).
// Shape: a missing out pointer is replaced by a dummy in its own dead argument slot; the three lookups are the const out-parameter
// GetInterface with the result unchecked (the uninitialised local shares the key's slot); same-TU auto_inline(off) LowerBound
// stand-ins as in 0x006D8170.
struct CTCBase;
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};

#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap::LowerBound const 0x004365B0 (STLport lower_bound over the 8-byte
// {type, component} entries): VC7.1 then knows the key's address does not escape.
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline bool GetInterface(const T** ppOut, long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *ppOut = (const T*)((it == map->last || type < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    bool IsUnderScriptSupervision() const;                  // 0x004C7E90
};
struct CThingAICreature : CThing {
    unsigned char _pad4c[0x1c9 - 0x4c];
    unsigned char AllowedToFollowHero : 1;                  // +0x1c9 bit 0
};
struct CTCHeroStats {
    unsigned char _pad00[0x70];
    long RenownLevel;                                       // +0x70
    long GetMaxBoastingCrowdForRenownLevel(long level) const;   // 0x0057AB14
};
struct CTCCreatureOpinionOfHero {
    bool IsWillingToStartFollowingHero() const;             // 0x006818A0
    bool IsWillingToContinueFollowingHero() const;          // 0x006818F0
};

template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T> struct CRetailList {                     // STLport list: the header node pointer
    _List_node<T>* _M_node;
    long size() const
    {
        long count = 0;
        const _List_node<T>* it = _M_node->_M_next;
        while (it != _M_node) {
            it = it->_M_next;
            ++count;
        }
        return count;
    }
};
struct CFollowerPointer { void* vftable; void* PData; };    // CIntelligentPointer<CThing>

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
};
class CTCFollowed : public CTCBase {
public:
    CRetailList<CFollowerPointer> Followers;                // +0x0c
    bool IsAIFollower(CThingAICreature* creature) const;    // 0x006D4D70
    bool IsCreatureValidFollower(CThingAICreature* creature, bool* temporary) const;
};

bool CTCFollowed::IsCreatureValidFollower(CThingAICreature* creature, bool* temporary) const
{
    bool dummy;
    bool* isTemporary = &dummy;
    if (temporary)
        isTemporary = temporary;
    *isTemporary = !creature->IsUnderScriptSupervision() && ParentThing->HasTC(0x51) && creature->HasTC(0x4f);
    if (creature->AllowedToFollowHero) {
        if (*isTemporary) {
            const CTCHeroStats* stats;
            ParentThing->GetInterface(&stats, 4);
            if (!IsAIFollower(creature) && Followers.size() >= stats->GetMaxBoastingCrowdForRenownLevel(stats->RenownLevel))
                return false;
            const CTCCreatureOpinionOfHero* opinion;
            creature->GetInterface(&opinion, 0x4f);
            const CTCFollowed* followed;
            ParentThing->GetInterface(&followed, 0xbc);
            if (followed->IsAIFollower(creature))
                return opinion->IsWillingToContinueFollowingHero();
            return opinion->IsWillingToStartFollowingHero();
        }
        return true;
    }
    return false;
}
