// std::pair<const CDateAndTime,CWideString>::pair(const CDateAndTime& a, const CWideString& b) -- build a (timestamp, wide string) map entry: copy the 28-byte date, copy-construct the string
// Leaf: ??0?$pair
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005921A4, length 36 bytes (`ret 8`, unaligned: a /Os module).
// Caller: 0x00594A42, which builds `value_type(date, text)` in a frame temporary (`lea ecx,[ebp-28h]`) for a map insert
//   (the date is a by-reference argument `[ebp+8]`, the string a frame local).
// Layout: Ego_r.pdb CDateAndTime is 0x1c bytes (Year, Month, Day, Hour, Minute, Second, Millisecond: seven dwords, POD);
//   CWideString is one storage pointer, so the pair is 0x20 bytes with the string at +0x1c.
// Callee: CWideString::CWideString(const CWideString&) @ 0x0099B720.
// Shape: STLport's two-argument constructor `pair(const T1& a, const T2& b) : first(a), second(b)`; the POD date copies as a
//   7-dword block (`push 7 ; pop ecx ; rep movsd`, the size TU's constant load), the string argument is pushed before the copy
//   and the string constructor runs on `lea ecx,[ebx+1Ch]`; `this` is kept in ebx and returned.
#pragma optimize("s", on)

class CDateAndTime { public:                       // 0x1c bytes, POD
    long          Year;                     // +0x00
    unsigned long Month;                    // +0x04
    unsigned long Day;                      // +0x08
    unsigned long Hour;                     // +0x0c
    unsigned long Minute;                   // +0x10
    unsigned long Second;                   // +0x14
    unsigned long Millisecond;              // +0x18
};

class CWideString {
public:
    void* storage_;
    CWideString(const CWideString& other);  // 0x0099B720
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
}

template struct std::pair<const CDateAndTime, CWideString>;