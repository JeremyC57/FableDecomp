// CUnknown::~CUnknown() -- DirectShow BaseClasses (combase.h): the empty inline virtual destructor, out of line for the EH unwind funclets; only the CBaseObject base destructor remains
// Leaf: ??1CUnknown@@UAE@XZ
// Vendor naming V2 (plan phase 1, one address, one name; V1 decision D2): 0x00CACAF0 ??1CMyBaseObject@@QAE@XZ (was ??1CBaseObject@@QAE@XZ): Fable's strmbase renames combase.h CBaseObject to CMyBaseObject (RTTI of vtable 0x0129D08C), which keeps it apart from the engine's CBaseObject 0x0099A380 / 0x0099A430 without a namespace
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA7780, 19 bytes; called only from EH unwind funclets.
// Identity: DirectShow BaseClasses combase.h `virtual ~CUnknown() {}`; the landed CBaseAllocator dtor's EH map (FuncInfo 0x0137143C)
//   calls 0x00CA7780 for the CUnknown base. CBaseObject is the empty base at +4.
// Callees: CBaseObject::~CBaseObject 0x00CACAF0.
// Shape: the null-tested this-adjust to the CBaseObject base, then the tail jump.
struct __declspec(novtable) IUnknownRetail {
    virtual long __stdcall QueryInterface(const void* iid, void** object) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
};
struct __declspec(novtable) INonDelegatingUnknown {
    virtual long __stdcall NonDelegatingQueryInterface(const void* iid, void** object) = 0;
    virtual unsigned long __stdcall NonDelegatingAddRef() = 0;
    virtual unsigned long __stdcall NonDelegatingRelease() = 0;
};
class CMyBaseObject {                                     // strmbase object counter (no data in the release build)
public:
    ~CMyBaseObject();                                     // 0x00CACAF0
};
class __declspec(novtable) CUnknown : public INonDelegatingUnknown, public CMyBaseObject {
    IUnknownRetail* const m_pUnknown;                   // +0x04
protected:
    long m_cRef;                                        // +0x08
public:
    CUnknown(IUnknownRetail* outer);
    virtual ~CUnknown() {}
};

void DestroyUnknown(CUnknown* unknown)
{
    unknown->CUnknown::~CUnknown();
}