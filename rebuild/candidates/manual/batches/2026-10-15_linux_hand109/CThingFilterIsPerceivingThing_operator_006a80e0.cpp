// CThingFilter_IsPerceivingThing::operator()(const CThing& thing) const -- whether the thing's perception component (kind 0x34) is perceiving the filter's thing
// Leaf: ??RCThingFilter_IsPerceivingThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A80E0, length 82 bytes.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (const instantiation), the component's
// perceived-thing getter @ 0x007446A0 (`add ecx,0Ch ; jmp CIntelligentPointer::Get`; manifest "_Cons_val<...>" propagated).
// Identity: the catalog's "CThingDeadCreature::WorldUpdateBulletTime" is a propagated label; two of the three callers sit in
// GetAllTCsInRoughArea<CTCPerceiveThing, CFilter_Bind2<CThingFilter_IsPerceivingThing, CThingFilter_IsNot>, ...>
// (0x0084BC0D, 0x0091BBF5) with ecx = the filter object; Ego_r.pdb has `??RCThingFilter_IsPerceivingThing@@QBE_NABVCThing@@@Z`.
// Layout (retail, local model): the thing's component-presence bitset at +0x20 (kind k = word k>>5, bit k&31) and the
// sorted {kind, component} vector map at +0x44 {first, last} (no engine header names them).
// Shape: the out-parameter GetInterface template on the const thing (the component comes back in eax: `mov eax,[eax+4]
// ; mov ecx,eax`), key homed in a frame/argument slot and passed by address to the const LowerBound.
// The component local is NOT initialised: when the bit is clear retail calls the getter on the stale argument slot
// (`mov eax,[esp+0Ch]`), exactly the landed CThingNoise::PassToNoiseComponent 0x008386E0 shape; the comparison with the
// filter's thing is `getter() == m_thing` as a full-register result (`sub eax,[edi] ; neg ; sbb eax,eax ; inc eax`).
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

struct CTCPerceiveThing {                       // component kind 0x34
    const CThing* GetPerceivedThing() const;    // 0x007446A0
};

struct CThingFilter_IsPerceivingThing {
    const CThing* m_thing;                      // +0x00
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_IsPerceivingThing::operator()(const CThing& thing) const
{
    CTCPerceiveThing* perceive;
    thing.GetInterface(&perceive, 0x34);
    return perceive->GetPerceivedThing() == m_thing;
}
