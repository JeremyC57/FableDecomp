// _global::Construct_PairCharStringContainedBank_008eba50(std::pair<const CCharString,NBankFileManager::CContainedBank>*, const std::pair<const CCharString,NBankFileManager::CContainedBank>&) -- STLport _Construct: placement copy-construct one map node value whose value leads with a three-word POD block
// Leaf: Construct_PairCharStringContainedBank_008eba50
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EBA50, length 53 bytes (+ int3 padding).
// Callee: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Shape: /Gr TU (__fastcall: destination in ecx, source in edx). `new (p) T(v)` null-tests the placement result, then the
// implicit memberwise copy: the CCharString copy constructor; the 12-byte POD block at +4 is copied through `lea`
// pointers to both copies (eax/ecx, three word moves through edx), then the two trailing words at fixed offsets
// +0x10/+0x14 straight off the element pointers.
// Model: the value's members are laid out flat in the element. Nesting them in a CContainedBank struct does not
// reproduce the shape: a trivial nested struct is copied as one 20-byte block (the sibling 0x009AA520 shape, bumped
// pointers), a nested struct with a user/non-trivial copy re-bases the trailing words onto the nested pointers, and a
// user copy constructor on the block flattens it to fixed offsets.
// The manifest label `_Cons_val<allocator<pair<CCharString,CContainedBank>>>` is propagated; the body is STLport
// _Construct. Member names are descriptive.
inline void* operator new(unsigned int, void* p) { return p; }

class CCharString { public:
    void* m_pData;                               // +0x00
    CCharString(const CCharString& other);       // 0x0099EC30
};
struct SThreeWords {                             // 12 bytes, POD (block copy)
    long m_a, m_b, m_c;
};
struct CCharStringBankPair {                     // std::pair<const CCharString, NBankFileManager::CContainedBank>, flat
    const CCharString first;                     // +0x00
    SThreeWords m_range;                         // +0x04 CContainedBank leading block
    long m_first;                                // +0x10
    long m_second;                               // +0x14
};

void __fastcall Construct_PairCharStringContainedBank_008eba50(CCharStringBankPair* p, const CCharStringBankPair& value)
{
    new (p) CCharStringBankPair(value);
}