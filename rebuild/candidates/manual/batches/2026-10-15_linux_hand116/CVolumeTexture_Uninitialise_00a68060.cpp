// CVolumeTexture::Uninitialise() -- if a texture is held: probe its reference count (AddRef + Release), when this is the last user tell the render statistics it is gone, release it and clear the members
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A68060, length 85 bytes.
// Leaf: Uninitialise
// Callees: COM AddRef (+0x04) / Release (+0x08), GFGetSystemManager @ 0x009A4EC0 (__fastcall), the render statistics'
// texture-removal method @ 0x00A0AA70 (system +0x60 -> +0x08; the manifest label "$E2" there is wrong), CVolumeTexture::ReleaseTexture @ 0x00A67F60.
// Shape: `Release() == 1` compare; the system manager taken into a named local before the texture argument is loaded
// (call first, then the chained member loads); the three clears in +0x0c, +0x04, +0x08 order.
struct CRenderStatistics {
    void RemoveTexture(void* texture, float weight);    // 0x00A0AA70
};
struct IDirect3DDevice9;
struct CRenderSystem {
    unsigned char      _pad00[8];
    CRenderStatistics* m_pStatistics;       // +0x08
    unsigned char      _pad0c[0x58 - 0x0c];
    IDirect3DDevice9*  m_pDevice;           // +0x58
};
struct CSystemManager {
    unsigned char  _pad00[0x60];
    CRenderSystem* m_pRender;               // +0x60
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0
struct IDirect3DVolumeTexture9 {
    virtual long __stdcall QueryInterface(const void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
};
struct CVolumeTexture {
    virtual ~CVolumeTexture();
    IDirect3DVolumeTexture9* m_pTexture;    // +0x04
    long                     m_format;      // +0x08
    long                     m_levels;      // +0x0c
    void Uninitialise();
    void ReleaseTexture();                  // 0x00A67F60
};
void CVolumeTexture::Uninitialise()
{
    if (m_pTexture) {
        m_pTexture->AddRef();
        if (m_pTexture->Release() == 1) {
            CSystemManager* system = GFGetSystemManager();
            system->m_pRender->m_pStatistics->RemoveTexture(m_pTexture, 1.0f);
        }
        ReleaseTexture();
        m_levels = 0;
        m_pTexture = 0;
        m_format = 0;
    }
}
