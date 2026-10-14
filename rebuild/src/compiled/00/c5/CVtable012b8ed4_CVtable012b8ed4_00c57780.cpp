// CVtable_012b8ed4::CVtable_012b8ed4() -- this class's vtable (0x012B8ED4, no RTTI), the dwords +0x604 / +0x618 and the bytes +0x614 / +0x615 cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C57780, length 48 bytes.
// The chunk label is propagated; named by its vtable. Shape: no frame, one zero register, `ret`.
struct CVtable_012b8ed4 {                          // vtable 0x012B8ED4
    virtual void v00();
    unsigned char _pad04[0x600];
    long          m_604;                           // +0x604
    unsigned char _pad608[0xc];
    bool          m_614;                           // +0x614
    bool          m_615;                           // +0x615
    unsigned char _pad616[2];
    long          m_618;                           // +0x618
    CVtable_012b8ed4();
};

CVtable_012b8ed4::CVtable_012b8ed4()
    : m_604(0)
{
    m_618 = 0;                                     // retail stores the dwords before the bytes
    m_614 = false;
    m_615 = false;
}