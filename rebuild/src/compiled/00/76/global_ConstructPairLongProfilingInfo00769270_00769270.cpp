// _global::Construct_PairLongProfilingInfo_00769270(std::pair<const long,NProfileTimer::CProfilingInfo>*, const std::pair<const long,NProfileTimer::CProfilingInfo>&) -- STLport _Construct: placement copy-construct one map<long,CProfilingInfo> node value
// Leaf: Construct_PairLongProfilingInfo_00769270
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00769270, length 41 bytes (+ int3 padding).
// Callee: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Shape: /Gr TU (__fastcall: destination in ecx, source in edx). `new (p) T(v)` null-tests the placement result, then the
// implicit pair copy constructor memberwise: the long key, the CProfilingInfo flag byte (+4), its name CCharString (+8,
// out-of-line copy constructor) and the trailing word (+0xC).
// The manifest label `_Cons_val<allocator<...>>` is propagated; the body is STLport _Construct. CProfilingInfo member
// names are descriptive.
inline void* operator new(unsigned int, void* p) { return p; }

class CCharString { public:
    void* m_pData;                               // +0x00
    CCharString(const CCharString& other);       // 0x0099EC30
};
namespace NProfileTimer {
class CProfilingInfo { public:                          // 12 bytes
    bool m_enabled;                              // +0x00
    CCharString m_name;                          // +0x04
    long m_time;                                 // +0x08
};
}
struct CLongProfilingPair {                      // std::pair<const long, NProfileTimer::CProfilingInfo>
    const long first;                            // +0x00
    NProfileTimer::CProfilingInfo second;        // +0x04
};

void __fastcall Construct_PairLongProfilingInfo_00769270(CLongProfilingPair* p, const CLongProfilingPair& value)
{
    new (p) CLongProfilingPair(value);
}