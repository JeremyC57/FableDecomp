// CGraphicInfoFrame::CFrameData::GetUnloadableMemory() -- with an async bank read pending (+0x30): its byte count (+0x10) once it has finished, else 0; otherwise the byte length of mip level 0 of the frame's texture (+0x1c -> texture at +8), or 0 when there is none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FD830, length 64 bytes.
// Callees: CBankFileAsyncData::IsFinished @ 0x009D54E0 (a bool in al), CTexture::GetLevelByteLength @ 0x009F9E80 (on the texture, level 0). Shape: three
// `xor eax,eax` returns folded into one tail, the frame pointer walked through esi.
class CBankFileAsyncData { public:
    unsigned char _pad[0x10];
    long          m_byteCount;                     // +0x10
public:
    bool IsFinished() const;   // 0x009D54E0
};
struct CTexture_009fd830 {
    long GetLevelByteLength(long level);           // 0x009F9E80
};
struct CGraphicFrameData_009fd830 {
    unsigned char       _pad[8];
    CTexture_009fd830*  m_pTexture;                // +0x08
};

struct CFrameData {
    unsigned char               _pad[0x1c];
    CGraphicFrameData_009fd830* m_pGraphic;        // +0x1c
    unsigned char               _pad20[0x10];
    CBankFileAsyncData*         m_pAsyncRead;      // +0x30
    long GetUnloadableMemory();
};

long CFrameData::GetUnloadableMemory()
{
    if (m_pAsyncRead) {
        if (m_pAsyncRead->IsFinished())
            return m_pAsyncRead->m_byteCount;
        return 0;
    }
    CGraphicFrameData_009fd830* graphic = m_pGraphic;
    if (graphic && graphic->m_pTexture)
        return graphic->m_pTexture->GetLevelByteLength(0);
    return 0;
}