// CVectorMap_Morph20::Add(const EHeroMorphType& key, const SMorphEntry20& value) -- CVectorMap<EHeroMorphType, 20-byte entry>: appends a {key, entry} pair to the vector at +0 and marks the map dirty (+0xd, re-sorted on the next lookup)
// Leaf: ?Add@CVectorMap_Morph20
// Retail: Fable.exe @ 0x0081D7D0, 77 bytes (+ int3 padding). Callee: the pair vector's push_back @ 0x0081D620 (manifest "OnInitLeaf" labels are propagated).
// Label: the manifest's "CTextureMorphs::Add" is propagated: the body is a CVectorMap<K,V>::Add instantiation (family
// 0x006FCF40 / 0x0077F270: vector at +0, empty CKeyPairCompareLess +0xc, Dirty +0xd); no caller references it. The value
// type is a 20-byte record (PDB CVectorMap<EHeroMorphType, CTextureMorphEntry> is CTextureMorphs::Morphs; the entry
// type here is named by its size because the instantiation could not be tied to one map).
// Shape: the pair temporary (key + entry) is built by an inline pair constructor with a POD entry copied dword by dword
// through two alternating registers, interleaved with the push_back set-up; no destructor, then the Dirty store.
struct SMorphEntry20 {
    long m_words[20 / 4];
};

struct SMorphPair20 {
    long             first;                       // EHeroMorphType
    SMorphEntry20   second;
    SMorphPair20(const long& key, const SMorphEntry20& value) : first(key), second(value) { }
};

struct CMorphPairVector20 {
    SMorphPair20* m_pFirst;
    SMorphPair20* m_pLast;
    SMorphPair20* m_pEnd;
    void PushBack(const SMorphPair20& pair);   // 0x0081D620
};

struct CVectorMap_Morph20 {
    CMorphPairVector20 m_vector;             // +0x00
    unsigned char      Compare;                    // +0x0c (empty CKeyPairCompareLess)
    bool               Dirty;                      // +0x0d
    void Add(const long& key, const SMorphEntry20& value);
};

void CVectorMap_Morph20::Add(const long& key, const SMorphEntry20& value)
{
    {
        SMorphPair20 pair(key, value);
        m_vector.PushBack(pair);
    }
    Dirty = true;
}
