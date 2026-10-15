// NGameText::CDataBank::LookupValue(unsigned long key) -- find the text entry whose CRC key matches in the bank's sorted key/entry vector map at +0xd0; 0 when absent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CC410, length 64 bytes.
// Callees: CVectorMap<unsigned long, CDefString>::LowerBound(const unsigned long&) @ 0x009D20D0 (an
// out-of-line binary search over the 8-byte {key, value} pairs, returning the element pointer).
// Owner: the engine text bank (0x013B86A0 -> +0x14, NGameText::CDataBank per rtti-vtables.tsv); the manifest
// label "CVectorMap::LookupValue" names the inlined member. The map is {first +0xd0, last +0xd4, end +0xd8}.
// Shape (wave 24p's map-find rule): the inline `Find` is the ternary `(it == end || key < it->first) ? end : it`
// with the by-value key of LookupValue bound by reference (its slot address is what LowerBound receives:
// `lea eax,[esp+0Ch]`; a by-value Find parameter is copied back into the same slot first), `last`
// cached in esi across the call for the first compare and RE-READ (`cmp eax,[edi+0D4h]`) by the caller's
// `it == End()` test; `cmp ecx,[eax] ; jae` is `key < it->first` unsigned; the value word (+4) is returned
// in eax, `ret 4`.
namespace NGameText {

struct SKeyEntry {                             // CVectorMap<unsigned long, CDefString> element (8 bytes)
    unsigned long m_key;
    void*         m_value;                     // the CDefString word
};

struct CKeyEntryVectorMap {                    // CVectorMap: a sorted 12-byte retail vector
    SKeyEntry* m_pFirst;                       // +0x00
    SKeyEntry* m_pLast;                        // +0x04
    SKeyEntry* m_pEnd;                         // +0x08
    SKeyEntry* LowerBound(const unsigned long& key);        // 0x009D20D0
    SKeyEntry* End() { return m_pLast; }
    SKeyEntry* Find(const unsigned long& key)      // by reference: a by-value parameter is re-homed in the caller's slot
    {
        SKeyEntry* it = LowerBound(key);
        return (it == End() || key < it->m_key) ? End() : it;
    }
};

struct CDataBank {
    unsigned char       _pad00[0xd0];
    CKeyEntryVectorMap  m_entries;             // +0xd0
    void* LookupValue(unsigned long key);
};

void* CDataBank::LookupValue(unsigned long key)
{
    SKeyEntry* it = m_entries.Find(key);
    if (it != m_entries.End())
        return it->m_value;
    return 0;
}

}
