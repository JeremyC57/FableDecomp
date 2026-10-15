// CCompressedMemoryDataInputStream::CCompressedMemoryDataInputStream(const char* name, IUnknown* outer, long* result, IPin* pin) -- (label propagated; really the DirectShow base-class CRendererPosPassThru constructor) build the CPosPassThru seeking pass-through and zero the renderer's media times
// Leaf: ??0CRendererPosPassThru
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CAFCE0, 124 bytes (128 with padding).
// Callees: CUnknown::CUnknown(name, outer) 0x00CACC80 (InterlockedIncrement of the object count, then
//          m_pUnknown = outer ? outer : this, m_cRef = 0), InitializeCriticalSection through the import
//          slot 0x0143FEE4.
//
// Shape: the manifest's CCompressedMemoryDataInputStream label is wrong: the three vtables at +0/+4/+8
// (0x012BF940/F8F8/F8E4, then 0x012BF9F0/F9A8/F994; no RTTI) and E_POINTER (0x80004003) are the
// DirectShow strmbase classes. CPosPassThru : IMediaSeeking (+0), CMediaPosition (+4: IMediaPosition,
// then CUnknown at +8 whose INonDelegatingUnknown vptr it carries). CMediaPosition is AM_NOVTABLE, so its
// inline constructor only contributes CUnknown's out-of-line call and its CBaseDispatch member's
// `m_pti = NULL` (+0x14, stored BEFORE the CPosPassThru vtables); CPosPassThru stores its three vtables,
// then m_pPin (+0x18) and `*phr = E_POINTER` for a null pin. CRendererPosPassThru then stores its own
// three vtables, constructs its CCritSec (+0x1c, InitializeCriticalSection inline) and zeroes the two
// REFERENCE_TIMEs (+0x38/+0x40, 8-aligned after the 24-byte critical section) and sets m_bReset = TRUE
// (+0x48). MIDL interfaces are `__declspec(novtable)` (no interface vtable stores).
struct CRITICAL_SECTION_ { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
extern "C" __declspec(dllimport) void __stdcall InitializeCriticalSection(CRITICAL_SECTION_* section);   // [0x0143FEE4]

struct IPin;
struct IUnknown_;
const long E_POINTER_ = 0x80004003L;

struct __declspec(novtable) INonDelegatingUnknown {
    virtual long __stdcall NonDelegatingQueryInterface(const void* iid, void** object) = 0;
    virtual unsigned long __stdcall NonDelegatingAddRef() = 0;
    virtual unsigned long __stdcall NonDelegatingRelease() = 0;
};

struct CBaseObject { };

struct __declspec(novtable) CUnknown : INonDelegatingUnknown, CBaseObject {
    IUnknown_* m_pUnknown;                  // +0x04 within CUnknown
    volatile long m_cRef;                   // +0x08
    CUnknown(const char* name, IUnknown_* outer);   // 0x00CACC80
};

struct __declspec(novtable) IMediaSeeking {
    virtual long __stdcall QueryInterface(const void* iid, void** object) = 0;
};

struct __declspec(novtable) IMediaPosition {
    virtual long __stdcall QueryInterface(const void* iid, void** object) = 0;
};

struct CBaseDispatch {
    void* m_pti;                            // ITypeInfo*
    CBaseDispatch() : m_pti(0) { }
};

struct __declspec(novtable) CMediaPosition : IMediaPosition, CUnknown {
    CBaseDispatch m_basedisp;               // +0x14 (object offsets)
    CMediaPosition(const char* name, IUnknown_* outer) : CUnknown(name, outer) { }
};

struct CPosPassThru : IMediaSeeking, CMediaPosition {
    IPin* m_pPin;                           // +0x18
    CPosPassThru(const char* name, IUnknown_* outer, long* result, IPin* pin)
        : CMediaPosition(name, outer), m_pPin(pin)
    {
        if (pin == 0) {
            *result = E_POINTER_;
            return;
        }
    }
    virtual long __stdcall QueryInterface(const void* iid, void** object);
    virtual long __stdcall NonDelegatingQueryInterface(const void* iid, void** object);
    virtual unsigned long __stdcall NonDelegatingAddRef();
    virtual unsigned long __stdcall NonDelegatingRelease();
};

struct CCritSec {
    CRITICAL_SECTION_ m_CritSec;
    CCritSec() { InitializeCriticalSection(&m_CritSec); }
};

struct CRendererPosPassThru : CPosPassThru {
    CCritSec m_PositionLock;                // +0x1c
    __int64 m_StartMedia;                   // +0x38
    __int64 m_EndMedia;                     // +0x40
    long m_bReset;                          // +0x48 BOOL
    CRendererPosPassThru(const char* name, IUnknown_* outer, long* result, IPin* pin);
    virtual long __stdcall QueryInterface(const void* iid, void** object);
    virtual long __stdcall NonDelegatingQueryInterface(const void* iid, void** object);
};

CRendererPosPassThru::CRendererPosPassThru(const char* name, IUnknown_* outer, long* result, IPin* pin)
    : CPosPassThru(name, outer, result, pin), m_StartMedia(0), m_EndMedia(0), m_bReset(1)
{
}
