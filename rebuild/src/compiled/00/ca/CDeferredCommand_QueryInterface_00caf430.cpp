// CDeferredCommand::QueryInterface(const _GUID& riid, void** ppv) -- DirectShow DECLARE_IUNKNOWN delegation: forward IDeferredCommand::QueryInterface to the owning object's IUnknown (GetOwner()->QueryInterface)
// Leaf: ?QueryInterface@CDeferredCommand@@UAGJABU_GUID@@PAPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CAF430, 15 bytes, __stdcall: `this` (the IDeferredCommand sub-object) at [esp+4], the owner loaded
//   from [this-8] (CUnknown::m_pUnknown) and written back over `this`, then `jmp [ecx]` (IUnknown slot 0).
// Identity: slot 0 of the non-RTTI IDeferredCommand vtable 0x012BF8B8; its slots 3.. are the landed CDeferredCommand members (Cancel 0x00CAEC80, Confidence 0x00CAECB0, Postpone 0x00CAECF0, GetHResult 0x00CAECC0); ctlutil.h CDeferredCommand : CUnknown, IDeferredCommand. Slots 1 / 2 (0x00CAF440 AddRef, 0x00CAF450 Release) and the scalar deleting destructor 0x00CAF460 follow.
//   Library: DirectShow BaseClasses (strmbase, DX9 SDK): combase.h DECLARE_IUNKNOWN, the same macro body as the landed
//   CBasePin::AddRef 0x00CA53F0 (slot 1 of 0x012BF3C0).
// Extent: the catalog row ran on into the next vtable slots; see SPLITS.tsv.
// Layout: DirectShow base classes (strmbase), modelled as the landed CBasePin::AddRef 0x00CA53F0 does:
//   CUnknown = INonDelegatingUnknown vptr +0x00, the release-build base object (no data), m_pUnknown +0x04, m_cRef +0x08;
//   the first interface sub-object at +0x0C.
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
class CBaseObject { public:                      // release build: the debug object-name/cookie fields are absent
};
struct CUnknown : INonDelegatingUnknown, CBaseObject {
    IUnknown* const m_pUnknown;                  // +0x04 the owner
    long      m_cRef;                            // +0x08
    IUnknown* GetOwner() const { return m_pUnknown; }
};
struct IDeferredCommand : IUnknown { };

struct CDeferredCommand : CUnknown, IDeferredCommand {
    long __stdcall QueryInterface(const _GUID& riid, void** ppv);
};

long __stdcall CDeferredCommand::QueryInterface(const _GUID& riid, void** ppv)
{
    return GetOwner()->QueryInterface(riid, ppv);
}