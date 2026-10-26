// CIndexBuffer::Initialise(unsigned long length, unsigned long usage, long format, long pool, long count) -- create the Direct3D index buffer (device slot 27 via the system record @ 0x009A4EC0 -> +0x60 -> +0x58, the interface stored at +4, no shared handle) and keep the length at +8 and the index count at +0xc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC1F70, length 64 bytes.
// Shape: the device in eax with its vtable in ecx, seven pushes, `ret 14h`.
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
class CIndexBuffer { public:
    virtual void v00();
    IDirect3DIndexBuffer_Retail* m_pBuffer;        // +0x04
    unsigned long                m_length;         // +0x08
    long                         m_count;          // +0x0c
    void Initialise(unsigned long length, unsigned long usage, long format, long pool, long count);
};

void CIndexBuffer::Initialise(unsigned long length, unsigned long usage, long format, long pool, long count)
{
    GFGetSystemManager()->m_pRender->m_pDevice->CreateIndexBuffer(length, usage, format, pool, &m_pBuffer, 0);
    m_length = length;
    m_count = count;
}