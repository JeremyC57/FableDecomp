// _global::Construct_PairCharStringContainedBank_009aa520(std::pair<const CCharString,NBankFileManager::CContainedBank>*, const std::pair<const CCharString,NBankFileManager::CContainedBank>&) -- STLport _Construct: placement copy-construct one map node value
// Leaf: Construct_PairCharStringContainedBank_009aa520
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009AA520, length 53 bytes.
// Callee: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Shape: /Gr TU (__fastcall: destination in ecx, source in edx). `new (p) T(v)` null-tests the placement result, then the
// implicit pair copy constructor: the CCharString copy constructor, then the 20-byte POD CContainedBank copied word by
// word through both pointers re-based to +4 (`add edi,4 ; add esi,4`).
// The manifest label `_Cons_val<allocator<...>>` is propagated; the body is STLport _Construct.
inline void* operator new(unsigned int, void* p) { return p; }

struct CCharString {
    void* m_pData;                               // +0x00
    CCharString(const CCharString& other);       // 0x0099EC30
};
namespace NBankFileManager {
struct CContainedBank {                          // 20 bytes, POD
    long m_words[5];
};
}
struct CCharStringBankPair {                     // std::pair<const CCharString, NBankFileManager::CContainedBank>
    const CCharString first;                     // +0x00
    NBankFileManager::CContainedBank second;     // +0x04
};

void __fastcall Construct_PairCharStringContainedBank_009aa520(CCharStringBankPair* p, const CCharStringBankPair& value)
{
    new (p) CCharStringBankPair(value);
}