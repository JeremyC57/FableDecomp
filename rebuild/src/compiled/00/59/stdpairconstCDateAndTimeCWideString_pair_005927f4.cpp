// std::pair<const CDateAndTime,CWideString>::pair(const pair& other) -- copy a (timestamp, wide string) map entry: the 28-byte date as a block, the string by its copy constructor
// Leaf: ??0?$pair
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005927F4, length 38 bytes (`ret 4`, unaligned: a /Os module).
// Callers: 0x00593083 / 0x005938C7 (the map's node construction: `_Construct(&node->_M_value_field, value)`).
// Layout: Ego_r.pdb CDateAndTime is 0x1c bytes (seven dwords, POD); CWideString is one storage pointer at +0x1c.
// Callee: CWideString::CWideString(const CWideString&) @ 0x0099B720.
// Shape: STLport's explicit memberwise copy constructor `pair(const pair& o) : first(o.first), second(o.second)`; the source
//   pointer is copied to esi and advanced to `&o.second` (`add eax,1Ch`) before the 7-dword `rep movsd`, then pushed.
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
    pair(const pair& other) : first(other.first), second(other.second) { }
};
}

template struct std::pair<const CDateAndTime, CWideString>;