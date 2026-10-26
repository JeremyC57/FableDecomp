// CVectorMap<long, CIntelligentPointer<CThing> >::FindValue(const CIntelligentPointer<CThing>&) -- linear search of the sorted pair vector for the entry whose pointer refers to the same item as the argument; the entry, or end() when none does
// Leaf: FindValue
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A2B50, length 66 bytes (+ int3 padding). No callers in the
// shipped build (an unreferenced template instantiation).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the entry's pointer, then the argument's).
// The chunk label "CTCCreatureGenerator::RemoveCreatureTriggerer" is propagated: `this` is the vector
// itself ({first, last} at +0/+4) and the 12-byte element is pair<long, CIntelligentPointer<CThing> >
// (key +0, pointer +4), the element of the CVectorMap<long, ...> whose LowerBound instantiations
// (0x006A29B0, 0x006A2A10) sit next to it (CTCCarrying::CarrySlots has this type).
// Shape: a top-tested loop over a pointer local (esi) with the argument in ebx and the entry's item kept
// in ebp across the second call; `last` is re-read from `this` after every call and once more for the
// not-found return; the found entry returns from esi.
class CBaseObject;

struct CBaseIntelligentPointer {                            // 8 bytes: vtable + item link
    virtual void v00();
    void* m_pItem;                                          // +4
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};

struct SCarrySlot {                                         // pair<long, CIntelligentPointer<CThing> >: 12 bytes
    long                    first;                          // +0
    CBaseIntelligentPointer second;                         // +4
};

struct CVectorMap_LongThingPointer {                        // CVectorMap<long, CIntelligentPointer<CThing> >
    SCarrySlot* First;                                      // +0
    SCarrySlot* Last;                                       // +4
    SCarrySlot* FindValue(const CBaseIntelligentPointer& value);
};

SCarrySlot* CVectorMap_LongThingPointer::FindValue(const CBaseIntelligentPointer& value)
{
    for (SCarrySlot* it = First; it != Last; ++it)
        if (it->second.GetPItem() == value.GetPItem())
            return it;
    return Last;
}