// CDeferredCommand::~CDeferredCommand() -- the DirectShow base-class destructor (empty body): release the +0x48 COM pointer (its
// smart-pointer member's destructor), destroy m_DispParams (+0x34), then the CUnknown base (its CMyBaseObject destructor); EH-framed
// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/ctlutil.cpp) ??1CDeferredCommand@@UAE@XZ -- 122 bytes verified against Fable.exe 0x00CAF480
// Leaf: ??1CDeferredCommand@@UAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAF480, length 122 bytes (padded to 128).
// Identity: the vendor proof above; its only caller is the scalar deleting destructor 0x00CAF460 in slot 3 of vtable 0x012BF8D4, which
//   reloc_bindings.tsv binds as ??_7CDeferredCommand@@6BCUnknown@@@ (stored by the landed CDeferredCommand::CDeferredCommand
//   0x00CAF230 next to ??_7CDeferredCommand@@6BIDeferredCommand@@@ 0x012BF8B8); the member destructor 0x00CAEBB0 is the vendor-proven
//   ??1CDispParams@@QAE@XZ. The catalog's "CVtable_012bf8d4" was a vtable placeholder; agentFAM08 respelled the local vendor model
//   (CMember_00caebb0 -> CDispParams, m_member -> m_DispParams). Fable's strmbase renames combase.h CBaseObject to CMyBaseObject
//   (vendor naming V2: ??1CMyBaseObject@@QAE@XZ 0x00CACAF0).
// Shape (retry 10b of the landing): the COM pointer at +0x48 is a smart-pointer MEMBER whose inline destructor releases it (the
//   destructor body is empty): the release then dereferences the member's `this`, not the destructor's, so VC7.1 keeps the
//   `test esi,esi` null check of the base conversions. EH state 1 for the body, byte store of state 0 around the member destructor,
//   -1 for the base. Layout (retail): CUnknown {vptr +0, CMyBaseObject +4, m_cRef +8}, m_DispParams +0x34, COM pointer +0x48.
struct __declspec(novtable) IUnknownRetail {
    virtual long __stdcall QueryInterface(const void* iid, void** object) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
};
struct __declspec(novtable) INonDelegatingUnknownRetail {
    virtual long __stdcall NonDelegatingQueryInterface(const void* iid, void** object) = 0;
    virtual unsigned long __stdcall NonDelegatingAddRef() = 0;
    virtual unsigned long __stdcall NonDelegatingRelease() = 0;
};
class CMyBaseObject {                                // strmbase object counter base (4 bytes in this build)
public:
    const char* m_pName;
    ~CMyBaseObject();                                // 0x00CACAF0
};
class __declspec(novtable) CUnknown : public INonDelegatingUnknownRetail, public CMyBaseObject {
public:
    long m_cRef;                                   // +0x08 within CUnknown
    virtual ~CUnknown() {}
};
struct CComPointer_Retail {                       // inline smart pointer: releases its interface
    IUnknownRetail* p;
    ~CComPointer_Retail() { if (p) p->Release(); }
};
struct CDispParams {                               // ctlutil.h: DISPPARAMS-derived argument block
    unsigned char _body[0x14];
    ~CDispParams();                                // 0x00CAEBB0
};
class __declspec(novtable) CDeferredCommand : public CUnknown {
public:
    unsigned char    _pad0c[0x28];                 // +0x0c (IDeferredCommand vptr, queue, target, time, ...)
    CDispParams      m_DispParams;                 // +0x34
    CComPointer_Retail m_pObject;                  // +0x48
    ~CDeferredCommand();
};
CDeferredCommand::~CDeferredCommand()
{
}