// C3DMeshMaterial::CFilenames::CFilenames() -- the reference count at +4 set to 1, this class's vtable (0x0129EDB4), then the four CCharString members at +8..+0x14 default-constructed (0x0099E4B0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABEF60, length 52 bytes.
// Callees: CCharString::CCharString @ 0x0099E4B0 (x4). The chunk label "CMapFilenames::CMapFilenames" is propagated; the vtable 0x0129EDB4 is C3DMeshMaterial::CFilenames'. Shape: the count stored BEFORE the vtable (a `novtable` base's inline constructor), the first string's address formed before the stores, `ret`.
struct CCharString {
    void* m_pStringData;
    CCharString();                                 // 0x0099E4B0
};
struct __declspec(novtable) CCountedBase {         // the base's vtable store is folded into the derived one
    virtual void v00();
    long m_references;                             // +0x04
    CCountedBase() : m_references(1) { }
};
namespace C3DMeshMaterial {
struct CFilenames : CCountedBase {                 // vtable 0x0129EDB4
    virtual void v00();
    CCharString m_diffuse;                         // +0x08
    CCharString m_normal;                          // +0x0c
    CCharString m_specular;                        // +0x10
    CCharString m_environment;                     // +0x14
    CFilenames();
};
}

C3DMeshMaterial::CFilenames::CFilenames()
    : CCountedBase()
{
}