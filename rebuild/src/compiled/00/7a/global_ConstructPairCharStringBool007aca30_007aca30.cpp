// _global::Construct_PairCharStringBool_007aca30(std::pair<const CCharString,bool>*, const std::pair<const CCharString,bool>&) -- STLport _Construct: placement copy-construct one map<CCharString,bool> node value
// Leaf: Construct_PairCharStringBool_007aca30
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007ACA30, length 25 bytes.
// Callee: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Shape: /Gr TU (__fastcall: destination in ecx, source in edx); `new (p) T(v)` null-tests the placement result, then the
// implicit pair copy constructor: the CCharString copy constructor (thiscall, ecx = destination, source pushed) and the
// bool byte. The manifest label `_Cons_val<allocator<...>>` is propagated; the body is STLport _Construct.
inline void* operator new(unsigned int, void* p) { return p; }

class CCharString { public:
    void* m_pData;                               // +0x00 (shared string body)
    CCharString(const CCharString& other);       // 0x0099EC30
};
struct CCharStringBoolPair {                     // std::pair<const CCharString, bool>
    const CCharString first;                     // +0x00
    bool second;                                 // +0x04
};

void __fastcall Construct_PairCharStringBool_007aca30(CCharStringBoolPair* p, const CCharStringBoolPair& value)
{
    new (p) CCharStringBoolPair(value);
}