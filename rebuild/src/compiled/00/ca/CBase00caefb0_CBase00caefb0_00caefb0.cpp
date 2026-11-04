// CAudioObjectBase00caefb0::~CAudioObjectBase00caefb0() -- EH-framed destructor of a strmbase-style COM object: release the +0x10 COM pointer (its smart-pointer member's destructor), then the CUnknown base at +4 (its CBaseObject destructor at +8)
// Retry 10b: the COM pointer at +10h is a smart-pointer MEMBER whose inline destructor releases it (the destructor body is empty):
// the release then dereferences the member's `this`, not the destructor's, so VC7.1 keeps the `test esi,esi` null check of
// the base conversions; a release written in the destructor body dereferences `this` and the check is dropped.
// Leaf: ??1CBase_00caefb0
// Vendor naming V2 (plan phase 1, one address, one name; V1 decision D2): 0x00CACAF0 ??1CMyBaseObject@@QAE@XZ (was ??1CBaseObject@N_00cacaf0@@QAE@XZ): Fable's strmbase renames combase.h CBaseObject to CMyBaseObject (RTTI of vtable 0x0129D08C), which keeps it apart from the engine's CBaseObject 0x0099A380 / 0x0099A430 without a namespace
// Vendor naming wave V1 (one address, one name): 0x00CAEFB0 ??1CAudioObjectBase00caefb0@@UAE@XZ (was ??1CBase_00caefb0@@UAE@XZ)
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAEFB0, length 118 bytes.
// Label correction: the catalog's "CVolume::ReleaseVolume" is propagated. Callers: the derived destructors 0x00CAF070
// (`this ? this+4 : 0` then a tail jump here) and 0x00CAFD90 (deletes its +0x1c critical section first), so this is a base
// destructor of the same no-RTTI CUnknown family as the landed CVtable_012bf83c 0x00CAA410.
// Callees: the +0x10 COM pointer's Release (slot 2), CBaseObject::~CBaseObject @ 0x00CACAF0.
// Layout (retail): interface vptr +0, CUnknown {vptr +4, CBaseObject +8, m_cRef +0xc}, COM pointer +0x10.
// Shape: EH state 0 for the body, -1 for the base; retail null-checks both base conversions (`test esi,esi` then
// `lea eax,[esi+4] ; test eax,eax`) before `lea ecx,[eax+4]`.
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
class __declspec(novtable) CAudioObjectBase00caefb0 : public IUnknownRetail, public CUnknown {
public:
    CComPointer_Retail m_pObject;                  // +0x10
    ~CAudioObjectBase00caefb0();
};
CAudioObjectBase00caefb0::~CAudioObjectBase00caefb0()
{
}