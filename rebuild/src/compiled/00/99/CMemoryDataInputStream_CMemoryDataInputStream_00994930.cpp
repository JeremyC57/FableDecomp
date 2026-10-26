// CMemoryDataInputStream::CMemoryDataInputStream(const unsigned char* data, long size) -- the CBaseClassNonCopyable base (0x0099A2F0), then the stream members: the size at +8 and +0x14, the data at +0xc and +0x18, the position (+4) and flags (+0x10) cleared, this class's vtable (0x0123675C)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00994930, length 48 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0. Shape: the five state stores precede this class's vtable store (a second, non-polymorphic base with an inline constructor), the data copy at +0x18 follows it, `ret 8`.
struct CBaseClassNonCopyable {
    virtual void v00();
    CBaseClassNonCopyable();                       // 0x0099A2F0
};

struct SMemoryStreamState_00994930 {              // a second, non-polymorphic base: its stores precede the derived vtable store
    long                 m_position;               // +0x04
    long                 m_size;                   // +0x08
    const unsigned char* m_pData;                  // +0x0c
    long                 m_flags;                  // +0x10
    long                 m_sizeCopy;               // +0x14
    SMemoryStreamState_00994930(const unsigned char* data, long size) : m_position(0), m_size(size), m_pData(data), m_flags(0), m_sizeCopy(size) { }
};

class CMemoryDataInputStream : public CBaseClassNonCopyable, public SMemoryStreamState_00994930 { public:   // vtable 0x0123675C
    const unsigned char* m_pDataCopy;              // +0x18
    CMemoryDataInputStream(const unsigned char* data, long size);
};

CMemoryDataInputStream::CMemoryDataInputStream(const unsigned char* data, long size)
    : CBaseClassNonCopyable(), SMemoryStreamState_00994930(data, size), m_pDataCopy(data)
{
}