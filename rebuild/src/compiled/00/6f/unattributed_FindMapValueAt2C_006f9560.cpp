// _unattributed::FindMapValueAt2C(unsigned long key) -- pointer to the 8-byte value of the entry keyed `key` in the sorted map at +0x2C, or null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F9560, oracle length 61 bytes.
// Callees: CVectorMap<unsigned long, 8-byte value>::LowerBound(const ulong&) @ 0x006FAF80 (manifest label names the
// CFactionManager instantiation; the label is propagated across byte-identical lower-bound bodies).
// Identity: the manifest label "C3DMeshStats::HasDummyObject" is propagated and rejected (this returns a
// pointer, not a bool). The neighbours (0x006F9430 NAnimationEvents::CManager::EventExists, 0x006F95A0
// CManager::GetExtraEventsBetweenTimes) suggest NAnimationEvents::CManager, but the PDB carries no layout
// for it, so the function lands as _unattributed with a descriptive leaf. The unsigned `jae` is the key compare.
//
// Retail bytes:  push ebx ; push esi ; push edi ; mov edi,ecx ; lea eax,[esp+10h] ; lea esi,[edi+2Ch] ; push eax ; mov ecx,esi ; xor ebx,ebx ;
//                call LowerBound ; mov esi,[esi+4] ; cmp eax,esi ; je E ; mov ecx,[esp+10h] ; cmp ecx,[eax] ; jae K ; E: mov eax,esi ;
//                K: cmp eax,[edi+30h] ; je N ; pop edi ; pop esi ; add eax,4 ; pop ebx ; ret 4 ; N: pop edi ; pop esi ; mov eax,ebx ; pop ebx ; ret 4
// Shape: the map's Find() is inlined (lower bound, then end() when the key sorts before the element),
// the null result is kept in a register cleared before the call (`xor ebx,ebx` hoisted), and a hit
// returns the address of the entry's value (+4).
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
    SEntry* LowerBound(const unsigned long& key);   // 0x006FAF80
    SEntry* End() { return m_pLast; }
    SEntry* Find(const unsigned long& key)
    {
        SEntry* it = LowerBound(key);
        if (it == End() || key < it->first)
            it = End();
        return it;
    }
};

struct CUnattributed_006f9560 {
    unsigned char _pad_0x00[0x2c];
    CEntryMap     m_entries;               // +0x2C
    void* FindMapValueAt2C(unsigned long key);
};

void* CUnattributed_006f9560::FindMapValueAt2C(unsigned long key)
{
    void* result = 0;
    SEntry* it = m_entries.Find(key);
    if (it != m_entries.End())
        result = it->second;
    return result;
}