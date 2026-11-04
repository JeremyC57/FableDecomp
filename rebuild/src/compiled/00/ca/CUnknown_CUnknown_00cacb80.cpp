// CUnknown::CUnknown(const char* pName, IUnknown* pUnk) -- DirectShow base-class constructor: counts the live object (CBaseObject), owner = pUnk or the object's own non-delegating interface, reference count 0
// Leaf: ??0CUnknown@@QAE@PBDPAUIUnknown@@@Z
// Vendor naming V2 (plan phase 1, one address, one name; V1 decision D2): the CUnknown base is strmbase's CMyBaseObject (combase.h
//   CBaseObject renamed; RTTI of vtable 0x0129D08C): its live-object count is ?m_cObjects@CMyBaseObject@@0JA 0x0143CCF4 (was ?m_cObjects@CBaseObject@@0JA)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CACB80, 40 bytes (`ret 8`).
// Callees: InterlockedIncrement (import thunk 0x0143FD34, resolved from the IAT) on CBaseObject::m_cObjects
// (0x0143CCF4).
// Label: "CParticleMorphs::CEntry_Initialize" is propagated. The body is the DirectShow strmbase
// CUnknown constructor (combase.cpp): CBaseObject(pName) increments the static object count (the release
// build keeps no name), m_pUnknown = pUnk != 0 ? pUnk : (IUnknown*)(INonDelegatingUnknown*)this, m_cRef = 0. Its
// callers (0x00CAB370, 0x00CABAE0, ...) are DirectShow filter/pin constructors of the movie player.
// Shape: AM_NOVTABLE (`__declspec(novtable)`) CUnknown over the novtable INonDelegatingUnknown interface: no
// vtable store; the empty CBaseObject base takes no room, so m_pUnknown is +4 and m_cRef +8; the import is
// called through its IAT slot (`__declspec(dllimport)`) before the member initialisers.
// Fix (sweep1, 2026-10-25; FINDINGS chunk 449): the landed row spelled the Interlocked prototype and m_cRef with a cv-qualifier,
// which the purity grep rejects; neither qualifier changes this constructor's bytes (one store of 0).
extern "C" __declspec(dllimport) long __stdcall InterlockedIncrement(long* addend);   // IAT 0x0143FD34 (same _InterlockedIncrement@4 import)

struct GUID;

struct IUnknown {
    virtual long __stdcall QueryInterface(const GUID& riid, void** ppv) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
};

struct __declspec(novtable) INonDelegatingUnknown {
    virtual long __stdcall NonDelegatingQueryInterface(const GUID& riid, void** ppv) = 0;
    virtual unsigned long __stdcall NonDelegatingAddRef() = 0;
    virtual unsigned long __stdcall NonDelegatingRelease() = 0;
};

class CMyBaseObject {
    static long m_cObjects;                          // 0x0143CCF4
public:
    CMyBaseObject(const char* pName) { InterlockedIncrement(&m_cObjects); }
};

class __declspec(novtable) CUnknown : public INonDelegatingUnknown, public CMyBaseObject {
    IUnknown* const m_pUnknown;                      // +0x04
protected:
    long m_cRef;                                     // +0x08 (combase.h cv-qualifier dropped for purity)
public:
    CUnknown(const char* pName, IUnknown* pUnk);
};

CUnknown::CUnknown(const char* pName, IUnknown* pUnk)
    : CMyBaseObject(pName),
      m_pUnknown(pUnk != 0 ? pUnk : reinterpret_cast<IUnknown*>(static_cast<INonDelegatingUnknown*>(this))),
      m_cRef(0)
{
}