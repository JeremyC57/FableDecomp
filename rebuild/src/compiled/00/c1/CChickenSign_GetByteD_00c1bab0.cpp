// CChickenSign::GetByteD() const -- returns the byte field at +0x3 of the packed parameter record, widened to 16 bits
// Leaf: ?GetByteD@CChickenSign@@QBEGXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1BAB0, 6 bytes: movzx ax, byte ptr [ecx+3] ; ret
// Identity: the landed CMaskedChickenSign rows (SetWordC 0x00C27BA0, GetWordC 0x00C27BC0, SetByteD 0x00C27BF0,
//   MergeWordC 0x00C27D40, MergeByteD 0x00C27D80) declare and call it as `unsigned short GetByteD() const // 0x00C1BAB0`
//   (reloc binding ?GetByteD@CChickenSign@@QBEGXZ), the read twin of SetByteD 0x00C1BBF0 (+3). The catalog's
//   "CTCInventoryBase::GetCanItemBeQuickAssigned" is a propagated label; CChickenSign is the landed descriptive
//   family name of the libpk packed 20-byte parameter record (no PDB covers libpk).
// Shape: an unsigned char member returned as unsigned short gives the 16-bit `movzx ax`.
struct CChickenSign {                           // the packed 20-byte parameter record (landed CChickenSign::Init 0x00C1BC90)
    unsigned char  m_level;                     // +0x00
    unsigned char  m_offset;                    // +0x01
    unsigned char  m_flags;                     // +0x02
    unsigned char  m_byteD;                     // +0x03
    short          m_wordB;                     // +0x04
    unsigned short m_wordC;                     // +0x06
    short          m_scale;                     // +0x08
    unsigned short m_wordA;                     // +0x0a
    unsigned short m_wordE;                     // +0x0c
    unsigned short m_time;                      // +0x0e
    unsigned short m_rangeMin;                  // +0x10
    unsigned short m_rangeMax;                  // +0x12
    unsigned short GetByteD() const;
};

unsigned short CChickenSign::GetByteD() const
{
    return m_byteD;
}