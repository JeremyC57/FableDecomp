// CMemoryDataInputStream::CMemoryDataInputStream(const void* data, unsigned long size) -- the CBaseClassNonCopyable base (0x0099A2F0), then the stream members: the size at +8 and +0x14, the data at +0xc and +0x18, the position (+4) and flags (+0x10) cleared, this class's vtable (0x0123675C)
// Leaf: ??0CMemoryDataInputStream@@QAE@PBXK@Z
// NF131 (2026-11-20, q6182): own parameter list respelled in place to the PDB (const void*, unsigned long): Ego_r.pdb 0x00AFCA20 + FableWin.pdb
//   0x030849C0 ??0CMemoryDataInputStream@@QAE@PBXK@Z (also engine/CMemoryDataInputStream.h); was ??0CMemoryDataInputStream@@QAE@PBEJ@Z.
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
    CMemoryDataInputStream(const void* data, unsigned long size);
};

CMemoryDataInputStream::CMemoryDataInputStream(const void* data, unsigned long size)
    : CBaseClassNonCopyable(), SMemoryStreamState_00994930((const unsigned char*)data, size), m_pDataCopy((const unsigned char*)data)
{
}