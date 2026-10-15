// CThingFilter_IsInFaction::operator()(const CThing& thing) const -- whether the thing's faction component (kind 0x57) lists the filter's faction; false without one
// Leaf: ??RCThingFilter_IsInFaction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A8880, length 78 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (const instantiation), the component's
// membership query @ 0x00778AB0 (searches its list at +0xc/+0x10 for the value; manifest "CHitParameters::
// IsDivineWrathStrike" is propagated), tail-jumped with the filter's word reusing the argument slot.
// Identity: the catalog's "CThingCreatureBase::IsBlocking" is a propagated label (Ego_r.pdb's IsBlocking is the
// argument-less 0x00664D00). This is a __thiscall functor (`ret 4`, callers set ecx to the filter object) whose one
// caller is GetPFirstThingInRoughArea<CThingFilter_IsAICreatureMatchingTypeAndFactionNearby> (0x0096F3B0); the class
// name is descriptive (real name unknown), the shape is the twin of CThingFilter_IsAlliedWithFaction 0x006C8C50.
// Layout (retail, local model): the thing's component-presence bitset at +0x20 (kind k = word k>>5, bit k&31) and the
// sorted {kind, component} vector map at +0x44 {first, last} (no engine header names them).
// Shape: the out-parameter GetInterface template on the const thing (the component comes back in eax: `mov eax,[eax+4]
// ; mov ecx,eax`), key homed in a frame/argument slot and passed by address to the const LowerBound.
// The filter's value is read into ecx before the component (`mov ecx,[edi]`), then stored over the argument slot for the
// tail jump: `return tc->IsMember(m_faction);` as the last statement.
struct CTCBase;

struct STCEntry {                               // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long     m_kind;
    CTCBase* m_pTC;
};

struct CTCMap {                                 // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const long* kind) const;        // 0x004365B0 (CVectorMap<ETCInterfaceType,...>::LowerBound)
};

struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                 // +0x20 which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap         m_tcs;                       // +0x44
    template<class T> bool GetInterface(T** ppOut, long kind) const   // the landed out-parameter GetInterface template
    {
        if (m_tcMask[kind >> 5] & (1UL << (kind & 31))) {
            long type = kind;
            const CTCMap* map = &m_tcs;
            STCEntry* it = map->LowerBound(&type);
            if (it == map->m_pLast || kind < it->m_kind)
                it = map->m_pLast;
            *ppOut = (T*)it->m_pTC;
            return true;
        }
        return false;
    }
};

struct CTCFactionMembership {                   // component kind 0x57
    bool IsMember(long faction) const;          // 0x00778AB0
};

struct CThingFilter_IsInFaction {
    long m_faction;                             // +0x00
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_IsInFaction::operator()(const CThing& thing) const
{
    CTCFactionMembership* membership;
    if (thing.GetInterface(&membership, 0x57))
        return membership->IsMember(m_faction);
    return false;
}
