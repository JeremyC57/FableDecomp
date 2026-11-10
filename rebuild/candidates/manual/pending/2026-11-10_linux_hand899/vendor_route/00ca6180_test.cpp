// Behaviour test for CBaseInputPin::AddRef`adjustor{4}' @ 0x00CA6180: AddRef called through the pin's IQualityControl interface reaches the same
// DECLARE_IUNKNOWN body as through IPin (the thunk only moves `this` to the IPin sub-object), which delegates to the owner.
#include <cstdio>
typedef long HRESULT; typedef unsigned long ULONG;
struct GUID { unsigned long a; };
struct IUnknown {
    virtual HRESULT QueryInterface(const GUID& riid, void** ppv) = 0;
    virtual ULONG AddRef() = 0;
    virtual ULONG Release() = 0;
};
struct IPin : IUnknown { virtual HRESULT Connect() = 0; };
struct IQualityControl : IUnknown { virtual HRESULT Notify() = 0; };
struct COwner : IUnknown {
    int qi, add, rel;
    COwner() : qi(0), add(0), rel(0) { }
    HRESULT QueryInterface(const GUID&, void** ppv) { ++qi; *ppv = this; return 0; }
    ULONG AddRef() { return ++add; }
    ULONG Release() { return ++rel; }
};
struct CUnknown { void* vfptr_stand_in; IUnknown* m_pUnknown; long m_cRef; IUnknown* GetOwner() const { return m_pUnknown; } };
struct CBaseInputPin : CUnknown, IPin, IQualityControl {
    HRESULT QueryInterface(const GUID& riid, void** ppv) { return GetOwner()->QueryInterface(riid, ppv); }
    ULONG AddRef() { return GetOwner()->AddRef(); }
    ULONG Release() { return GetOwner()->Release(); }
    HRESULT Connect() { return 0; }
    HRESULT Notify() { return 0; }
};
int main()
{
    COwner owner;
    CBaseInputPin pin; pin.m_pUnknown = &owner;
    IQualityControl* q = &pin;
    IPin* p = &pin;
    void* out = 0; GUID g = { 1 };
    q->AddRef(); p->AddRef();
    bool ok = owner.add == 2 && owner.qi == 0 && owner.rel == 0;
    if (!ok) { std::printf("00ca6180_TEST FAIL\n"); return 1; }
    std::printf("00ca6180_TEST PASS\n");
    return 0;
}
