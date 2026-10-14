#pragma optimize("y", off)
#pragma optimize("s", on)
// CRecord_0056e545::CRecord_0056e545(long, long, long, long, long) -- five dword arguments copied into +0..+0xc and +0x14, a float at +0x10 zeroed, two flag bytes at +0x18/+0x19 cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0056E545, length 51 bytes.
// Callees: none. A frame-pointer body with `[ebp+8]` argument reads and no spills: the size-optimised TU with
// global optimisation and /Oy off (`optimize("gsy",off)` then `("s",on)`, see GOTCHAS). The float is stored
// first through `fldz` right after `mov eax,ecx`; the bytes are immediate stores. The manifest label
// "CDateAndTime::CDateAndTime" is propagated from a same-length body (CDateAndTime has no float or bytes),
// so the class is named by address.
struct CRecord_0056e545 {
    long          m_first;         // +0x00
    long          m_second;        // +0x04
    long          m_third;         // +0x08
    long          m_fourth;        // +0x0c
    float         m_time;          // +0x10
    long          m_fifth;         // +0x14
    unsigned char m_flagA;         // +0x18
    unsigned char m_flagB;         // +0x19
    CRecord_0056e545(long first, long second, long third, long fourth, long fifth);
};

CRecord_0056e545::CRecord_0056e545(long first, long second, long third, long fourth, long fifth)
    : m_first(first), m_second(second), m_third(third), m_fourth(fourth), m_time(0.0f), m_fifth(fifth),
      m_flagA(0), m_flagB(0)
{
}