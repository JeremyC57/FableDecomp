// CBase_00caefb0::~CBase_00caefb0() -- EH-framed destructor of a strmbase-style COM object: release the +0x10 COM pointer, then the CUnknown base at +4 (its CBaseObject destructor at +8)
// Grade: DIFFER(113v118) -- base-destructor null checks: retail tests `this` (`test esi,esi`) and then the CUnknown pointer (`lea eax,[esi+4] ; test eax,eax`), ours only the second; the same one-level-short null check parks 0x00CAF480; body and EH states match
// Leaf: ??1CBase_00caefb0
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
class __declspec(novtable) CBase_00caefb0 : public IUnknownRetail, public CUnknown {
public:
    IUnknownRetail* m_pObject;                     // +0x10
    ~CBase_00caefb0();
};
CBase_00caefb0::~CBase_00caefb0()
{
    if (m_pObject)
        m_pObject->Release();
}
