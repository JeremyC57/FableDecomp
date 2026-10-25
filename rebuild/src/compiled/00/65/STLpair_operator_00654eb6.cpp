// std::pair::operator=(const pair& other) -- assignment of pair<C3DVector, CCharString>: block-copy the vector, CCharString::operator= the string
// Leaf: ??4?$pair@VC3DVector@@VCCharString@@@std@@QAEAAU01@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00654EB6, 36 bytes, __fastcall `ret 4`, returns `this` (size-optimised TU: odd address).
// Identity: the assignment twin of the copy constructor 0x00654DD4 (same 16-byte pair<C3DVector, CCharString>); one caller,
//   0x0065583C in the same TU (the STLport copy/assign loop). Callee: CCharString::operator=(const CCharString&) @ 0x0099EFB0.
// Pragma: #pragma optimize("s", on) (size TU).
// Shape: C3DVector declares no assignment operator, so `first = p.first` is VC's bitwise implicit copy (three `movsd` through
//   esi/edi), interleaved with the setup of the string assignment; `this` stays in ebx for the return.
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

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x0C
    pair& operator=(const pair& p)
    {
        first = p.first;
        second = p.second;
        return *this;
    }
};
template pair<C3DVector, CCharString>& pair<C3DVector, CCharString>::operator=(const pair<C3DVector, CCharString>&);
}