// _global::Construct_PairSkeletalMorphEntryFloat_005b1b20(std::pair<CSkeletalMorphs::CEntry,float>*, const std::pair<CSkeletalMorphs::CEntry,float>&) -- STLport _Construct: placement copy-construct one 12-byte pair<CEntry,float>
// Grade: DIFFER(ours v retail) -- 21v21: the third word moves through edx (`mov edx,[edx+8] ; mov [ecx+8],edx`) where retail reuses eax
// Leaf: Construct_PairSkeletalMorphEntryFloat_005b1b20
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B1B20, length 21 bytes.
// Shape: /Gr TU (__fastcall: destination in ecx, source in edx, no callee-saved registers). `new (p) T(v)` null-tests
// the placement result, then the implicit copy is three word moves through eax at fixed offsets.
// The manifest label `_Cons_val<allocator<...>>` is propagated; the body is STLport _Construct. CEntry is modelled by its
// size (two words).
inline void* operator new(unsigned int, void* p) { return p; }

struct CSkeletalMorphs {
    struct CEntry {                              // 8 bytes, POD
        long m_morph;                            // +0x00
        long m_bone;                             // +0x04
    };
};
struct CSkeletalEntryFloatPair {                 // std::pair<CSkeletalMorphs::CEntry, float>
    CSkeletalMorphs::CEntry first;               // +0x00
    float second;                                // +0x08
};

void __fastcall Construct_PairSkeletalMorphEntryFloat_005b1b20(CSkeletalEntryFloatPair* p, const CSkeletalEntryFloatPair& value)
{
    new (p) CSkeletalEntryFloatPair(value);
}
