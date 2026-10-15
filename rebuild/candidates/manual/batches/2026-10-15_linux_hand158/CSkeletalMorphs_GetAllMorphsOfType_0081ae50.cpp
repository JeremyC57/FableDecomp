// CSkeletalMorphs::GetAllMorphsOfType(unsigned long type, vector<CSkeletalMorphs::CEntry>* out) -- copy every morph entry whose key equals type (the equal range of the sorted map) into out; false when there is none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081AE50, length 117 bytes (`ret 8`).
// Callees: the map's LowerBound @ 0x00819F70 and UpperBound @ 0x00819FD0 (key by reference; catalog labels name other
// CVectorMap instantiations -- byte-twin propagation), out->resize(n) @ 0x0081DF50 (catalog: vector<pair<CCharString,long>>::resize, propagated).
// Layout: as 0x0081ADC0 -- Morphs (12-byte vector of {key, CEntry}) at +0; CEntry is 8 bytes {long, ushort, bool}.
// Shape: the copy loop is signed (`jle`/`jl`), out's begin re-read each pass (`*(begin()+i)`), the source walked as a
// post-incremented pointer `(first++)->second` (puts `add ecx,0Ch` between the two dword copies); the false exit is the
// fall-out `return false` after the `if (first != last) {...}` block.
// Leaf: GetAllMorphsOfType
namespace NSkeletalMorphs {
struct CEntry {
    long           Morph;                           // +0x0
    unsigned short Index;                           // +0x4
    bool           Enabled;                         // +0x6
};
}
struct SMorphPair { unsigned long first; NSkeletalMorphs::CEntry second; };   // 12 bytes

struct CEntryVector {                               // vector<CSkeletalMorphs::CEntry>
    NSkeletalMorphs::CEntry* _M_start;
    NSkeletalMorphs::CEntry* _M_finish;
    NSkeletalMorphs::CEntry* _M_end_of_storage;
    NSkeletalMorphs::CEntry* begin() { return _M_start; }
    NSkeletalMorphs::CEntry& operator[](unsigned long n) { return *(begin() + n); }
    void resize(unsigned long n);                   // 0x0081DF50
};

struct CSkeletalMorphs {
    SMorphPair* _M_start;                           // Morphs (CVectorMap<unsigned long, CEntry>) at +0
    SMorphPair* _M_finish;
    SMorphPair* _M_end_of_storage;
    SMorphPair* LowerBound(const unsigned long& key);   // 0x00819F70
    SMorphPair* UpperBound(const unsigned long& key);   // 0x00819FD0
    bool GetAllMorphsOfType(unsigned long type, CEntryVector* out);
};

bool CSkeletalMorphs::GetAllMorphsOfType(unsigned long type, CEntryVector* out)
{
    SMorphPair* first = LowerBound(type);
    SMorphPair* last = UpperBound(type);
    if (first != last) {
        long count = last - first;
        out->resize(count);
        for (long i = 0; i < count; ++i)
            (*out)[i] = (first++)->second;
        return true;
    }
    return false;
}
