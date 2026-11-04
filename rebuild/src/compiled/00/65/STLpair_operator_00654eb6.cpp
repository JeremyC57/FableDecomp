// std::pair::operator=(const pair& other) -- assignment of pair<C3DVector, CCharString>: block-copy the vector, CCharString::operator= the string
// Leaf: ??4?$pair@VC3DVector@@VCCharString@@@std@@QAEAAU01@ABU01@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00654EB6, 36 bytes, __fastcall `ret 4`, returns `this` (size-optimised TU: odd address).
// Identity: the assignment twin of the copy constructor 0x00654DD4 (same 16-byte pair<C3DVector, CCharString>); one caller,
//   0x0065583C in the same TU (the STLport copy/assign loop). Callee: CCharString::operator=(const CCharString&) @ 0x0099EFB0.
// Pragma: #pragma optimize("s", on) (size TU).
// Shape: C3DVector declares no assignment operator, so `first = p.first` is VC's bitwise implicit copy (three `movsd` through
//   esi/edi), interleaved with the setup of the string assignment; `this` stays in ebx for the return.
#pragma optimize("s", on)

#include "engine/C3DVector.h"

class CCharString {
public:
    void* PData;                                        // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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