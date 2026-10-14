// CTCCarrying::HasCarrySlot(long slot) -- is there an entry for the slot in the CarrySlots map?
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A11B0, oracle length 51 bytes.
// Callees: CVectorMap<long,CIntelligentPointer<CThing>>::LowerBound(const long&) @ 0x006A2A10 (manifest).
// Identity: the manifest label "C3DMeshStats::HasDummyObject" is propagated and rejected. The neighbours
// (0x006A1120 CTCCarrying::GetCarriedThing(long), 0x006A1330 CTCCarrying::IsSlotPrimarySlot(long)) and the PDB
// layout (struct_layouts_egor.tsv: CTCCarrying +0xC CarrySlots = CVectorMap<long,CIntelligentPointer<CThing>>)
// fix the class; the signed `jge` is the long key compare. The leaf is descriptive (the PDB has no function names).
//
// Retail bytes:  push esi ; push edi ; mov edi,ecx ; lea eax,[esp+0Ch] ; lea esi,[edi+0Ch] ; push eax ; mov ecx,esi ; call LowerBound ;
//                mov esi,[esi+4] ; cmp eax,esi ; je E ; mov ecx,[esp+0Ch] ; cmp ecx,[eax] ; jge K ; E: mov eax,esi ; K: mov ecx,[edi+10h] ;
//                xor edx,edx ; cmp eax,ecx ; setne dl ; pop edi ; mov al,dl ; pop esi ; ret 4
// Shape: the map's Find() is inlined -- lower bound, then "not found" when the bound is end() or the key
// sorts before the element's key -- and the result is compared with end() (re-read from the map, the
// retail vector's `last` pointer at +4) to give the bool.
struct CIntelligentPointer { void* m_vtable; void* m_pThing; };

struct SCarrySlotEntry {                   // pair<long, CIntelligentPointer<CThing>>
    long                first;
    CIntelligentPointer second;
};

struct CCarrySlotMap {                     // retail CVectorMap<long,CIntelligentPointer<CThing>>: sorted vector + compare + dirty
    SCarrySlotEntry* m_pFirst;
    SCarrySlotEntry* m_pLast;
    SCarrySlotEntry* m_pEnd;
    bool             m_compare;
    bool             m_dirty;
    SCarrySlotEntry* LowerBound(const long& key);   // 0x006A2A10
    SCarrySlotEntry* End() { return m_pLast; }
    SCarrySlotEntry* Find(const long& key)
    {
        SCarrySlotEntry* it = LowerBound(key);
        if (it == End() || key < it->first)
            it = End();
        return it;
    }
};

struct CTCCarrying {
    virtual void v00();
    unsigned char _pad_0x04[0x0c - 0x04];  // +0x4 .. +0xC: CTCBase state
    CCarrySlotMap CarrySlots;              // +0xC
    bool HasCarrySlot(long slot);
};

bool CTCCarrying::HasCarrySlot(long slot)
{
    return CarrySlots.Find(slot) != CarrySlots.End();
}