// CUnknown::NonDelegatingRelease() -- DirectShow BaseClasses: InterlockedDecrement of m_cRef; at zero the count is bumped back to 1 (re-entrancy guard) and the object deletes itself, else max(m_cRef, 1)
// Leaf: ?NonDelegatingRelease@CUnknown
// Retail: Fable.exe @ 0x00CACD20, 60 bytes (+ int3 padding). Slot 2 of seven CUnknown-derived vtables (CTextureRenderer's
// primary 0x0129D08C and six BaseClasses objects at 0x012BF4A0..0x012BF99C). Label: the manifest's "CTextureRenderer::vfunc_2".
// Callees: InterlockedDecrement (IAT 0x0143FD38), the virtual destructor (slot 3, `push 1 ; call [edx+0Ch]`).
// Identification: Fable links Microsoft's DirectShow BaseClasses (amfilter / renbase / combase) for its movie renderer
// CTextureRenderer (the DX9 SDK Texture3D9 sample class, primary vtable 0x0129D08C); the primary-vtable slot of this body
// and its callers' slots line up with the BaseClasses declaration order (CUnknown 0-3, CBaseFilter StreamTime 4,
// CBaseRenderer SourceThreadCanWait 10, OnRenderStart/OnRenderEnd 14/15, ScheduleSample 21, GetSampleTimes 22,
// ShouldDrawSampleNow 23, Render 42, DoRenderSample 43, CheckMediaType 44) and with the member offsets (CBaseFilter:
// m_pClock +0x18, m_tStart +0x20, m_clsid +0x28; CBaseRenderer: m_ThreadSignal +0x58, m_bStreaming +0x64).
// Layout: CUnknown = INonDelegatingUnknown vptr +0, m_pUnknown +4, `volatile LONG m_cRef` +8 (the release-build
// CBaseObject is empty); STDMETHODIMP_(ULONG) is `__stdcall` with `this` on the stack (`ret 4`).
// Shape: combase.cpp of the era: `m_cRef++` on the volatile count is load / inc / store; the else leg re-reads m_cRef
// once for `max(ULONG(m_cRef), 1ul)` (written as a local so the volatile is read once).
extern "C" __declspec(dllimport) long __stdcall InterlockedDecrement(long volatile* addend);   // IAT 0x0143FD38

struct GUID;

struct INonDelegatingUnknown {
    virtual long __stdcall NonDelegatingQueryInterface(const GUID& iid, void** object);   // slot 0
    virtual unsigned long __stdcall NonDelegatingAddRef();                                // slot 1
    virtual unsigned long __stdcall NonDelegatingRelease();                               // slot 2
};

class CUnknown : public INonDelegatingUnknown {
public:
    const void*   m_pUnknown;                      // +0x04 owner of this object
protected:
    volatile long m_cRef;                          // +0x08 number of reference counts
public:
    virtual ~CUnknown();                           // slot 3
    unsigned long __stdcall NonDelegatingRelease();
};

unsigned long __stdcall CUnknown::NonDelegatingRelease()
{
    // If the reference count drops to zero delete ourselves
    long lRef = InterlockedDecrement(&m_cRef);
    if (lRef == 0) {
        // COM rules say we must protect against re-entrancy: bump the count to 1, then delete ourselves.
        m_cRef++;
        delete this;
        return 0;
    } else {
        unsigned long cRef = (unsigned long)m_cRef;
        return cRef > 1ul ? cRef : 1ul;
    }
}