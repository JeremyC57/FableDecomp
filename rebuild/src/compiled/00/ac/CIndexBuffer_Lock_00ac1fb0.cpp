// CIndexBuffer::Lock(unsigned long flags, unsigned long offset, unsigned long size) -- lock the Direct3D index buffer (+4): a zero size means the whole buffer (its description's Size, slot 13), the render statistics object (system record +0x60 -> +8) is told about the lock (0x00A0AA70 with 1.0f), then COM slot 11 locks (offset, size, flags) and the mapped pointer is returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC1FB0, length 96 bytes.
// Shape: a 0x14-byte frame for the description, the size in edi, the mapped pointer homed in the dead size slot, `ret 0Ch`.
struct SIndexBufferDesc_Retail { long Format; long Type; unsigned long Usage; long Pool; unsigned long Size; };   // 0x14 bytes
struct IDirect3DIndexBuffer_Retail {              // COM: __stdcall methods, `this` pushed
    virtual void __stdcall v000(); virtual void __stdcall v001(); virtual void __stdcall v002(); virtual void __stdcall v003(); virtual void __stdcall v004(); virtual void __stdcall v005(); virtual void __stdcall v006(); virtual void __stdcall v007(); virtual void __stdcall v008(); virtual void __stdcall v009();
    virtual void __stdcall v010();
    virtual long __stdcall Lock(unsigned long offset, unsigned long size, void** ppData, unsigned long flags);   // slot 11 (+0x2c)
    virtual void __stdcall v012();
    virtual long __stdcall GetDesc(SIndexBufferDesc_Retail* desc);   // slot 13 (+0x34)
};
struct CRenderStatistics_Retail {
    void NotifyLock(IDirect3DIndexBuffer_Retail* buffer, float weight);   // 0x00A0AA70 (an empty `ret 8` stub, labelled "$E2")
};
struct SRenderDevice_Retail {
    unsigned char             _pad[8];
    CRenderStatistics_Retail* m_pStatistics;       // +0x08
};
struct SDeviceRecord_013ca618 {
    unsigned char          _pad[0x60];
    SRenderDevice_Retail*  m_pRender;              // +0x60
};
SDeviceRecord_013ca618* __cdecl GetDeviceRecord_009a4ec0();   // 0x009A4EC0 (labelled "GFGetSystemManager")
struct CIndexBuffer {
    virtual void v00();
    IDirect3DIndexBuffer_Retail* m_pBuffer;        // +0x04
    void* Lock(unsigned long flags, unsigned long offset, unsigned long size);
};

void* CIndexBuffer::Lock(unsigned long flags, unsigned long offset, unsigned long size)
{
    if (size == 0) {
        SIndexBufferDesc_Retail desc;
        m_pBuffer->GetDesc(&desc);
        size = desc.Size;
    }
    SDeviceRecord_013ca618* record = GetDeviceRecord_009a4ec0();   // the call first, the argument next, then the chain
    record->m_pRender->m_pStatistics->NotifyLock(m_pBuffer, 1.0f);
    void* data;
    m_pBuffer->Lock(offset, size, &data, flags);
    return data;
}