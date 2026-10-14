// _unattributed::FindMapValueAt78(unsigned long key) -- pointer to the 8-byte value of the entry keyed `key` in the sorted map at +0x78, or null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009AD350, oracle length 54 bytes.
// Callees: CVectorMap<unsigned long, 8-byte value>::LowerBound(const ulong&) @ 0x009B2230 (manifest label names the
// CFactionManager instantiation; the label is propagated across byte-identical lower-bound bodies).
// Identity: the manifest label "C3DMeshStats::HasDummyObject" is propagated and rejected (this returns a
// pointer, not a bool). The neighbours are CDefinitionManager and CFactionDatabase code (0x009AD2E0
// CFactionDatabase::FindFactionBySymbolName); the PDB CFactionManager keeps its faction map at +0, not
// +0x78, so the function lands as _unattributed with a descriptive leaf. The unsigned `jae` is the key compare.
//
// Retail bytes:  push esi ; push edi ; mov edi,ecx ; lea eax,[esp+0Ch] ; lea esi,[edi+78h] ; push eax ; mov ecx,esi ; call LowerBound ;
//                mov esi,[esi+4] ; cmp eax,esi ; je E ; mov ecx,[esp+0Ch] ; cmp ecx,[eax] ; jae K ; E: mov eax,esi ; K: cmp eax,[edi+7Ch] ;
//                pop edi ; pop esi ; jne H ; xor eax,eax ; ret 4 ; H: add eax,4 ; ret 4
// Shape: the map's Find() is inlined (lower bound, then end() when the key sorts before the element);
// a miss returns null, a hit the address of the entry's value (+4). Unlike 0x006F9560 the null is
// produced at the return (`xor eax,eax` after the compare), the early-return form.
struct SEntry {                            // pair<unsigned long, 8-byte value>
    unsigned long first;
    unsigned char second[8];
};

struct CEntryMap {                         // retail CVectorMap<unsigned long, V>: sorted vector {first, last, end} + compare + dirty
    SEntry* m_pFirst;
    SEntry* m_pLast;
    SEntry* m_pEnd;
    bool    m_compare;
    bool    m_dirty;
    SEntry* LowerBound(const unsigned long& key);   // 0x009B2230
    SEntry* End() { return m_pLast; }
    SEntry* Find(const unsigned long& key)
    {
        SEntry* it = LowerBound(key);
        if (it == End() || key < it->first)
            it = End();
        return it;
    }
};

struct CUnattributed_009ad350 {
    unsigned char _pad_0x00[0x78];
    CEntryMap     m_entries;               // +0x78
    void* FindMapValueAt78(unsigned long key);
};

void* CUnattributed_009ad350::FindMapValueAt78(unsigned long key)
{
    SEntry* it = m_entries.Find(key);
    if (it == m_entries.End())
        return 0;
    return it->second;
}
