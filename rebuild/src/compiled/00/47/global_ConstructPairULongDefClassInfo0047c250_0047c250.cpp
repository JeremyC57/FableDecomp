// _global::Construct_PairULongDefClassInfo_0047c250(std::pair<const unsigned long,CDefinitionManager::CDefClassInfo>*, const std::pair<const unsigned long,CDefinitionManager::CDefClassInfo>&) -- STLport _Construct: placement copy-construct one map<unsigned long,CDefClassInfo> node value
// Leaf: Construct_PairULongDefClassInfo_0047c250
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047C250, length 47 bytes.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, the copy constructor of the 12-byte container
//          member at +0xC @ 0x0047BA50.
// Shape: /Gr TU (__fastcall: destination in ecx, source in edx). `new (p) T(v)` null-tests the placement result, then the
// implicit memberwise copy, every member at a fixed offset from the two element pointers: the unsigned long key, the
// CDefClassInfo name CCharString (+4), the word at +8 and the out-of-line copy of the container at +0xC (source
// pointer bumped, `add edi,0Ch ; push edi`).
// Model: the CDefClassInfo members are laid out flat in the element. A nested CDefClassInfo struct (implicit, user or
// pair-level copy constructor) makes the lane re-base both pointers onto the nested object (`lea esi,[edx+4]`) and
// drops the esi/edi prologue that retail keeps.
// The manifest label `_Cons_val<allocator<...>>` is propagated; the body is STLport _Construct. Member names are
// descriptive.
inline void* operator new(unsigned int, void* p) { return p; }

class CCharString { public:
    void* m_pData;                               // +0x00
    CCharString(const CCharString& other);       // 0x0099EC30
};
struct CDefInfoContainer {                       // 12-byte container (vector-like)
    void* m_first;
    void* m_last;
    void* m_end;
    CDefInfoContainer(const CDefInfoContainer& other);   // 0x0047BA50
};
struct CULongDefClassInfoPair {                  // std::pair<const unsigned long, CDefinitionManager::CDefClassInfo>, flat
    const unsigned long first;                   // +0x00
    CCharString m_name;                          // +0x04 CDefClassInfo
    long m_index;                                // +0x08
    CDefInfoContainer m_defs;                    // +0x0c
};

void __fastcall Construct_PairULongDefClassInfo_0047c250(CULongDefClassInfoPair* p, const CULongDefClassInfoPair& value)
{
    new (p) CULongDefClassInfoPair(value);
}