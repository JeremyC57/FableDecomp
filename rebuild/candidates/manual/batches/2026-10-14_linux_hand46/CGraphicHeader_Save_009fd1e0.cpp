// CGraphicHeader::Save(CDataOutputStream* stream) -- write the 0x1c-byte header through the stream's virtual slot 4, then the pixel format at +0xc (CDataOutputStream::WritePixelFormat @ 0x00993F60)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FD1E0, length 34 bytes.
// Callees: the stream's virtual slot 4 (+0x10: `this`, 0x1c), CDataOutputStream::WritePixelFormat @ 0x00993F60 (the format at +0xc by address). Shape: the stream in edi,
// `this` in esi, the format address by `add esi,0Ch`, `ret 4`.
struct SPixelFormat_009fd1e0 { unsigned char _body[16]; };
struct CDataOutputStream {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void Write(const void* data, long length);   // slot 4 (+0x10)
    void WritePixelFormat(SPixelFormat_009fd1e0* format);   // 0x00993F60
};

struct CGraphicHeader {                            // 0x1c bytes
    unsigned char          _pad[0xc];
    SPixelFormat_009fd1e0  m_format;               // +0x0c
    void Save(CDataOutputStream* stream);
};

void CGraphicHeader::Save(CDataOutputStream* stream)
{
    stream->Write(this, 0x1c);
    stream->WritePixelFormat(&m_format);
}
