// _STL::pair::pair(const pair& other) -- copy constructor of pair<C3DVector, CCharString>: the vector memberwise, then the string's copy constructor
// Leaf: ??0?$pair@VC3DVector@@VCCharString@@@_STL@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00654DD4, 41 bytes, __fastcall `ret 4`, returns `this` (size-optimised TU: odd address).
// Identity: called by the STLport `_Construct(pair*, const pair&)` helpers of the same TU (0x006555C3: `if (p) new (p) T(v)`,
//   the uninitialized_copy loop 0x006555CE stepping 0x10 bytes, 0x0065560D, 0x00656DBC, 0x00656F07): a 16-byte element
//   whose first 12 bytes are copied as three dwords and whose +0xC word goes through CCharString's copy constructor. The
//   catalog's "operator=" is the twin 0x00654EB6; this body constructs (CCharString(const CCharString&) @ 0x0099EC30).
// Pragma: #pragma optimize("s", on) (size TU).
// Shape: C3DVector has a user memberwise copy constructor (X, Y, Z loaded and stored one by one through ecx); the pair copy
//   constructor is `first(p.first), second(p.second)` (the same code VC emits for the implicit one).
// Callee: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
#pragma optimize("s", on)

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};

class CCharString {
public:
    void* PData;                                        // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x0C
    pair(const pair& p) : first(p.first), second(p.second) {}
};
template pair<C3DVector, CCharString>::pair(const pair<C3DVector, CCharString>&);
}