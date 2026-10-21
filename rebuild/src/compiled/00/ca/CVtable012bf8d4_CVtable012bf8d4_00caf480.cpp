// CVtable_012bf8d4::~CVtable_012bf8d4() -- EH-framed destructor of a strmbase-style COM object: release the +0x48 COM pointer (its smart-pointer member's destructor), destroy the +0x34 member, then the CUnknown base (its CBaseObject destructor)
// Retry 10b: the COM pointer at +48h is a smart-pointer MEMBER whose inline destructor releases it (the destructor body is empty):
// the release then dereferences the member's `this`, not the destructor's, so VC7.1 keeps the `test esi,esi` null check of
// the base conversions; a release written in the destructor body dereferences `this` and the check is dropped.
// Leaf: ??1CVtable_012bf8d4
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAF480, length 122 bytes (padded to 128).
// Label correction: the catalog's "CVolume::ReleaseVolume" is propagated. The only caller is the scalar deleting destructor
// 0x00CAF460 in slot 3 of the primary vtable 0x012BF8D4 (no-RTTI module: slots 0..2 NonDelegatingQueryInterface 0x00CAEC30,
// AddRef 0x00CACD00, Release 0x00CACD20), the same CUnknown family as the landed CVtable_012bf83c 0x00CAA410.
// Callees: the +0x48 COM pointer's Release (slot 2), the +0x34 member's destructor @ 0x00CAEBB0, CBaseObject::~CBaseObject
// @ 0x00CACAF0 (strmbase object counter).
// Layout (retail): CUnknown {vptr +0, CBaseObject +4, m_cRef +8}, member +0x34, COM pointer +0x48.
// Shape: EH state 1 for the body, byte store of state 0 around the member destructor, -1 for the base; the CBaseObject
// conversion is null-checked (`test esi,esi ; je ; xor ecx,ecx`).
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
class CBaseObject {                                // strmbase object counter base (4 bytes in this build)
public:
    const char* m_pName;
    ~CBaseObject();                                // 0x00CACAF0
};
class __declspec(novtable) CUnknown : public INonDelegatingUnknownRetail, public CBaseObject {
public:
    long m_cRef;                                   // +0x08 within CUnknown
    virtual ~CUnknown() {}
};
struct CComPointer_Retail {                       // inline smart pointer: releases its interface
    IUnknownRetail* p;
    ~CComPointer_Retail() { if (p) p->Release(); }
};
struct CMember_00caebb0 {
    unsigned char _body[0x14];
    ~CMember_00caebb0();                           // 0x00CAEBB0
};
class __declspec(novtable) CVtable_012bf8d4 : public CUnknown {
public:
    unsigned char    _pad0c[0x28];                 // +0x0c
    CMember_00caebb0 m_member;                     // +0x34
    CComPointer_Retail m_pObject;                  // +0x48
    ~CVtable_012bf8d4();
};
CVtable_012bf8d4::~CVtable_012bf8d4()
{
}