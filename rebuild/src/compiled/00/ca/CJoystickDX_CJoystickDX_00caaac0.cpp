// CJoystickDX::~CJoystickDX() -- EH-framed destructor: the block at +0x3c freed through operator delete, the COM device at +0x18 released and nulled when present, then the member at +4 destroyed out of line (@ 0x00CACAF0)
// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/amfilter.cpp) ??1CBaseFilter@@UAE@XZ -- 101 bytes verified against Fable.exe 0x00CAAAC0
// Vendor naming V2 (plan phase 1, one address, one name; V1 decision D2): 0x00CACAF0 ??1CMyBaseObject@@QAE@XZ (was ??1CMember_00caaac0@@QAE@XZ): the +4 subobject is strmbase's CMyBaseObject (combase.h CBaseObject renamed; RTTI of vtable 0x0129D08C)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAAAC0, oracle length 101 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov eax,[esi+3Ch] ; push eax ; mov dword ptr [esp+14h],0 ; call operator delete ; mov eax,[esi+18h] ; add esp,4 ; test eax,eax ; je skip ; mov ecx,[eax] ; push eax ; call [ecx+8] ; mov dword ptr [esi+18h],0 ; skip: lea ecx,[esi+4] ; mov dword ptr [esp+10h],-1 ; call ~CMember ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// The body (EH state 0, the member at +4 alive) frees a raw block and releases a COM-style
// interface (`__stdcall` slot 2 with `this` pushed). The class stores no vtable. The class comes
// from the manifest label; the rest is descriptive.
// Funclet fix (2026-11-09, chunk 896): retail's unwind funclet 0x01216E60 is `mov ecx,[ebp-10h] ; jmp 0x00CA7780` (CUnknown::~CUnknown on `this`): strmbase's CUnknown is a BASE at +0 (AM_NOVTABLE, virtual ~CUnknown() {}), not a CMyBaseObject member at +4 (which gave `add ecx,4`). The destructor is virtual (strmbase ??1CBaseFilter@@UAE@XZ); novtable keeps it free of vtable stores. Body bytes unchanged.
struct IUnknown_00caaac0 {                 // COM-style: `this` is pushed as the first stack argument
    virtual long __stdcall QueryInterface();
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();   // slot 2
};

struct __declspec(novtable) INonDelegatingUnknown {
    virtual long __stdcall NonDelegatingQueryInterface(const void* iid, void** object) = 0;
    virtual unsigned long __stdcall NonDelegatingAddRef() = 0;
    virtual unsigned long __stdcall NonDelegatingRelease() = 0;
};
class CMyBaseObject {                      // strmbase object counter (no data in the release build)
public:
    ~CMyBaseObject();                      // 0x00CACAF0
};
class __declspec(novtable) CUnknown : public INonDelegatingUnknown, public CMyBaseObject {
    void* const m_pUnknown;                // +0x04
protected:
    long m_cRef;                           // +0x08
public:
    virtual ~CUnknown() {}                 // out-of-line copy 0x00CA7780 (the unwind funclet calls it on `this`)
};

struct __declspec(novtable) CJoystickDX : public CUnknown {   // strmbase CBaseFilter: CUnknown base at +0 (AM_NOVTABLE)
    unsigned char _pad[0xc];               // +0x0c: the IBaseFilter and IAMovieSetup vptrs, m_State
    IUnknown_00caaac0* m_pDevice;          // +0x18
    unsigned char _pad2[0x20];
    void* m_pBlock;                        // +0x3c
    ~CJoystickDX();
};

CJoystickDX::~CJoystickDX()
{
    ::operator delete(m_pBlock);
    if (m_pDevice) {
        m_pDevice->Release();
        m_pDevice = 0;
    }
}