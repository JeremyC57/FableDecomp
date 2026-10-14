// CRecord_006a92b0::CRecord_006a92b0(long, long, long, long, unsigned char) -- four dword arguments and one byte copied into a 20-byte record
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A92B0, length 39 bytes.
// Callees: none. `ret 14h` = five argument slots (the byte is passed in a full slot and read with `mov cl`);
// the loads alternate between ecx and edx (the scheduler pipelines the copy). The manifest label
// "CDateAndTime::CDateAndTime" is propagated from a same-length body (CDateAndTime has seven dwords and
// no byte), so the class is named by address.
struct CRecord_006a92b0 {
    long          m_first;         // +0x00
    long          m_second;        // +0x04
    long          m_third;         // +0x08
    long          m_fourth;        // +0x0c
    unsigned char m_flag;          // +0x10
    CRecord_006a92b0(long first, long second, long third, long fourth, unsigned char flag);
};

CRecord_006a92b0::CRecord_006a92b0(long first, long second, long third, long fourth, unsigned char flag)
    : m_first(first), m_second(second), m_third(third), m_fourth(fourth), m_flag(flag)
{
}