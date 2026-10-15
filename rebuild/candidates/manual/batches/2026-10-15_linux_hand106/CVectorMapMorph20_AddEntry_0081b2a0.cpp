// CVectorMap_Morph20::AddEntry(const SMorphEntry20& entry) -- CVectorMap<EHeroMorphType, 20-byte entry keyed by its first word>: appends {entry.key, entry}, marks the map dirty, then sorts the pair vector at once and clears the flag
// Leaf: ?AddEntry@CVectorMap_Morph20
// Retail: Fable.exe @ 0x0081B2A0, 93 bytes (+ int3 padding).
// Callees: the pair vector's push_back @ 0x0081D620 (shared with the Add instantiation 0x0081D7D0), the SGI
// sort(first, last, CKeyPairCompareLess) @ 0x0081E030 (a /Gr algorithm TU: first in ecx, last in edx, the 1-byte
// comparator pushed; 0x2AAAAAAB / 4 = the 0x18-byte pair stride; manifest "std::make_heap_Sift").
// Label: the manifest's "CTextureMorphs::Add" is propagated: this is a one-argument member of the same CVectorMap
// instantiation as 0x0081D7D0 (vector +0, Compare +0xc, Dirty +0xd); the pair's key is the entry's first word
// (`mov edx,ecx` stores the one load twice). Its neighbour 0x0081B240 has the same Add-then-sort tail. No caller
// references it, so the member is named for what it does.
// Shape: the inline Add (pair temporary, push_back, Dirty = true) followed by an inline Sort (sort over the vector with
// the comparator byte zero-extended into a register, Dirty = false); the Dirty = true store is kept before the call.
struct SMorphEntry20 {
    long m_words[20 / 4];                          // m_words[0] = the morph type (key)
};

struct SMorphPair20 {
    long          first;                           // EHeroMorphType
    SMorphEntry20 second;
    SMorphPair20(const long& key, const SMorphEntry20& value) : first(key), second(value) { }
};

struct CMorphPairVector20 {
    SMorphPair20* m_pFirst;
    SMorphPair20* m_pLast;
    SMorphPair20* m_pEnd;
    void PushBack(const SMorphPair20& pair);       // 0x0081D620
};

struct CKeyPairCompareLess { };

void __fastcall SortMorphPairs(SMorphPair20* first, SMorphPair20* last, CKeyPairCompareLess compare);   // 0x0081E030

struct CVectorMap_Morph20 {
    CMorphPairVector20  m_vector;                  // +0x00
    CKeyPairCompareLess Compare;                   // +0x0c (empty)
    bool                Dirty;                     // +0x0d

    void Add(const long& key, const SMorphEntry20& value)
    {
        {
            SMorphPair20 pair(key, value);
            m_vector.PushBack(pair);
        }
        Dirty = true;
    }
    void Sort()
    {
        SortMorphPairs(m_vector.m_pFirst, m_vector.m_pLast, Compare);
        Dirty = false;
    }
    void AddEntry(const SMorphEntry20& entry);
};

void CVectorMap_Morph20::AddEntry(const SMorphEntry20& entry)
{
    Add(entry.m_words[0], entry);
    Sort();
}
