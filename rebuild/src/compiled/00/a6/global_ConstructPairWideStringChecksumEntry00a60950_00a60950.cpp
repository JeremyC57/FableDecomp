// _global::Construct_PairWideStringChecksumEntry_00a60950(std::pair<const CWideString,CChecksumCacheFile::CEntry>*, const std::pair<const CWideString,CChecksumCacheFile::CEntry>&) -- STLport _Construct: placement copy-construct one map<CWideString,CEntry> node value
// Leaf: Construct_PairWideStringChecksumEntry_00a60950
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A60950, length 32 bytes.
// Callee: CWideString::CWideString(const CWideString&) @ 0x0099B720.
// Shape: /Gr TU (__fastcall: destination in ecx, source in edx; esi/edi kept for the block move). `new (p) T(v)`
// null-tests the placement result, then the implicit pair copy constructor: the CWideString copy constructor and the
// 32-byte POD CEntry as one 8-dword `rep movsd` through bumped pointers (+4).
// The manifest label `_Cons_val<allocator<...>>` is propagated; the body is STLport _Construct. CEntry is modelled by
// its size (8 words).
inline void* operator new(unsigned int, void* p) { return p; }

class CWideString { public:
    void* m_pData;                               // +0x00
    CWideString(const CWideString& other);       // 0x0099B720
};
struct CChecksumCacheFile {
    struct CEntry {                              // 32 bytes, POD
        long m_words[8];
    };
};
struct CWideStringEntryPair {                    // std::pair<const CWideString, CChecksumCacheFile::CEntry>
    const CWideString first;                     // +0x00
    CChecksumCacheFile::CEntry second;           // +0x04
};

void __fastcall Construct_PairWideStringChecksumEntry_00a60950(CWideStringEntryPair* p, const CWideStringEntryPair& value)
{
    new (p) CWideStringEntryPair(value);
}