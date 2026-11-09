// CVolume_00caa790::~CVolume_00caa790() -- EH-framed destructor: the COM interface at +0x14 released (`__stdcall` slot 2 with `this` pushed) and nulled when present, then the member at +4 destroyed out of line (@ 0x00CACAF0)
// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/amfilter.cpp) ??1CBaseMediaFilter@@UAE@XZ -- 89 bytes verified against Fable.exe 0x00CAA790
// Vendor naming V2 (plan phase 1, one address, one name; V1 decision D2): 0x00CACAF0 ??1CMyBaseObject@@QAE@XZ (was ??1CMember_00caa790@@QAE@XZ): the +4 subobject is strmbase's CMyBaseObject (combase.h CBaseObject renamed; RTTI of vtable 0x0129D08C)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAA790, oracle length 89 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push esi ; mov esi,ecx ; mov [esp+4],esi ; mov eax,[esi+14h] ; test eax,eax ; mov dword ptr [esp+10h],0 ; je skip ; mov ecx,[eax] ; push eax ; call [ecx+8] ; mov dword ptr [esi+14h],0 ; skip: lea ecx,[esi+4] ; mov dword ptr [esp+10h],-1 ; call ~CMember ; mov ecx,[esp+8] ; pop esi ; mov fs:[0],ecx ; add esp,10h ; ret
// No vtable store, so the class is not polymorphic (or `novtable`); EH state 0 covers the body
// (the member at +4 is alive). The manifest label (`ReleaseVolume`) is propagated garbage for a
// destructor; the class is named by address.
// Funclet fix (2026-11-09, chunk 896): retail's unwind funclet 0x01216DE0 is `mov ecx,[ebp-10h] ; jmp 0x00CA7780` (CUnknown::~CUnknown on `this`): strmbase's CUnknown is a BASE at +0 (AM_NOVTABLE, virtual ~CUnknown() {}), not a CMyBaseObject member at +4 (which gave `add ecx,4`). The destructor is virtual (strmbase ??1CBaseMediaFilter@@UAE@XZ); novtable keeps it free of vtable stores. Body bytes unchanged.
struct IUnknown_00caa790 {                 // COM-style: `this` is pushed as the first stack argument
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

struct __declspec(novtable) CVolume_00caa790 : public CUnknown {   // strmbase CBaseMediaFilter: CUnknown base at +0 (AM_NOVTABLE)
    unsigned char _pad[0x8];               // +0x0c: the IMediaFilter vptr, m_State
    IUnknown_00caa790* m_pTexture;         // +0x14
    ~CVolume_00caa790();
};

CVolume_00caa790::~CVolume_00caa790()
{
    if (m_pTexture) {
        m_pTexture->Release();
        m_pTexture = 0;
    }
}