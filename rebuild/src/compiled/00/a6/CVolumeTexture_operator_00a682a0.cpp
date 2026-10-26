// CVolumeTexture::operator=(const CVolumeTexture& other) -- uninitialise, copy texture/format/levels, AddRef the shared texture
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A682A0, length 52 bytes (+ int3 padding).
// Leaf: ??4CVolumeTexture
// Callees: CVolumeTexture::Uninitialise @ 0x00A68060, COM AddRef (slot 1, +0x04).
// Shape: the copy constructor's body (0x00A67DD0) after an Uninitialise call; the other's pointer re-read for the null test.
struct IDirect3DVolumeTexture9 {
    virtual long __stdcall QueryInterface(const void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
};
struct CBase {
    virtual ~CBase();
    CBase();                                // 0x0099A2D0
};
class CVolumeTexture : public CBase { public:             // vtable 0x0129DC18
    IDirect3DVolumeTexture9* m_pTexture;    // +0x04
    long                     m_format;      // +0x08
    long                     m_levels;      // +0x0c
    CVolumeTexture(const CVolumeTexture& other);
    CVolumeTexture& operator=(const CVolumeTexture& other);
    void Uninitialise();                    // 0x00A68060
    void ReleaseTexture();                  // 0x00A67F60
};
CVolumeTexture& CVolumeTexture::operator=(const CVolumeTexture& other)
{
    Uninitialise();
    m_pTexture = other.m_pTexture;
    m_format = other.m_format;
    m_levels = other.m_levels;
    if (other.m_pTexture)
        m_pTexture->AddRef();
    return *this;
}