// CFrameData::Sizeof() -- 0x44 bytes of header plus 4 plus the graphic frame data's size (+0x1c, when present) plus the managed texture's size (+0x28, when it holds data at +0x2c)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FDBE0, length 64 bytes.
// Callees: CGraphicFrameData::Sizeof @ 0x009FCBF0, CManagedTexture::Sizeof @ 0x00A69E60 (on the member at +0x28). Shape: the `+4` folded onto the call's result
// or the constant 4, then `lea edi,[eax+44h]`, the texture's data word tested inline.
struct CGraphicFrameData_009fdbe0 {
    long Sizeof();                                 // 0x009FCBF0
};
struct CManagedTexture_009fdbe0 {
    long  m_0;
    void* m_pData;                                 // +0x04
    bool HasData() const { return m_pData != 0; }
    long Sizeof();                                 // 0x00A69E60
};

struct CFrameData {
    unsigned char               _pad[0x1c];
    CGraphicFrameData_009fdbe0* m_pGraphic;        // +0x1c
    unsigned char               _pad20[8];
    CManagedTexture_009fdbe0    m_texture;         // +0x28
    long Sizeof();
};

long CFrameData::Sizeof()
{
    long size = (m_pGraphic ? m_pGraphic->Sizeof() + 4 : 4) + 0x44;
    if (m_texture.HasData())
        size += m_texture.Sizeof();
    return size;
}