// CVolumeTexture::CVolumeTexture(const CVolumeTexture& other) -- copy constructor: base, vtable, copy texture/format/levels, AddRef the shared texture
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A67DD0, length 58 bytes.
// Leaf: ??0CVolumeTexture
// Callees: CBase::CBase() @ 0x0099A2D0, COM AddRef (slot 1, +0x04). Vtable 0x0129DC18 = CVolumeTexture (rtti-vtables.tsv).
// Shape: three member copies through ecx/edx/ecx, then the OTHER's pointer is re-read for the null test (`[eax+4]`) while the
// AddRef goes through this object's copy (`[esi+4]`): `if (other.m_pTexture) m_pTexture->AddRef();`.
struct IDirect3DVolumeTexture9 {
    virtual long __stdcall QueryInterface(const void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
};
struct CBase {
    virtual ~CBase();
    CBase();                                // 0x0099A2D0
};
struct CVolumeTexture : CBase {             // vtable 0x0129DC18
    IDirect3DVolumeTexture9* m_pTexture;    // +0x04
    long                     m_format;      // +0x08
    long                     m_levels;      // +0x0c
    CVolumeTexture(const CVolumeTexture& other);
    CVolumeTexture& operator=(const CVolumeTexture& other);
    void Uninitialise();                    // 0x00A68060
    void ReleaseTexture();                  // 0x00A67F60
};
CVolumeTexture::CVolumeTexture(const CVolumeTexture& other)
    : m_pTexture(other.m_pTexture), m_format(other.m_format), m_levels(other.m_levels)
{
    if (other.m_pTexture)
        m_pTexture->AddRef();
}