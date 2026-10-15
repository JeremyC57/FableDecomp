// CVectorMap_Morph24::Add(const EHeroMorphType& key, const SMorphEntry24& value) -- CVectorMap<EHeroMorphType, 24-byte entry>: appends a {key, entry} pair to the vector at +0 and marks the map dirty (+0xd, re-sorted on the next lookup)
// Leaf: ?Add@CVectorMap_Morph24
// Retail: Fable.exe @ 0x004D1EB0, 84 bytes (+ int3 padding). Callee: the pair vector's push_back @ 0x004D18E0 (manifest "OnInitLeaf" labels are propagated).
// Label: the manifest's "CTextureMorphs::Add" is propagated: the body is a CVectorMap<K,V>::Add instantiation (family
// 0x006FCF40 / 0x0077F270: vector at +0, empty CKeyPairCompareLess +0xc, Dirty +0xd); no caller references it. The value
// type is a 24-byte record (PDB CVectorMap<EHeroMorphType, CTextureMorphEntry> is CTextureMorphs::Morphs; the entry
// type here is named by its size because the instantiation could not be tied to one map).
// Shape: the pair temporary (key + entry) is built by an inline pair constructor with a POD entry copied dword by dword
// through two alternating registers, interleaved with the push_back set-up; no destructor, then the Dirty store.
struct SMorphEntry24 {
    long m_words[24 / 4];
};

struct SMorphPair24 {
    long             first;                       // EHeroMorphType
    SMorphEntry24   second;
    SMorphPair24(const long& key, const SMorphEntry24& value) : first(key), second(value) { }
};

struct CMorphPairVector24 {
    SMorphPair24* m_pFirst;
    SMorphPair24* m_pLast;
    SMorphPair24* m_pEnd;
    void PushBack(const SMorphPair24& pair);   // 0x004D18E0
};

struct CVectorMap_Morph24 {
    CMorphPairVector24 m_vector;             // +0x00
    unsigned char      Compare;                    // +0x0c (empty CKeyPairCompareLess)
    bool               Dirty;                      // +0x0d
    void Add(const long& key, const SMorphEntry24& value);
};

void CVectorMap_Morph24::Add(const long& key, const SMorphEntry24& value)
{
    {
        SMorphPair24 pair(key, value);
        m_vector.PushBack(pair);
    }
    Dirty = true;
}
