// CShaderRenderManager::UpdateLightGlobals(const CCharString& name) -- when the owner's device exists, convert the name to a full wide path and create a resource from it with a fixed 16-byte descriptor, keeping the handle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0B710, length 124 bytes (`ret 4`).
// Callees: CAFile::ConvertPathToFullPath @ 0x00999110 (`__fastcall`, hidden result in ecx, path in edx),
// the wide string's character accessor @ 0x0099AF10, ~CWideString @ 0x0099B510, the device's virtual slot 10 (+0x28).
// Label: the address sits among the CRenderManagerCore / CNamedOwner members at 0x00A0B0A0..0x00A0B6E0, so the
// catalog's CShaderRenderManager::UpdateLightGlobals is a propagated name; the class is modelled descriptively.
// Shape: the converted string lives in the dead argument slot (VC reuses it once `name` has been loaded into edx).
// The descriptor's bytes are all cleared first and the third set afterwards (store order +8,+9,+b..+e then +a=1).
// The device is a named local read after the conversion: it and its vtable go into esi/ebx before the pushes.
struct CCharString { void* m_pData; };
struct CWideString {
    void* m_pData;
    operator const unsigned short*() const;   // 0x0099AF10
    ~CWideString();                           // 0x0099B510
};
CWideString __fastcall ConvertPathToFullPath(const CCharString& path);   // 0x00999110

struct SCreateDesc {
    long a;                                   // +0x0 = 2
    long b;                                   // +0x4 = 2
    bool f0, f1, f2, f3, f4, f5, f6;          // +0x8..+0xe
};
struct CDevice {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual long Create(const unsigned short* name, const SCreateDesc* desc);   // slot 10 (+0x28)
};
struct COwner { unsigned char _pad[0x40]; CDevice* m_pDevice; };   // device at +0x40
struct CShaderRenderManager {
    unsigned char _pad[0xc];
    COwner* m_pOwner;                         // +0x0c
    long    m_handle;                         // +0x10
    void UpdateLightGlobals(const CCharString& name);
};

void CShaderRenderManager::UpdateLightGlobals(const CCharString& name)
{
    if (m_pOwner->m_pDevice) {
        SCreateDesc d;
        d.a = 2;
        d.b = 2;
        d.f0 = false;
        d.f1 = false;
        d.f3 = false;
        d.f4 = false;
        d.f5 = false;
        d.f6 = false;
        d.f2 = true;
        CWideString w = ConvertPathToFullPath(name);
        CDevice* dev = m_pOwner->m_pDevice;
        m_handle = dev->Create(w, &d);
    }
}