// CIndexBuffer::CIndexBuffer(unsigned long length, unsigned long usage, long format, long pool, long count) -- this class's vtable (0x0129EF2C), the buffer pointer (+4) and the word at +0x10 cleared, then the Direct3D index buffer created (device slot 27 via the system record @ 0x009A4EC0 -> +0x60 -> +0x58) with the length kept at +8 and the index count at +0xc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC2010, length 96 bytes.
// Shape: `&m_pBuffer` in edi before the vtable store, the length re-read into edi after that push, `mov eax,esi`, `ret 14h`.
struct IDirect3DIndexBuffer_Retail;
struct IDirect3DDevice_Retail {                    // COM: __stdcall methods, `this` pushed
    virtual void __stdcall v000(); virtual void __stdcall v001(); virtual void __stdcall v002(); virtual void __stdcall v003(); virtual void __stdcall v004(); virtual void __stdcall v005(); virtual void __stdcall v006(); virtual void __stdcall v007(); virtual void __stdcall v008(); virtual void __stdcall v009();
    virtual void __stdcall v010(); virtual void __stdcall v011(); virtual void __stdcall v012(); virtual void __stdcall v013(); virtual void __stdcall v014(); virtual void __stdcall v015(); virtual void __stdcall v016(); virtual void __stdcall v017(); virtual void __stdcall v018(); virtual void __stdcall v019();
    virtual void __stdcall v020(); virtual void __stdcall v021(); virtual void __stdcall v022(); virtual void __stdcall v023(); virtual void __stdcall v024(); virtual void __stdcall v025(); virtual void __stdcall v026();
    virtual long __stdcall CreateIndexBuffer(unsigned long length, unsigned long usage, long format, long pool, IDirect3DIndexBuffer_Retail** ppBuffer, void* sharedHandle);   // slot 27 (+0x6c)
};
struct SRenderDevice_Retail {
    unsigned char            _pad[0x58];
    IDirect3DDevice_Retail*  m_pDevice;            // +0x58
};
class CSystemManager { public:
    unsigned char          _pad[0x60];
    SRenderDevice_Retail*  m_pRender;              // +0x60
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0 (labelled "GFGetSystemManager")
class CIndexBuffer { public:                              // vtable 0x0129EF2C
    virtual void v00();
    IDirect3DIndexBuffer_Retail* m_pBuffer;        // +0x04
    unsigned long                m_length;         // +0x08
    long                         m_count;          // +0x0c
    long                         PAllocatedBuffer;             // +0x10
    CIndexBuffer(unsigned long length, unsigned long usage, long format, long pool, long count);
};

CIndexBuffer::CIndexBuffer(unsigned long length, unsigned long usage, long format, long pool, long count)
    : PAllocatedBuffer(0)
{
    m_pBuffer = 0;
    GFGetSystemManager()->m_pRender->m_pDevice->CreateIndexBuffer(length, usage, format, pool, &m_pBuffer, 0);
    m_length = length;
    m_count = count;
}