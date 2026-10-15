// CUnknown::NonDelegatingAddRef() -- DirectShow strmbase CUnknown: InterlockedIncrement(&m_cRef), returns max((unsigned long)m_cRef, 1)
// Leaf: NonDelegatingAddRef
// Retail: Fable.exe @ 0x00CACD00, 31 bytes (`ret 4`: __stdcall with `this` pushed).
// Callees: InterlockedIncrement (IAT 0x0143FD34).
// Label: the manifest's "CTCTavernGameCardPairs::GetCardOrder" is propagated. The body sits in slot 1
// of the CTextureRenderer vtable 0x0129D08C (a DirectShow CBaseVideoRenderer: slot 0/1/2 =
// NonDelegatingQueryInterface/AddRef/Release) and is combase.cpp's CUnknown::NonDelegatingAddRef.
// Layout as the landed CUnknown constructor (0x00CACB80): novtable INonDelegatingUnknown at +0,
// empty CBaseObject, m_pUnknown +4, volatile m_cRef +8.
// Shape: the increment's result is dropped (release build: the ASSERT is gone), `m_cRef` is re-read
//        after the call and clamped with the windef.h `max` macro: `cmp eax,1 ; ja ; mov eax,1`. The
//        reference-returning Dinkumware max keeps both operands in frame slots, and a `volatile` m_cRef
//        would be loaded twice by the macro, so this SDK revision's m_cRef is a plain LONG.
extern "C" __declspec(dllimport) long __stdcall InterlockedIncrement(long volatile* addend);   // IAT 0x0143FD34

struct GUID;

#define max(a, b) (((a) > (b)) ? (a) : (b))     // windef.h

struct __declspec(novtable) INonDelegatingUnknown {
    virtual long __stdcall NonDelegatingQueryInterface(const GUID& riid, void** ppv) = 0;
    virtual unsigned long __stdcall NonDelegatingAddRef() = 0;
    virtual unsigned long __stdcall NonDelegatingRelease() = 0;
};

class CBaseObject {
};

struct IUnknown;

class __declspec(novtable) CUnknown : public INonDelegatingUnknown, public CBaseObject {
    IUnknown* const m_pUnknown;                      // +0x04
protected:
    long m_cRef;                                     // +0x08
public:
    CUnknown(const char* pName, IUnknown* pUnk);
    unsigned long __stdcall NonDelegatingAddRef();
};

unsigned long __stdcall CUnknown::NonDelegatingAddRef()
{
    InterlockedIncrement(&m_cRef);
    return max((unsigned long)m_cRef, 1ul);
}