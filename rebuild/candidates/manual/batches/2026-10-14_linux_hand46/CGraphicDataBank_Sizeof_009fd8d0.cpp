// CGraphicDataBank::Sizeof() -- 0x324 bytes plus 0x2c per entry of the 44-byte vector at +0x1e0 plus 16 per element of the 16-byte vector at +0x1ec
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FD8D0, length 64 bytes.
// Callees: none. Shape: the 44-byte element count through the reciprocal multiply (0x2E8BA2E9, `sar 3`, the sign fix-up) then `imul 2Ch`, the 16-byte count by `sar 4 ; shl 4`,
// both folded into one `lea`.
struct SEntry44_009fd8d0 { unsigned char _body[0x2c]; };
struct SEntry16_009fd8d0 { unsigned char _body[0x10]; };

struct CGraphicDataBank {
    unsigned char      _pad[0x1e0];
    SEntry44_009fd8d0* m_pEntriesFirst;            // +0x1e0
    SEntry44_009fd8d0* m_pEntriesLast;             // +0x1e4
    SEntry44_009fd8d0* m_pEntriesEnd;              // +0x1e8
    SEntry16_009fd8d0* m_pSlotsFirst;              // +0x1ec
    SEntry16_009fd8d0* m_pSlotsLast;               // +0x1f0
    SEntry16_009fd8d0* m_pSlotsEnd;                // +0x1f4
    long Sizeof();
};

long CGraphicDataBank::Sizeof()
{
    return (m_pEntriesEnd - m_pEntriesFirst) * 0x2c + (m_pSlotsEnd - m_pSlotsFirst) * 0x10 + 0x324;
}
