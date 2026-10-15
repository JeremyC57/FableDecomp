// CThingFilter_IsAlliedWithFaction::operator()(const CThing& thing) const -- whether the thing's enemy component (kind 0x49) says it is allied with the filter's faction; false without one
// Leaf: ??RCThingFilter_IsAlliedWithFaction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C8C50, length 77 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (const instantiation),
// CTCEnemy::IsAlliedWithFaction @ 0x0076C7F0 (tail-jumped with the filter's faction reusing the argument slot).
// Identity: the catalog's "CThingCreatureBase::IsBlocking" is a propagated label; the two callers are
// GetPBestThing<CFilter_Bind2<CThingFilter_IsNot, CThingFilter_IsAlliedWithFaction>, ...> (0x00986DF5) and
// CAIStateGroup_OpinionReactionRoot::MarriageCommentIsRunnable, both with ecx = the filter object (`ret 4` __thiscall).
// Layout (retail, local model): the thing's component-presence bitset at +0x20 (kind k = word k>>5, bit k&31) and the
// sorted {kind, component} vector map at +0x44 {first, last} (no engine header names them).
// Shape: the out-parameter GetInterface template on the const thing (the component comes back in eax: `mov eax,[eax+4]
// ; mov ecx,eax`), key homed in a frame/argument slot and passed by address to the const LowerBound.
// The filter's faction is read into ecx before the component (`mov ecx,[edi]`), then stored over the argument slot for the
// tail jump: `return enemy->IsAlliedWithFaction(m_faction);` as the last statement; kind 0x49 = `test ch,2` on word 2.
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

struct CTCEnemy {                               // component kind 0x49
    bool IsAlliedWithFaction(long faction) const;   // 0x0076C7F0
};

struct CThingFilter_IsAlliedWithFaction {
    long m_faction;                             // +0x00
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_IsAlliedWithFaction::operator()(const CThing& thing) const
{
    CTCEnemy* enemy;
    if (thing.GetInterface(&enemy, 0x49))
        return enemy->IsAlliedWithFaction(m_faction);
    return false;
}
