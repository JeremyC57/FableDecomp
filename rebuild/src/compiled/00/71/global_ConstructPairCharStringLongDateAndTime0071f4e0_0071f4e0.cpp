// _global::Construct_PairCharStringLongDateAndTime_0071f4e0(std::pair<const CCharString,std::pair<long,CDateAndTime> >*, const std::pair<const CCharString,std::pair<long,CDateAndTime> >&) -- STLport _Construct: placement copy-construct one map node value
// Leaf: Construct_PairCharStringLongDateAndTime_0071f4e0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071F4E0, length 38 bytes.
// Callee: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Shape: /Gr TU (__fastcall: destination in ecx, source in edx; esi/edi kept for the block move). `new (p) T(v)`
// null-tests the placement result, then the implicit pair copy constructor: the CCharString copy constructor, the long
// (fixed offset +4), and the 28-byte POD CDateAndTime as one 7-dword `rep movsd` through bumped pointers (+8).
// The inner pair needs a user memberwise copy constructor: the implicit one copies the whole 32 bytes as one block.
// The manifest label `_Cons_val<allocator<...>>` is propagated; the body is STLport _Construct.
inline void* operator new(unsigned int, void* p) { return p; }

struct CCharString {
    void* m_pData;                               // +0x00
    CCharString(const CCharString& other);       // 0x0099EC30
};
struct CDateAndTime {                            // 28 bytes, POD
    long m_words[7];
};
struct CLongDateAndTimePair {                    // std::pair<long, CDateAndTime>
    long first;                                  // +0x00
    CDateAndTime second;                         // +0x04
    CLongDateAndTimePair(const CLongDateAndTimePair& o) : first(o.first), second(o.second) { }   // memberwise (STLport pair copy)
};
struct CCharStringLongDatePair {                 // std::pair<const CCharString, std::pair<long, CDateAndTime> >
    const CCharString first;                     // +0x00
    CLongDateAndTimePair second;                 // +0x04
};

void __fastcall Construct_PairCharStringLongDateAndTime_0071f4e0(CCharStringLongDatePair* p, const CCharStringLongDatePair& value)
{
    new (p) CCharStringLongDatePair(value);
}