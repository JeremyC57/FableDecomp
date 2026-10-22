// CBasePin::AddRef() -- DirectShow DECLARE_IUNKNOWN delegation: forward IPin::AddRef to the owning object's IUnknown (GetOwner()->AddRef())
// Leaf: ?AddRef@CBasePin@@UAGKXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA53F0, 16 bytes, __stdcall: `this` (the IPin sub-object) at [esp+4], the
//   owner loaded from [this-8] (CUnknown::m_pUnknown) and written back over `this`, then `jmp [ecx+4]` (IUnknown slot 1).
//   Slot 1 of the non-RTTI IPin vtable 0x012BF3C0 (slot 0 0x00CA53C0 = the same delegation to QueryInterface, slot 2
//   0x00CA5510 = Release); the IQualityControl vtable 0x012BF3A8 reaches it through the `sub [esp+4],4` adjustor
//   0x00CA6180 -- CBasePin's layout (CUnknown, then IPin, then IQualityControl 4 bytes later). Every DECLARE_IUNKNOWN
//   pin class compiles to these same bytes (folded); named for the DirectShow base class that declares it.
// Label: the manifest's `CScriptThing::operator==` is propagated (wrong).
// Layout: DirectShow base classes (strmbase), modelled locally as the landed CBasePin constructor 0x00CAC070 does:
//   CUnknown = INonDelegatingUnknown vptr +0x00, the release-build CBaseObject (no data), m_pUnknown +0x04,
//   m_cRef +0x08; IPin at +0x0C, IQualityControl at +0x10 (CBasePin::m_pName follows at +0x14).
struct _GUID;
struct IUnknown {
    virtual long __stdcall QueryInterface(const _GUID& riid, void** ppv) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
};
struct INonDelegatingUnknown {
    virtual long __stdcall NonDelegatingQueryInterface(const _GUID& riid, void** ppv) = 0;
    virtual unsigned long __stdcall NonDelegatingAddRef() = 0;
    virtual unsigned long __stdcall NonDelegatingRelease() = 0;
};
struct CBaseObject {                             // release build: the debug object-name/cookie fields are absent
};
struct CUnknown : INonDelegatingUnknown, CBaseObject {
    IUnknown* const m_pUnknown;                  // +0x04 the owner
    long      m_cRef;                            // +0x08
    IUnknown* GetOwner() const { return m_pUnknown; }
};
struct IPin : IUnknown { };
struct IQualityControl : IUnknown { };

struct CBasePin : CUnknown, IPin, IQualityControl {
    unsigned long __stdcall AddRef();
};

unsigned long __stdcall CBasePin::AddRef()
{
    return GetOwner()->AddRef();
}