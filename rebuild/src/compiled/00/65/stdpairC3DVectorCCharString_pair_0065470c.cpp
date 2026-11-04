// std::pair<C3DVector,CCharString>::pair(const C3DVector& a, const CCharString& b) -- build a (position, name) pair: copy the three coordinates, copy-construct the string
// Leaf: ??0?$pair
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065470C, length 41 bytes (`ret 8`, unaligned: a /Os module).
// Callers: 0x0064DE90 / 0x0064DFA1.
// Layout: C3DVector {X, Y, Z} at +0 (0xc bytes), CCharString at +0xc; the pair is 0x10 bytes.
// Callee: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Shape: STLport's `pair(const T1& a, const T2& b) : first(a), second(b)`; the string reference is pushed first
//   (`push dword ptr [esp+0Ch]`, the size TU's pass-through push), the vector copies as three dwords through ecx/eax (C3DVector's
//   memberwise user copy constructor: the implicit one is a `movsd` block copy) and
//   the string constructor runs on `lea ecx,[esi+0Ch]`.
#pragma optimize("s", on)

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
};

#include "engine/C3DVector.h"

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
}

template struct std::pair<C3DVector, CCharString>;