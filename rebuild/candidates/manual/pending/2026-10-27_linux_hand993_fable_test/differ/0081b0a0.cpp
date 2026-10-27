// CTextureMorphs::GetAllMorphsOfType(EHeroMorphType type, CArray<CTextureMorphEntry>& out) -- copies every morph entry keyed by the given type (the sorted vector map's lower..upper bound range) into the output array, sized first with blank entries; false when the type has none
// Grade: DIFFER(178v177) -- 178v177: zero constant kept in ebx across the resize call (`cmp esi,ebx`) with out in ebp, retail zero in edx / out in ebx / copy temp ebp pushed inside the loop; body otherwise identical
// Leaf: GetAllMorphsOfType
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081B0A0, 177 bytes (+15 int3). `ret 8`, bool in al.
// Callees: CVectorMap<EHeroMorphType,CTextureMorphEntry>::LowerBound(const EHeroMorphType*) 0x00819D50 and UpperBound
//          0x00819DB0 (the key by address: the parameter's own slot; the manifest labels name other instantiations),
//          CArray<CTextureMorphEntry>::resize(unsigned long, const CTextureMorphEntry&) 0x0081DAA0 (manifest
//          "vector<CBrainBehaviour>::resize" is propagated).
// Shape: both bounds taken before the range test; the count is the signed 24-byte element distance (0x2AAAAAAB multiply,
//        `sar edx,2`, sign fix); the blank entry is a zeroed frame record built with one zero register after the count is
//        known and passed by reference; the copy loop reads the array start afresh each pass (`mov edx,[ebx]`, the byte
//        offset kept in ecx -- STLport `*(begin() + i)`), steps the map iterator by 0x18 and copies the five words of the
//        value through one register.
// Layout: Ego_r.pdb CTextureMorphs: Morphs +0x00 (CVectorMap: a sorted vector of {EHeroMorphType, CTextureMorphEntry} pairs,
//         24 bytes each); CTextureMorphEntry (20 bytes): MorphType, TextureLayer, ReplacingTextureIndex, BankIndex, Blend.
typedef long EHeroMorphType;
typedef long ECompositeBlendType;
struct CTextureMorphEntry {
    EHeroMorphType      MorphType;              // +0x00
    long                TextureLayer;           // +0x04
    long                ReplacingTextureIndex;  // +0x08
    long                BankIndex;              // +0x0c
    ECompositeBlendType Blend;                  // +0x10
    CTextureMorphEntry() : MorphType(0), TextureLayer(0), ReplacingTextureIndex(0), BankIndex(0), Blend(0) { }
};
struct SMorphPair {                             // std::pair<EHeroMorphType, CTextureMorphEntry>
    EHeroMorphType     first;
    CTextureMorphEntry second;
};
struct CMorphMap {                              // CVectorMap<EHeroMorphType, CTextureMorphEntry, CKeyPairCompareLess<...>>
    SMorphPair* m_first;
    SMorphPair* m_last;
    SMorphPair* m_end;
    bool        m_sorted;
    const SMorphPair* LowerBound(const EHeroMorphType* key);   // 0x00819D50
    const SMorphPair* UpperBound(const EHeroMorphType* key);   // 0x00819DB0
};
struct CMorphArray {                            // CArray<CTextureMorphEntry> = STLport vector (12 bytes)
    CTextureMorphEntry* _M_start;
    CTextureMorphEntry* _M_finish;
    CTextureMorphEntry* _M_end_of_storage;
    CTextureMorphEntry* begin() const { return _M_start; }
    void resize(unsigned long count, const CTextureMorphEntry& value);   // 0x0081DAA0
    void resize(unsigned long count) { resize(count, CTextureMorphEntry()); }   // STLport resize(n): the default-constructed temporary built inside
};
struct CTextureMorphs {
    CMorphMap Morphs;                           // +0x00
    bool GetAllMorphsOfType(EHeroMorphType type, CMorphArray& out);
};
bool CTextureMorphs::GetAllMorphsOfType(EHeroMorphType type, CMorphArray& out)
{
    const SMorphPair* first = Morphs.LowerBound(&type);
    const SMorphPair* last = Morphs.UpperBound(&type);
    if (first != last) {
        long count = last - first;
        out.resize(count);
        for (long i = 0; i < count; ++i)
            *(out.begin() + i) = first[i].second;
        return true;
    }
    return false;
}
